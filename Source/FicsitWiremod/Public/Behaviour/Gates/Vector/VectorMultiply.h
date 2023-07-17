// 

#pragma once

#include "CoreMinimal.h"
#include "Behaviour/FGWiremodBuildable.h"
#include "VectorMultiply.generated.h"

UCLASS()
class FICSITWIREMOD_API AVectorMultiply : public AFGWiremodBuildable
{
	GENERATED_BODY()

public:
	virtual void Process_Implementation(double DeltaTime) override
	{
		Out = GetConnection(0).GetVector() * GetConnection(1).GetVector();
	}

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override
	{
		Super::GetLifetimeReplicatedProps(OutLifetimeProps);

		DOREPLIFETIME(AVectorMultiply, Out)
	}

	UPROPERTY(Replicated, SaveGame)
	FVector Out;
};
