// 

#pragma once

#include "CoreMinimal.h"
#include "Behaviour/FGWiremodBuildable.h"
#include "Timer.generated.h"

UCLASS()
class FICSITWIREMOD_API ATimer : public AFGWiremodBuildable
{
	GENERATED_BODY()

public:
	virtual void Process_Implementation(double DeltaTime) override
	{
		Out = false;

		auto Run = GetConnection(0).GetBool();
		if(!Run)
		{
			CurrentTime = 0;
			return;
		}
		
		auto Time = GetConnection(1).GetFloat();
		auto Loop = GetConnection(2).GetBool();

		CurrentTime += DeltaTime;
		if(CurrentTime >= Time)
		{
			Out = true;
			if(Loop) CurrentTime = 0;
			else CurrentTime = Time;
		}
	}

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override
	{
		Super::GetLifetimeReplicatedProps(OutLifetimeProps);

		DOREPLIFETIME(ATimer, Out)
		DOREPLIFETIME(ATimer, CurrentTime)
	}

	UPROPERTY(Replicated, SaveGame)
	bool Out;

	UPROPERTY(Replicated, SaveGame)
	double CurrentTime;
};
