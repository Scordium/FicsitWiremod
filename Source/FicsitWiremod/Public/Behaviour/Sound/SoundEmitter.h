// 

#pragma once

#include "CoreMinimal.h"
#include "Components/SynthComponent.h"
#include "Behaviour/FGWiremodBuildable.h"
#include "SoundEmitter.generated.h"

UCLASS()
class FICSITWIREMOD_API ASoundEmitter : public AFGWiremodBuildable
{
	GENERATED_BODY()

public:

	virtual void Process_Implementation(float DeltaTime) override
	{
		if(!AudioSource) return;
		
		auto Play = GetConnection(0).GetBool();
		if(Play)
		{
			if(!AudioSource->IsPlaying()) AudioSource->Start();
			auto Frequency= FMath::Max(GetConnection(1).GetFloat(), 1.f);
			auto Volume = GetConnection(2).GetFloat();

			SetVolumeAndFrequency(Volume, Frequency);
		}
		else AudioSource->Stop();
	}

	UFUNCTION(BlueprintImplementableEvent)
	void SetVolumeAndFrequency(float Volume, float Frequency);

	UPROPERTY(EditAnywhere, Instanced, BlueprintReadWrite)
	USynthComponent* AudioSource = CreateDefaultSubobject<USynthComponent>("AudioSource");
	
};
