// Copyright Coffee Stain Studios. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "FGStorySubsystem.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "WiremodUtils.generated.h"

/**
 * 
 */
UCLASS()
class FICSITWIREMOD_API UWiremodUtils : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

	UFUNCTION(BlueprintCallable)
	static void CallSchematicPurchased(AFGStorySubsystem* storySubsystem, TSubclassOf<UFGSchematic> schematic)
	{
		storySubsystem->OnSchematicPurchased(schematic);
	}
};
