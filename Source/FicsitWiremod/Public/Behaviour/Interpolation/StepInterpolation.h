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
	virtual void ServerProcess_Implementation(double DeltaTime) override
	{
		auto A = GetConnection(0).GetFloat();
		auto B = GetConnection(1).GetFloat();
		auto Alpha = GetConnection(2).GetFloat();
		int Steps = GetConnection(3).GetFloat();
		
		Out = FMath::InterpStep(A, B, Alpha, Steps);
	}
	
	virtual void GetLifetimeReplicatedProps( TArray<FLifetimeProperty>& OutLifetimeProps ) const override
	{
		Super::GetLifetimeReplicatedProps(OutLifetimeProps);
		DOREPLIFETIME(AStepInterpolation, Out);
	}

	UPROPERTY(Replicated, SaveGame, VisibleInstanceOnly)
	double Out;
};
