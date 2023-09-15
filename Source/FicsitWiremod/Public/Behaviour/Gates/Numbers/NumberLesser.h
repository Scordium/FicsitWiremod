// 

#pragma once

#include "CoreMinimal.h"
#include "Behaviour/FGWiremodBuildable.h"
#include "NumberLesser.generated.h"

UCLASS()
class FICSITWIREMOD_API ANumberLesser : public AFGWiremodBuildable
{
	GENERATED_BODY()

public:
	virtual void ServerProcess_Implementation(double DeltaTime) override
	{
		Out = GetConnection(0).GetFloat() < GetConnection(1).GetFloat();
	}

	virtual void GetLifetimeReplicatedProps( TArray<FLifetimeProperty>& OutLifetimeProps ) const override
	{
		Super::GetLifetimeReplicatedProps(OutLifetimeProps);

		DOREPLIFETIME(ANumberLesser, Out);
	}


	UPROPERTY(Replicated, VisibleInstanceOnly)
	bool Out;
};
