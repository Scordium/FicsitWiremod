// 

#pragma once

#include "CoreMinimal.h"
#include "MultistateWiremodBuildable.h"
#include "Behaviour/FGWiremodBuildable.h"
#include "NumberFractional.generated.h"

UCLASS()
class FICSITWIREMOD_API ANumberFractional : public AMultistateWiremodBuildable
{
	GENERATED_BODY()

public:
	virtual void ServerProcess_Implementation(double DeltaTime) override
	{
		if (CurrentStateIndex == 0) Out = FMath::Fractional(GetConnection(0).GetFloat());
		else if (CurrentStateIndex == 1)
		{
			ArrayOut = GetConnection(0).GetFloatArray();

			for (int i = 0; i < ArrayOut.Num(); i++)
			{
				ArrayOut[i] = FMath::Fractional(ArrayOut[i]);
			}
		}
	}

	virtual void OnStateSelected_Internal(int Index) override
	{
		Super::OnStateSelected_Internal(Index);

		Out = 0;
		ArrayOut.Empty();
	}

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override
	{
		Super::GetLifetimeReplicatedProps(OutLifetimeProps);

		DOREPLIFETIME(ANumberFractional, Out)
		DOREPLIFETIME(ANumberFractional, ArrayOut)
	}

	UPROPERTY(SaveGame, Replicated)
	double Out;

	UPROPERTY(SaveGame, Replicated)
	TArray<double> ArrayOut;
};
