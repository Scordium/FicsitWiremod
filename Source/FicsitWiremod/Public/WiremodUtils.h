// Copyright Coffee Stain Studios. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "FGBuildableDoor.h"
#include "FGStorySubsystem.h"
#include "FGTimeSubsystem.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Patching/NativeHookManager.h"
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


	UFUNCTION(BlueprintCallable)
	static void SetTime(AFGTimeOfDaySubsystem* subsystem, float hour, float minute, float second)
	{
	    hour = (int)hour%24;
	    minute = (int)minute%60;
	    second = (int)second%60;
	
		float finalTime = hour * 3600 + minute * 60 + second;
		subsystem->SetDaySeconds(finalTime);
	}
};
