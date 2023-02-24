// 

#pragma once

#include "CoreMinimal.h"
#include "Behaviour/FGWiremodBuildable.h"
#include "NOT.generated.h"

UCLASS()
class FICSITWIREMOD_API ANOT : public AFGWiremodBuildable
{
	GENERATED_BODY()

public:

	virtual void Process_Implementation(float DeltaTime) override
	{
		Out = !WM::GetFunctionBoolResult(GetConnection(0));
	}

	virtual void GetLifetimeReplicatedProps( TArray<FLifetimeProperty>& OutLifetimeProps ) const override
	{
		Super::GetLifetimeReplicatedProps(OutLifetimeProps);

		DOREPLIFETIME(ANOT, Out);
	}

	UPROPERTY(Replicated, VisibleInstanceOnly)
	bool Out;
};
