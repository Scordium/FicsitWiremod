// 

#pragma once

#include "CoreMinimal.h"
#include "FGCharacterPlayer.h"
#include "FGPlayerState.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "OwnerDataFunctions.generated.h"

UENUM(BlueprintType)
enum EWiremodInteractionRule
{
	NoOne,
	OwnerOnly,
	Anyone
};

USTRUCT(BlueprintType)
struct FWiremodOwnerData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame)
	FString OwnerId;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame)
	FString OwnerName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame)
	FText CustomName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame)
	TEnumAsByte<EWiremodInteractionRule> Connecting = EWiremodInteractionRule::Anyone;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame)
	TEnumAsByte<EWiremodInteractionRule> Disconnecting = EWiremodInteractionRule::Anyone;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame)
	TEnumAsByte<EWiremodInteractionRule> Configuring = EWiremodInteractionRule::Anyone;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame)
	bool AllowDismantle = true;

	bool IsOwner(UObject* entity) const { return OwnerId == GetUserIDOrDefault(entity); }
	bool HasOwner() const {return !OwnerId.IsEmpty();}
	
	bool GetCanConfigure(UObject* Actor) const
	{
		if(Configuring == NoOne) return !HasOwner() || IsOwner(Actor);
		if(Configuring == OwnerOnly && !IsOwner(Actor)) return false;

		//Leaving this in case i'll want some custom logic for this later
		return true;
	}

	bool GetCanConnect(UObject* Actor) const
	{
		if(Connecting == NoOne) return false;
		if(Connecting == OwnerOnly && !IsOwner(Actor)) return false;
		
		return true;
	}

	bool GetCanDisconnect(UObject* Actor) const
	{
		if(Disconnecting == NoOne) return false;
		if(Disconnecting == OwnerOnly && !IsOwner(Actor)) return false;
		
		return true;
	}

	void SetOwner(UObject* NewOwner, UObject* Setter)
	{
		if(!GetCanConfigure(Setter)) return;

		OwnerId = GetUserIDOrDefault(NewOwner);
		OwnerName = GetUsernameOrDefault(NewOwner);
		Connecting = IsValid(NewOwner) ? OwnerOnly : Anyone;
		Disconnecting = IsValid(NewOwner) ? OwnerOnly : Anyone;
	}

	void SetConnectingRule(EWiremodInteractionRule Rule, UObject* Setter)
	{
		if(!GetCanConfigure(Setter)) return;
		Connecting = Rule;
	}

	void SetDisconnectingRule(EWiremodInteractionRule Rule, UObject* Setter)
	{
		if(!GetCanConfigure(Setter)) return;
		Disconnecting = Rule;
	}

	void SetConfiguringRule(EWiremodInteractionRule Rule, UObject* Setter)
	{
		if(!GetCanConfigure(Setter)) return;
		Configuring = Rule;
	}

	static AFGPlayerState* GetState(UObject* entity)
	{
		if(auto player = Cast<AFGCharacterPlayer>(entity))
			return Cast<AFGPlayerState>(player->GetPlayerState());

		return nullptr;
	}
	static FString GetUserIDOrDefault(UObject* entity)
	{
		if(auto state = GetState(entity)) return state ? state->GetUserID() : FString();
		return "";
	}
	static FString GetUsernameOrDefault(UObject* entity)
	{
		if(auto state = GetState(entity)) return state ? state->GetPlayerName() : FString();
		return "";
	}

	bool operator ==(const FWiremodOwnerData& Other) const
	{
		return
		OwnerId == Other.OwnerId
		&& CustomName.EqualTo(Other.CustomName)
		&& AllowDismantle == Other.AllowDismantle
		&& Configuring == Other.Configuring
		&& Disconnecting == Other.Disconnecting
		&& Connecting == Other.Connecting;
	}

};

/**
 * 
 */
UCLASS()
class FICSITWIREMOD_API UOwnerDataFunctions : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintPure)
	static bool GetCanConfigure(const FWiremodOwnerData& Data, UObject* Actor)
	{
		return Data.GetCanConfigure(Actor);
	}

	UFUNCTION(BlueprintPure)
	static bool HasOwner(const FWiremodOwnerData& Data){ return Data.HasOwner(); }

	UFUNCTION(BlueprintPure)
	static bool IsOwner(const FWiremodOwnerData& Data, UObject* Actor) { return Data.IsOwner(Actor); }
};
