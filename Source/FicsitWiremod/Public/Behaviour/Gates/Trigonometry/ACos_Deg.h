// 

#pragma once

#include "CoreMinimal.h"
#include "Behaviour/FGWiremodBuildable.h"
#include "ACos_Deg.generated.h"

UCLASS()
class FICSITWIREMOD_API AACos_Deg : public AFGWiremodBuildable
{
	GENERATED_BODY()

public:
	virtual void ServerProcess_Implementation(double DeltaTime) override
	{
		Out = UKismetMathLibrary::DegAcos(GetConnection(0).GetFloat());
	}

	virtual void GetLifetimeReplicatedProps( TArray<FLifetimeProperty>& OutLifetimeProps ) const override
	{
		Super::GetLifetimeReplicatedProps(OutLifetimeProps);

		DOREPLIFETIME(AACos_Deg, Out);
	}


	UPROPERTY(Replicated, VisibleInstanceOnly)
	double Out;
};
