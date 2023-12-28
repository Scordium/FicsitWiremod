// 

#pragma once

#include "CoreMinimal.h"
#include "Behaviour/FGWiremodBuildable.h"
#include "BreakTrainTimeTable.generated.h"

UCLASS()
class FICSITWIREMOD_API ABreakTrainTimeTable : public AFGWiremodBuildable
{
	GENERATED_BODY()

public:
	virtual void ServerProcess_Implementation(double DeltaTime) override
	{
		auto VehicleEntity = Cast<AFGRailroadVehicle>(GetConnection(0).GetEntity());
		if(!VehicleEntity)
		{
			Table = nullptr;
			return;
		}

		auto Train = VehicleEntity->GetTrain();
		if(Train) Table = Train->GetTimeTable();
		else Table = nullptr;
	}

	UFUNCTION()
	TArray<FTimeTableStop> GetStops()
	{
		TArray<FTimeTableStop> Out;
		if(Table) Table->GetStops(Out);
		return Out;
	}

	UFUNCTION()
	FTimeTableStop GetCurrentStop()
	{
		if(Table) return Table->GetStop(Table->GetCurrentStop());
		return FTimeTableStop();
	}

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override
	{
		Super::GetLifetimeReplicatedProps(OutLifetimeProps);

		DOREPLIFETIME(ABreakTrainTimeTable, Table)
	}
	
	UPROPERTY(Replicated)
	AFGRailroadTimeTable* Table;
};
