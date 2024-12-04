// 

#pragma once

#include "CoreMinimal.h"
#include "MultistateWiremodBuildable.h"
#include "Behaviour/FGWiremodBuildable.h"
#include "VectorAdd.generated.h"

UCLASS()
class FICSITWIREMOD_API AVectorAdd : public AMultistateWiremodBuildable
{
	GENERATED_BODY()

public:
	virtual void ServerProcess_Implementation(double DeltaTime) override
	{
		switch (CurrentStateIndex)
		{
			//Default mode - Vector + Vector
		case 0: Out = GetConnection(0).GetVector() + GetConnection(1).GetVector(); break;
			//Number mode - Vector + Float
		case 1: Out = GetConnection(0).GetVector() + GetConnection(1).GetFloat(); break;
			//Single number mode - Vector[] + Float
		case 2:
			{
				ArrayOut = GetConnection(0).GetVectorArray();
				const auto Number = GetConnection(1).GetFloat();

				for (int i = 0; i < ArrayOut.Num(); i++)
				{
					ArrayOut[i] = ArrayOut[i] + Number;
				}
				break;
			}
			//Single mode - Vector[] + Vector
		case 3:
			{
				ArrayOut = GetConnection(0).GetVectorArray();
				const auto Vector = GetConnection(1).GetVector();

				for (int i = 0; i < ArrayOut.Num(); i++)
				{
					ArrayOut[i] += Vector;
				}
				break;
			}
			//Array mode - Vector[] + Vector[]
		case 4:
			{
				ArrayOut.Empty();

				const auto ArrayA = GetConnection(0).GetVectorArray();
				const auto ArrayB = GetConnection(1).GetVectorArray();

				for (int i = 0; i < FMath::Max(ArrayA.Num(), ArrayB.Num()); i++)
				{
					if (ArrayA.IsValidIndex(i))
					{
						const auto BValue = ArrayB.IsValidIndex(i) ? ArrayB[i] : FVector::ZeroVector;
						
						ArrayOut.Add(ArrayA[i] + BValue);
					}
					else ArrayOut.Add(ArrayB[i]);
				}
				
				break;
			}
			//Array number mode - Vector[] + Float[]
		case 5:
			{
				ArrayOut = GetConnection(0).GetVectorArray();
				const auto NumberArray = GetConnection(1).GetFloatArray();

				for (int i = 0; i < ArrayOut.Num(); i++)
				{
					if (!NumberArray.IsValidIndex(i)) break;
					
					ArrayOut[i] = ArrayOut[i] + NumberArray[i];
				}
				
				break;
			}

		default: break;
		}
		
		
	}

	virtual void OnStateSelected_Internal(int Index) override
	{
		Super::OnStateSelected_Internal(Index);

		Out = FVector::ZeroVector;
	}

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override
	{
		Super::GetLifetimeReplicatedProps(OutLifetimeProps);

		DOREPLIFETIME(AVectorAdd, Out)
		DOREPLIFETIME(AVectorAdd, ArrayOut)
	}

	UPROPERTY(Replicated, SaveGame)
	FVector Out;

	UPROPERTY(Replicated, SaveGame)
	TArray<FVector> ArrayOut;
};
