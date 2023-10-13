// 

#pragma once

#include "CoreMinimal.h"
#include "Behaviour/FGWiremodBuildable.h"
#include "TFlipFlop.generated.h"

UCLASS()
class FICSITWIREMOD_API ATFlipFlop : public AFGWiremodBuildable
{
	GENERATED_BODY()

public:
	virtual void ServerProcess_Implementation(double DeltaTime) override
	{
		bool Clock = GetConnection(1).GetBool();

		if(Clock)
		{
			if(HasClockedLastFrame || !GetConnection(0).GetBool()) return;
			
			Out = !Out;
			HasClockedLastFrame = true;
		}
		else HasClockedLastFrame = false;
	}

	virtual void GetLifetimeReplicatedProps( TArray<FLifetimeProperty>& OutLifetimeProps ) const override
	{
		Super::GetLifetimeReplicatedProps(OutLifetimeProps);

		DOREPLIFETIME(ATFlipFlop, Out);
	};
	
	UPROPERTY(Replicated, SaveGame, VisibleInstanceOnly)
	bool Out;

	UPROPERTY(SaveGame, VisibleInstanceOnly)
	bool HasClockedLastFrame;
};
