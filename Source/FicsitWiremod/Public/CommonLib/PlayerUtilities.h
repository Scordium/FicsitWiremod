// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FGPlayerState.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "PlayerUtilities.generated.h"

/**
 * 
 */
UCLASS()
class FICSITWIREMOD_API UPlayerUtilities : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	
	UFUNCTION(BlueprintPure)
	static FString GetUserIdSafe(AFGPlayerState* PlayerState)
	{
		if(!PlayerState) return "";
		
		const auto AccountId = PlayerState->GetUniqueNetId().GetV2();
		return UE::Online::FOnlineIdRegistryRegistry::Get().ToLogString(AccountId);
	}

	UFUNCTION(BlueprintPure)
	static FString GetUsernameSafe(AFGPlayerState* PlayerState)
	{
		if(!PlayerState) return "";
		return PlayerState->GetPlayerName();
	}
};
