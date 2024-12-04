// 

#pragma once

#include "CoreMinimal.h"
#include "Behaviour/FGWiremodBuildable.h"
#include "Behaviour/MultistateWiremodBuildable.h"
#include "NumberMax.generated.h"

UCLASS()
class FICSITWIREMOD_API ANumberMax : public AMultistateWiremodBuildable
{
	GENERATED_BODY()

public:
	virtual void ServerProcess_Implementation(double DeltaTime) override
	{
		//Default mode, pick one out of two numbers
		if(CurrentStateIndex == 0) Out = FMath::Max(GetConnection(0).GetFloat(), GetConnection(1).GetFloat());
		//Array single mode, pick one value in array
		else if (CurrentStateIndex == 1) Out = FMath::Max(GetConnection(0).GetFloatArray());
		//Array mode - pick values between two arrays
		else if (CurrentStateIndex == 2)
		{
			const auto ArrayA = GetConnection(0).GetFloatArray();
			const auto ArrayB = GetConnection(1).GetFloatArray();
			ArrayOut.Empty();

			for (int i = 0; i < FMath::Max(ArrayA.Num(), ArrayB.Num()); i++)
			{
				if (ArrayA.IsValidIndex(i))
				{
					if (ArrayB.IsValidIndex(i)) ArrayOut.Add(FMath::Max(ArrayA[i], ArrayB[i]));
					else ArrayOut.Add(ArrayA[i]);
				}
				else ArrayOut.Add(ArrayB[i]);
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

		DOREPLIFETIME(ANumberMax, Out);
		DOREPLIFETIME(ANumberMax, ArrayOut);
	}


	UPROPERTY(Replicated, SaveGame)
	double Out;

	UPROPERTY(Replicated, SaveGame)
	TArray<double> ArrayOut;
};
