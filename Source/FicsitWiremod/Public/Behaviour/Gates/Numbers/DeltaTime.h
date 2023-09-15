// 

#pragma once

#include "CoreMinimal.h"
#include "Behaviour/FGWiremodBuildable.h"
#include "DeltaTime.generated.h"

UCLASS()
class FICSITWIREMOD_API ADeltaTime : public AFGWiremodBuildable
{
	GENERATED_BODY()

public:
	virtual void ServerProcess_Implementation(double DeltaTime) override { Out = DeltaTime; }

	virtual void GetLifetimeReplicatedProps( TArray<FLifetimeProperty>& OutLifetimeProps ) const override
	{
		Super::GetLifetimeReplicatedProps(OutLifetimeProps);

		DOREPLIFETIME(ADeltaTime, Out);
	}


	UPROPERTY(Replicated, VisibleInstanceOnly)
	double Out;
};
