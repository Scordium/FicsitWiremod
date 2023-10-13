// 

#pragma once

#include "CoreMinimal.h"
#include "Behaviour/MultistateWiremodBuildable.h"
#include "ExpoInterpolation.generated.h"

UCLASS()
class FICSITWIREMOD_API AExpoInterpolation : public AMultistateWiremodBuildable
{
	GENERATED_BODY()

public:
	virtual void ServerProcess_Implementation(double DeltaTime) override
	{
		auto A = GetConnection(0).GetFloat();
		auto B = GetConnection(1).GetFloat();
		auto Alpha = GetConnection(2).GetFloat();
		
		//0 - Expo IN
		if(CurrentStateIndex == 0) Out = FMath::InterpExpoIn(A, B, Alpha);
		//1 - Expo IN OUT
		else if(CurrentStateIndex == 1) Out = FMath::InterpExpoInOut(A, B, Alpha);
		//2 - Expo OUT
		else if(CurrentStateIndex == 2) Out = FMath::InterpExpoOut(A, B, Alpha);
	}
	
	virtual void GetLifetimeReplicatedProps( TArray<FLifetimeProperty>& OutLifetimeProps ) const override
	{
		Super::GetLifetimeReplicatedProps(OutLifetimeProps);
		DOREPLIFETIME(AExpoInterpolation, Out);
	}

	UPROPERTY(Replicated, SaveGame, VisibleInstanceOnly)
	double Out;
};
