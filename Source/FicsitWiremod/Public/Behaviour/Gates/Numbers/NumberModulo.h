// 

#pragma once

#include "CoreMinimal.h"
#include "Behaviour/FGWiremodBuildable.h"
#include "NumberModulo.generated.h"

UCLASS()
class FICSITWIREMOD_API ANumberModulo : public AFGWiremodBuildable
{
	GENERATED_BODY()

public:
	virtual void Process_Implementation(double DeltaTime) override
	{
		Out = FMath::Fmod(GetConnection(0).GetFloat(), GetConnection(1).GetFloat());
	}

	virtual void GetLifetimeReplicatedProps( TArray<FLifetimeProperty>& OutLifetimeProps ) const override
	{
		Super::GetLifetimeReplicatedProps(OutLifetimeProps);

		DOREPLIFETIME(ANumberModulo, Out);
	}


	UPROPERTY(Replicated, VisibleInstanceOnly)
	double Out;
};
