// 

#pragma once

#include "CoreMinimal.h"
#include "Behaviour/FGWiremodBuildable.h"
#include "BoolArrayToInt.generated.h"

UCLASS()
class FICSITWIREMOD_API ABoolArrayToInt : public AFGWiremodBuildable
{
	GENERATED_BODY()

public:
	virtual void ServerProcess_Implementation(double DeltaTime) override
	{
		auto Array = GetConnection(0).GetBoolArray();

		Out = 0;
		for(int i = Array.Num() - 1; i >= 0; i--) Out |= Array[i] << i;
	}

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override
	{
		Super::GetLifetimeReplicatedProps(OutLifetimeProps);

		DOREPLIFETIME(ABoolArrayToInt, Out)
	}


	UPROPERTY(Replicated, SaveGame)
	int Out;
};
