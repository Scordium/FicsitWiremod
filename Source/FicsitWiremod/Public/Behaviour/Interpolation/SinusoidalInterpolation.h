// 

#pragma once

#include "CoreMinimal.h"
#include "Behaviour/MultistateWiremodBuildable.h"
#include "SinusoidalInterpolation.generated.h"

UCLASS()
class FICSITWIREMOD_API ASinusoidalInterpolation : public AMultistateWiremodBuildable
{
	GENERATED_BODY()

public:
	virtual void ServerProcess_Implementation(double DeltaTime) override
	{
		auto A = GetConnection(0).GetFloat();;
		auto B = GetConnection(1).GetFloat();;
		auto Alpha = GetConnection(2).GetFloat();;
		
		//0 - Sin IN
		if(CurrentStateIndex == 0) Out = FMath::InterpSinIn(A, B, Alpha);
		//1 - Sin IN OUT
		else if(CurrentStateIndex == 1) Out = FMath::InterpSinInOut(A, B, Alpha);
		//2 - Sin OUT
		else if(CurrentStateIndex == 2) Out = FMath::InterpSinOut(A, B, Alpha);
	}
	
	virtual void GetLifetimeReplicatedProps( TArray<FLifetimeProperty>& OutLifetimeProps ) const override
	{
		Super::GetLifetimeReplicatedProps(OutLifetimeProps);
		DOREPLIFETIME(ASinusoidalInterpolation, Out);
	}

	UPROPERTY(Replicated, SaveGame, VisibleInstanceOnly)
	double Out;
};
