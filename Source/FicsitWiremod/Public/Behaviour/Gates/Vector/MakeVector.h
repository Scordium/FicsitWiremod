// 

#pragma once

#include "CoreMinimal.h"
#include "Behaviour/FGWiremodBuildable.h"
#include "MakeVector.generated.h"

UCLASS()
class FICSITWIREMOD_API AMakeVector : public AFGWiremodBuildable
{
	GENERATED_BODY()

public:
	virtual void Process_Implementation(float DeltaTime) override
	{
		auto X = GetConnection(0).GetFloat();
		auto Y = GetConnection(1).GetFloat();
		auto Z = GetConnection(2).GetFloat();

		Out = FVector(X, Y, Z);
	}


	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override
	{
		Super::GetLifetimeReplicatedProps(OutLifetimeProps);

		DOREPLIFETIME(AMakeVector, Out)
	}

	UPROPERTY(Replicated, SaveGame)
	FVector Out;
};
