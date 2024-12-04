// 

#pragma once

#include "CoreMinimal.h"
#include "Behaviour/FGWiremodBuildable.h"
#include "Behaviour/MultistateWiremodBuildable.h"
#include "AddNumbers.generated.h"

UCLASS()
class FICSITWIREMOD_API AAddNumbers : public AMultistateWiremodBuildable
{
	GENERATED_BODY()

public:
	virtual void ServerProcess_Implementation(double DeltaTime) override
	{
		switch (CurrentStateIndex)
		{
			//Default mode - sum of all connected inputs
		case 0:
			{
				TArray<FConnectionData> Connected;
				GetAllConnected(Connected);

				double Output = 0;
				for (auto Data : Connected)
				{
					Output += Data.GetFloat();
				}

				Out = Output;
				break;
			}
			//Array mode - sum of all array elements
		case 1:
			{
				auto Array = GetConnection(0).GetFloatArray();
				double Output = 0;

				for(auto Element : Array) Output += Element;
				Out = Output;
				break;
			}
			//Array single add mode - Add a value to each element in the array
		case 2:
			{
				Out_Array = GetConnection(0).GetFloatArray();
				const double Value = GetConnection(1).GetFloat();;

				for(int i = 0; i < Out_Array.Num(); i++) Out_Array[i] += Value;
				break;
			}

			//Array add mode - Return sum of individual elements in two arrays (Out[I] = A[I] + B[I])
		case 3:
			{
				Out_Array.Empty();
				const auto ArrayA = GetConnection(0).GetFloatArray();
				const auto ArrayB = GetConnection(1).GetFloatArray();

				for (int i = 0; i < FMath::Max(ArrayA.Num(), ArrayB.Num()); i++)
				{
					if (ArrayA.IsValidIndex(i))
					{
						const auto BValue = ArrayB.IsValidIndex(i) ? ArrayB[i] : 0;

						Out_Array.Add(ArrayA[i] + BValue);
					}
					else Out_Array.Add(ArrayB[i]);
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
		Out_Array.Empty();
	}

	virtual void GetLifetimeReplicatedProps( TArray<FLifetimeProperty>& OutLifetimeProps ) const override
	{
		Super::GetLifetimeReplicatedProps(OutLifetimeProps);

		DOREPLIFETIME(AAddNumbers, Out);
		DOREPLIFETIME(AAddNumbers, Out_Array);
	}


	UPROPERTY(Replicated, SaveGame, VisibleInstanceOnly)
	double Out;

	UPROPERTY(Replicated, SaveGame, VisibleInstanceOnly)
	TArray<double> Out_Array;
};
