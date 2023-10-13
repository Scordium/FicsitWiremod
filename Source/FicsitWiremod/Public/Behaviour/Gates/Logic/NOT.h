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

	virtual void ServerProcess_Implementation(double DeltaTime) override
	{
		Out = !GetConnection(0).GetBool();
	}

	virtual void GetLifetimeReplicatedProps( TArray<FLifetimeProperty>& OutLifetimeProps ) const override
	{
		Super::GetLifetimeReplicatedProps(OutLifetimeProps);

		DOREPLIFETIME(ANOT, Out);
	}

	UPROPERTY(Replicated, SaveGame, VisibleInstanceOnly)
	bool Out;
};
