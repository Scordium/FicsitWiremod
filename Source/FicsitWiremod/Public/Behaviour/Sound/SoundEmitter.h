// 

#pragma once

#include "CoreMinimal.h"
#include "../FGWiremodBuildable.h"
#include "SoundEmitter.generated.h"

UCLASS()
class FICSITWIREMOD_API ASoundEmitter : public AFGWiremodBuildable
{
	GENERATED_BODY()

public:

	virtual void ClientProcess_Implementation(double DeltaTime) override
	{
		auto Frequency= FMath::Max(GetConnection(1).GetFloat(100), 1.f);
		auto Volume = GetConnection(2).GetFloat(1);
		auto Range = GetConnection(3).GetFloat(1000);

		SetSoundSettings(Frequency, Volume, Range);
		
		if(GetConnection(0).GetBool()) StartSound();
		else StopSound();
	}

	UFUNCTION(BlueprintImplementableEvent)
	void StartSound();

	UFUNCTION(BlueprintImplementableEvent)
	void StopSound();

	UFUNCTION(BlueprintImplementableEvent)
	void SetSoundSettings(double Frequency, double Volume, double Range);
};
