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
		float Value = WM_GetFloat(0);
		float InMin = WM_GetFloat(1);
		float InMax = WM_GetFloat(2);
		float OutMin = WM_GetFloat(3);
		float OutMax = WM_GetFloat(4);

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
