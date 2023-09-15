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
	virtual void ServerProcess_Implementation(double DeltaTime) override
	{
		Out.X = GetConnection(0).GetFloat();
		Out.Y = GetConnection(1).GetFloat();
		Out.Z = GetConnection(2).GetFloat();
	}


	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override
	{
		Super::GetLifetimeReplicatedProps(OutLifetimeProps);

		DOREPLIFETIME(AMakeVector, Out)
	}

	UPROPERTY(Replicated, SaveGame)
	FVector Out;
};
