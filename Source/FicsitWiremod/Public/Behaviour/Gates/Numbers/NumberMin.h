// 

#pragma once

#include "CoreMinimal.h"
#include "Behaviour/FGWiremodBuildable.h"
#include "NumberMin.generated.h"

UCLASS()
class FICSITWIREMOD_API ANumberMin : public AFGWiremodBuildable
{
	GENERATED_BODY()

public:
	virtual void ServerProcess_Implementation(double DeltaTime) override
	{
		Out = FMath::Min(GetConnection(0).GetFloat(), GetConnection(1).GetFloat());
	}

	virtual void GetLifetimeReplicatedProps( TArray<FLifetimeProperty>& OutLifetimeProps ) const override
	{
		Super::GetLifetimeReplicatedProps(OutLifetimeProps);

		DOREPLIFETIME(ANumberMin, Out);
	}


	UPROPERTY(Replicated, SaveGame, VisibleInstanceOnly)
	double Out;
};
