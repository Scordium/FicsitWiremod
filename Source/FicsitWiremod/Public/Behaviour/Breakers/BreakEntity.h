// 

#pragma once

#include "CoreMinimal.h"
#include "FGFreightWagon.h"
#include "FGRailroadVehicle.h"
#include "FGVehicle.h"
#include "Behaviour/FGWiremodBuildable.h"
#include "WheeledVehicles/FGWheeledVehicle.h"
#include "BreakEntity.generated.h"

UCLASS()
class FICSITWIREMOD_API ABreakEntity : public AFGWiremodBuildable
{
	GENERATED_BODY()

public:
	virtual void ServerProcess_Implementation(double DeltaTime) override
	{
		Entity = GetConnection(0).GetEntity();
	}

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override
	{
		Super::GetLifetimeReplicatedProps(OutLifetimeProps);

		DOREPLIFETIME(ABreakEntity, Entity)
	}

	UFUNCTION()
	FString GetEntityName(){ return Entity ? Entity->GetClass()->GetName() : FString(); }

	UFUNCTION()
	FString GetDisplayName()
	{
		//If it's a player, try to return playername
		if(auto Player = Cast<AFGCharacterPlayer>(Entity)) return Player->GetPlayerState() ? Player->GetPlayerState()->GetPlayerName() : "Offline player";

		//If it's a buildable, try to return buildable name
		if(auto Buildable = Cast<AFGBuildable>(Entity))
		{
			if(auto CircuitryBuildable = Cast<AFGWiremodBuildable>(Buildable))
				return CircuitryBuildable->GetCustomNameOrDefault().ToString();

			return Buildable->mDisplayName.ToString();
		}

		//If it's a vehicle, try to return vehicle name
		if (auto Vehicle = Cast<AFGVehicle>(Entity)) return Vehicle->mDisplayName.ToString();

		//If it's a creature, then we can only return the navmesh agent name or use a switch block which is stupid.
		if(auto Wildlife = Cast<AFGCreature>(Entity)) return Wildlife->GetNavAgentName().ToString();

		//If it's something else, then return the default name
		return GetEntityName();
	}

	UFUNCTION()
	UFGInventoryComponent* GetEntityInventory()
	{
		//Player
		if(auto Player = Cast<AFGCharacterPlayer>(Entity)) return Player->GetInventory();

		//Vehicle
		if(auto Vehicle = Cast<AFGVehicle>(Entity))
		{
			//Train
			if(auto Train = Cast<AFGFreightWagon>(Vehicle))
				return Train->GetFreightInventory();
			
			//Default
			if(auto Default = Cast<AFGWheeledVehicle>(Vehicle))
				return Default->GetStorageInventory();
		}

		//Lizard doggo
		if(auto Creature = Cast<AFGCreature>(Entity))
		{
			return Cast<UFGInventoryComponent>(Creature->GetComponentByClass(UFGInventoryComponent::StaticClass()));
		}

		//Default
		return nullptr;
	}

	UFUNCTION()
	FVector GetEntityPosition() { return Entity ? Entity->GetActorLocation() : FVector(); }

	UFUNCTION()
	FVector GetEntityRotation() { return Entity ? Entity->GetActorRotation().Vector() : FVector(); }

	UFUNCTION()
	FVector GetEntityVelocity() { return Entity ? Entity->GetVelocity() : FVector(); }
	
	UFUNCTION()
	FVector GetEntityUpVector() { return Entity ? Entity->GetActorUpVector() : FVector(); }

	UFUNCTION()
	FVector GetEntityForwardVector() { return Entity ? Entity->GetActorForwardVector() : FVector(); }

	UFUNCTION()
	FVector GetEntityRightVector() { return Entity ? Entity->GetActorRightVector() : FVector(); }

	UFUNCTION()
	bool GetIsPlayer() { return IsValid(Cast<AFGCharacterPlayer>(Entity)); }

	UFUNCTION()
	bool GetIsHostile()
	{
		if(auto Creature = Cast<AFGCreature>(Entity)) return !Creature->IsPassiveCreature();
		return false;
	}

	UFUNCTION()
	bool GetIsFriendly()
	{
		if(auto Creature = Cast<AFGCreature>(Entity)) return Creature->IsPassiveCreature();
		return false;
	}

	UFUNCTION()
	bool GetIsVehicle() { return IsValid(Cast<AFGVehicle>(Entity)); }
	
	UPROPERTY(Replicated, SaveGame)
	AActor* Entity;
};
