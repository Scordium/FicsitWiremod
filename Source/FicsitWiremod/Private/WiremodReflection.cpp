// Copyright Coffee Stain Studios. All Rights Reserved.


#include "WiremodReflection.h"

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
#include "UI/FGSignBuildingWidget.h"


static UFunction* GetFunction(const FNewConnectionData& data)
{
	if(!data.Object) return nullptr;
	if(!IsValid(data.Object)) return nullptr;

	
	UFunction* function = data.Object->FindFunction(data.FunctionName);
	return function;
}


bool UWiremodReflection::GetFunctionBoolResult(const FNewConnectionData& data, bool defaultValue)
{
	if(data.ConnectionType.GetValue() == ConnectionData) return GetFunctionBoolResult(GetRecursiveData(data), defaultValue);


	
	if(!data.Object) return defaultValue;
	if(!IsValid(data.Object)) return defaultValue;

	if(data.Object->GetClass()->ImplementsInterface(IIConstantsDistributor::UClassType::StaticClass()))
	{
		auto rawValue = IIConstantsDistributor::Execute_GetValue(data.Object, data.FunctionName.ToString());
		return rawValue.StoredBool;
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
	if(data.ConnectionType.GetValue() == ConnectionData) return GetFunctionStringResult(GetRecursiveData(data), defaultValue);


	
	if(!data.Object || !IsValid(data.Object)) return defaultValue;

	if(data.Object->GetClass()->ImplementsInterface(IIConstantsDistributor::UClassType::StaticClass()))
	{
		auto rawValue = IIConstantsDistributor::Execute_GetValue(data.Object, data.FunctionName.ToString());
		return rawValue.StoredString;
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
	if(data.ConnectionType.GetValue() == ConnectionData) return GetFunctionNumberResult(GetRecursiveData(data), defaultValue);

	
	if(!data.Object) return defaultValue;
	if(!IsValid(data.Object)) return defaultValue;

	if(data.Object->GetClass()->ImplementsInterface(IIConstantsDistributor::UClassType::StaticClass()))
	{
		auto rawValue = IIConstantsDistributor::Execute_GetValue(data.Object, data.FunctionName.ToString());
		return rawValue.StoredFloat;
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
	if(data.ConnectionType.GetValue() == ConnectionData) return GetFunctionVectorResult(GetRecursiveData(data), defaultValue);

	
	if(!data.Object) return defaultValue;
	if(!IsValid(data.Object)) return defaultValue;
	
	if(data.Object->GetClass()->ImplementsInterface(IIConstantsDistributor::UClassType::StaticClass()))
	{
		auto rawValue = IIConstantsDistributor::Execute_GetValue(data.Object, data.FunctionName.ToString());
		return rawValue.StoredVector;
	}

	auto function = GetFunction(data);
	if(!function) return defaultValue;
	
	struct{FVector RetVal;} params;
	data.Object->ProcessEvent(function, &params);
	
	return params.RetVal;
}

FLinearColor UWiremodReflection::GetFunctionColorResult(const FNewConnectionData& data, FLinearColor defaultValue)
{
	if(data.ConnectionType.GetValue() == ConnectionData) return GetFunctionColorResult(GetRecursiveData(data), defaultValue);
	
	if(!data.Object || !IsValid(data.Object)) return defaultValue;

	if(data.Object->GetClass()->ImplementsInterface(IIConstantsDistributor::UClassType::StaticClass()))
	{
		auto rawValue = IIConstantsDistributor::Execute_GetValue(data.Object, data.FunctionName.ToString());
		return rawValue.StoredColor;
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

TArray<FString> UWiremodReflection::GetFunctionStringArray(const FNewConnectionData& data)
{
	TArray<FString> defaultValue = *new TArray<FString>;

	if(data.ConnectionType.GetValue() == ConnectionData) return GetFunctionStringArray(GetRecursiveData(data));
	
	auto function = GetFunction(data);
	if(!function) return defaultValue;

	struct{ TArray<FString> RetVal; } params;

	data.Object->ProcessEvent(function, &params);
	
	return params.RetVal;
}

UFGInventoryComponent* UWiremodReflection::GetFunctionInventory(const FNewConnectionData& data)
{
	if(data.ConnectionType.GetValue() == ConnectionData) return GetFunctionInventory(GetRecursiveData(data));
	
	auto function = GetFunction(data);
	if(!function) return nullptr;
	
	struct{ UFGInventoryComponent* RetVal; } params;
	data.Object->ProcessEvent(function, &params);
	
	return params.RetVal;
}

FInventoryStack UWiremodReflection::GetFunctionStackResult(const FNewConnectionData& data)
{
	if(data.ConnectionType.GetValue() == ConnectionData) return GetFunctionStackResult(GetRecursiveData(data));

	
	FInventoryStack defaultValue = *new FInventoryStack;
    	
	auto function = GetFunction(data);
	if(!function) return defaultValue;
    
	struct{
		FInventoryStack RetVal;
	} params;
    
	data.Object->ProcessEvent(function, &params);
	
	return params.RetVal;
}

TArray<FInventoryStack> UWiremodReflection::GetFunctionInventoryStackArrays(const FNewConnectionData& data)
{
	if(data.ConnectionType.GetValue() == ConnectionData) return GetFunctionInventoryStackArrays(GetRecursiveData(data));
	
	TArray<FInventoryStack> defaultValue = *new TArray<FInventoryStack>;
    	
	auto function = GetFunction(data);
	if(!function) return defaultValue;
    
	struct{ TArray<FInventoryStack> RetVal; } params;
    
    data.Object->ProcessEvent(function, &params);
	
    return params.RetVal;
}

UFGPowerCircuit* UWiremodReflection::GetFunctionPowerCircuitResult(const FNewConnectionData& data)
{
	if(data.ConnectionType.GetValue() == ConnectionData) return GetFunctionPowerCircuitResult(GetRecursiveData(data));
	UFGPowerCircuit* defaultValue = nullptr;
    	
	auto function = GetFunction(data);
	if(!function) return defaultValue;


	struct{ UFGPowerCircuit* RetVal; } params;
    
	data.Object->ProcessEvent(function, &params);
	
	return params.RetVal;
	
}

AActor* UWiremodReflection::GetFunctionEntityResult(const FNewConnectionData& data)
{
	if(data.ConnectionType.GetValue() == ConnectionData) return GetFunctionEntityResult(GetRecursiveData(data));
	AActor* defaultValue = nullptr;

	if(data.FunctionName == "Self") return Cast<AActor>(data.Object);
	
	auto function = GetFunction(data);
	if(!function) return defaultValue;
	
	struct{ AActor* RetVal; } params;
    
	data.Object->ProcessEvent(function, &params);
	
	return params.RetVal;
}

TArray<AActor*> UWiremodReflection::GetFunctionEntityArray(const FNewConnectionData& data)
{
	if(data.ConnectionType.GetValue() == ConnectionData) return GetFunctionEntityArray(GetRecursiveData(data));
	auto defaultValue = new TArray<AActor*>;
	
	auto function = GetFunction(data);
	if(!function) return *defaultValue;

	struct{ TArray<AActor*> RetVal; } params;
    
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
		if(sign->mTextElementToDataMap[data.FunctionName.ToString()] == value_) return;

		FPrefabSignData signData;

		sign->GetSignPrefabData(signData);
		
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
		if(data.FunctionName == "Emissive")
		{
			if(sign->mEmissive == value_) return;
			
			FPrefabSignData signData;
			sign->GetSignPrefabData(signData);

			signData.Emissive = value_;

			sign->SetPrefabSignData(signData);
		}
		else if(data.FunctionName == "Glossiness")
		{
			if(sign->mGlossiness == value_) return;
			
			FPrefabSignData signData;
			sign->GetSignPrefabData(signData);

			signData.Glossiness = value_;

			sign->SetPrefabSignData(signData);
		}
		else
		{
			if(sign->mIconElementToDataMap[data.FunctionName.ToString()] == trunc(value_)) return;
			
			FPrefabSignData signData;
			sign->GetSignPrefabData(signData);

			signData.IconElementData[data.FunctionName.ToString()] = trunc(value_);

			sign->SetPrefabSignData(signData);
		}

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
		if(data.FunctionName == "TextColor")
		{
			if(sign->mForegroundColor == value_) return;

			FPrefabSignData signData;
			sign->GetSignPrefabData(signData);

			signData.ForegroundColor = value_;

			sign->SetPrefabSignData(signData);
		}
		else if(data.FunctionName == "BackgroundColor")
		{
			if(sign->mBackgroundColor == value_) return;

			FPrefabSignData signData;
			sign->GetSignPrefabData(signData);

			signData.BackgroundColor = value_;

			sign->SetPrefabSignData(signData);
		}
		else if(data.FunctionName == "AuxColor")
		{
			if(sign->mAuxilaryColor == value_) return;

			FPrefabSignData signData;
			sign->GetSignPrefabData(signData);

			signData.AuxiliaryColor = value_;

			sign->SetPrefabSignData(signData);
		}

		
		return;
	}


	auto function = GetFunction(data);
	if(!function) return;
	
	struct {
		FLinearColor val;
	} params {value_};
	
	data.Object->ProcessEvent(function, &params);
}

void UWiremodReflection::HandleDynamicConnections(TArray<FDynamicConnectionData> connections)
{
	for (auto ConnectionData : connections)
	{
		bool HasNullPtr = !ConnectionData.Transmitter.Object || !ConnectionData.Receiver.Object;
		bool HasInvalid = !IsValid(ConnectionData.Transmitter.Object) || !IsValid(ConnectionData.Receiver.Object);
		
		if(HasNullPtr || HasInvalid)
		{
			connections.Remove(ConnectionData);
			continue;
		}
		
		HandleDynamicConnection(ConnectionData.Transmitter, ConnectionData.Receiver);
	}
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



