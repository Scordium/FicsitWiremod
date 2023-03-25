// 

#pragma once

#include "CoreMinimal.h"
#include "FGCircuitSubsystem.h"
#include "FGPowerInfoComponent.h"
#include "Behaviour/FGWiremodBuildable.h"
#include "Buildables/FGBuildablePowerStorage.h"
#include "BreakPowerCircuit.generated.h"

UCLASS()
class FICSITWIREMOD_API ABreakPowerCircuit : public AFGWiremodBuildable
{
	GENERATED_BODY()

public:
	virtual void Process_Implementation(float DeltaTime) override
	{
		Circuit = WM_GetPowerCircuit(0);

		if(Circuit.IsValid())
		{
			Circuit->GetStats(CachedStats);
			
			//Reset fuse if it was tripped
			if(WM_GetBool(1)) Circuit.Get()->ResetFuse();

			// Trip fuse
			if(WM_GetBool(2))
			{
				Circuit.Get()->mIsFuseTriggered = true;
				//Circuit.Get()->StatFuseTriggered();
				AFGCircuitSubsystem::Get(GetWorld())->PowerCircuit_OnFuseSet();
			}

			//Drain batteries
			if(WM_GetBool(3))
			{
				for(UFGPowerInfoComponent* component : Circuit.Get()->mPowerInfos)
				{
					if(auto battery = Cast<AFGBuildablePowerStorage>(component->GetOwner()->GetRootComponent()->GetOwner()))
					{
						battery->mBatteryInfo->SetPowerStore(0);
					}
				}	
			} //No idea what to do here
		}
		else CachedStats = FPowerCircuitStats();
	}


	UFUNCTION()
	FORCEINLINE float GetBatterySumInput()
	{
		if(!Circuit.IsValid()) return 0;
		return Circuit.Get()->GetBatterySumPowerInput();
	}

	UFUNCTION()
	FORCEINLINE float GetBatterySumOutput()
	{
		if(!Circuit.IsValid()) return 0;
		return Circuit.Get()->GetBatterySumPowerOutput();
	}

	UFUNCTION()
	FORCEINLINE float GetBatteryStoredPower()
	{
		if(!Circuit.IsValid()) return 0;
		return Circuit.Get()->GetBatterySumPowerStore();
	}
	
	UFUNCTION()
	FORCEINLINE float GetBatteryStorageCapacity()
	{
		if(!Circuit.IsValid()) return 0;
		return Circuit.Get()->GetBatterySumPowerStoreCapacity();
	}

	UFUNCTION()
	FORCEINLINE float GetBatteryStoredPowerPercent()
	{
		if(!Circuit.IsValid()) return 0;
		return Circuit.Get()->GetBatterySumPowerStorePercent();
	}

	UFUNCTION()
	FORCEINLINE float GetTimeTillBatteriesFull()
	{
		if(!Circuit.IsValid()) return -1;
		return Circuit.Get()->GetTimeToBatteriesFull();
	}

	UFUNCTION()
	FORCEINLINE float GetTimeTillBatteriesEmpty()
	{
		if(!Circuit.IsValid()) return -1;
		return Circuit.Get()->GetTimeToBatteriesEmpty();
	}

	UFUNCTION()
	FORCEINLINE bool GetHasBatteries()
	{
		if(!Circuit.IsValid()) return false;
		return Circuit.Get()->HasBatteries();
	}

	UFUNCTION()
	FORCEINLINE bool GetHasPower()
	{
		if(!Circuit.IsValid()) return false;
		return Circuit.Get()->HasPower();
	}

	UFUNCTION()
	FORCEINLINE bool GetIsFuseTriggered()
	{
		if(!Circuit.IsValid()) return false;
		return Circuit.Get()->IsFuseTriggered();
	}

	UFUNCTION()
	FORCEINLINE float GetPowerProduced()
	{
		return CachedStats.PowerProduced;
	}

	UFUNCTION()
	FORCEINLINE float GetPowerConsumed()
	{
		return CachedStats.PowerConsumed;
	}
	
	UFUNCTION()
	FORCEINLINE float GetMaximumPowerConsumption()
	{
		return CachedStats.MaximumPowerConsumption;
	}

	UFUNCTION()
	FORCEINLINE float GetBatteryPowerInput()
	{
		return CachedStats.BatteryPowerInput;
	}

	UFUNCTION()
	FORCEINLINE float GetPowerProductionCapacity()
	{
		return CachedStats.PowerProductionCapacity;
	}

	UFUNCTION()
	void GetProductionGraph(TArray<float>& Out)
	{
		CachedStats.GetPoints<&FPowerGraphPoint::Produced>(Out);
	}

	UFUNCTION()
	void GetConsumptionGraph(TArray<float>& Out)
	{
		CachedStats.GetPoints<&FPowerGraphPoint::Consumed>(Out);
	}

	UFUNCTION()
	void GetMaxConsumptionGraph(TArray<float>& Out)
	{
		CachedStats.GetPoints<&FPowerGraphPoint::MaximumConsumption>(Out);
	}

	UFUNCTION()
	void GetCapacityGraph(TArray<float>& Out)
	{
		CachedStats.GetPoints<&FPowerGraphPoint::ProductionCapacity>(Out);
	}

	virtual void GetLifetimeReplicatedProps( TArray<FLifetimeProperty>& OutLifetimeProps ) const override
	{
		Super::GetLifetimeReplicatedProps(OutLifetimeProps);
		DOREPLIFETIME(ABreakPowerCircuit, CachedStats)
		DOREPLIFETIME(ABreakPowerCircuit, Circuit)
	}
	
	UPROPERTY(Replicated, VisibleInstanceOnly)
	FPowerCircuitStats CachedStats;

	UPROPERTY(Replicated, VisibleInstanceOnly)
	TWeakObjectPtr<UFGPowerCircuit> Circuit;
};
