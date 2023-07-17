// 

#pragma once

#include "CoreMinimal.h"
#include "Behaviour/FGWiremodBuildable.h"
#include "VectorDistance.generated.h"

UCLASS()
class FICSITWIREMOD_API AVectorDistance : public AFGWiremodBuildable
{
	GENERATED_BODY()

public:
	virtual void Process_Implementation(double DeltaTime) override
	{
		Out = FVector::Distance(GetConnection(0).GetVector(), GetConnection(1).GetVector());
	}

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override
	{
		Super::GetLifetimeReplicatedProps(OutLifetimeProps);

		DOREPLIFETIME(AVectorDistance, Out)
	}

	UPROPERTY(Replicated, SaveGame)
	double Out;
};
