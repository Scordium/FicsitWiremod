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
		float Value = GetConnection(0).GetFloat();;
		float Min = GetConnection(1).GetFloat();;
		float Max = GetConnection(2).GetFloat();;
		
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
