// 

#pragma once

#include "CoreMinimal.h"
#include "Behaviour/FGWiremodBuildable.h"
#include "LerpNumber.generated.h"

UCLASS()
class FICSITWIREMOD_API ALerpNumber : public AFGWiremodBuildable
{
	GENERATED_BODY()

public:
	virtual void Process_Implementation(double DeltaTime) override
	{
		auto A = GetConnection(0).GetFloat();
		auto B = GetConnection(1).GetFloat();
		auto Alpha = GetConnection(2).GetFloat();

		Out = A + Alpha*(B-A);
	}


	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override
	{
		Super::GetLifetimeReplicatedProps(OutLifetimeProps);

		DOREPLIFETIME(ALerpNumber, Out)
	}

	UPROPERTY(Replicated, SaveGame)
	double Out;
};
