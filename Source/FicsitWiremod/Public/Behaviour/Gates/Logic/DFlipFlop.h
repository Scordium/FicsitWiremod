// 

#pragma once

#include "CoreMinimal.h"
#include "Behaviour/FGWiremodBuildable.h"
#include "DFlipFlop.generated.h"

UCLASS()
class FICSITWIREMOD_API ADFlipFlop : public AFGWiremodBuildable
{
	GENERATED_BODY()

public:
	virtual void Process_Implementation(float DeltaTime) override
	{
		if(GetConnection(1).GetBool())
		{
			if(HasClockedLastFrame) return;
			
			Out = GetConnection(0).GetBool();
			HasClockedLastFrame = true;
		}
		else HasClockedLastFrame = false;
	}

	virtual void GetLifetimeReplicatedProps( TArray<FLifetimeProperty>& OutLifetimeProps ) const override
	{
		Super::GetLifetimeReplicatedProps(OutLifetimeProps);

		DOREPLIFETIME(ADFlipFlop, Out);
	};
	
	UPROPERTY(Replicated, VisibleInstanceOnly)
	bool Out;

	UPROPERTY(VisibleInstanceOnly)
	bool HasClockedLastFrame;
};
