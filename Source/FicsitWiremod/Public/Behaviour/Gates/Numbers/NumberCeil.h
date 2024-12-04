// 

#pragma once

#include "CoreMinimal.h"
#include "MultistateWiremodBuildable.h"
#include "Behaviour/FGWiremodBuildable.h"
#include "NumberCeil.generated.h"

UCLASS()
class FICSITWIREMOD_API ANumberCeil : public AMultistateWiremodBuildable
{
	GENERATED_BODY()

public:
	virtual void ServerProcess_Implementation(double DeltaTime) override
	{
		if (CurrentStateIndex == 0) Out = FMath::CeilToInt(GetConnection(0).GetFloat());
		else if (CurrentStateIndex == 1)
		{
			ArrayOut = GetConnection(0).GetFloatArray();

			for (int i = 0; i < ArrayOut.Num(); i++)
			{
				ArrayOut[i] = FMath::CeilToDouble(ArrayOut[i]);
			}
		}
	}

	virtual void OnStateSelected_Internal(int Index) override
	{
		Super::OnStateSelected_Internal(Index);

		Out = 0;
		ArrayOut.Empty();
	}

	virtual void GetLifetimeReplicatedProps( TArray<FLifetimeProperty>& OutLifetimeProps ) const override
	{
		Super::GetLifetimeReplicatedProps(OutLifetimeProps);

		DOREPLIFETIME(ANumberCeil, Out);
		DOREPLIFETIME(ANumberCeil, ArrayOut);
	}
	
	UPROPERTY(Replicated, SaveGame)
	int Out;

	UPROPERTY(Replicated, SaveGame)
	TArray<double> ArrayOut;
};
