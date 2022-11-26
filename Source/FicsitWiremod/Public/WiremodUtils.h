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

	UFUNCTION(BlueprintCallable)
	static FFactoryCustomizationData InitCustomizationData(FLinearColor primaryColor, FLinearColor secondaryColor, AFGGameState* gameState)
	{
		auto data = FFactoryCustomizationData();
		data.ColorSlot = 254;
		data.OverrideColorData.PrimaryColor = primaryColor;
		data.OverrideColorData.SecondaryColor = secondaryColor;
		data.Initialize(gameState);

		return data;
	}

	UFUNCTION(BlueprintCallable, meta=(DefaultToSelf))
	static AFGBuildableBlueprintDesigner* GetBlueprintDesigner(AFGBuildable* Buildable)
	{
		if(!Buildable) return nullptr;
		return Buildable->GetBlueprintDesigner();
	}

	UFUNCTION(BlueprintCallable, meta=(DefaultToSelf))
	static void SetCanEverTick(AFGBuildable* Buildable, bool Enabled)
	{
		if (!Buildable) return;

		Buildable->mFactoryTickFunction.bCanEverTick = Enabled;
	}
	
};
