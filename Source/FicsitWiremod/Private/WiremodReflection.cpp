// Copyright Coffee Stain Studios. All Rights Reserved.


#include "WiremodReflection.h"

#include "DynamicReturnValue.h"
#include "FGBuildableDoor.h"
#include "FGGameState.h"
#include "FGPipeConnectionComponent.h"
#include "FGTrainStationIdentifier.h"
#include "FGVersionFunctionLibrary.h"
#include "IConstantsDistributor.h"
#include "Buildables/FGBuildableLightsControlPanel.h"
#include "Buildables/FGBuildablePipeReservoir.h"
#include "Buildables/FGBuildableRailroadSignal.h"
#include "Buildables/FGBuildableRailroadStation.h"
#include "Buildables/FGBuildableWidgetSign.h"
#include "Components/WidgetComponent.h"
#include "DynamicValue/BoolArrayDynamicValue.h"
#include "DynamicValue/BoolDynamicValue.h"
#include "DynamicValue/ColorArrayDynamicValue.h"
#include "DynamicValue/ColorDynamicValue.h"
#include "DynamicValue/EntityArrayDynamicValue.h"
#include "DynamicValue/EntityDynamicValue.h"
#include "DynamicValue/InventoryArrayDynamicValue.h"
#include "DynamicValue/InventoryDynamicValue.h"
#include "DynamicValue/ItemStackArrayDynamicValue.h"
#include "DynamicValue/ItemStackDynamicValue.h"
#include "DynamicValue/NumberArrayDynamicValue.h"
#include "DynamicValue/NumberDynamicValue.h"
#include "DynamicValue/PowerGridArrayDynamicValue.h"
#include "DynamicValue/PowerGridDynamicValue.h"
#include "DynamicValue/StringArrayDynamicValue.h"
#include "DynamicValue/StringDynamicValue.h"
#include "DynamicValue/VectorArrayDynamicValue.h"
#include "DynamicValue/VectorDynamicValue.h"
#include "UI/FGSignBuildingWidget.h"



bool UWiremodReflection::GetFunctionBoolResult(const FNewConnectionData& data, bool defaultValue)
{
	if(!data.Object) return defaultValue;
	if(!IsValid(data.Object)) return defaultValue;
	
	if(data.FunctionName.ToString() == "WM_GET_SELECT_FUNC") return GetFunctionBoolResult(GetRecursiveData(data), defaultValue);
	
	if(data.Object->GetClass()->ImplementsInterface(IIConstantsDistributor::UClassType::StaticClass()))
	{
		auto rawValue = IIConstantsDistributor::Execute_GetValue(data.Object, data.FunctionName.ToString());
		return rawValue.StoredBool;
	}
	else if(data.FunctionName == "WM_DOORCONTROL_FUNC")
	{
		if(auto door = Cast<AFGBuildableDoor>(data.Object))
		{
			return door->mDoorState == EDoorState::DS_Closed;
		}
	}
	else if(data.Object->GetClass()->ImplementsInterface(IDynamicReturnValue::UClassType::StaticClass()))
	{
		auto rawValue = IDynamicReturnValue::Execute_GetValue(data.Object, data.FunctionName.ToString());
		if(auto castValue = Cast<UBoolDynamicValue>(rawValue)) return castValue->Value;
	}
	else if(auto panel = Cast<AFGBuildableLightsControlPanel>(data.Object))
	{

		if(data.FunctionName == "IsLightEnabled") return panel->IsLightEnabled();
		else return panel->GetLightControlData().IsTimeOfDayAware;
	}


	
	auto function = GetFunction(data);
	if(!function) return defaultValue;

	struct{bool RetVal;} params;
	data.Object->ProcessEvent(function, &params);
	
	return params.RetVal;
}

FString UWiremodReflection::GetFunctionStringResult(const FNewConnectionData& data, FString defaultValue)
{
	if(!data.Object) return defaultValue;
	if(!IsValid(data.Object)) return defaultValue;
	
	if(data.FunctionName.ToString() == "WM_GET_SELECT_FUNC") return GetFunctionStringResult(GetRecursiveData(data), defaultValue);

	if(data.Object->GetClass()->ImplementsInterface(IIConstantsDistributor::UClassType::StaticClass()))
	{
		auto rawValue = IIConstantsDistributor::Execute_GetValue(data.Object, data.FunctionName.ToString());
		return rawValue.StoredString;
	}
	else if(data.Object->GetClass()->ImplementsInterface(IDynamicReturnValue::UClassType::StaticClass()))
	{
		auto rawValue = IDynamicReturnValue::Execute_GetValue(data.Object, data.FunctionName.ToString());
		if(auto castValue = Cast<UStringDynamicValue>(rawValue)) return castValue->Value;
	}
	else if(auto sign = Cast<AFGBuildableWidgetSign>(data.Object))
	{
		if(sign->mTextElementToDataMap.Contains(data.FunctionName.ToString()))
			return sign->mTextElementToDataMap[data.FunctionName.ToString()];
		else return defaultValue;
	}
	else if(auto station = Cast<AFGBuildableRailroadStation>(data.Object))
	{
		return station->GetStationIdentifier()->GetStationName().ToString();
	}
	
	
	auto function = GetFunction(data);
	if(!function) return defaultValue;

	struct{FString RetVal;} params;
	data.Object->ProcessEvent(function, &params);

	
	return params.RetVal;
}

float UWiremodReflection::GetFunctionNumberResult(const FNewConnectionData& data, float defaultValue)
{
	if(!data.Object) return defaultValue;
	if(!IsValid(data.Object)) return defaultValue;
	
	if(data.FunctionName.ToString() == "WM_GET_SELECT_FUNC") return GetFunctionNumberResult(GetRecursiveData(data), defaultValue);

	if(data.Object->GetClass()->ImplementsInterface(IIConstantsDistributor::UClassType::StaticClass()))
	{
		auto rawValue = IIConstantsDistributor::Execute_GetValue(data.Object, data.FunctionName.ToString());
		return rawValue.StoredFloat;
	}
	else if(data.Object->GetClass()->ImplementsInterface(IDynamicReturnValue::UClassType::StaticClass()))
	{
		auto rawValue = IDynamicReturnValue::Execute_GetValue(data.Object, data.FunctionName.ToString());
		if(auto castValue = Cast<UNumberDynamicValue>(rawValue)) return castValue->Value;
	}
	
	if(auto panel = Cast<AFGBuildableLightsControlPanel>(data.Object))
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
	else if(auto station = Cast<AFGBuildableTrainPlatform>(data.Object))
	{
		return (float)station->GetDockingStatus();
	}
	else if(auto roadSignal = Cast<AFGBuildableRailroadSignal>(data.Object))
	{
		if(data.FunctionName.ToString() == "GetAspect") return (float)roadSignal->GetAspect();
		else return (float)roadSignal->GetBlockValidation();
	}
	
	
	auto function = GetFunction(data);
	if(!function) return defaultValue;

	if(data.ConnectionType.GetValue() == Integer)
	{
		struct{int RetVal;} params;

		data.Object->ProcessEvent(function, &params);
	
		return (float)params.RetVal;
	}
	else
	{
		struct{float RetVal;} params;

		data.Object->ProcessEvent(function, &params);
	
		return params.RetVal;
	}
}

FVector UWiremodReflection::GetFunctionVectorResult(const FNewConnectionData& data, FVector defaultValue)
{
	if(!data.Object) return defaultValue;
	if(!IsValid(data.Object)) return defaultValue;
	
	if(data.FunctionName.ToString() == "WM_GET_SELECT_FUNC") return GetFunctionVectorResult(GetRecursiveData(data), defaultValue);
	
	if(data.Object->GetClass()->ImplementsInterface(IIConstantsDistributor::UClassType::StaticClass()))
	{
		auto rawValue = IIConstantsDistributor::Execute_GetValue(data.Object, data.FunctionName.ToString());
		return rawValue.StoredVector;
	}
	else if(data.Object->GetClass()->ImplementsInterface(IDynamicReturnValue::UClassType::StaticClass()))
	{
		auto rawValue = IDynamicReturnValue::Execute_GetValue(data.Object, data.FunctionName.ToString());
		if(auto castValue = Cast<UVectorDynamicValue>(rawValue)) return castValue->Value;
	}

	auto function = GetFunction(data);
	if(!function) return defaultValue;
	
	struct{FVector RetVal;} params;
	data.Object->ProcessEvent(function, &params);
	
	return params.RetVal;
}

FLinearColor UWiremodReflection::GetFunctionColorResult(const FNewConnectionData& data, FLinearColor defaultValue)
{
	if(!data.Object) return defaultValue;
	if(!IsValid(data.Object)) return defaultValue;
	
	if(data.FunctionName.ToString() == "WM_GET_SELECT_FUNC") return GetFunctionColorResult(GetRecursiveData(data), defaultValue);

	if(data.Object->GetClass()->ImplementsInterface(IIConstantsDistributor::UClassType::StaticClass()))
	{
		auto rawValue = IIConstantsDistributor::Execute_GetValue(data.Object, data.FunctionName.ToString());
		return rawValue.StoredColor;
	}
	else if(data.Object->GetClass()->ImplementsInterface(IDynamicReturnValue::UClassType::StaticClass()))
	{
		auto rawValue = IDynamicReturnValue::Execute_GetValue(data.Object, data.FunctionName.ToString());
		if(auto castValue = Cast<UColorDynamicValue>(rawValue)) return castValue->Value;
	}
	
	if(auto sign = Cast<AFGBuildableWidgetSign>(data.Object))
	{
		if(data.FunctionName == "TextColor") return sign->mForegroundColor;
		if(data.FunctionName == "BackgroundColor") return sign->mBackgroundColor;
		if(data.FunctionName == "AuxColor") return sign->mAuxilaryColor;
		
		
		return defaultValue;
	}
	
	auto function = GetFunction(data);
	if(!function) return defaultValue;

	struct{FLinearColor RetVal;} params;
	data.Object->ProcessEvent(function, &params);
	
	return params.RetVal;
}

UFGInventoryComponent* UWiremodReflection::GetFunctionInventory(const FNewConnectionData& data)
{
	if(!data.Object) return nullptr;
	if(!IsValid(data.Object)) return nullptr;
	
	if(data.FunctionName.ToString() == "WM_GET_SELECT_FUNC") return GetFunctionInventory(GetRecursiveData(data));
	else if(data.Object->GetClass()->ImplementsInterface(IDynamicReturnValue::UClassType::StaticClass()))
	{
		auto rawValue = IDynamicReturnValue::Execute_GetValue(data.Object, data.FunctionName.ToString());
		if(auto castValue = Cast<UInventoryDynamicValue>(rawValue)) return castValue->Value;
	}
	
	auto function = GetFunction(data);
	if(!function) return nullptr;
	
	struct{ UFGInventoryComponent* RetVal; } params;
	data.Object->ProcessEvent(function, &params);
	
	return params.RetVal;
}

FInventoryStack UWiremodReflection::GetFunctionStackResult(const FNewConnectionData& data)
{
	FInventoryStack defaultValue = FInventoryStack();
	
	if(!data.Object) return defaultValue;
	if(!IsValid(data.Object)) return defaultValue;
	
	if(data.FunctionName.ToString() == "WM_GET_SELECT_FUNC") return GetFunctionStackResult(GetRecursiveData(data));
	else if(data.Object->GetClass()->ImplementsInterface(IDynamicReturnValue::UClassType::StaticClass()))
	{
		auto rawValue = IDynamicReturnValue::Execute_GetValue(data.Object, data.FunctionName.ToString());
		if(auto castValue = Cast<UItemStackDynamicValue>(rawValue)) return castValue->Value;
	}
    	
	auto function = GetFunction(data);
	if(!function) return defaultValue;
    
	struct{
		FInventoryStack RetVal;
	} params;
    
	data.Object->ProcessEvent(function, &params);
	
	return params.RetVal;
}

UFGPowerCircuit* UWiremodReflection::GetFunctionPowerCircuitResult(const FNewConnectionData& data)
{
	UFGPowerCircuit* defaultValue = nullptr;
	
	if(!data.Object) return defaultValue;
	if(!IsValid(data.Object)) return defaultValue;
	
	if(data.FunctionName.ToString() == "WM_GET_SELECT_FUNC") return GetFunctionPowerCircuitResult(GetRecursiveData(data));
	else if(data.Object->GetClass()->ImplementsInterface(IDynamicReturnValue::UClassType::StaticClass()))
	{
		auto rawValue = IDynamicReturnValue::Execute_GetValue(data.Object, data.FunctionName.ToString());
		if(auto castValue = Cast<UPowerGridDynamicValue>(rawValue)) return castValue->Value;
	}
    	
	auto function = GetFunction(data);
	if(!function) return defaultValue;


	struct{ UFGPowerCircuit* RetVal; } params;
    
	data.Object->ProcessEvent(function, &params);
	
	return params.RetVal;
	
}

AActor* UWiremodReflection::GetFunctionEntityResult(const FNewConnectionData& data)
{
	AActor* defaultValue = nullptr;

	if(!data.Object) return defaultValue;
	if(!IsValid(data.Object)) return defaultValue;
	
	if(data.FunctionName.ToString() == "WM_GET_SELECT_FUNC") return GetFunctionEntityResult(GetRecursiveData(data));
	else if(data.Object->GetClass()->ImplementsInterface(IDynamicReturnValue::UClassType::StaticClass()))
	{
		auto rawValue = IDynamicReturnValue::Execute_GetValue(data.Object, data.FunctionName.ToString());
		if(auto castValue = Cast<UEntityDynamicValue>(rawValue)) return castValue->Value;
	}

	if(data.FunctionName == "Self") return Cast<AActor>(data.Object);
	
	auto function = GetFunction(data);
	if(!function) return defaultValue;
	
	struct{ AActor* RetVal; } params;
    
	data.Object->ProcessEvent(function, &params);
	
	return params.RetVal;
}


//Array Get


TArray<bool> UWiremodReflection::GetBoolArray(const FNewConnectionData& data)
{
	TArray<bool> defaultValue = *new TArray<bool>;
	if(!data.Object) return defaultValue;
	if(!IsValid(data.Object)) return defaultValue;

	if(data.FunctionName.ToString() == "WM_GET_SELECT_FUNC") return GetBoolArray(GetRecursiveData(data));
	else if(data.Object->GetClass()->ImplementsInterface(IDynamicReturnValue::UClassType::StaticClass()))
	{
		auto rawValue = IDynamicReturnValue::Execute_GetValue(data.Object, data.FunctionName.ToString());
		if(auto castValue = Cast<UBoolArrayDynamicValue>(rawValue)) return castValue->Value;
	}

		
	auto function = GetFunction(data);
	if(!function) return defaultValue;

	struct{ TArray<bool> RetVal; } params;

	data.Object->ProcessEvent(function, &params);
	
	return params.RetVal;
}

TArray<FString> UWiremodReflection::GetStringArray(const FNewConnectionData& data)
{
	TArray<FString> defaultValue = TArray<FString>();
	if(!data.Object) return defaultValue;
	if(!IsValid(data.Object)) return defaultValue;

	if(data.FunctionName.ToString() == "WM_GET_SELECT_FUNC") return GetStringArray(GetRecursiveData(data));
	else if(data.Object->GetClass()->ImplementsInterface(IDynamicReturnValue::UClassType::StaticClass()))
	{
		auto rawValue = IDynamicReturnValue::Execute_GetValue(data.Object, data.FunctionName.ToString());
		if(auto castValue = Cast<UStringArrayDynamicValue>(rawValue)) return castValue->Value;
	}
		
	auto function = GetFunction(data);
	if(!function) return defaultValue;

	struct{ TArray<FString> RetVal; } params;

	data.Object->ProcessEvent(function, &params);
	
	return params.RetVal;
}

TArray<float> UWiremodReflection::GetNumberArray(const FNewConnectionData& data)
{
	TArray<float> defaultValue = TArray<float>();
	if(!data.Object) return defaultValue;
	if(!IsValid(data.Object)) return defaultValue;
	
	if(data.FunctionName.ToString() == "WM_GET_SELECT_FUNC") return GetNumberArray(GetRecursiveData(data));
	else if(data.Object->GetClass()->ImplementsInterface(IDynamicReturnValue::UClassType::StaticClass()))
	{
		auto rawValue = IDynamicReturnValue::Execute_GetValue(data.Object, data.FunctionName.ToString());
		if(auto castValue = Cast<UNumberArrayDynamicValue>(rawValue)) return castValue->Value;
	}
		
	auto function = GetFunction(data);
	if(!function) return defaultValue;

	struct{ TArray<float> RetVal; } params;

	data.Object->ProcessEvent(function, &params);
	
	return params.RetVal;
}
	
TArray<FVector> UWiremodReflection::GetVectorArray(const FNewConnectionData& data)
{
	auto defaultValue = TArray<FVector>();
	if(!data.Object) return defaultValue;
	if(!IsValid(data.Object)) return defaultValue;
	
	if(data.FunctionName.ToString() == "WM_GET_SELECT_FUNC") return GetVectorArray(GetRecursiveData(data));
	else if(data.Object->GetClass()->ImplementsInterface(IDynamicReturnValue::UClassType::StaticClass()))
	{
		auto rawValue = IDynamicReturnValue::Execute_GetValue(data.Object, data.FunctionName.ToString());
		if(auto castValue = Cast<UVectorArrayDynamicValue>(rawValue)) return castValue->Value;
	}

	
	auto function = GetFunction(data);
	if(!function) return defaultValue;

	struct{ TArray<FVector> RetVal; } params;
	data.Object->ProcessEvent(function, &params);
	
	return params.RetVal;
}

TArray<FLinearColor> UWiremodReflection::GetColorArray(const FNewConnectionData& data)
{
	auto defaultValue = TArray<FLinearColor>();
	if(!data.Object) return defaultValue;
	if(!IsValid(data.Object)) return defaultValue;
	
	if(data.FunctionName.ToString() == "WM_GET_SELECT_FUNC") return GetColorArray(GetRecursiveData(data));
	else if(data.Object->GetClass()->ImplementsInterface(IDynamicReturnValue::UClassType::StaticClass()))
	{
		auto rawValue = IDynamicReturnValue::Execute_GetValue(data.Object, data.FunctionName.ToString());
		if(auto castValue = Cast<UColorArrayDynamicValue>(rawValue)) return castValue->Value;
	}

		
	
	auto function = GetFunction(data);
	if(!function) return defaultValue;

	struct{ TArray<FLinearColor> RetVal; } params;
	data.Object->ProcessEvent(function, &params);
	
	return params.RetVal;
}

TArray<UFGInventoryComponent*> UWiremodReflection::GetInventoryArray(const FNewConnectionData& data)
{
	auto defaultValue = TArray<UFGInventoryComponent*>();
	if(!data.Object) return defaultValue;
	if(!IsValid(data.Object)) return defaultValue;
	
	if(data.FunctionName.ToString() == "WM_GET_SELECT_FUNC") return GetInventoryArray(GetRecursiveData(data));
	else if(data.Object->GetClass()->ImplementsInterface(IDynamicReturnValue::UClassType::StaticClass()))
	{
		auto rawValue = IDynamicReturnValue::Execute_GetValue(data.Object, data.FunctionName.ToString());
		if(auto castValue = Cast<UInventoryArrayDynamicValue>(rawValue)) return castValue->Value;
	}
		
	
	auto function = GetFunction(data);
	if(!function) return defaultValue;

	struct{ TArray<UFGInventoryComponent*> RetVal; } params;
	data.Object->ProcessEvent(function, &params);
	
	return params.RetVal;
}

TArray<FInventoryStack> UWiremodReflection::GetItemStackArray(const FNewConnectionData& data)
{
	auto defaultValue = TArray<FInventoryStack>();
	if(!data.Object) return defaultValue;
	if(!IsValid(data.Object)) return defaultValue;
	
	if(data.FunctionName.ToString() == "WM_GET_SELECT_FUNC") return GetItemStackArray(GetRecursiveData(data));
	else if(data.Object->GetClass()->ImplementsInterface(IDynamicReturnValue::UClassType::StaticClass()))
	{
		auto rawValue = IDynamicReturnValue::Execute_GetValue(data.Object, data.FunctionName.ToString());
		if(auto castValue = Cast<UItemStackArrayDynamicValue>(rawValue)) return castValue->Value;
	}
	
	auto function = GetFunction(data);
	if(!function) return defaultValue;
    
	struct{ TArray<FInventoryStack> RetVal; } params;
	data.Object->ProcessEvent(function, &params);
	
	return params.RetVal;
}

TArray<AActor*> UWiremodReflection::GetEntityArray(const FNewConnectionData& data)
{
	auto defaultValue = TArray<AActor*>();
	if(!data.Object) return defaultValue;
	if(!IsValid(data.Object)) return defaultValue;
	
	if(data.FunctionName.ToString() == "WM_GET_SELECT_FUNC") return GetEntityArray(GetRecursiveData(data));
	else if(data.Object->GetClass()->ImplementsInterface(IDynamicReturnValue::UClassType::StaticClass()))
	{
		auto rawValue = IDynamicReturnValue::Execute_GetValue(data.Object, data.FunctionName.ToString());
		if(auto castValue = Cast<UEntityArrayDynamicValue>(rawValue)) return castValue->Value;
	}
	
	auto function = GetFunction(data);
	if(!function) return defaultValue;

	struct{ TArray<AActor*> RetVal; } params;
	data.Object->ProcessEvent(function, &params);
	
	return params.RetVal;
}

TArray<UFGPowerCircuit*> UWiremodReflection::GetPowerGridArray(const FNewConnectionData& data)
{
	auto defaultValue = TArray<UFGPowerCircuit*>();
	if(!data.Object) return defaultValue;
	if(!IsValid(data.Object)) return defaultValue;
	
	if(data.FunctionName.ToString() == "WM_GET_SELECT_FUNC") return GetPowerGridArray(GetRecursiveData(data));
	else if(data.Object->GetClass()->ImplementsInterface(IDynamicReturnValue::UClassType::StaticClass()))
	{
		auto rawValue = IDynamicReturnValue::Execute_GetValue(data.Object, data.FunctionName.ToString());
		if(auto castValue = Cast<UPowerGridArrayDynamicValue>(rawValue)) return castValue->Value;
	}
	
	auto function = GetFunction(data);
	if(!function) return defaultValue;

	struct{ TArray<UFGPowerCircuit*> RetVal; } params;
	data.Object->ProcessEvent(function, &params);
	
	return params.RetVal;
}




void UWiremodReflection::SetFunctionBoolValue(const FNewConnectionData& data, bool value_)
{
	if(!data.Object) return;
	if(!IsValid(data.Object)) return;

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

		auto panelData = panel->GetLightControlData();
		
		if(data.FunctionName == "IsLightEnabled")
		{
			if(panel->IsLightEnabled() == value_) return;
			
			panel->SetLightEnabled(value_);
			auto affected = panel->GetControlledBuildables(AFGBuildableLightSource::StaticClass());
			for (auto light : affected)
			{
				auto lightCasted = Cast<AFGBuildableLightSource>(light);
				lightCasted->SetLightEnabled(value_);
			}
		}
		else
		{
			if(panelData.IsTimeOfDayAware == value_) return;
			
			panelData.IsTimeOfDayAware = value_;
			panel->SetLightControlData(panelData);
			
			auto affected = panel->GetControlledBuildables(AFGBuildableLightSource::StaticClass());
			for (auto light : affected)
			{
				auto lightCasted = Cast<AFGBuildableLightSource>(light);
				lightCasted->SetLightControlData(panelData);
			}
		}
		

		return;
	}
	


	
	auto function = GetFunction(data);
	if(!function) return;
	
	struct
	{
		bool val;
	} params{value_};
	
	data.Object->ProcessEvent(function, &params);
}

void UWiremodReflection::SetFunctionStringValue(const FNewConnectionData& data, FString value_)
{
	if(!data.Object) return;
	if(!IsValid(data.Object)) return;

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
	
	auto function = GetFunction(data);
	if(!function) return;
	
	struct { FString val; } params{value_};
	
	
	data.Object->ProcessEvent(function, &params);
}

void UWiremodReflection::SetFunctionNumberValue(const FNewConnectionData& data, float value_)
{
	if(!data.Object) return;
	if(!IsValid(data.Object)) return;

	if(auto panel = Cast<AFGBuildableLightsControlPanel>(data.Object))
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

	
	
	auto function = GetFunction(data);
	if(!function) return;

	if(data.ConnectionType.GetValue() == Integer)
	{
		struct {
			int val;
		} params {(int)value_};

		data.Object->ProcessEvent(function, &params);
	}
	else
	{
		struct {
			float val;
		} params {value_};
	
		data.Object->ProcessEvent(function, &params);
	}
}

void UWiremodReflection::SetFunctionColorValue(const FNewConnectionData& data, FLinearColor value_)
{

	if(!data.Object) return;
	if(!IsValid(data.Object)) return;
	

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


	auto function = GetFunction(data);
	if(!function) return;
	
	struct {
		FLinearColor val;
	} params {value_};
	
	data.Object->ProcessEvent(function, &params);
}


void UWiremodReflection::HandleDynamicConnection(const FNewConnectionData& transmitter, const FNewConnectionData& receiver)
{
	switch (receiver.ConnectionType)
	{
	case Boolean:
		SetFunctionBoolValue(receiver, GetFunctionBoolResult(transmitter));
		break;
	case Number:
		SetFunctionNumberValue(receiver, GetFunctionNumberResult(transmitter));
		break;
	case String:
		SetFunctionStringValue(receiver, GetFunctionStringResult(transmitter));
		break;
	case Color:
		SetFunctionColorValue(receiver, GetFunctionColorResult(transmitter));
		break;

		default: break;
		
	}

	
}



