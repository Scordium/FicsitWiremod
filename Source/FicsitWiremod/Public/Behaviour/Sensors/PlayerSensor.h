// 

#pragma once

#include "CoreMinimal.h"
#include "Behaviour/FGWiremodBuildable.h"
#include "CommonLib/PlayerUtilities.h"
#include "PlayerSensor.generated.h"

UCLASS()
class FICSITWIREMOD_API APlayerSensor : public AFGWiremodBuildable
{
	GENERATED_BODY()

public:
	virtual void ServerProcess_Implementation(double DeltaTime) override
	{
		auto Players = GetWorld()->GetGameState()->PlayerArray;

		PlayerList.Empty();
		for (auto BaseState : Players)
		{
			auto PlayerState = Cast<AFGPlayerState>(BaseState);
			if (!PlayerState) continue;
			
			PlayerList.Add(PlayerState->GetPawn());

			if (UPlayerUtilities::GetUserIdSafe(PlayerState) == TrackedPlayerId) Player = PlayerState->GetPawn();
		}
	}

	UFUNCTION(BlueprintCallable)
	void SetTargetPlayer(FString PlayerId, UObject* Setter)
	{
		PERMISSION_CHECK(Setter);
		TrackedPlayerId = PlayerId;
		Player = nullptr;
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

	UPROPERTY(Replicated, BlueprintReadOnly)
	TArray<AActor*> PlayerList;
};
