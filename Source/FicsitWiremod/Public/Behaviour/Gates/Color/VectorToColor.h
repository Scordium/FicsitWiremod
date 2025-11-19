// 

#pragma once

#include "CoreMinimal.h"
#include "MultistateWiremodBuildable.h"
#include "Behaviour/FGWiremodBuildable.h"
#include "VectorToColor.generated.h"

UCLASS()
class FICSITWIREMOD_API AVectorToColor : public AMultistateWiremodBuildable
{
	GENERATED_BODY()

public:
	virtual void ServerProcess_Implementation(double DeltaTime) override
	{
		if (CurrentStateIndex == 0)
		{
			auto Vector = GetConnection(0).GetVector();
			auto Alpha = GetConnection(1).GetFloat(1);

			Out = FLinearColor(Vector);
			Out.A = Alpha;	
		}
		else if (CurrentStateIndex == 1)
		{
			auto const VectorArray = GetConnection(0).GetVectorArray();
			auto const AlphaArray = GetConnection(1).GetFloatArray();

			OutArray.SetNum(VectorArray.Num());

			for (int i = 0; i < VectorArray.Num(); i++)
			{
				auto Alpha = AlphaArray.Num() > i ? AlphaArray[i] : 1;
				
				OutArray[i] = FLinearColor(VectorArray[i]);
				OutArray[i].A = Alpha;
			}
		}
		else if (CurrentStateIndex == 2)
		{
			auto const VectorArray = GetConnection(0).GetVectorArray();
			auto const Alpha = GetConnection(1).GetFloat(1);

			OutArray.SetNum(VectorArray.Num());

			for (int i = 0; i < VectorArray.Num(); i++)
			{
				OutArray[i] = FLinearColor(VectorArray[i]);
				OutArray[i].A = Alpha;
			}
		}
	}

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override
	{
		Super::GetLifetimeReplicatedProps(OutLifetimeProps);

		DOREPLIFETIME(AVectorToColor, Out)
		DOREPLIFETIME(AVectorToColor, OutArray)
	}

	virtual void OnStateSelected_Internal(int Index) override
	{
		Super::OnStateSelected_Internal(Index);

		Out = FLinearColor();
		OutArray.Empty();
	}

	

	UPROPERTY(Replicated, SaveGame)
	FLinearColor Out;

	UPROPERTY(Replicated, SaveGame)
	TArray<FLinearColor> OutArray;
};
