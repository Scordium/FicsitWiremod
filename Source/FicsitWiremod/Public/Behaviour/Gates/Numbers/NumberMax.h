// 

#pragma once

#include "CoreMinimal.h"
#include "Behaviour/FGWiremodBuildable.h"
#include "Behaviour/MultistateWiremodBuildable.h"
#include "NumberMax.generated.h"

UCLASS()
class FICSITWIREMOD_API ANumberMax : public AMultistateWiremodBuildable
{
	GENERATED_BODY()

public:
	virtual void ServerProcess_Implementation(double DeltaTime) override
	{
		//0 - Default mode, pick one out of two numbers
		if(CurrentStateIndex == 0) Out = FMath::Max(GetConnection(0).GetFloat(), GetConnection(1).GetFloat());
		//1 - Array mode, pick one value in array
		else if (CurrentStateIndex == 1) Out = FMath::Max(GetConnection(0).GetFloatArray());
	}

	virtual void GetLifetimeReplicatedProps( TArray<FLifetimeProperty>& OutLifetimeProps ) const override
	{
		Super::GetLifetimeReplicatedProps(OutLifetimeProps);

		DOREPLIFETIME(ANumberMax, Out);
	}


	UPROPERTY(Replicated, SaveGame, VisibleInstanceOnly)
	double Out;
};
