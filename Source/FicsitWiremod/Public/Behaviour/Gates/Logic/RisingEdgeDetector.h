// 

#pragma once

#include "CoreMinimal.h"
#include "Behaviour/FGWiremodBuildable.h"
#include "RisingEdgeDetector.generated.h"

UCLASS()
class FICSITWIREMOD_API ARisingEdgeDetector : public AFGWiremodBuildable
{
	GENERATED_BODY()

public:
	virtual void ServerProcess_Implementation(double DeltaTime) override
	{
		bool Value = GetConnection(0).GetBool();

		if(Value)
		{
			if(HasChangedLastFrame) Out = false;
			else
			{
				Out = true;
				HasChangedLastFrame = true;
			}
		}
		else
		{
			Out = false;
			HasChangedLastFrame = false;
		}
	}

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override
	{
		Super::GetLifetimeReplicatedProps(OutLifetimeProps);

		DOREPLIFETIME(ARisingEdgeDetector, Out);
	}


	UPROPERTY(Replicated, SaveGame, VisibleInstanceOnly)
	bool Out;

	UPROPERTY(VisibleInstanceOnly, SaveGame)
	bool HasChangedLastFrame;
};
