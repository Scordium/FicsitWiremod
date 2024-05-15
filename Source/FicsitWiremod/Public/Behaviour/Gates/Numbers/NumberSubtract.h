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
		//Default mode subtract B from A
		if(CurrentStateIndex == 0) Out = GetConnection(0).GetFloat() - GetConnection(1).GetFloat();
		//Alt mode - subtract all subsequent elements from the first element
		else if(CurrentStateIndex == 1)
		{
			const auto Array = GetConnection(0).GetFloatArray();
			if(Array.Num() == 0) Out = 0;
			else if(Array.Num() == 1) Out = Array[0];
			else
			{
				double ArrOut = Array[0];
				for (int i = 1; i < Array.Num(); i++)
				{
					ArrOut -= Array[i];
				}

				Out = ArrOut;
			}
		}
	}

	virtual void GetLifetimeReplicatedProps( TArray<FLifetimeProperty>& OutLifetimeProps ) const override
	{
		Super::GetLifetimeReplicatedProps(OutLifetimeProps);

		DOREPLIFETIME(ANumberSubtract, Out);
	}
	
	UPROPERTY(Replicated, SaveGame, VisibleInstanceOnly)
	double Out;
};
