// 

#pragma once

#include "CoreMinimal.h"
#include "FGBuildableDoor.h"
#include "FGCharacterPlayer.h"
#include "FGPipeSubsystem.h"
#include "Buildables/FGBuildablePipeReservoir.h"
#include "Buildables/FGBuildableRailroadSignal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "ReflectionExternalFunctions.generated.h"

/**
 * 
 */
UCLASS()
class FICSITWIREMOD_API UReflectionExternalFunctions : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:

	static void ChangeDoorState(UObject* Actor, bool State)
	{
		if(auto door = Cast<AFGBuildableDoor>(Actor))
		{
			if(State)
			{
				if(door->mDoorState == EDoorState::DS_Closed) return;
				
				door->mDoorState = EDoorState::DS_Closed;
				door->OnDoorConfigurationChanged(EDoorConfiguration::DC_Closed);
				door->SetDoorLightFeedbackState(EDoorConfiguration::DC_Closed);
				door->BeginClosing();
			}
			else
			{
				if(door->mDoorState == EDoorState::DS_Open) return;
				
				door->mDoorState = EDoorState::DS_Open;
				door->OnDoorConfigurationChanged(EDoorConfiguration::DC_Open);
				door->SetDoorLightFeedbackState(EDoorConfiguration::DC_Open);
				door->BeginOpening();
			}
			return;
		}
	}

	static void ChangeRailroadSignalState(UObject* Actor, bool State)
	{
		if(auto RailSignal = Cast<AFGBuildableRailroadSignal>(Actor))
		{
			if(RailSignal->GetObservedBlock().IsValid())
				RailSignal->GetObservedBlock().Pin().Get()->SetIsPathBlock(State);
		}
	}

	static void FlushTank(UObject* Actor)
	{
		if(auto fluidTank = Cast<AFGBuildablePipeReservoir>(Actor))
		{
			fluidTank->mFluidBox.Reset();
		}
	}

	static void FlushNetwork(UObject* Actor)
	{
		if(auto fluidTank = Cast<AFGBuildablePipeReservoir>(Actor))
		{
			AFGPipeSubsystem::Get(fluidTank->GetWorld())->FlushPipeNetworkFromIntegrant(fluidTank);
		}
	}

	static void ExecuteOnUse(UObject* Actor)
	{
		if(auto object = Cast<AFGBuildable>(Actor))
		{
			if(auto character = Cast<AFGCharacterPlayer>(object->GetWorld()->GetFirstPlayerController()->GetCharacter()))
			{
				auto useState = FUseState();
				useState.UseLocation = object->GetTransform().GetLocation();
				AFGBuildable::Execute_OnUse(object, character, useState);
			}
		}
	}

	//Ficsit farming compatibility func
	static void ChangeDoggoHouseDoorState(UObject* Actor, bool State)
	{
		if(auto Property = Cast<FBoolProperty>(FindProperty(Actor, "mDoorState")))
		{
			Property->SetPropertyValue_InContainer(Actor, State);
			ProcessFunction(Actor, "OnDoorStateChanged_Event", nullptr);
			ProcessFunction(Actor, "CalculateDoorCollision", nullptr);
		}
	}



	static bool ProcessFunction(UObject* Object, FName FuncName, void* params)
	{
		if(IsValid(Object))
		{
			auto function = GetFunction(Object, FuncName);
			if(function)
			{
				Object->ProcessEvent(function, params);
				return true;
			}
		}
		return false;
	}

	static UFunction* GetFunction(UObject* Object, FName FuncName)
	{
		if(!Object) return nullptr;
		if(!IsValid(Object)) return nullptr;
		
		UFunction* function = Object->FindFunction(FuncName);
		return function;
	}

	static FProperty* FindProperty(UObject* Object, FName PropertyName)
	{
		if(!Object) return nullptr;
		return Object->GetClass()->FindPropertyByName(PropertyName);
	}
};
