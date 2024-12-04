// 

#pragma once

#include "CoreMinimal.h"
#include "Behaviour/FGWiremodBuildable.h"
#include "Behaviour/MultistateWiremodBuildable.h"
#include "NumberSubtract.generated.h"

UCLASS()
class FICSITWIREMOD_API ANumberSubtract : public AMultistateWiremodBuildable
{
	GENERATED_BODY()

public:
	virtual void ServerProcess_Implementation(double DeltaTime) override
	{
		switch (CurrentStateIndex)
		{
			//Default mode - subtract B from A
		case 0:
			{
				Out = GetConnection(0).GetFloat() - GetConnection(1).GetFloat();
				break;
			}
			//Array mode - subtract from first to last element
		case 1:
			{
				auto Array = GetConnection(0).GetFloatArray();
				double Output = 0;

				for(auto Element : Array) Output -= Element;
				Out = Output;
				break;
			}
			//Array single subtract mode - Subtract a value from each element in the array
		case 2:
			{
				ArrayOut = GetConnection(0).GetFloatArray();
				const double Value = GetConnection(1).GetFloat();;

				for(int i = 0; i < ArrayOut.Num(); i++) ArrayOut[i] -= Value;
				break;
			}

			//Array add mode - Subtract individual elements from each other in two arrays (Out[I] = A[I] - B[I])
		case 3:
			{
				ArrayOut.Empty();
				const auto ArrayA = GetConnection(0).GetFloatArray();
				const auto ArrayB = GetConnection(1).GetFloatArray();

				for (int i = 0; i < FMath::Max(ArrayA.Num(), ArrayB.Num()); i++)
				{
					if (ArrayA.IsValidIndex(i))
					{
						const auto BValue = ArrayB.IsValidIndex(i) ? ArrayB[i] : 0;

						ArrayOut.Add(ArrayA[i] - BValue);
					}
					else ArrayOut.Add(ArrayB[i]);
				}

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

		DOREPLIFETIME(ANumberSubtract, Out);
	}
	
	UPROPERTY(Replicated, SaveGame, VisibleInstanceOnly)
	double Out;

	UPROPERTY(Replicated, SaveGame)
	TArray<double> ArrayOut;
};
