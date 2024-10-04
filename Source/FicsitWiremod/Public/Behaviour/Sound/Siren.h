// 

#pragma once

#include "CoreMinimal.h"
#include "../MultistateWiremodBuildable.h"
#include "Behaviour/MultistateWiremodBuildable.h"
#include "Components/AudioComponent.h"
#include "Runtime/Engine/Classes/Components/AudioComponent.h"
#include "Sound/SoundAttenuation.h"
#include "Sound/SoundWave.h"
#include "Siren.generated.h"

UCLASS()
class FICSITWIREMOD_API ASiren : public AMultistateWiremodBuildable
{
	GENERATED_BODY()

public:

	ASiren() : Super()
	{
		AudioSource->AttachToComponent(RootComponent, FAttachmentTransformRules(EAttachmentRule::KeepRelative, false));
	}
	
	virtual void ClientProcess_Implementation(double DeltaTime) override
	{
		if(!AudioSource) return;

		auto Volume = FMath::Clamp(GetConnection(1).GetFloat(1), 0.f, 5.f) / 2.f;
		auto Distance = GetConnection(2).GetFloat(5000);
		auto Pitch =  FMath::Clamp(GetConnection(3).GetFloat(1), 0.1f, 10.f);
			
		AudioSource->AdjustVolume(0, Volume);
		AudioSource->SetPitchMultiplier(Pitch);
		AttenuationSettings.FalloffDistance = Distance;
		AudioSource->AdjustAttenuation(AttenuationSettings);
		
		auto Play = GetConnection(0).GetBool();

		if(!Play) AudioSource->Stop();
		else
		{
			if(!AudioSource->IsPlaying())
			{
				AudioSource->SetSound(Sounds[CurrentStateIndex]);
				AudioSource->Play();
			}
		}
	}

	virtual void OnStateSelected_Internal(int Index) override
	{
		Super::OnStateSelected_Internal(Index);

		AudioSource->Stop();
		AudioSource->SetSound(Sounds[CurrentStateIndex]);
	}



	UPROPERTY(BlueprintReadWrite, EditAnywhere, Instanced)
	UAudioComponent* AudioSource = CreateDefaultSubobject<UAudioComponent>("AudioSource");
	
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
	TArray<USoundWave*> Sounds;

	FSoundAttenuationSettings AttenuationSettings = FSoundAttenuationSettings();
};
