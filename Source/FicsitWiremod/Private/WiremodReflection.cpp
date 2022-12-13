// Copyright Coffee Stain Studios. All Rights Reserved.


#include "WiremodReflection.h"

#include "FGBuildableDoor.h"
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
#include "UI/FGSignBuildingWidget.h"

#define Dynamic IIConstantsDistributor::Execute_GetValue

static bool IsDynamic(UObject* Object)
{
	if(!Object) return false;

	return Object->GetClass()->ImplementsInterface(IIConstantsDistributor::UClassType::StaticClass());
}

bool UWiremodReflection::GetFunctionBoolResult(const FNewConnectionData& data, bool defaultValue)
{
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
	return ProcessFunction(data, params).RetVal;
}

FString UWiremodReflection::GetFunctionStringResult(const FNewConnectionData& data, FString defaultValue)
{
	
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
	return ProcessFunction(data, params).RetVal;
}


float UWiremodReflection::GetFunctionNumberResult(const FNewConnectionData& data, float defaultValue)
{
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
		return ProcessFunction(data, params).RetVal;
	}
	else
	{
		struct{float RetVal;} params{defaultValue};
		return ProcessFunction(data, params).RetVal;
	}
}

FVector UWiremodReflection::GetFunctionVectorResult(const FNewConnectionData& data, FVector defaultValue)
{
	if(IsDynamic(data.Object))
		return Dynamic(data.Object, data.FunctionName.ToString()).StoredVector;
	
	struct{FVector RetVal;} params{defaultValue};
	return ProcessFunction(data, params).RetVal;
}

FLinearColor UWiremodReflection::GetFunctionColorResult(const FNewConnectionData& data, FLinearColor defaultValue)
{
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
	return ProcessFunction(data, params).RetVal;
}

UFGInventoryComponent* UWiremodReflection::GetFunctionInventory(const FNewConnectionData& data)
{
	if(IsDynamic(data.Object))
		return Dynamic(data.Object, data.FunctionName.ToString()).Inventory;
	
	struct{ UFGInventoryComponent* RetVal; } params{};
	return ProcessFunction(data, params).RetVal;
}

FInventoryStack UWiremodReflection::GetFunctionStackResult(const FNewConnectionData& data)
{
	if(IsDynamic(data.Object))
		return Dynamic(data.Object, data.FunctionName.ToString()).Stack;
    
	struct{FInventoryStack RetVal;} params;
    return ProcessFunction(data, params).RetVal;
}

UFGPowerCircuit* UWiremodReflection::GetFunctionPowerCircuitResult(const FNewConnectionData& data)
{
	if(IsDynamic(data.Object))
		return Dynamic(data.Object, data.FunctionName.ToString()).PowerGrid;
	
	struct{ UFGPowerCircuit* RetVal; } params{};
	return ProcessFunction(data, params).RetVal;
}

AActor* UWiremodReflection::GetFunctionEntityResult(const FNewConnectionData& data)
{
	if(data.FunctionName == "Self") return Cast<AActor>(data.Object);

	if(IsDynamic(data.Object))
		return Dynamic(data.Object, data.FunctionName.ToString()).Entity;
	
	struct{ AActor* RetVal; } params{};
	return ProcessFunction(data, params).RetVal;
}


TArray<bool> UWiremodReflection::GetBoolArray(const FNewConnectionData& data)
{
	if(IsDynamic(data.Object))
		return Dynamic(data.Object, data.FunctionName.ToString()).BoolArr;
		
	struct{ TArray<bool> RetVal; } params;
	return ProcessFunction(data, params).RetVal;
}

TArray<FString> UWiremodReflection::GetStringArray(const FNewConnectionData& data)
{
	if(IsDynamic(data.Object))
		return Dynamic(data.Object, data.FunctionName.ToString()).StringArr;

	struct{ TArray<FString> RetVal; } params;
	return ProcessFunction(data, params).RetVal;
}

TArray<float> UWiremodReflection::GetNumberArray(const FNewConnectionData& data)
{
	if(IsDynamic(data.Object))
		return Dynamic(data.Object, data.FunctionName.ToString()).NumberArr;
	
	struct{ TArray<float> RetVal; } params;
	return ProcessFunction(data, params).RetVal;
}
	
TArray<FVector> UWiremodReflection::GetVectorArray(const FNewConnectionData& data)
{
	if(IsDynamic(data.Object))
		return Dynamic(data.Object, data.FunctionName.ToString()).VectorArr;
	
	struct{ TArray<FVector> RetVal; } params;
	return ProcessFunction(data, params).RetVal;
}

TArray<FLinearColor> UWiremodReflection::GetColorArray(const FNewConnectionData& data)
{
	if(IsDynamic(data.Object))
		return Dynamic(data.Object, data.FunctionName.ToString()).ColorArr;
	
	struct{ TArray<FLinearColor> RetVal; } params;
	return ProcessFunction(data, params).RetVal;
}

TArray<UFGInventoryComponent*> UWiremodReflection::GetInventoryArray(const FNewConnectionData& data)
{
	if(IsDynamic(data.Object))
		return Dynamic(data.Object, data.FunctionName.ToString()).InventoryArr;
	
	struct{ TArray<UFGInventoryComponent*> RetVal; } params;
	return ProcessFunction(data, params).RetVal;
}

TArray<FInventoryStack> UWiremodReflection::GetItemStackArray(const FNewConnectionData& data)
{
	if(IsDynamic(data.Object))
		return Dynamic(data.Object, data.FunctionName.ToString()).StackArr;
	
	struct{ TArray<FInventoryStack> RetVal; } params;
	return ProcessFunction(data, params).RetVal;
}

TArray<AActor*> UWiremodReflection::GetEntityArray(const FNewConnectionData& data)
{
	if(IsDynamic(data.Object))
		return Dynamic(data.Object, data.FunctionName.ToString()).EntityArr;

	struct{ TArray<AActor*> RetVal; } params;
	return ProcessFunction(data, params).RetVal;
}

TArray<UFGPowerCircuit*> UWiremodReflection::GetPowerGridArray(const FNewConnectionData& data)
{
	if(IsDynamic(data.Object))
		return Dynamic(data.Object, data.FunctionName.ToString()).PowerGridArr;

	struct{ TArray<UFGPowerCircuit*> RetVal; } params;
	return ProcessFunction(data, params).RetVal;
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
	ProcessFunction(data, params);
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
	ProcessFunction(data, params);
}

void UWiremodReflection::SetFunctionNumberValue(const FNewConnectionData& data, float value_)
{
	if(data.FunctionName == "WM_RAILSWITCH_FUNC")
	{
		if(auto railSwitch = Cast<AFGBuildableRailroadSwitchControl>(data.Object))
		{
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
		ProcessFunction(data, params);
	}
	else
	{
		struct {float val;} params {value_};
		ProcessFunction(data, params);
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
	ProcessFunction(data, params);
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
	default: break;
	}
}


void UWiremodReflection::FillDynamicStructFromData(const FNewConnectionData& data, FDynamicValue& ReturnValue)
{
	if(!IsValid(data.Object)) return;
	ReturnValue.ConnectionType = data.ConnectionType;
	switch (data.ConnectionType)
	{
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
	default: break;
	}
}




