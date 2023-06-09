// 

#pragma once

#include "CoreMinimal.h"
#include "Behaviour/FGWiremodBuildable.h"
#include "VectorNumberDivide.generated.h"

UCLASS()
class FICSITWIREMOD_API AVectorNumberDivide : public AFGWiremodBuildable
{
	GENERATED_BODY()

public:
	virtual void Process_Implementation(float DeltaTime) override
	{
		Out = GetConnection(0).GetVector() / GetConnection(1).GetFloat();
	}

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override
	{
		Super::GetLifetimeReplicatedProps(OutLifetimeProps);

		DOREPLIFETIME(AVectorNumberDivide, Out)
	}

	UPROPERTY(Replicated, SaveGame)
	FVector Out;
};
