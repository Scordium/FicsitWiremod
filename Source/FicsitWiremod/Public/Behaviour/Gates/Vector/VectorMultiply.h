// 

#pragma once

#include "CoreMinimal.h"
#include "MultistateWiremodBuildable.h"
#include "Behaviour/FGWiremodBuildable.h"
#include "VectorMultiply.generated.h"

UCLASS()
class FICSITWIREMOD_API AVectorMultiply : public AMultistateWiremodBuildable
{
	GENERATED_BODY()

public:
	virtual void ServerProcess_Implementation(double DeltaTime) override
	{
		
		switch (CurrentStateIndex)
		{
			//Default mode - Vector * Vector
		case 0: Out = GetConnection(0).GetVector() * GetConnection(1).GetVector(); break;
			//Number mode - Vector * Float
		case 1: Out = Out = GetConnection(0).GetVector() * GetConnection(1).GetFloat(); break;
			//Single number mode - Vector[] * Float
		case 2:
			{
				ArrayOut = GetConnection(0).GetVectorArray();
				const auto Number = GetConnection(1).GetFloat(1);

				for (int i = 0; i < ArrayOut.Num(); i++)
				{
					ArrayOut[i] *= Number;
				}
				
				break;
			}
			//Single mode - Vector[] * Vector
		case 3:
			{
				ArrayOut = GetConnection(0).GetVectorArray();
				const auto Vector = GetConnection(1).GetVector(FVector::OneVector);

				for (int i = 0; i < ArrayOut.Num(); i++)
				{
					ArrayOut[i] *= Vector;
				}
				
				break;
			}
			//Array mode - Vector[] * Vector[]
		case 4:
			{
				ArrayOut.Empty();

				const auto ArrayA = GetConnection(0).GetVectorArray();
				const auto ArrayB = GetConnection(1).GetVectorArray();

				for (int i = 0; i < FMath::Max(ArrayA.Num(), ArrayB.Num()); i++)
				{
					if (ArrayA.IsValidIndex(i))
					{
						const auto BMultiplier = ArrayB.IsValidIndex(i) ? ArrayB[i] : FVector::OneVector;
						
						ArrayOut.Add(ArrayA[i] * BMultiplier);
					}
					else ArrayOut.Add(ArrayB[i]);
				}
				
				break;
			}
			//Array number mode - Vector[] * Float[]
		case 5:
			{
				ArrayOut = GetConnection(0).GetVectorArray();
				const auto NumberArray = GetConnection(1).GetFloatArray();

				for (int i = 0; i < ArrayOut.Num(); i++)
				{
					if (!NumberArray.IsValidIndex(i)) break;
					
					ArrayOut[i] *= NumberArray[i];
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
		ArrayOut.Empty();
	}

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override
	{
		Super::GetLifetimeReplicatedProps(OutLifetimeProps);

		DOREPLIFETIME(AVectorMultiply, Out)
		DOREPLIFETIME(AVectorMultiply, ArrayOut)
	}

	UPROPERTY(Replicated, SaveGame)
	FVector Out;

	UPROPERTY(Replicated, SaveGame)
	TArray<FVector> ArrayOut;
};
