// 

#pragma once

#include "CoreMinimal.h"
#include "Behaviour/FGWiremodBuildable.h"
#include "RemapNumber.generated.h"

UCLASS()
class FICSITWIREMOD_API ARemapNumber : public AFGWiremodBuildable
{
	GENERATED_BODY()

public:
	virtual void ServerProcess_Implementation(double DeltaTime) override
	{
		double Value = GetConnection(0).GetFloat();;
		double InMin = GetConnection(1).GetFloat();;
		double InMax = GetConnection(2).GetFloat();;
		double OutMin = GetConnection(3).GetFloat();;
		double OutMax = GetConnection(4).GetFloat();;

		Out = FMath::GetMappedRangeValueUnclamped(
			FVector2D(InMin, InMax),
			FVector2D(OutMin, OutMax),
			Value
			);
	}

	virtual void GetLifetimeReplicatedProps( TArray<FLifetimeProperty>& OutLifetimeProps ) const override
	{
		Super::GetLifetimeReplicatedProps(OutLifetimeProps);

		DOREPLIFETIME(ARemapNumber, Out);
	}
	
	UPROPERTY(Replicated, VisibleInstanceOnly)
	double Out;
};
