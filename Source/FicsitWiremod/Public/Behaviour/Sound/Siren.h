// 

#pragma once

#include "CoreMinimal.h"
#include "Behaviour/MultistateWiremodBuildable.h"
#include "Siren.generated.h"

UCLASS()
class FICSITWIREMOD_API ASiren : public AMultistateWiremodBuildable
{
	GENERATED_BODY()

public:
	virtual void Process_Implementation(float DeltaTime) override
	{
		if(!AudioSource) return;

		auto Play = GetConnection(0).GetBool();

		if(!Play) AudioSource->Stop();
		else
		{
			if(!AudioSource->IsPlaying())
			{
				AudioSource->SetSound(Sounds[CurrentStateIndex]);
				AudioSource->Play();
			}
			
			auto Volume = FMath::Clamp(GetConnection(1).GetFloat(1), 0.f, 5.f);
			auto Distance = GetConnection(2).GetFloat(5000);
			auto Pitch =  FMath::Clamp(GetConnection(3).GetFloat(1), 0.1f, 10.f);
			
			AudioSource->AdjustVolume(0, Volume);
			
			AudioSource->AttenuationSettings->Attenuation.FalloffDistance = Distance;
			AudioSource->AdjustAttenuation(AudioSource->AttenuationSettings->Attenuation);
			
			AudioSource->SetPitchMultiplier(Pitch);
		}
	}



	UPROPERTY(BlueprintReadWrite, EditAnywhere, Instanced)
	UAudioComponent* AudioSource = CreateDefaultSubobject<UAudioComponent>("AudioSource");
	
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
	TArray<USoundWave*> Sounds;
};
