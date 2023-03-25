// 

#pragma once

#include "CoreMinimal.h"
#include "KismetAnimationLibrary.h"
#include "Behaviour/FGWiremodBuildable.h"
#include "PerlinNoise1D.generated.h"

UCLASS()
class FICSITWIREMOD_API APerlinNoise1D : public AFGWiremodBuildable
{
	GENERATED_BODY()

public:
	virtual void Process_Implementation(float DeltaTime) override
	{
		float Value = WM_GetFloat(0);
		float Min = WM_GetFloat(1);
		float Max = WM_GetFloat(2);
		
		Out = UKismetAnimationLibrary::K2_MakePerlinNoiseAndRemap(Value, Min, Max);
	}

	virtual void GetLifetimeReplicatedProps( TArray<FLifetimeProperty>& OutLifetimeProps ) const override
	{
		Super::GetLifetimeReplicatedProps(OutLifetimeProps);
		DOREPLIFETIME(APerlinNoise1D, Out);
	}

	UPROPERTY(VisibleInstanceOnly, Replicated)
	float Out;
};
