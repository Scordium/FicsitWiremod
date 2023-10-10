// 

#pragma once

#include "CoreMinimal.h"
#include "EnhancedInputComponent.h"
#include "WiremodUtils.h"
#include "WiretoolWidget.h"
#include "Behaviour/WiremodRemoteCalls.h"
#include "Buildables/FGBuildable.h"
#include "Equipment/WiremodBaseTool.h"
#include "Utility/CircuitryInputMappings.h"
#include "Utility/WiremodBlueprintUtils.h"
#include "Utility/WiremodGameWorldModule.h"
#include "WiremodTool.generated.h"

UCLASS()
class FICSITWIREMOD_API AWiremodTool : public AWiremodBaseTool
{
	GENERATED_BODY()

	void OnScrollDownPress(const FInputActionValue& Value){ if(Value.Get<bool>()) OnScrollDown(); }
	void OnScrollUpPress(const FInputActionValue& Value){ if(Value.Get<bool>()) OnScrollUp(); }
	
	void OnScrollDown(){ if(Widget) Widget->ScrollListDown(SelectedConnection.ConnectionType, HasSelectedConnection()); }
	void OnScrollUp(){ if(Widget) Widget->ScrollListUp(SelectedConnection.ConnectionType, HasSelectedConnection()); }
	void OnSwitchSnapMode() { SnapToCenter = !SnapToCenter; }
	void OnSwitchConnectMode() { MultiConnectMode = !MultiConnectMode; }

public:
	virtual void Tick(float DeltaSeconds) override
	{
		if(!IsLocallyControlled()) return;
		if(!Widget) return;
		
		if(!HasSelectedConnection())
		{
			if(auto WirePreview = UWiremodGameWorldModule::Self->WirePreviewActor)
				WirePreview->DestroyAllSplineMeshes();
		}
		else
		{
			bool SuccessfulHit;
			FVector Location;
			GetTargetLookAt(100000000, ETraceTypeQuery::TraceTypeQuery1, Location, SuccessfulHit);

			FVector LastPoint = (IsValid(CurrentTarget) && CanConnectAny() && SnapToCenter) ? CurrentTarget->GetActorLocation() : Location;

			SelectedConnection.WirePositions[SelectedConnection.WirePositions.Num() - 1] = LastPoint;


			UWiremodGameWorldModule::Self->WirePreviewActor->AssignedConnection.Transmitter.WireColor = CircuitryConfig::GetDefaultWireColor();
			UWiremodGameWorldModule::Self->WirePreviewActor->DrawWireFromPoints(SelectedConnection.WirePositions);
		}
		
		bool SuccessfulHit;
		FVector Location;
		AActor* HitActor = GetTargetLookAt(CircuitryConfig::GetTraceDistance(), ETraceTypeQuery::TraceTypeQuery1, Location, SuccessfulHit);
		if(!SuccessfulHit)
		{
			Widget->ClearUI();
			SetOutline(nullptr);
			CurrentTarget = nullptr;
			return;
		}
		if(HitActor != CurrentTarget) SelectionChanged(HitActor);
	}

	UFUNCTION(BlueprintCallable)
	void SelectionChanged(AActor* NewTarget)
	{
		Widget->ClearUI();
		auto Building = Cast<AFGBuildable>(NewTarget); 
		if(!Building || !UWiremodBlueprintUtils::IsObjectCompatible(Building)) HandleUnknownTarget(Building);
		else
		{
			CurrentTarget = Building;
			SetOutline(Building);
			Widget->CreateConnectionsList(HasSelectedConnection() ? Input : Output, SelectedConnection.ConnectionType, Building);
		}
	}

	UFUNCTION(BlueprintCallable)
	void SetOutline(AActor* Object, EOutlineColor Color = EOutlineColor::OC_USABLE) const
	{
		auto Outline = GetInstigatorCharacter()->GetOutline();
		Outline->HideOutline();

		if(Object) Outline->ShowOutline(Object, Color);
	}

protected:

	UFUNCTION(BlueprintCallable)
	void ReturnToIdle(bool ForceReset)
	{
		if(ForceReset || !CircuitryConfig::GetShouldToolsKeepStateOnUnequip())
		{
			CurrentTarget = nullptr;
			SelectedConnection = FConnectionData();
		}

		SetOutline(nullptr);
		HideHudHints();
	}

	UFUNCTION(BlueprintCallable)
	void SelectTargetAsEntity()
	{
		if(!IsLocallyControlled()) return;
		
		bool SuccessfulHit;
		FVector Location;
		AActor* HitActor = GetTargetLookAt(CircuitryConfig::GetTraceDistance(), ETraceTypeQuery::TraceTypeQuery1, Location, SuccessfulHit);

		if(!SuccessfulHit) return;

		SelectedConnection = FConnectionData();

		SelectedConnection.Object = HitActor;
		SelectedConnection.FunctionName = FName("Self");
		SelectedConnection.ConnectionType = Entity;
		SelectedConnection.WireColor = CircuitryConfig::GetDefaultWireColor();
		SelectedConnection.WireHidden = CircuitryConfig::GetIsWireDefaultHidden();
		SelectedConnection.WirePositions = TArray
		{
			SnapToCenter ? HitActor->GetActorLocation() : Location,
			FVector::ZeroVector
		};
		
		DisplayHudHints();
		ShowNotification(SelectTargetAsEntityText);
		SelectionChanged(HitActor);
	}
	
	UFUNCTION(BlueprintPure)
	bool CanConnect()
	{
		if(!CurrentTarget) return false;
		if(Widget->CurrentIndex == -1 || Widget->MaxIndex == 0) return false;
		if(!HasSelectedConnection())
		{
			TArray<FBuildingConnection> Connections;
			int Count;
			FBuildableNote Note;
			UWiremodBlueprintUtils::GetAvailableConnections(CurrentTarget, Output, Connections, Count, Note);

			return Count > 0;
		}
		else
		{
			TArray<FBuildingConnection> Connections;
			int Count;
			FBuildableNote Note;
			UWiremodBlueprintUtils::GetAvailableConnections(CurrentTarget, Input, Connections, Count, Note);
			if(Count == 0) return false;
			return UConnectionTypeFunctions::IsValidConnectionPair(Connections[Widget->CurrentIndex].ConnectionType, SelectedConnection.ConnectionType);
		}
	}

	UFUNCTION(BlueprintPure)
	bool CanConnectAny()
	{
		if(!CurrentTarget) return false;
		TArray<FBuildingConnection> Connections;
		int Count;
		FBuildableNote Note;
		UWiremodBlueprintUtils::GetAvailableConnections(CurrentTarget, Input, Connections, Count, Note);

		for(auto Connection : Connections){
			if(UConnectionTypeFunctions::IsValidConnectionPair(Connection.ConnectionType, SelectedConnection.ConnectionType)){
				return true;
			}
		}

		return false;
	}

	UFUNCTION(BlueprintCallable)
	void AddNewWirePoint()
	{
		if(!IsLocallyControlled()) return;

		bool SuccessfulHit;
		FVector Location;
		GetTargetLookAt(CircuitryConfig::GetTraceDistance(), ETraceTypeQuery::TraceTypeQuery1, Location, SuccessfulHit);

		if(!SuccessfulHit) return;

		SelectedConnection.WirePositions.Insert(Location, SelectedConnection.WirePositions.Num() - 1);
	}

	UFUNCTION(BlueprintCallable)
	void RemoveWirePoint()
	{
		if(!IsLocallyControlled()) return;

		int NewSize = SelectedConnection.WirePositions.Num() - 1;
		NewSize = FMath::Clamp(NewSize, 2, MAX_int32);
		SelectedConnection.WirePositions.SetNum(NewSize);
	}

	UFUNCTION(BlueprintCallable)
	void OnConnectionSelected()
	{
		if(!IsLocallyControlled()) return;
		
		if(!CanConnect()) return;
		auto RCO = Cast<UWiremodRemoteCalls>(Cast<AFGPlayerController>(GetWorld()->GetFirstPlayerController())->GetRemoteCallObjectOfClass(UWiremodRemoteCalls::StaticClass()));
		auto Setter = UWiremodBlueprintUtils::GetSetterObject();
		auto Index = Widget->CurrentIndex;
		auto Data = Widget->GetConnectionData();
		
		if(HasSelectedConnection())
		{
			if(UWiremodBlueprintUtils::IsCircuitry(CurrentTarget))
			{
				RCO->ConnectWiremodObject(CurrentTarget, SelectedConnection, Index, Setter);
			}
			else
			{
				auto DynamicData = FDynamicConnectionData(SelectedConnection, Data);
				RCO->ConnectNonWiremodObject(DynamicData, Index, Setter);
			}

			if(!MultiConnectMode) SelectedConnection = FConnectionData();
		}
		else
		{
			bool SuccessfulHit;
			FVector Location;
			AActor* HitActor = GetTargetLookAt(CircuitryConfig::GetTraceDistance(), ETraceTypeQuery::TraceTypeQuery1, Location, SuccessfulHit);

			if(SuccessfulHit)
			{
				SelectedConnection = Data;
				SelectedConnection.WirePositions = TArray
				{
					SnapToCenter ? HitActor->GetActorLocation() : Location,
					FVector::ZeroVector
				};
			}
		}

		SelectionChanged(CurrentTarget);
		DisplayHudHints();
	}

	UFUNCTION(BlueprintCallable)
	void OnResetPressed()
	{
		if(!IsLocallyControlled()) return;
		
		if(HasSelectedConnection())
		{
			if(UWiremodBlueprintUtils::IsObjectCompatible(CurrentTarget))
			{
				auto RCO = Cast<UWiremodRemoteCalls>(Cast<AFGPlayerController>(GetWorld()->GetFirstPlayerController())->GetRemoteCallObjectOfClass(UWiremodRemoteCalls::StaticClass()));
				auto Setter = UWiremodBlueprintUtils::GetSetterObject();
				auto Index = Widget->CurrentIndex;

				RCO->ResetConnections(CurrentTarget, Index, Setter);


				FFormatNamedArguments Args;
				Args.Add("ObjectName", Cast<AFGBuildable>(CurrentTarget)->mDisplayName);
				Args.Add("InputName", FText::FromString(Widget->GetConnectionName()));
				
				auto NotificationText = FText::Format(ResetConnectionsSuccessFormat, Args);

				ShowNotification(NotificationText);
				SelectionChanged(CurrentTarget);
			}
			else
			{
				ReturnToIdle(true);
				ShowNotification(ReleaseConnectionSuccessText);
			}

			DisplayHudHints();
		}
		else ShowNotification(ResetConnectionsNoteText);
	}


	virtual void WasEquipped_Implementation() override
	{
		if(IsLocallyControlled())
		{
			Super::WasEquipped_Implementation();
			DisplayHudHints();
			
			auto EnhancedInput = Cast<UEnhancedInputComponent>(InputComponent);
			auto InputsContext = GetFirstContextOfType<UCircuitryInputMappings>();
			EnhancedInput->BindAction(InputsContext->PrimaryKey, ETriggerEvent::Triggered, this, &AWiremodTool::OnConnectionSelected);
			EnhancedInput->BindAction(InputsContext->SecondaryKey, ETriggerEvent::Triggered, this, &AWiremodTool::OnScrollDown);
			EnhancedInput->BindAction(InputsContext->ScrollDown, ETriggerEvent::Triggered, this, &AWiremodTool::OnScrollDownPress);
			EnhancedInput->BindAction(InputsContext->ScrollUp, ETriggerEvent::Triggered, this, &AWiremodTool::OnScrollUpPress);
			EnhancedInput->BindAction(InputsContext->AuxKey, ETriggerEvent::Triggered, this, &AWiremodTool::OnResetPressed);
			EnhancedInput->BindAction(InputsContext->SpecialAction1, ETriggerEvent::Triggered, this, &AWiremodTool::OnSwitchSnapMode);
			EnhancedInput->BindAction(InputsContext->SpecialAction2, ETriggerEvent::Triggered, this, &AWiremodTool::OnSwitchConnectMode);
			EnhancedInput->BindAction(InputsContext->AltAction1, ETriggerEvent::Triggered, this, &AWiremodTool::AddNewWirePoint);
			EnhancedInput->BindAction(InputsContext->AltAction2, ETriggerEvent::Triggered, this, &AWiremodTool::RemoveWirePoint);
		}
	}
	
	virtual void WasUnEquipped_Implementation() override
	{
		if(!IsLocallyControlled()) return;
		Super::WasUnEquipped_Implementation();
		
		ReturnToIdle(false);
		if(auto WirePreview = UWiremodGameWorldModule::Self->WirePreviewActor)
			WirePreview->DestroyAllSplineMeshes();
	}
	
	void HandleUnknownTarget(AActor* Target)
	{
		SetOutline(Target, EOutlineColor::OC_RED);
		CurrentTarget = nullptr;

		Widget->ClearUI();
		if(auto Building = Cast<AFGBuildable>(Target)) Widget->ShowBuildingDisqualifier(FText::FromString("Unknown building: " + Building->mDisplayName.ToString()));
		else if(auto Wire = Cast<AConnectionWireBase>(Target)) Widget->ShowWireInfo(Wire);
	}
	

	UFUNCTION(BlueprintPure)
	FORCEINLINE bool HasSelectedConnection() const { return IsValid(SelectedConnection.Object); }
	
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite)
	AActor* CurrentTarget;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite)
	FConnectionData SelectedConnection;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite)
	UWiretoolWidget* Widget;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite)
	bool SnapToCenter = true;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite)
	bool MultiConnectMode = false;


	UPROPERTY(EditDefaultsOnly)
	FText ResetConnectionsNoteText;

	UPROPERTY(EditDefaultsOnly)
	FText ResetConnectionsSuccessFormat;

	UPROPERTY(EditDefaultsOnly)
	FText ReleaseConnectionSuccessText;

	UPROPERTY(EditDefaultsOnly)
	FText SelectTargetAsEntityText;
};
