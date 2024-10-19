// 

#pragma once

#include "CoreMinimal.h"
#include "FGCircuitSubsystem.h"
#include "FGPowerInfoComponent.h"
#include "FGCircuitSubsystem.h"
#include "Behaviour/FGWiremodBuildable.h"
#include "Buildables/FGBuildablePowerStorage.h"
#include "BreakPowerCircuit.generated.h"

class UFGPowerInfoComponent;

UCLASS()
class FICSITWIREMOD_API ABreakPowerCircuit : public AFGWiremodBuildable
{
	GENERATED_BODY()

public:
	virtual void ServerProcess_Implementation(double DeltaTime) override
	{
		Circuit = GetConnection(0).GetCircuit();

		if(Circuit)
		{
			Circuit->GetStats(CachedStats);
			
			//Reset fuse if it was tripped
			if(GetConnection(1).GetBool()) Circuit->ResetFuse();

			// Trip fuse
			if(GetConnection(2).GetBool() && !Circuit->mIsFuseTriggered)
			{
				Circuit->mIsFuseTriggered = true;
				AFGCircuitSubsystem::Get(GetWorld())->PowerCircuit_OnFuseSet({Circuit});
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
			}
		}
		else CachedStats = FPowerCircuitStats();
	}


	UFUNCTION()
	FORCEINLINE double GetBatterySumInput()
	{
		if(!Circuit) return 0;
		return Circuit->GetBatterySumPowerInput();
	}

	UFUNCTION()
	FORCEINLINE double GetBatterySumOutput()
	{
		if(!Circuit) return 0;
		return Circuit->GetBatterySumPowerOutput();
	}

	UFUNCTION()
	FORCEINLINE double GetBatteryStoredPower()
	{
		if(!Circuit) return 0;
		return Circuit->GetBatterySumPowerStore();
	}
	
	UFUNCTION()
	FORCEINLINE double GetBatteryStorageCapacity()
	{
		if(!Circuit) return 0;
		return Circuit->GetBatterySumPowerStoreCapacity();
	}

	UFUNCTION()
	FORCEINLINE double GetBatteryStoredPowerPercent()
	{
		if(!Circuit) return 0;
		return Circuit->GetBatterySumPowerStorePercent();
	}

	UFUNCTION()
	FORCEINLINE double GetTimeTillBatteriesFull()
	{
		if(!Circuit) return -1;
		return Circuit->GetTimeToBatteriesFull();
	}

	UFUNCTION()
	FORCEINLINE double GetTimeTillBatteriesEmpty()
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
	FORCEINLINE double GetPowerProduced()
	{
		return CachedStats.PowerProduced;
	}

	UFUNCTION()
	FORCEINLINE double GetPowerConsumed()
	{
		return CachedStats.PowerConsumed;
	}
	
	UFUNCTION()
	FORCEINLINE double GetMaximumPowerConsumption()
	{
		return CachedStats.MaximumPowerConsumption;
	}

	UFUNCTION()
	FORCEINLINE double GetBatteryPowerInput()
	{
		return CachedStats.BatteryPowerInput;
	}

	UFUNCTION()
	FORCEINLINE double GetPowerProductionCapacity()
	{
		return CachedStats.PowerProductionCapacity;
	}

	UFUNCTION()
	TArray<double> GetProductionGraph() { return GetPoints<&FPowerGraphPoint::Produced>(); }

	UFUNCTION()
	TArray<double> GetConsumptionGraph(){ return GetPoints<&FPowerGraphPoint::Consumed>(); }

	UFUNCTION()
	TArray<double> GetMaxConsumptionGraph(){ return GetPoints<&FPowerGraphPoint::MaximumConsumption>(); }

	UFUNCTION()
	TArray<double> GetCapacityGraph(){ return GetPoints<&FPowerGraphPoint::ProductionCapacity>(); }

	template<float FPowerGraphPoint::*Member>
	TArray<double> GetPoints()
	{
		TArray<double> Out;

		TArray<float> Values;
		CachedStats.GetPoints<Member>(Values);
		for(auto Value : Values) Out.Add(Value);

		return Out;
	}

	virtual void GetLifetimeReplicatedProps( TArray<FLifetimeProperty>& OutLifetimeProps ) const override
	{
		Super::GetLifetimeReplicatedProps(OutLifetimeProps);
		DOREPLIFETIME(ABreakPowerCircuit, CachedStats)
		DOREPLIFETIME(ABreakPowerCircuit, Circuit)
	}
	
	UPROPERTY(Replicated, SaveGame, VisibleInstanceOnly)
	FPowerCircuitStats CachedStats;

	UPROPERTY(Replicated, VisibleInstanceOnly)
	UFGPowerCircuit* Circuit;
};
