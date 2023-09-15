// 

#pragma once

#include "CoreMinimal.h"
#include "Behaviour/FGWiremodBuildable.h"
#include "BoolToNumber.generated.h"

UCLASS()
class FICSITWIREMOD_API ABoolToNumber : public AFGWiremodBuildable
{
	GENERATED_BODY()

public:
	virtual void ServerProcess_Implementation(double DeltaTime) override
	{
		Out = GetConnection(0).GetBool();
	}

	virtual void GetLifetimeReplicatedProps( TArray<FLifetimeProperty>& OutLifetimeProps ) const override
	{
		Super::GetLifetimeReplicatedProps(OutLifetimeProps);

		DOREPLIFETIME(ABoolToNumber, Out);
	}


	UPROPERTY(Replicated, VisibleInstanceOnly)
	int Out;
};
