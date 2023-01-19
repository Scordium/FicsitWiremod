// Copyright Coffee Stain Studios. All Rights Reserved.


#include "WiremodReflection.h"

#include "FGBuildableDoor.h"
#include "FGCircuitSubsystem.h"
#include "FGGameState.h"
#include "FGPipeConnectionComponent.h"
#include "FGTrainStationIdentifier.h"
#include "IConstantsDistributor.h"
#include "Buildables/FGBuildableLightsControlPanel.h"
#include "Buildables/FGBuildablePipeReservoir.h"
#include "Buildables/FGBuildableRailroadSignal.h"
#include "Buildables/FGBuildableRailroadStation.h"
#include "Buildables/FGBuildableRailroadSwitchControl.h"
#include "Buildables/FGBuildableWidgetSign.h"
#include "Components/WidgetComponent.h"
#include "Kismet/KismetStringLibrary.h"
#include "Kismet/KismetTextLibrary.h"
#include "UI/FGSignBuildingWidget.h"

#define Dynamic IIConstantsDistributor::Execute_GetValue

static bool IsDynamic(UObject* Object)
{
	if(!Object) return false;

	return Object->GetClass()->ImplementsInterface(IIConstantsDistributor::UClassType::StaticClass());
}

bool UWiremodReflection::GetFunctionBoolResult(const FNewConnectionData& data, bool defaultValue)
{

	if(data.FromProperty)
		return GetBoolFromProperty(data, defaultValue);
		
	
	if(IsDynamic(data.Object))
		return Dynamic(data.Object, data.FunctionName.ToString()).StoredBool;
	
	
	if(data.FunctionName == "WM_DOORCONTROL_FUNC")
	{
		if(auto door = Cast<AFGBuildableDoor>(data.Object)) 
			return door->mDoorState == EDoorState::DS_Closed;
	}
	else if(auto panel = Cast<AFGBuildableLightsControlPanel>(data.Object))
	{
		if(data.FunctionName == "IsTimeOfDayAware")
			return panel->GetLightControlData().IsTimeOfDayAware;
	}

	
	struct{bool RetVal;} params{defaultValue};
	if(!ProcessFunction(data, &params)) return GetBoolFromProperty(data, defaultValue);
	return params.RetVal;
}

bool UWiremodReflection::GetBoolFromProperty(const FNewConnectionData& data, bool defaultValue)
{
	auto val = GetProperty<FBoolProperty>(data);
	if(!val) return defaultValue;
	return val->GetPropertyValue_InContainer(data.Object);
}


FString UWiremodReflection::GetFunctionStringResult(const FNewConnectionData& data, FString defaultValue)
{

	if(data.FromProperty)
		return GetStringFromProperty(data, defaultValue);
	
	if(IsDynamic(data.Object))
		return Dynamic(data.Object, data.FunctionName.ToString()).StoredString;
	
	if(auto sign = Cast<AFGBuildableWidgetSign>(data.Object))
	{
		if(sign->mTextElementToDataMap.Contains(data.FunctionName.ToString()))
			return sign->mTextElementToDataMap[data.FunctionName.ToString()];
		else return defaultValue;
	}
	else if(auto station = Cast<AFGBuildableRailroadStation>(data.Object))
	{
		return station->GetStationIdentifier()->GetStationName().ToString();
	}
	
	
	struct{FString RetVal;} params{defaultValue};
	if(!ProcessFunction(data, &params))
		return GetStringFromProperty(data, defaultValue);
	return params.RetVal;
}

FString UWiremodReflection::GetStringFromProperty(const FNewConnectionData& data, FString defaultValue)
{
	auto val = GetProperty<FStrProperty>(data);
	if(!val) return defaultValue;
	return val->GetPropertyValue_InContainer(data.Object);
}


float UWiremodReflection::GetFunctionNumberResult(const FNewConnectionData& data, float defaultValue)
{
	if(data.FromProperty)
		return GetNumberFromProperty(data, defaultValue);
	
	if(IsDynamic(data.Object))
		return Dynamic(data.Object, data.FunctionName.ToString()).StoredFloat;
	
	else if(auto panel = Cast<AFGBuildableLightsControlPanel>(data.Object))
	{
		//Control panels store their data in struct, so to get the values we have to manually disassemble the struct.
		if(data.FunctionName == "ColorSlotIndex") return panel->GetLightControlData().ColorSlotIndex;
		else return panel->GetLightControlData().Intensity;
	}
	else if(auto sign = Cast<AFGBuildableWidgetSign>(data.Object))
	{
		if(data.FunctionName == "Emissive") return sign->mEmissive;
		else if(data.FunctionName == "Glossiness") return sign->mGlossiness;
		else
		{
			if(sign->mIconElementToDataMap.Contains(data.FunctionName.ToString()))
				return sign->mIconElementToDataMap[data.FunctionName.ToString()];
			else return defaultValue;
		}
	}
	
	if(IsInteger(data))
	{
		struct{int RetVal;} params{(int)defaultValue};
		if(!ProcessFunction(data, &params))
			return GetNumberFromProperty(data, defaultValue);
		return params.RetVal;
	}
	else
	{
		struct{float RetVal;} params{defaultValue};
		if(!ProcessFunction(data, &params))
			return GetNumberFromProperty(data, defaultValue);
		return params.RetVal;
	}
}

float UWiremodReflection::GetNumberFromProperty(const FNewConnectionData& data, float defaultValue)
{
	if(IsInteger(data))
	{
		auto val = GetProperty<FIntProperty>(data);
		if(!val) return defaultValue;
		return val->GetPropertyValue_InContainer(data.Object);
	}
	else
	{
		auto val = GetProperty<FFloatProperty>(data) ;
		if(!val) return defaultValue;
		return val->GetPropertyValue_InContainer(data.Object);
	}
}

FVector UWiremodReflection::GetFunctionVectorResult(const FNewConnectionData& data, FVector defaultValue)
{
	if(data.FromProperty)
		return GetVectorFromProperty(data, defaultValue);
	
	if(IsDynamic(data.Object))
		return Dynamic(data.Object, data.FunctionName.ToString()).StoredVector;
	
	struct{FVector RetVal;} params{defaultValue};
	if(!ProcessFunction(data, &params))
		return GetVectorFromProperty(data, defaultValue);
	return params.RetVal;
}

FVector UWiremodReflection::GetVectorFromProperty(const FNewConnectionData& data, FVector defaultValue)
{
	auto val = GetProperty<FStructProperty>(data);
	if(!val) return defaultValue;
	return *val->ContainerPtrToValuePtr<FVector>(data.Object);
}

FLinearColor UWiremodReflection::GetFunctionColorResult(const FNewConnectionData& data, FLinearColor defaultValue)
{
	
	if(data.FromProperty)
		return GetColorFromProperty(data, defaultValue);
	
	if(IsDynamic(data.Object))
		return Dynamic(data.Object, data.FunctionName.ToString()).StoredColor;
	
	if(auto sign = Cast<AFGBuildableWidgetSign>(data.Object))
	{
		if(data.FunctionName == "TextColor") return sign->mForegroundColor;
		if(data.FunctionName == "BackgroundColor") return sign->mBackgroundColor;
		if(data.FunctionName == "AuxColor") return sign->mAuxilaryColor;
		
		return defaultValue;
	}
	
	struct{FLinearColor RetVal;} params{defaultValue};
	if(!ProcessFunction(data, &params))
		return GetColorFromProperty(data, defaultValue);
	return params.RetVal;
}

FLinearColor UWiremodReflection::GetColorFromProperty(const FNewConnectionData& data, FLinearColor defaultValue)
{
	auto val = GetProperty<FStructProperty>(data);
	if(!val) return defaultValue;
	return *val->ContainerPtrToValuePtr<FLinearColor>(data.Object);
}

UFGInventoryComponent* UWiremodReflection::GetFunctionInventory(const FNewConnectionData& data)
{
	if(data.FromProperty)
		return GetInventoryFromProperty(data);

	
	if(IsDynamic(data.Object))
		return Dynamic(data.Object, data.FunctionName.ToString()).Inventory;
	
	struct{ UFGInventoryComponent* RetVal; } params{};
	if(!ProcessFunction(data, &params))
		return GetInventoryFromProperty(data);
	return params.RetVal;
}

UFGInventoryComponent* UWiremodReflection::GetInventoryFromProperty(const FNewConnectionData& data)
{
	auto val = GetProperty<FObjectProperty>(data);
	if(!val) return nullptr;
	return val->ContainerPtrToValuePtr<UFGInventoryComponent>(data.Object);
}

FInventoryStack UWiremodReflection::GetFunctionStackResult(const FNewConnectionData& data)
{
	if(data.FromProperty)
		return GetItemStackFromProperty(data);
	
	if(IsDynamic(data.Object))
		return Dynamic(data.Object, data.FunctionName.ToString()).Stack;
    
	struct{FInventoryStack RetVal;} params;
	if(!ProcessFunction(data, &params))
		return GetItemStackFromProperty(data);
	return params.RetVal;
}

FInventoryStack UWiremodReflection::GetItemStackFromProperty(const FNewConnectionData& data)
{
	auto val = GetProperty<FStructProperty>(data);
	if(!val) return FInventoryStack();
 	return *val->ContainerPtrToValuePtr<FInventoryStack>(data.Object);
}

UFGPowerCircuit* UWiremodReflection::GetFunctionPowerCircuitResult(const FNewConnectionData& data)
{
	if(data.FromProperty)
		return GetPowerCircuitFromProperty(data);

	
	if(IsDynamic(data.Object))
		return Dynamic(data.Object, data.FunctionName.ToString()).PowerGrid;
	
	struct{ UFGPowerCircuit* RetVal; } params{};
	if(!ProcessFunction(data, &params))
		return GetPowerCircuitFromProperty(data);
	return params.RetVal;
}

UFGPowerCircuit* UWiremodReflection::GetPowerCircuitFromProperty(const FNewConnectionData& data)
{
	auto val = GetProperty<FObjectProperty>(data);
	if(!val) return nullptr;
	return val->ContainerPtrToValuePtr<UFGPowerCircuit>(data.Object);
}

AActor* UWiremodReflection::GetFunctionEntityResult(const FNewConnectionData& data)
{
	if(data.FunctionName == "Self") return Cast<AActor>(data.Object);

	if(data.FromProperty)
		return GetEntityFromProperty(data);
	
	if(IsDynamic(data.Object))
		return Dynamic(data.Object, data.FunctionName.ToString()).Entity;
	
	struct{ AActor* RetVal; } params{};
	if(!ProcessFunction(data, &params))
		return GetEntityFromProperty(data);
	return params.RetVal;
}

AActor* UWiremodReflection::GetEntityFromProperty(const FNewConnectionData& data)
{
	auto val = GetProperty<FObjectProperty>(data);
	if(!val) return nullptr;
	return val->ContainerPtrToValuePtr<AActor>(data.Object);
}

TSubclassOf<UFGRecipe> UWiremodReflection::GetFunctionRecipeResult(const FNewConnectionData& data)
{
	TSubclassOf<UFGRecipe> out;

	if(data.FromProperty)
		out = GetRecipeFromProperty(data);
	else if(IsDynamic(data.Object))
		out = Dynamic(data.Object, data.FunctionName.ToString()).Recipe; 
	else
	{
		struct{TSubclassOf<UFGRecipe> RetVal; } params{};	
		if(!ProcessFunction(data, &params))
			out = GetRecipeFromProperty(data);
		out = params.RetVal;
	}
	
	if(!out.GetDefaultObject()) return TSubclassOf<UFGRecipe>();
	return TSubclassOf<UFGRecipe>(out.GetDefaultObject()->GetClass());
}

TSubclassOf<UFGRecipe> UWiremodReflection::GetRecipeFromProperty(const FNewConnectionData& data)
{
	auto val = GetProperty<FObjectProperty>(data);
	if(!val) return nullptr;
    return *val->ContainerPtrToValuePtr<TSubclassOf<UFGRecipe>>(data.Object);
}


TArray<bool> UWiremodReflection::GetBoolArray(const FNewConnectionData& data)
{
	if(data.FromProperty)
		return GetBoolArrayFromProperty(data);
	
	if(IsDynamic(data.Object))
		return Dynamic(data.Object, data.FunctionName.ToString()).BoolArr;
		
	struct{ TArray<bool> RetVal; } params;
	if(!ProcessFunction(data, &params))
		return GetBoolArrayFromProperty(data);
	return params.RetVal;
}

TArray<bool> UWiremodReflection::GetBoolArrayFromProperty(const FNewConnectionData& data)
{
	auto val = GetProperty<FArrayProperty>(data);
	if(!val) return TArray<bool>();
	return *val->ContainerPtrToValuePtr<TArray<bool>>(data.Object);
}

TArray<FString> UWiremodReflection::GetStringArray(const FNewConnectionData& data)
{
	if(data.FromProperty)
		return GetStringArrayFromProperty(data);
	
	if(IsDynamic(data.Object))
		return Dynamic(data.Object, data.FunctionName.ToString()).StringArr;

	struct{ TArray<FString> RetVal; } params;
	if(!ProcessFunction(data, &params))
		return GetStringArrayFromProperty(data);
	return params.RetVal;
}

TArray<FString> UWiremodReflection::GetStringArrayFromProperty(const FNewConnectionData& data)
{
	auto val = GetProperty<FArrayProperty>(data);
	if(!val) return TArray<FString>();
	return *val->ContainerPtrToValuePtr<TArray<FString>>(data.Object);
}

TArray<float> UWiremodReflection::GetNumberArray(const FNewConnectionData& data)
{
	if(data.FromProperty)
		return GetNumberArrayFromProperty(data);
	
	if(IsDynamic(data.Object))
		return Dynamic(data.Object, data.FunctionName.ToString()).NumberArr;
	
	struct{ TArray<float> RetVal; } params;
	if(!ProcessFunction(data, &params))
		return GetNumberArrayFromProperty(data);
	return params.RetVal;
}

TArray<float> UWiremodReflection::GetNumberArrayFromProperty(const FNewConnectionData& data)
{
	auto val = GetProperty<FArrayProperty>(data);
	if(!val) return TArray<float>();
	return *val->ContainerPtrToValuePtr<TArray<float>>(data.Object);
}

TArray<FVector> UWiremodReflection::GetVectorArray(const FNewConnectionData& data)
{
	if(data.FromProperty)
		return GetVectorArrayFromProperty(data);
	
	if(IsDynamic(data.Object))
		return Dynamic(data.Object, data.FunctionName.ToString()).VectorArr;
	
	struct{ TArray<FVector> RetVal; } params;
	if(!ProcessFunction(data, &params))
		return GetVectorArrayFromProperty(data);
	return params.RetVal;
}

TArray<FVector> UWiremodReflection::GetVectorArrayFromProperty(const FNewConnectionData& data)
{
	auto val = GetProperty<FArrayProperty>(data);
	if(!val) return TArray<FVector>();
	return *val->ContainerPtrToValuePtr<TArray<FVector>>(data.Object);
}

TArray<FLinearColor> UWiremodReflection::GetColorArray(const FNewConnectionData& data)
{
	if(data.FromProperty)
		return GetColorArrayFromProperty(data);
	
	if(IsDynamic(data.Object))
		return Dynamic(data.Object, data.FunctionName.ToString()).ColorArr;
	
	struct{ TArray<FLinearColor> RetVal; } params;
	if(!ProcessFunction(data, &params))
		return GetColorArrayFromProperty(data);
	return params.RetVal;
}

TArray<FLinearColor> UWiremodReflection::GetColorArrayFromProperty(const FNewConnectionData& data)
{
	auto val = GetProperty<FArrayProperty>(data);
	if(!val) return TArray<FLinearColor>();
	return *val->ContainerPtrToValuePtr<TArray<FLinearColor>>(data.Object);
}

TArray<UFGInventoryComponent*> UWiremodReflection::GetInventoryArray(const FNewConnectionData& data)
{
	if(data.FromProperty)
		return GetInventoryArrayFromProperty(data);
	
	if(IsDynamic(data.Object))
		return Dynamic(data.Object, data.FunctionName.ToString()).InventoryArr;
	
	struct{ TArray<UFGInventoryComponent*> RetVal; } params;
	if(!ProcessFunction(data, &params))
		return GetInventoryArrayFromProperty(data);
	return params.RetVal;
}

TArray<UFGInventoryComponent*> UWiremodReflection::GetInventoryArrayFromProperty(const FNewConnectionData& data)
{
	auto val = GetProperty<FArrayProperty>(data);
	if(!val) return TArray<UFGInventoryComponent*>();
	return *val->ContainerPtrToValuePtr<TArray<UFGInventoryComponent*>>(data.Object);
}

TArray<FInventoryStack> UWiremodReflection::GetItemStackArray(const FNewConnectionData& data)
{
	if(data.FromProperty)
		return GetItemStackArrayFromProperty(data);
	
	if(IsDynamic(data.Object))
		return Dynamic(data.Object, data.FunctionName.ToString()).StackArr;
	
	struct{ TArray<FInventoryStack> RetVal; } params;
	if(!ProcessFunction(data, &params))
		return GetItemStackArrayFromProperty(data);
	return params.RetVal;
}

TArray<FInventoryStack> UWiremodReflection::GetItemStackArrayFromProperty(const FNewConnectionData& data)
{
	auto val = GetProperty<FArrayProperty>(data);
	if(!val) return TArray<FInventoryStack>();
	return *val->ContainerPtrToValuePtr<TArray<FInventoryStack>>(data.Object);
}

TArray<AActor*> UWiremodReflection::GetEntityArray(const FNewConnectionData& data)
{
	if(data.FromProperty)
		return GetEntityArrayFromProperty(data);
	
	if(IsDynamic(data.Object))
		return Dynamic(data.Object, data.FunctionName.ToString()).EntityArr;

	struct{ TArray<AActor*> RetVal; } params;
	if(!ProcessFunction(data, &params))
		return GetEntityArrayFromProperty(data);
	return params.RetVal;
}

TArray<AActor*> UWiremodReflection::GetEntityArrayFromProperty(const FNewConnectionData& data)
{
	auto val = GetProperty<FArrayProperty>(data);
	if(!val) return TArray<AActor*>();
	return *val->ContainerPtrToValuePtr<TArray<AActor*>>(data.Object);
}

TArray<UFGPowerCircuit*> UWiremodReflection::GetPowerGridArray(const FNewConnectionData& data)
{
	if(data.FromProperty)
		return GetPowerGridArrayFromProperty(data);
	
	if(IsDynamic(data.Object))
		return Dynamic(data.Object, data.FunctionName.ToString()).PowerGridArr;

	struct{ TArray<UFGPowerCircuit*> RetVal; } params;
	if(!ProcessFunction(data, &params))
		return GetPowerGridArrayFromProperty(data);
	return params.RetVal;
}

TArray<UFGPowerCircuit*> UWiremodReflection::GetPowerGridArrayFromProperty(const FNewConnectionData& data)
{
	auto val = GetProperty<FArrayProperty>(data);
	if(!val) return TArray<UFGPowerCircuit*>();
	return *val->ContainerPtrToValuePtr<TArray<UFGPowerCircuit*>>(data.Object);
}

TArray< TSubclassOf<UFGRecipe> > UWiremodReflection::GetRecipeArray(const FNewConnectionData& data)
{
	if(data.FromProperty)
		return GetRecipeArrayFromProperty(data);
	
	if(IsDynamic(data.Object))
		return Dynamic(data.Object, data.FunctionName.ToString()).RecipeArr;

	struct{TArray< TSubclassOf<UFGRecipe> > RetVal; } params{};
	if(!ProcessFunction(data, &params))
		return GetRecipeArrayFromProperty(data);
	return params.RetVal;
}

TArray<TSubclassOf<UFGRecipe>> UWiremodReflection::GetRecipeArrayFromProperty(const FNewConnectionData& data)
{
	auto val = GetProperty<FArrayProperty>(data);
	if(!val) return TArray<TSubclassOf<UFGRecipe>>();
	return *val->ContainerPtrToValuePtr<TArray<TSubclassOf<UFGRecipe>>>(data.Object);
}


void UWiremodReflection::SetFunctionBoolValue(const FNewConnectionData& data, bool value_)
{
	
	if(data.FunctionName == "WM_FLUSHTANK_FUNC" && value_)
	{
		if(auto fluidTank = Cast<AFGBuildablePipeReservoir>(data.Object))
		{
			fluidTank->mFluidBox.Reset();
			return;
		}
	}
	else if(data.FunctionName == "WM_DOORCONTROL_FUNC")
	{
		if(auto door = Cast<AFGBuildableDoor>(data.Object))
		{
			if(value_)
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
	else if(data.FunctionName == "WM_ON_USE_FUNC" && value_)
	{
		if(auto object = Cast<AFGBuildable>(data.Object))
		{
			if(auto character = Cast<AFGCharacterPlayer>(object->GetWorld()->GetFirstPlayerController()->GetCharacter()))
			{
				auto useState = FUseState();
				useState.UseLocation = object->GetTransform().GetLocation();
				AFGBuildable::Execute_OnUse(object, character, useState);

				return;
			}
		}
	}
	
	if(auto panel = Cast<AFGBuildableLightsControlPanel>(data.Object))
	{
		
		if(data.FunctionName == "IsLightEnabled")
		{
			if(panel->IsLightEnabled() == value_) return;
			
			panel->SetLightEnabled(value_);
			auto affected = panel->GetControlledBuildables(AFGBuildableLightSource::StaticClass());
			for (auto light : affected)
			{
				Cast<AFGBuildableLightSource>(light)->SetLightEnabled(value_);
			}
		}
		else
		{
			auto panelData = panel->GetLightControlData();
			if(panelData.IsTimeOfDayAware == value_) return;
			
			panelData.IsTimeOfDayAware = value_;
			panel->SetLightControlData(panelData);
			
			auto affected = panel->GetControlledBuildables(AFGBuildableLightSource::StaticClass());
			for (auto light : affected)
			{
				Cast<AFGBuildableLightSource>(light)->SetLightControlData(panelData);
			}
		}
		

		return;
	}
	
	
	struct{bool val;} params{value_};
	ProcessFunction(data, &params);
}

void UWiremodReflection::SetFunctionStringValue(const FNewConnectionData& data, FString value_)
{
	if(auto sign = Cast<AFGBuildableWidgetSign>(data.Object))
	{
		FPrefabSignData signData;

		sign->GetSignPrefabData(signData);
		
		if(signData.TextElementData.Contains(data.FunctionName.ToString()))
			if(signData.TextElementData[data.FunctionName.ToString()] == value_) return;
		
		signData.TextElementData[data.FunctionName.ToString()] = value_;
		sign->SetPrefabSignData(signData);
		
		return;
	}
	else if(auto station = Cast<AFGBuildableRailroadStation>(data.Object))
	{
		station->GetStationIdentifier()->SetStationName(FText::FromString(value_));
		return;
	}
	
	struct { FString val; } params{value_};
	ProcessFunction(data, &params);
}

void UWiremodReflection::SetFunctionNumberValue(const FNewConnectionData& data, float value_)
{
	if(data.FunctionName == "WM_RAILSWITCH_FUNC")
	{
		if(auto railSwitch = Cast<AFGBuildableRailroadSwitchControl>(data.Object))
		{
			//If switch is not in the position that we want, switch it to the next one.
			if(railSwitch->GetSwitchPosition() != trunc(value_)) railSwitch->ToggleSwitchPosition();
			return;
		}
	}
	else if(auto panel = Cast<AFGBuildableLightsControlPanel>(data.Object))
	{
		auto panelData = panel->GetLightControlData();

		if(data.FunctionName == "ColorSlotIndex")
		{
			if(panelData.ColorSlotIndex == trunc(value_)) return;
			
			panelData.ColorSlotIndex = trunc(value_);
		}
		else
		{
			if(panelData.Intensity == value_) return;
			
			panelData.Intensity = value_;
		}

		panel->SetLightControlData(panelData);

		auto affected = panel->GetControlledBuildables(AFGBuildableLightSource::StaticClass());

		for (auto light : affected)
			Cast<AFGBuildableLightSource>(light)->SetLightControlData(panelData);
		
		
		return;
	}
	else if (auto sign = Cast<AFGBuildableWidgetSign>(data.Object))
	{
		FPrefabSignData signData;
		sign->GetSignPrefabData(signData);
		
		if(data.FunctionName == "Emissive")
		{
			if(signData.Emissive == value_) return;
			signData.Emissive = value_;
		}
		else if(data.FunctionName == "Glossiness")
		{
			if(signData.Glossiness == value_) return;
			signData.Glossiness = value_;
		}
		else
		{
			if(signData.IconElementData.Contains(data.FunctionName.ToString()))
				if(signData.IconElementData[data.FunctionName.ToString()] == trunc(value_)) return;
			
			signData.IconElementData[data.FunctionName.ToString()] = trunc(value_);
		}

		sign->SetPrefabSignData(signData);
		return;
	}
	else if(data.FunctionName == "SetPendingPotential")
	{
		if(auto building = Cast<AFGBuildableFactory>(data.Object))
		{
			if (building->GetPendingPotential() == value_) return;
		}
	}

	if(IsInteger(data))
	{
		struct {int val;} params {(int)value_};
		ProcessFunction(data, &params);
	}
	else
	{
		struct {float val;} params {value_};
		ProcessFunction(data, &params);
	}
}

void UWiremodReflection::SetFunctionColorValue(const FNewConnectionData& data, FLinearColor value_)
{
	if(auto sign = Cast<AFGBuildableWidgetSign>(data.Object))
	{

		FPrefabSignData signData;
		sign->GetSignPrefabData(signData);
		
		if(data.FunctionName == "TextColor")
		{
			if(signData.ForegroundColor == value_) return;
			signData.ForegroundColor = value_;
		}
		else if(data.FunctionName == "BackgroundColor")
		{
			if(signData.BackgroundColor == value_) return;
			signData.BackgroundColor = value_;
		}
		else if(data.FunctionName == "AuxColor")
		{
			if(signData.AuxiliaryColor == value_) return;
			signData.AuxiliaryColor = value_;
		}

		sign->SetPrefabSignData(signData);
		return;
	}
	
	
	struct {FLinearColor val;} params {value_};
	ProcessFunction(data, &params);
}

void UWiremodReflection::SetFunctionRecipeValue(const FNewConnectionData& data, TSubclassOf<UFGRecipe> value_)
{
	if(!value_.GetDefaultObject()) return;
	
	struct{TSubclassOf<UFGRecipe> val;} params{TSubclassOf<UFGRecipe>(value_.GetDefaultObject()->GetClass())};
	ProcessFunction(data, &params);
}


void UWiremodReflection::HandleDynamicConnection(const FNewConnectionData& transmitter, const FNewConnectionData& receiver)
{
	switch (receiver.ConnectionType)
	{
	case Boolean:SetFunctionBoolValue(receiver, GetFunctionBoolResult(transmitter));break;
	case Number:
	case Integer:SetFunctionNumberValue(receiver, GetFunctionNumberResult(transmitter));break;
	case String:SetFunctionStringValue(receiver, GetFunctionStringResult(transmitter));break;
	case Color:SetFunctionColorValue(receiver, GetFunctionColorResult(transmitter));break;
	case Recipe: SetFunctionRecipeValue(receiver, GetFunctionRecipeResult(transmitter)); break;
	default: break;
	}
}


void UWiremodReflection::FillDynamicStructFromData(const FNewConnectionData& data, FDynamicValue& ReturnValue)
{
	if(!IsValid(data.Object)) return;
	ReturnValue.ConnectionType = data.ConnectionType;
	switch (data.ConnectionType)
	{
	case Unknown: break;
	case Boolean:ReturnValue.StoredBool = GetFunctionBoolResult(data);break;
	case Number:
	case Integer:ReturnValue.StoredFloat = GetFunctionNumberResult(data); break;
	case String: ReturnValue.StoredString = GetFunctionStringResult(data); break;
	case Vector: ReturnValue.StoredVector = GetFunctionVectorResult(data); break;
	case Inventory: ReturnValue.Inventory = GetFunctionInventory(data); break;
	case PowerGrid: ReturnValue.PowerGrid = GetFunctionPowerCircuitResult(data);break;
	case Entity: ReturnValue.Entity = GetFunctionEntityResult(data);break;
	case Color: ReturnValue.StoredColor = GetFunctionColorResult(data);break;
	case ArrayOfBoolean: ReturnValue.BoolArr = GetBoolArray(data); break;
	case ArrayOfNumber: ReturnValue.NumberArr = GetNumberArray(data); break;
	case ArrayOfString: ReturnValue.StringArr = GetStringArray(data); break;
	case ArrayOfVector: ReturnValue.VectorArr = GetVectorArray(data); break;
	case ArrayOfEntity: ReturnValue.EntityArr = GetEntityArray(data); break;
	case ArrayOfColor: ReturnValue.ColorArr = GetColorArray(data); break;
	case Stack: ReturnValue.Stack = GetFunctionStackResult(data); break;
	case ArrayOfStack: ReturnValue.StackArr = GetItemStackArray(data); break;
	case ArrayOfPowerGrid: ReturnValue.PowerGridArr = GetPowerGridArray(data); break;
	case ArrayOfInventory: ReturnValue.InventoryArr = GetInventoryArray(data); break;
	case Recipe: ReturnValue.Recipe = GetFunctionRecipeResult(data); break;
	case ArrayOfRecipe: ReturnValue.RecipeArr = GetRecipeArray(data); break;
	default:
		UE_LOG(LogTemp, Error, TEXT("[WIREMOD] Failed to find a switch case for EConnectionType::%d in function FILL_DYNAMIC_STRUCT"), (int)data.ConnectionType);
		break;
	}
}




