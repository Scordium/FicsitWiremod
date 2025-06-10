// 

#pragma once

#include "CoreMinimal.h"
#include "CircuitryWireSupport.h"
#include "EnhancedInputComponent.h"
#include "WiretoolWidget.h"
#include "WiremodBaseTool.h"
#include "FGCharacterPlayer.h"
#include "Behaviour/WiremodRemoteCalls.h"
#include "Buildables/FGBuildable.h"
#include "Runtime/CoreUObject/Public/UObject/ScriptInterface.h"
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
	
	void OnScrollDown(){ if(Widget.GetObject()) IWiringToolWidget::Execute_ScrollListDown(Widget.GetObject(), SelectedConnection.ConnectionType, HasSelectedConnection()); }
	void OnScrollUp(){ if(Widget.GetObject()) IWiringToolWidget::Execute_ScrollListUp(Widget.GetObject(), SelectedConnection.ConnectionType, HasSelectedConnection()); }
	void OnSwitchSnapMode() { SnapToCenter = !SnapToCenter; }
	void OnSwitchConnectMode() { MultiConnectMode = !MultiConnectMode; }

public:
	virtual void Tick(float DeltaSeconds) override
	{
		if(!IsLocallyControlled()) return;
		if(!Widget.GetObject()) return;
		
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

			SelectedConnection.WirePositions[SelectedConnection.WirePositions.Num() - 1] = GetNextWirePointPosition(LastPoint);


			UWiremodGameWorldModule::Self->WirePreviewActor->AssignedConnection.Transmitter.WireColor = CircuitryConfig::GetDefaultWireColor();
			UWiremodGameWorldModule::Self->WirePreviewActor->DrawWireFromPoints(SelectedConnection.WirePositions);
		}
		
		bool SuccessfulHit;
		FVector Location;
		AActor* HitActor = GetTargetLookAt(CircuitryConfig::GetTraceDistance(), ETraceTypeQuery::TraceTypeQuery1, Location, SuccessfulHit);
		if(!SuccessfulHit)
		{
			IWiringToolWidget::Execute_ClearUI(Widget.GetObject());
			CurrentTarget = nullptr;
			SetOutline(nullptr);
			return;
		}
		if(HitActor != CurrentTarget) SelectionChanged(HitActor);
	}

	UFUNCTION(BlueprintCallable)
	void SelectionChanged(AActor* NewTarget)
	{
		TargetWireSupport = Cast<ACircuitryWireSupport>(NewTarget);
		
		IWiringToolWidget::Execute_ClearUI(Widget.GetObject());
		auto Building = Cast<AFGBuildable>(NewTarget); 
		if(!Building || !UWiremodBlueprintUtils::IsObjectCompatible(Building)) HandleUnknownTarget(Building);
		else
		{
			CurrentTarget = Building;
			SetOutline(Building);
			IWiringToolWidget::Execute_CreateConnectionsList(Widget.GetObject(), HasSelectedConnection() ? Input : Output, SelectedConnection.ConnectionType, Building);
		}
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
		if(IWiringToolWidget::Execute_GetCurrentIndex(Widget.GetObject()) == -1 || IWiringToolWidget::Execute_GetMaxIndex(Widget.GetObject()) == 0) return false;
		if(!HasSelectedConnection())
		{
			TArray<FBuildingConnection> Connections;
			FBuildableNote Note;
			UWiremodBlueprintUtils::GetAvailableConnections(CurrentTarget, Output, Connections, Note);

			return Connections.Num() > 0;
		}
		else
		{
			TArray<FBuildingConnection> Connections;
			FBuildableNote Note;
			UWiremodBlueprintUtils::GetAvailableConnections(CurrentTarget, Input, Connections, Note);
			if(Connections.Num() == 0) return false;
			return UConnectionTypeFunctions::IsValidConnectionPair(Connections[IWiringToolWidget::Execute_GetCurrentIndex(Widget.GetObject())].ConnectionType, SelectedConnection.ConnectionType);
		}
	}

	UFUNCTION(BlueprintPure)
	bool CanConnectAny()
	{
		if(!CurrentTarget) return false;
		TArray<FBuildingConnection> Connections;
		FBuildableNote Note;
		UWiremodBlueprintUtils::GetAvailableConnections(CurrentTarget, Input, Connections, Note);

		for(auto& Connection : Connections){
			if(UConnectionTypeFunctions::IsValidConnectionPair(Connection.ConnectionType, SelectedConnection.ConnectionType)){
				return true;
			}
		}

		return false;
	}

	void AddNewWirePoint() { AddNewWirePoint(false); }
	UFUNCTION(BlueprintCallable)
	void AddNewWirePoint(bool CheckSupportsOnly = false)
	{
		if(!IsLocallyControlled()) return;

		if (TargetWireSupport)
		{
			AddNewWirePoint(TargetWireSupport->GetSnapPositionWorldSpace());
			return;
		}

		if (CheckSupportsOnly) return;

		bool SuccessfulHit;
		FVector Location;
		GetTargetLookAt(CircuitryConfig::GetTraceDistance(), ETraceTypeQuery::TraceTypeQuery1, Location, SuccessfulHit);

		if(!SuccessfulHit) return;

		AddNewWirePoint(Location);
	}

	void AddNewWirePoint(const FVector& Location)
	{
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
	FVector GetNextWirePointPosition(const FVector& FallbackPosition)
	{
		if (TargetWireSupport) return TargetWireSupport->GetSnapPositionWorldSpace();

		return FallbackPosition;
	}

	UFUNCTION(BlueprintCallable)
	void OnConnectionSelected()
	{
		if(!IsLocallyControlled()) return;
		
		if(!CanConnect())
		{
			AddNewWirePoint(true);
			return;
		}
		auto RCO = Cast<UWiremodRemoteCalls>(Cast<AFGPlayerController>(GetWorld()->GetFirstPlayerController())->GetRemoteCallObjectOfClass(UWiremodRemoteCalls::StaticClass()));
		auto Setter = UWiremodBlueprintUtils::GetSetterObject();
		auto Index = IWiringToolWidget::Execute_GetCurrentIndex(Widget.GetObject());
		auto Data = IWiringToolWidget::Execute_GetConnectionData(Widget.GetObject());
		
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
				auto Index = IWiringToolWidget::Execute_GetCurrentIndex(Widget.GetObject());
				if(Index != -1)
				{
					RCO->ResetConnections(CurrentTarget, Index, Setter);
					FFormatNamedArguments Args;
					Args.Add("ObjectName", Cast<AFGBuildable>(CurrentTarget)->mDisplayName);
					Args.Add("InputName", FText::FromString(IWiringToolWidget::Execute_GetConnectionName(Widget.GetObject())));
				
					auto NotificationText = FText::Format(ResetConnectionsSuccessFormat, Args);

					ShowNotification(NotificationText);
				}
				
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
		CurrentTarget = nullptr;
		SetOutline(Target, EOutlineColor::OC_RED);
		
		IWiringToolWidget::Execute_ClearUI(Widget.GetObject());
		
		if(auto Building = Cast<AFGBuildable>(Target))
			IWiringToolWidget::Execute_ShowBuildingDisqualifier(Widget.GetObject(), FText::FromString("Unknown building: " + Building->mDisplayName.ToString()));
	}
	

	UFUNCTION(BlueprintPure)
	FORCEINLINE bool HasSelectedConnection() const { return IsValid(SelectedConnection.Object); }
	
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite)
	AActor* CurrentTarget;

	UPROPERTY(BlueprintReadOnly)
	ACircuitryWireSupport* TargetWireSupport;

	
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite)
	FConnectionData SelectedConnection;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite)
	TScriptInterface<IWiringToolWidget> Widget;

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
