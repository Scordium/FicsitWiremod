// 

#pragma once

#include "CoreMinimal.h"
#include "Behaviour/FGWiremodBuildable.h"
#include "NumberExponent.generated.h"

UCLASS()
class FICSITWIREMOD_API ANumberExponent : public AFGWiremodBuildable
{
	GENERATED_BODY()

public:
	virtual void ServerProcess_Implementation(double DeltaTime) override
	{
		Out = FMath::Exp(GetConnection(0).GetFloat());
	}

	virtual void GetLifetimeReplicatedProps( TArray<FLifetimeProperty>& OutLifetimeProps ) const override
	{
		Super::GetLifetimeReplicatedProps(OutLifetimeProps);

		DOREPLIFETIME(ANumberExponent, Out);
	}


	UPROPERTY(Replicated, VisibleInstanceOnly)
	double Out;
};
