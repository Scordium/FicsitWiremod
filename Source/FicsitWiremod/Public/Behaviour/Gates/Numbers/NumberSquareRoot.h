// 

#pragma once

#include "CoreMinimal.h"
#include "Behaviour/FGWiremodBuildable.h"
#include "NumberSquareRoot.generated.h"

UCLASS()
class FICSITWIREMOD_API ANumberSquareRoot : public AFGWiremodBuildable
{
	GENERATED_BODY()

public:
	virtual void ServerProcess_Implementation(double DeltaTime) override
	{
		Out = FMath::Sqrt(GetConnection(0).GetFloat());
	}

	virtual void GetLifetimeReplicatedProps( TArray<FLifetimeProperty>& OutLifetimeProps ) const override
	{
		Super::GetLifetimeReplicatedProps(OutLifetimeProps);

		DOREPLIFETIME(ANumberSquareRoot, Out);
	}
	
	UPROPERTY(Replicated, SaveGame, VisibleInstanceOnly)
	double Out;
};
