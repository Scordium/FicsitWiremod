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
	virtual void Process_Implementation(float DeltaTime) override
	{
		float Value = GetConnection(0).GetFloat();;
		float InMin = GetConnection(1).GetFloat();;
		float InMax = GetConnection(2).GetFloat();;
		float OutMin = GetConnection(3).GetFloat();;
		float OutMax = GetConnection(4).GetFloat();;

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
	float Out;
};
