// 

#pragma once

#include "CoreMinimal.h"
#include "Behaviour/FGWiremodBuildable.h"
#include "Behaviour/MultistateWiremodBuildable.h"
#include "DivideNumbers.generated.h"

UCLASS()
class FICSITWIREMOD_API ADivideNumbers : public AMultistateWiremodBuildable
{
	GENERATED_BODY()

public:
	virtual void ServerProcess_Implementation(double DeltaTime) override
	{
		switch (CurrentStateIndex)
		{
			//Default mode - divide all connected from first to last
		case 0:
			{
				TArray<FConnectionData> Connected;
				GetAllConnected(Connected);

				double Result = 0;
				if (Connected.Num() > 0)
				{
					Result = Connected[0].GetFloat();

					for(int i = 1; i < Connected.Num(); i++)
						Result /= Connected[i].GetFloat();
				}
				
				Out = Result;
				break;
			}
			//Array mode - divide all array elements from first to last
		case 1:
			{
				auto Array = GetConnection(0).GetFloatArray();

				double Result = 0;
				if (Array.Num() > 0)
				{
					Result = Array[0];
					for(int i = 1; i < Array.Num(); i++) Result /= Array[i];
				}
				
				Out = Result;
				break;
			}
			//Array single mode - divide each element in array by number
		case 2:
			{
				auto Array = GetConnection(0).GetFloatArray();
				double Value = GetConnection(1).GetFloat();
				for(int i = 0; i < Array.Num(); i++) Array[i] /= Value;

				Out_Array = Array;
				break;
			}

			//Array mode - divide each element in array A by same index element from array B
		case 3:
			{
				Out_Array.Empty();
				const auto ArrayA = GetConnection(0).GetFloatArray();
				const auto ArrayB = GetConnection(1).GetFloatArray();

				for (int i = 0; i < FMath::Max(ArrayA.Num(), ArrayB.Num()); i++)
				{
					if (ArrayA.IsValidIndex(i))
					{
						const auto BValue = ArrayB.IsValidIndex(i) ? ArrayB[i] : 1;

						Out_Array.Add(ArrayA[i] / BValue);
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

		DOREPLIFETIME(ADivideNumbers, Out);
		DOREPLIFETIME(ADivideNumbers, Out_Array);
	}


	UPROPERTY(Replicated, SaveGame)
	double Out;

	UPROPERTY(Replicated, SaveGame)
	TArray<double> Out_Array;
};
