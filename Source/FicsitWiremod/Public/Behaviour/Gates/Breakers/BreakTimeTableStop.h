// 

#pragma once

#include "CoreMinimal.h"
#include "Behaviour/FGWiremodBuildable.h"
#include "BreakTimeTableStop.generated.h"

UCLASS()
class FICSITWIREMOD_API ABreakTimeTableStop : public AFGWiremodBuildable
{
	GENERATED_BODY()

public:
	virtual void ServerProcess_Implementation(double DeltaTime) override
	{
		Stop = GetConnection(0).GetTimeTableStop();
	}

	UFUNCTION()
	FString GetStationName()
	{
		if(Stop.Station) return Stop.Station->GetStationName().ToString();
		else return FString();
	}
	UFUNCTION()
	AActor* GetStation()
	{
		if(Stop.Station) return Stop.Station->GetStation();
		else return nullptr;
	}
	UFUNCTION()
	bool GetWaitForFullLoadUnload() { return Stop.DockingRuleSet.DockingDefinition == ETrainDockingDefinition::TDD_FullyLoadUnload; }
	UFUNCTION()
	double GetDockingDuration() { return Stop.DockingRuleSet.DockForDuration; }
	UFUNCTION()
	bool GetIsDurationAndRule() { return Stop.DockingRuleSet.IsDurationAndRule; }
	UFUNCTION()
	bool GetIgnoreFullUnload() { return Stop.DockingRuleSet.IgnoreFullLoadUnloadIfTransferBlockedByFilters; }
	UFUNCTION()
	TArray<TSubclassOf<UFGItemDescriptor>> GetLoadDescriptors() { return Stop.DockingRuleSet.LoadFilterDescriptors; }
	UFUNCTION()
	TArray<TSubclassOf<UFGItemDescriptor>> GetUnloadDescriptors() { return Stop.DockingRuleSet.UnloadFilterDescriptors; }
	
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override
	{
		Super::GetLifetimeReplicatedProps(OutLifetimeProps);

		DOREPLIFETIME(ABreakTimeTableStop, Stop)
	}
	
	UPROPERTY(Replicated)
	FTimeTableStop Stop;
};
