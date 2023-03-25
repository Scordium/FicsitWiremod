// 

#pragma once

#include "CoreMinimal.h"
#include "Behaviour/FGWiremodBuildable.h"
#include "StepInterpolation.generated.h"

UCLASS()
class FICSITWIREMOD_API AStepInterpolation : public AFGWiremodBuildable
{
	GENERATED_BODY()

public:
	virtual void Process_Implementation(float DeltaTime) override
	{
		float A = WM_GetFloat(0);
		float B = WM_GetFloat(1);
		float Alpha = WM_GetFloat(2);
		int Steps = WM_GetInt(3);
		
		Out = FMath::InterpStep(A, B, Alpha, Steps);
	}
	
	virtual void GetLifetimeReplicatedProps( TArray<FLifetimeProperty>& OutLifetimeProps ) const override
	{
		Super::GetLifetimeReplicatedProps(OutLifetimeProps);
		DOREPLIFETIME(AStepInterpolation, Out);
	}

	UPROPERTY(Replicated, VisibleInstanceOnly)
	float Out;
};
