// 

#pragma once

#include "CoreMinimal.h"
#include "MultistateWiremodBuildable.h"
#include "Behaviour/FGWiremodBuildable.h"
#include "BoolToNumber.generated.h"

UCLASS()
class FICSITWIREMOD_API ABoolToNumber : public AMultistateWiremodBuildable
{
	GENERATED_BODY()

public:
	virtual void ServerProcess_Implementation(double DeltaTime) override
	{
		if (CurrentStateIndex == 0)
		{
			Out = GetConnection(0).GetBool();
		}
		else if (CurrentStateIndex == 1)
		{
			auto const BoolArray = GetConnection(0).GetBoolArray();
			
			TArray<double> Values;
			for (bool Value : BoolArray)
			{
				Values.Add(Value);
			}

			OutArray = Values;
 		}
		
	}

	virtual void GetLifetimeReplicatedProps( TArray<FLifetimeProperty>& OutLifetimeProps ) const override
	{
		Super::GetLifetimeReplicatedProps(OutLifetimeProps);

		DOREPLIFETIME(ABoolToNumber, Out);
		DOREPLIFETIME(ABoolToNumber, OutArray);
	}

	virtual void OnStateSelected_Internal(int Index) override
	{
		Super::OnStateSelected_Internal(Index);

		Out = 0;
		OutArray.Empty();
	}


	UPROPERTY(Replicated, SaveGame, VisibleInstanceOnly)
	int Out;

	UPROPERTY(Replicated, SaveGame, VisibleInstanceOnly)
	TArray<double> OutArray;
};
