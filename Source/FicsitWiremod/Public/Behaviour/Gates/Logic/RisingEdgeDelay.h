// 

#pragma once

#include "CoreMinimal.h"
#include "Behaviour/FGWiremodBuildable.h"
#include "RisingEdgeDelay.generated.h"

UCLASS()
class FICSITWIREMOD_API ARisingEdgeDelay : public AFGWiremodBuildable
{
	GENERATED_BODY()

public:
	virtual void ServerProcess_Implementation(double DeltaTime) override
	{
		auto Value = GetConnection(0).GetBool();
		auto TimeDelay = GetConnection(1).GetFloat();

		if(!Value) CurrentTime = 0;
		else CurrentTime = FMath::Clamp(CurrentTime + DeltaTime/TimeDelay, 0, 1);
		
		Out = CurrentTime >= 1;
	}


	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override
	{
		Super::GetLifetimeReplicatedProps(OutLifetimeProps);

		DOREPLIFETIME(ARisingEdgeDelay, CurrentTime)
		DOREPLIFETIME(ARisingEdgeDelay, Out)
	}

	UPROPERTY(Replicated, SaveGame)
	double CurrentTime;

	UPROPERTY(Replicated, SaveGame)
	bool Out;
};
