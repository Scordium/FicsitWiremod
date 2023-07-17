// 

#pragma once

#include "CoreMinimal.h"
#include "Behaviour/FGWiremodBuildable.h"
#include "Synthesis/Classes/SynthComponents/SynthComponentToneGenerator.h"
#include "SoundEmitter.generated.h"

UCLASS()
class FICSITWIREMOD_API ASoundEmitter : public AFGWiremodBuildable
{
	GENERATED_BODY()

public:

	ASoundEmitter() : Super()
	{
		AudioSource->AttachToComponent(RootComponent, FAttachmentTransformRules(EAttachmentRule::KeepRelative, false));
	}

	virtual void Process_Implementation(double DeltaTime) override
	{
		if(!AudioSource) return;

		auto Frequency= FMath::Max(GetConnection(1).GetFloat(100), 1.f);
		auto Volume = GetConnection(2).GetFloat(1);
		auto Range = GetConnection(3).GetFloat(1000);

		AudioSource->SetVolume(Volume);
		AudioSource->SetFrequency(Frequency);
		AttenuationSettings.FalloffDistance = Range;
		AudioSource->GetAudioComponent()->AdjustAttenuation(AttenuationSettings);
		
		auto Play = GetConnection(0).GetBool();
		if(Play)
		{
			if(!AudioSource->IsPlaying()) AudioSource->Start();
		}
		else AudioSource->Stop();
	}

	UPROPERTY(EditAnywhere, Instanced, BlueprintReadWrite)
	USynthComponentToneGenerator* AudioSource = CreateDefaultSubobject<USynthComponentToneGenerator>("AudioSource");

	FSoundAttenuationSettings AttenuationSettings = FSoundAttenuationSettings();
};
