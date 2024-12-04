// 

#pragma once

#include "CoreMinimal.h"
#include "MultistateWiremodBuildable.h"
#include "Behaviour/FGWiremodBuildable.h"
#include "AbsoluteNumber.generated.h"

UCLASS()
class FICSITWIREMOD_API AAbsoluteNumber : public AMultistateWiremodBuildable
{
	GENERATED_BODY()

public:
	virtual void ServerProcess_Implementation(double DeltaTime) override
	{
		switch (CurrentStateIndex)
		{
			//Default mode - return absolute of float
		case 0: Out = FMath::Abs(GetConnection(0).GetFloat()); break;
			//Array mode - return array where all elements are absolute
		case 1:
			{
				ArrayOut = GetConnection(0).GetFloatArray();

				for (int i = 0; i < ArrayOut.Num(); i++)
					ArrayOut[i] = FMath::Abs(ArrayOut[i]);

				break;
			}

		default: break;
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

		DOREPLIFETIME(AAbsoluteNumber, Out);
		DOREPLIFETIME(AAbsoluteNumber, ArrayOut);
	}
	
	UPROPERTY(Replicated, SaveGame)
	double Out;

	UPROPERTY(Replicated, SaveGame)
	TArray<double> ArrayOut;
};
