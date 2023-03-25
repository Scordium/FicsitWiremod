// 

#pragma once

#include "CoreMinimal.h"
#include "Behaviour/MultistateWiremodBuildable.h"
#include "EaseInterpolation.generated.h"

UCLASS()
class FICSITWIREMOD_API AEaseInterpolation : public AMultistateWiremodBuildable
{
	GENERATED_BODY()

public:
	virtual void Process_Implementation(float DeltaTime) override
	{
		float A = WM_GetFloat(0);
		float B = WM_GetFloat(1);
		float Alpha = WM_GetFloat(2);
		float Exp = WM_GetFloat(3);
		
		//0 - Ease IN
		if(CurrentStateIndex == 0) Out = FMath::InterpEaseIn(A, B, Alpha, Exp);
		//1 - Ease IN OUT
		else if(CurrentStateIndex == 1) Out = FMath::InterpEaseInOut(A, B, Alpha, Exp);
		//2 - Ease OUT
		else if(CurrentStateIndex == 2) Out = FMath::InterpEaseOut(A, B, Alpha, Exp);
	}
	
	virtual void GetLifetimeReplicatedProps( TArray<FLifetimeProperty>& OutLifetimeProps ) const override
	{
		Super::GetLifetimeReplicatedProps(OutLifetimeProps);
		DOREPLIFETIME(AEaseInterpolation, Out);
	}

	UPROPERTY(Replicated, VisibleInstanceOnly)
	float Out;
};
