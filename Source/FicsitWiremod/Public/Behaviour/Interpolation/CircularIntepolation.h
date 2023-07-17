// 

#pragma once

#include "CoreMinimal.h"
#include "Behaviour/MultistateWiremodBuildable.h"
#include "CircularIntepolation.generated.h"

UCLASS()
class FICSITWIREMOD_API ACircularIntepolation : public AMultistateWiremodBuildable
{
	GENERATED_BODY()

public:
	virtual void Process_Implementation(double DeltaTime) override
	{
		auto A = GetConnection(0).GetFloat();
		auto B = GetConnection(1).GetFloat();
		auto Alpha = GetConnection(2).GetFloat();
		
		//0 - Circular IN
		if(CurrentStateIndex == 0) Out = FMath::InterpCircularIn(A, B, Alpha);
		//1 - Circular IN OUT
		else if(CurrentStateIndex == 1) Out = FMath::InterpCircularInOut(A, B, Alpha);
		//2 - Circular OUT
		else if(CurrentStateIndex == 2) Out = FMath::InterpCircularOut(A, B, Alpha);
	}
	
	virtual void GetLifetimeReplicatedProps( TArray<FLifetimeProperty>& OutLifetimeProps ) const override
	{
		Super::GetLifetimeReplicatedProps(OutLifetimeProps);
		DOREPLIFETIME(ACircularIntepolation, Out);
	}

	UPROPERTY(Replicated, VisibleInstanceOnly)
	double Out;
};
