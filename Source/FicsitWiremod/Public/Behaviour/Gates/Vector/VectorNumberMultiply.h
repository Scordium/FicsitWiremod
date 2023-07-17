// 

#pragma once

#include "CoreMinimal.h"
#include "Behaviour/FGWiremodBuildable.h"
#include "VectorNumberMultiply.generated.h"

UCLASS()
class FICSITWIREMOD_API AVectorNumberMultiply : public AFGWiremodBuildable
{
	GENERATED_BODY()

public:
	virtual void Process_Implementation(double DeltaTime) override
	{
		Out = GetConnection(0).GetVector() * GetConnection(1).GetFloat();
	}

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override
	{
		Super::GetLifetimeReplicatedProps(OutLifetimeProps);

		DOREPLIFETIME(AVectorNumberMultiply, Out)
	}

	UPROPERTY(Replicated, SaveGame)
	FVector Out;
};
