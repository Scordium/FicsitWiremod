// 

#pragma once

#include "CoreMinimal.h"
#include "Behaviour/FGWiremodBuildable.h"
#include "Behaviour/MultistateWiremodBuildable.h"
#include "NumberMin.generated.h"

UCLASS()
class FICSITWIREMOD_API ANumberMin : public AMultistateWiremodBuildable
{
	GENERATED_BODY()

public:
	virtual void ServerProcess_Implementation(double DeltaTime) override
	{
		//Default mode, pick one of two numbers
		if(CurrentStateIndex == 0) Out = FMath::Min(GetConnection(0).GetFloat(), GetConnection(1).GetFloat());
		//Array single mode, pick one value in array
		else if (CurrentStateIndex == 1) Out = FMath::Min(GetConnection(0).GetFloatArray());
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
					if (ArrayB.IsValidIndex(i)) ArrayOut.Add(FMath::Min(ArrayA[i], ArrayB[i]));
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

		DOREPLIFETIME(ANumberMin, Out);
		DOREPLIFETIME(ANumberMin, ArrayOut);
	}


	UPROPERTY(Replicated, SaveGame)
	double Out;

	UPROPERTY(Replicated, SaveGame)
	TArray<double> ArrayOut;
};
