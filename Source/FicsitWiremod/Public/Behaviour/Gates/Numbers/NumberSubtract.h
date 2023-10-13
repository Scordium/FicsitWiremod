// 

#pragma once

#include "CoreMinimal.h"
#include "Behaviour/FGWiremodBuildable.h"
#include "NumberSubtract.generated.h"

UCLASS()
class FICSITWIREMOD_API ANumberSubtract : public AFGWiremodBuildable
{
	GENERATED_BODY()

public:
	virtual void ServerProcess_Implementation(double DeltaTime) override
	{
		Out = GetConnection(0).GetFloat() - GetConnection(1).GetFloat();
	}

	virtual void GetLifetimeReplicatedProps( TArray<FLifetimeProperty>& OutLifetimeProps ) const override
	{
		Super::GetLifetimeReplicatedProps(OutLifetimeProps);

		DOREPLIFETIME(ANumberSubtract, Out);
	}
	
	UPROPERTY(Replicated, SaveGame, VisibleInstanceOnly)
	double Out;
};
