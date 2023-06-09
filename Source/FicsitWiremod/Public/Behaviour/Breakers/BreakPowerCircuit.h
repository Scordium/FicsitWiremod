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
		Circuit = GetConnection(0).GetCircuit();

		if(Circuit)
		{
			Circuit->GetStats(CachedStats);
			
			//Reset fuse if it was tripped
			if(GetConnection(1).GetBool()) Circuit->ResetFuse();

			// Trip fuse
			if(GetConnection(2).GetBool())
			{
				Circuit->mIsFuseTriggered = true;
				AFGCircuitSubsystem::Get(GetWorld())->PowerCircuit_OnFuseSet();
			}

			//Drain batteries
			if(GetConnection(3).GetBool())
			{
				for(UFGPowerInfoComponent* component : Circuit->mPowerInfos)
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
		if(!Circuit) return 0;
		return Circuit->GetBatterySumPowerInput();
	}

	UFUNCTION()
	FORCEINLINE float GetBatterySumOutput()
	{
		if(!Circuit) return 0;
		return Circuit->GetBatterySumPowerOutput();
	}

	UFUNCTION()
	FORCEINLINE float GetBatteryStoredPower()
	{
		if(!Circuit) return 0;
		return Circuit->GetBatterySumPowerStore();
	}
	
	UFUNCTION()
	FORCEINLINE float GetBatteryStorageCapacity()
	{
		if(!Circuit) return 0;
		return Circuit->GetBatterySumPowerStoreCapacity();
	}

	UFUNCTION()
	FORCEINLINE float GetBatteryStoredPowerPercent()
	{
		if(!Circuit) return 0;
		return Circuit->GetBatterySumPowerStorePercent();
	}

	UFUNCTION()
	FORCEINLINE float GetTimeTillBatteriesFull()
	{
		if(!Circuit) return -1;
		return Circuit->GetTimeToBatteriesFull();
	}

	UFUNCTION()
	FORCEINLINE float GetTimeTillBatteriesEmpty()
	{
		if(!Circuit) return -1;
		return Circuit->GetTimeToBatteriesEmpty();
	}

	UFUNCTION()
	FORCEINLINE bool GetHasBatteries()
	{
		if(!Circuit) return false;
		return Circuit->HasBatteries();
	}

	UFUNCTION()
	FORCEINLINE bool GetHasPower()
	{
		if(!Circuit) return false;
		return Circuit->HasPower();
	}

	UFUNCTION()
	FORCEINLINE bool GetIsFuseTriggered()
	{
		if(!Circuit) return false;
		return Circuit->IsFuseTriggered();
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
	UFGPowerCircuit* Circuit;
};
