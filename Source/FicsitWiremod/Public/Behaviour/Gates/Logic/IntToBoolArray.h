// 

#pragma once

#include "CoreMinimal.h"
#include "Behaviour/FGWiremodBuildable.h"
#include "IntToBoolArray.generated.h"

UCLASS()
class FICSITWIREMOD_API AIntToBoolArray : public AFGWiremodBuildable
{
	GENERATED_BODY()

public:
	AIntToBoolArray() : Super()
	{
		Out.SetNum(32);
	}
	
	virtual void ServerProcess_Implementation(double DeltaTime) override
	{
		int Value = GetConnection(0).GetFloat();

		for(int i = 31; i >= 0; i--) Out[i] = (Value >> i) & 0x1;
	}

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override
	{
		Super::GetLifetimeReplicatedProps(OutLifetimeProps);

		DOREPLIFETIME(AIntToBoolArray, Out)
	}

	UPROPERTY(Replicated, SaveGame)
	TArray<bool> Out;
};
