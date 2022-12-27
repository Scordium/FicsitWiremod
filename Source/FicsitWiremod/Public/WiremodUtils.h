// Copyright Coffee Stain Studios. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "FGBuildableDoor.h"
#include "FGFluidIntegrantInterface.h"
#include "FGPlayerController.h"
#include "FGPlayerState.h"
#include "FGResourceSinkSubsystem.h"
#include "FGStorySubsystem.h"
#include "FGTimeSubsystem.h"
#include "WiremodReflection.h"
#include "Equipment/FGEquipment.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Kismet/KismetStringLibrary.h"
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

	UFUNCTION(BlueprintCallable, meta=(DefaultToSelf = "Buildable"))
	static void SetCanEverTick(AFGBuildable* Buildable, bool Enabled)
	{
		if (!Buildable) return;

		Buildable->mFactoryTickFunction.bCanEverTick = Enabled;
	}


#define WM UWiremodReflection
#define STR UKismetStringLibrary
	UFUNCTION(BlueprintPure)
	static FString GetStringifiedValue(const FNewConnectionData& Value)
	{
		switch (Value.ConnectionType)
		{
		case Unknown: return "?";
		case Boolean: return WM::GetFunctionBoolResult(Value) ? "True" : "False";
		case Number: return STR::Conv_FloatToString(WM::GetFunctionNumberResult(Value));
		case String: return WM::GetFunctionStringResult(Value);
		case Vector: return STR::Conv_VectorToString(WM::GetFunctionVectorResult(Value));
		case Inventory: return "?";
		case PowerGrid: return "?";
		case Entity:
			if(auto player = Cast<AFGCharacterPlayer>(WM::GetFunctionEntityResult(Value)))
			{
				return UKismetSystemLibrary::GetObjectName(player) + "(Player " + player->GetPlayerState()->GetPlayerName() + ")";
			}
			return UKismetSystemLibrary::GetObjectName(WM::GetFunctionEntityResult(Value));
		case Recipe: return UFGRecipe::GetRecipeName(WM::GetFunctionRecipeResult(Value)).ToString();
		case Color: return STR::Conv_ColorToString(WM::GetFunctionColorResult(Value));
		case ArrayOfBoolean: return "[" + STR::Conv_IntToString(WM::GetBoolArray(Value).Num()) + " elements]";
		case ArrayOfNumber: return "[" + STR::Conv_IntToString(WM::GetNumberArray(Value).Num()) + " elements]";
		case ArrayOfString: return "[" + STR::Conv_IntToString(WM::GetStringArray(Value).Num()) + " elements]";
		case ArrayOfVector: return "[" + STR::Conv_IntToString(WM::GetVectorArray(Value).Num()) + " elements]";
		case ArrayOfEntity: return "[" + STR::Conv_IntToString(WM::GetEntityArray(Value).Num()) + " elements]";
		case ArrayOfColor: return "[" + STR::Conv_IntToString(WM::GetColorArray(Value).Num()) + " elements]";
		case ArrayOfInventory: return "[" + STR::Conv_IntToString(WM::GetInventoryArray(Value).Num()) + " elements]";
		case ArrayOfPowerGrid: return "[" + STR::Conv_IntToString(WM::GetPowerGridArray(Value).Num()) + " elements]";
		case ArrayOfStack: return "[" + STR::Conv_IntToString(WM::GetItemStackArray(Value).Num()) + " elements]";
		case ArrayOfRecipe: return "[" + STR::Conv_IntToString(WM::GetRecipeArray(Value).Num()) + " elements]";
		case Stack: return UFGItemDescriptor::GetItemName(WM::GetFunctionStackResult(Value).Item.GetItemClass()).ToString();
		case Integer: return STR::Conv_IntToString(WM::GetFunctionNumberResult(Value));
		case Any: return "How did this happen";
		case AnyArray: return "How did this happen[]";
		case AnyNonArray: return "How did this happen {}";
		default:
			UE_LOG(LogTemp, Error, TEXT("Failed to find switch case for EConnectionType::%d in function GET_STRINGIFIED_VALUE. Returning default value instead..."), (int)Value.ConnectionType);
			return "?";
		}
	}
	
};
