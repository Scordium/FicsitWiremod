// 

#pragma once

#include "CoreMinimal.h"
#include "FGCharacterPlayer.h"
#include "FGPlayerController.h"
#include "WiremodUtils.h"
#include "WiretoolWidget.h"
#include "Behaviour/WiremodRemoteCalls.h"
#include "Buildables/FGBuildable.h"
#include "Equipment/WiremodBaseTool.h"
#include "Utility/WiremodBlueprintUtils.h"
#include "Utility/WiremodGameWorldModule.h"
#include "WiremodTool.generated.h"

UCLASS()
class FICSITWIREMOD_API AWiremodTool : public AWiremodBaseTool
{
	GENERATED_BODY()

	AWiremodTool() : Super()
	{
		mEquipmentSlot = EEquipmentSlot::ES_ARMS;
		mAttachSocket = FName("hand_rSocket");
		mArmAnimation = EArmEquipment::AE_ShockShank;
	}

	void OnScrollDown(){ if(Widget) Widget->ScrollListDown(SelectedConnection.ConnectionType, HasSelectedConnection()); }
	void OnScrollUp(){ if(Widget) Widget->ScrollListUp(SelectedConnection.ConnectionType, HasSelectedConnection()); }
	void OnSwitchSnapMode() { SnapToCenter = !SnapToCenter; }
	void OnSwitchConnectMode() { MultiConnectMode = !MultiConnectMode; }

public:
	virtual void Tick(float DeltaSeconds) override
	{
		if(!IsLocallyControlled()) return;
		if(!Widget) return;
		
		if(!HasSelectedConnection()) UWiremodGameWorldModule::Self->WirePreviewActor->DestroyAllSplineMeshes();
		else
		{
			bool SuccessfulHit;
			FVector Location;
			GetTargetLookAt(100000000, ETraceTypeQuery::TraceTypeQuery1, Location, SuccessfulHit);

			FVector LastPoint = (IsValid(CurrentTarget) && CanConnectAny() && SnapToCenter) ? CurrentTarget->GetActorLocation() : Location;

			SelectedConnection.WirePositions[SelectedConnection.WirePositions.Num() - 1] = LastPoint;

			UWiremodUtils::DefaultWireColor(UWiremodGameWorldModule::Self->WirePreviewActor->AssignedConnection.Transmitter.WireColor);
			UWiremodGameWorldModule::Self->WirePreviewActor->DrawWireFromPoints(SelectedConnection.WirePositions);
		}
		
		bool SuccessfulHit;
		FVector Location;
		AActor* HitActor = GetTargetLookAt(UWiremodUtils::TraceDistance(), ETraceTypeQuery::TraceTypeQuery1, Location, SuccessfulHit);
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
		if(!Building || !UWiremodBlueprintUtils::IsRegistered(Building)) HandleUnknownTarget(Building);
		else
		{
			CurrentTarget = Building;
			SetOutline(Building);
			Widget->CreateConnectionsList(HasSelectedConnection() ? Input : Output, SelectedConnection.ConnectionType, Building);
		}
	}

	UFUNCTION(BlueprintCallable)
	void SetOutline(AActor* Object, TEnumAsByte<EOutlineColor> Color = EOutlineColor::OC_USABLE)
	{
		auto Outline = GetInstigatorCharacter()->GetOutline();
		Outline->HideOutline();

		if(Object) Outline->ShowOutline(Object, Color);
	}

protected:

	UFUNCTION(BlueprintCallable)
	void ReturnToIdle(bool ForceReset)
	{
		if(ForceReset || !UWiremodUtils::ShouldToolKeepState())
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
		AActor* HitActor = GetTargetLookAt(UWiremodUtils::TraceDistance(), ETraceTypeQuery::TraceTypeQuery1, Location, SuccessfulHit);

		if(!SuccessfulHit) return;

		SelectedConnection = FConnectionData();

		SelectedConnection.Object = HitActor;
		SelectedConnection.FunctionName = FName("Self");
		SelectedConnection.ConnectionType = Entity;
		UWiremodUtils::DefaultWireColor(SelectedConnection.WireColor);
		UWiremodUtils::WireDefaultHidden(SelectedConnection.WireHidden);
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
		GetTargetLookAt(UWiremodUtils::TraceDistance(), ETraceTypeQuery::TraceTypeQuery1, Location, SuccessfulHit);

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
			if(UWiremodBlueprintUtils::IsWiremod(CurrentTarget))
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
			AActor* HitActor = GetTargetLookAt(UWiremodUtils::TraceDistance(), ETraceTypeQuery::TraceTypeQuery1, Location, SuccessfulHit);

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
			if(UWiremodBlueprintUtils::IsRegistered(CurrentTarget))
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
			DisplayHudHints();
			InputComponent->BindAction("PrimaryFire", IE_Pressed, this, &AWiremodTool::OnConnectionSelected);
			InputComponent->BindAction("SecondaryFire", IE_Pressed, this, &AWiremodTool::OnScrollDown);
			InputComponent->BindAction("FicsitWiremod.ScrollDown", IE_Pressed, this, &AWiremodTool::OnScrollDown);
			InputComponent->BindAction("FicsitWiremod.ScrollUp", IE_Pressed, this, &AWiremodTool::OnScrollUp);
			InputComponent->BindAction("FicsitWiremod.NewWirePoint", IE_Pressed, this, &AWiremodTool::AddNewWirePoint);
			InputComponent->BindAction("FicsitWiremod.RemoveWirePoint", IE_Pressed, this, &AWiremodTool::RemoveWirePoint);
			InputComponent->BindAction("Reload", IE_Pressed, this, &AWiremodTool::OnResetPressed);
			InputComponent->BindAction("FicsitWiremod.SpecialAction", IE_Pressed, this, &AWiremodTool::OnSwitchSnapMode);
			InputComponent->BindAction("FicsitWiremod.SwitchConnectMode", IE_Pressed, this, &AWiremodTool::OnSwitchConnectMode);
		}
	}
	
	virtual void WasUnEquipped_Implementation() override
	{
		if(!IsLocallyControlled()) return;

		ReturnToIdle(false);
		UWiremodGameWorldModule::Self->WirePreviewActor->DestroyAllSplineMeshes();
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
