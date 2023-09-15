// 

#pragma once

#include "CoreMinimal.h"
#include "Behaviour/FGWiremodBuildable.h"
#include "VectorGridSnap.generated.h"

UCLASS()
class FICSITWIREMOD_API AVectorGridSnap : public AFGWiremodBuildable
{
	GENERATED_BODY()

public:
	virtual void ServerProcess_Implementation(double DeltaTime) override
	{
		auto Value = GetConnection(0).GetVector();
		auto GridSize = GetConnection(1).GetFloat();

		Out = Value.GridSnap(GridSize);
	}

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override
	{
		Super::GetLifetimeReplicatedProps(OutLifetimeProps);

		DOREPLIFETIME(AVectorGridSnap, Out)
	}

	UPROPERTY(SaveGame, Replicated)
	FVector Out;
};
