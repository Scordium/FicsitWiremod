// 

#pragma once

#include "CoreMinimal.h"
#include "Behaviour/FGWiremodBuildable.h"
#include "PlayerSensor.generated.h"

UCLASS()
class FICSITWIREMOD_API APlayerSensor : public AFGWiremodBuildable
{
	GENERATED_BODY()

public:
	virtual void ServerProcess_Implementation(double DeltaTime) override { FindPlayer(); }

	void FindPlayer()
	{
		TArray<AActor*> Players;
		UGameplayStatics::GetAllActorsOfClass(this, AFGCharacterPlayer::StaticClass(), Players);

		for(auto PlayerActor : Players)
		{
			auto CharacterPlayer = Cast<AFGCharacterPlayer>(PlayerActor);
			if(!CharacterPlayer) continue;

			auto PlayerState = Cast<AFGPlayerState>(CharacterPlayer->GetPlayerState());
			if(!PlayerState) continue;

			bool NetIdIsValid = PlayerState->GetUniqueNetId().GetV1().IsValid();
			if(!NetIdIsValid) continue;

			auto PlayerId = PlayerState->GetUserID();

			if(PlayerId == TrackedPlayerId)
			{
				Player = CharacterPlayer;
				return;
			}
		}

		Player = nullptr;
	}

	UFUNCTION(BlueprintCallable)
	void SetTargetPlayer(FString PlayerId, UObject* Setter)
	{
		if(!GetCanConfigure(Setter)) return;
		TrackedPlayerId = PlayerId;
		FindPlayer();
	}

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override
	{
		Super::GetLifetimeReplicatedProps(OutLifetimeProps);

		DOREPLIFETIME(APlayerSensor, Player)
		DOREPLIFETIME(APlayerSensor, TrackedPlayerId)
	}

	UPROPERTY(SaveGame, Replicated)
	FString TrackedPlayerId;

	UPROPERTY(Replicated, BlueprintReadOnly)
	AActor* Player;
};
