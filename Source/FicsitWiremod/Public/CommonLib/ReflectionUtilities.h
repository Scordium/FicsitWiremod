// 

#pragma once

#include "CoreMinimal.h"
#include "FGInventoryComponent.h"
#include "FGPowerCircuit.h"
#include "FGRecipe.h"
#include "FGTrainStationIdentifier.h"
#include "ReflectionExternalFunctions.h"
#include "Buildables/FGBuildableLightsControlPanel.h"
#include "Buildables/FGBuildableRailroadStation.h"
#include "Buildables/FGBuildableRailroadSwitchControl.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Utility/CircuitryLogger.h"
#include "ReflectionUtilities.generated.h"

#define REFLECTION_PARAMS UObject* Object, FName SourceName, bool FromProperty
#define REFLECTION_ARGS Object, SourceName, FromProperty
/**
 * 
 */

UINTERFACE(BlueprintType, Blueprintable)
class UDynamicValuePasser : public UInterface
{
	GENERATED_BODY()
};

class IDynamicValuePasser
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	UObject* GetValue(const FString& ValueName);
};

UCLASS()
class FICSITWIREMOD_API UReflectionUtilities : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	
	static bool GetBool(REFLECTION_PARAMS, bool DefaultValue = false) 
	{
		if(SourceName == "WM_DOORCONTROL_FUNC")
			return UReflectionExternalFunctions::GetDoorIsLocked(Object);
		else if(auto Panel = Cast<AFGBuildableLightsControlPanel>(Object); Panel && SourceName == "IsTimeOfDayAware")
			return Panel->GetLightControlData().IsTimeOfDayAware;
		else
			return GenericProcess(REFLECTION_ARGS,  DefaultValue);
	}
	
	static double GetFloat(REFLECTION_PARAMS, double DefaultValue = 0) 
	{
		if(auto panel = Cast<AFGBuildableLightsControlPanel>(Object))
		{
			//Control panels store their data in struct, so to get the values we have to manually disassemble the struct.
			if(SourceName == "ColorSlotIndex")
				return panel->GetLightControlData().ColorSlotIndex;
			else
				return panel->GetLightControlData().Intensity;
		}
		else if(auto Sign = Cast<AFGBuildableWidgetSign>(Object))
			return UReflectionExternalFunctions::GetSignValue(Sign, SourceName.ToString(), DefaultValue);
		else if(auto FluidTank = Cast<AFGBuildablePipeReservoir>(Object))
		{
			if(SourceName == "Content")
				return FluidTank->GetFluidBox()->Content;
			else
				return FluidTank->GetFluidBox()->MaxContent;
		}

		return NumericProcess(REFLECTION_ARGS, DefaultValue);
	}
	
	static FString GetString(REFLECTION_PARAMS, FString DefaultValue = "") 
	{
		if(auto Sign = Cast<AFGBuildableWidgetSign>(Object))
			return UReflectionExternalFunctions::GetSignText(Sign, SourceName.ToString(), DefaultValue);
		else if(auto station = Cast<AFGBuildableRailroadStation>(Object))
			return station->GetStationIdentifier()->GetStationName().ToString();
		else
			return GenericProcess(REFLECTION_ARGS,  DefaultValue);
	}
	
	static FVector GetVector(REFLECTION_PARAMS, FVector DefaultValue = FVector::ZeroVector) { return GenericProcess(REFLECTION_ARGS, DefaultValue); }
	static UFGInventoryComponent* GetInventory(REFLECTION_PARAMS) { return GenericProcess<UFGInventoryComponent*>(REFLECTION_ARGS); }
	static UFGPowerCircuit* GetCircuit(REFLECTION_PARAMS) { return GenericProcess<UFGPowerCircuit*>(REFLECTION_ARGS); }
	static AActor* GetEntity(REFLECTION_PARAMS) 
	{
		if(SourceName == "Self")
			return Cast<AActor>(Object);
		else
			return GenericProcess<AActor*>(REFLECTION_ARGS, nullptr);
	}
	
	static TSubclassOf<UFGRecipe> GetRecipe(REFLECTION_PARAMS) 
	{
		TSubclassOf<UFGRecipe> Out = GenericProcess(REFLECTION_ARGS,  TSubclassOf<UFGRecipe>());
		return Out.GetDefaultObject() ? TSubclassOf<UFGRecipe>(Out.GetDefaultObject()->GetClass()) : TSubclassOf<UFGRecipe>();
	}
	static FLinearColor GetColor(REFLECTION_PARAMS, FLinearColor DefaultValue = FLinearColor::Black) { return GenericProcess(REFLECTION_ARGS,  DefaultValue); }
	static FInventoryStack GetStack(REFLECTION_PARAMS) { return GenericProcess<FInventoryStack>(REFLECTION_ARGS); }
	static FItemAmount GetItemAmount(REFLECTION_PARAMS) { return GenericProcess<FItemAmount>(REFLECTION_ARGS); }
	static UTexture* GetTexture(REFLECTION_PARAMS) { return GenericProcess<UTexture*>(REFLECTION_ARGS); }

	static TArray<bool> GetBoolArray(REFLECTION_PARAMS) { return GenericProcess<TArray<bool>>(REFLECTION_ARGS); }
	static TArray<double> GetFloatArray(REFLECTION_PARAMS) { return GenericProcess<TArray<double>>(REFLECTION_ARGS); }
	static TArray<FString> GetStringArray(REFLECTION_PARAMS) { return GenericProcess<TArray<FString>>(REFLECTION_ARGS); }
	static TArray<FVector> GetVectorArray(REFLECTION_PARAMS) { return GenericProcess<TArray<FVector>>(REFLECTION_ARGS); }
	static TArray<UFGInventoryComponent*> GetInventoryArray(REFLECTION_PARAMS) { return GenericProcess<TArray<UFGInventoryComponent*>>(REFLECTION_ARGS); }
	static TArray<UFGPowerCircuit*> GetCircuitArray(REFLECTION_PARAMS) { return GenericProcess<TArray<UFGPowerCircuit*>>(REFLECTION_ARGS); }
	static TArray<AActor*> GetEntityArray(REFLECTION_PARAMS) { return GenericProcess<TArray<AActor*>>(REFLECTION_ARGS); }
	static TArray<TSubclassOf<UFGRecipe>> GetRecipeArray(REFLECTION_PARAMS) { return GenericProcess<TArray<TSubclassOf<UFGRecipe>>>(REFLECTION_ARGS); }
	static TArray<FLinearColor> GetColorArray(REFLECTION_PARAMS) { return GenericProcess<TArray<FLinearColor>>(REFLECTION_ARGS); }
	static TArray<FInventoryStack> GetStackArray(REFLECTION_PARAMS) { return GenericProcess<TArray<FInventoryStack>>(REFLECTION_ARGS); }
	static TArray<FItemAmount> GetItemAmountArray(REFLECTION_PARAMS) { return GenericProcess<TArray<FItemAmount>>(REFLECTION_ARGS); }
	static TArray<UTexture*> GetTextureArray(REFLECTION_PARAMS) { return GenericProcess<TArray<UTexture*>>(REFLECTION_ARGS); }

	template<typename T>
	static T GetUnmanaged(REFLECTION_PARAMS, T DefaultValue = T()){ return GenericProcess(REFLECTION_ARGS, DefaultValue); }

	static void SetBool(REFLECTION_PARAMS, bool Value)
	{
		if(SourceName.ToString().StartsWith("WM_") && Value)
		{
			if(SourceName == "WM_FLUSHTANK_FUNC") UReflectionExternalFunctions::FlushTank(Object);
			else if(SourceName == "WM_FLUSHNET_FUNC") UReflectionExternalFunctions::FlushNetwork(Object);
			else if(SourceName == "WM_ON_USE_FUNC") UReflectionExternalFunctions::ExecuteOnUse(Object);
		}
		else if(SourceName.ToString().StartsWith("WMCL_"))
		{
			if(SourceName == "WMCL_DOORCONTROL_FUNC") UReflectionExternalFunctions::ChangeDoorState(Object, Value);
			else if(SourceName == "WMCL_RAILSIGNAL_STOP") UReflectionExternalFunctions::ChangeRailroadSignalState(Object, Value);
			else if(SourceName == "WMCL_FF_DOGGODOOR") UReflectionExternalFunctions::ChangeDoggoHouseDoorState(Object, Value);
		}
	
		if(auto panel = Cast<AFGBuildableLightsControlPanel>(Object))
		{
		
			if(SourceName == "IsLightEnabled")
			{
				if(panel->IsLightEnabled() == Value) return;
			
				panel->SetLightEnabled(Value);
				auto affected = panel->GetControlledBuildables(AFGBuildableLightSource::StaticClass());
				for (auto light : affected)
				{
					if(auto Light = Cast<AFGBuildableLightSource>(light)) Light->SetLightEnabled(Value);
				}
			}
			else
			{
				auto panelData = panel->GetLightControlData();
				if(panelData.IsTimeOfDayAware == Value) return;
			
				panelData.IsTimeOfDayAware = Value;
				panel->SetLightDataOnControlledLights(panelData);
			}
		

			return;
		}

		GenericSet(REFLECTION_ARGS, Value);
	}

	static void SetFloat(REFLECTION_PARAMS, bool IsInt, double Value)
	{
		if(SourceName == "WM_RAILSWITCH_FUNC")
		{
			if(auto RailSwitch = Cast<AFGBuildableRailroadSwitchControl>(Object))
			{
				//If switch is not in the position that we want, switch it to the next one.
				if(RailSwitch->GetSwitchPosition() != trunc(Value)) RailSwitch->ToggleSwitchPosition();
				return;
			}
		}
		else if(auto Panel = Cast<AFGBuildableLightsControlPanel>(Object))
		{
			auto PanelData = Panel->GetLightControlData();

			if(SourceName == "ColorSlotIndex")
			{
				if(PanelData.ColorSlotIndex == trunc(Value)) return;
			
				PanelData.ColorSlotIndex = trunc(Value);
			}
			else
			{
				if(PanelData.Intensity == Value) return;
			
				PanelData.Intensity = Value;
			}

			Panel->SetLightDataOnControlledLights(PanelData);
			return;
		}
		else if (auto sign = Cast<AFGBuildableWidgetSign>(Object))
		{
			FPrefabSignData signData;
			sign->GetSignPrefabData(signData);
		
			if(SourceName == "Emissive")
			{
				if(signData.Emissive == Value) return;
				signData.Emissive = Value;
			}
			else if(SourceName == "Glossiness")
			{
				if(signData.Glossiness == Value) return;
				signData.Glossiness = Value;
			}
			else
			{
				if(signData.IconElementData.Contains(SourceName.ToString()))
					if(signData.IconElementData[SourceName.ToString()] == trunc(Value)) return;
			
				signData.IconElementData[SourceName.ToString()] = trunc(Value);
			}

			sign->SetPrefabSignData(signData);
			return;
		}
		else if(SourceName == "SetPendingPotential")
		{
			if(auto Building = Cast<AFGBuildableFactory>(Object))
			{
				if (Building->GetPendingPotential() == Value) return;
			}
		}

		if(IsInt)
			GenericSet<int>(REFLECTION_ARGS, Value);
		else
			GenericSet(REFLECTION_ARGS, Value);
	}

	static void SetString(REFLECTION_PARAMS, FString Value)
	{
		if(auto sign = Cast<AFGBuildableWidgetSign>(Object))
		{
			FPrefabSignData signData;

			sign->GetSignPrefabData(signData);
		
			if(signData.TextElementData.Contains(SourceName.ToString()))
				if(signData.TextElementData[SourceName.ToString()].Equals(Value)) return;
		
			signData.TextElementData[SourceName.ToString()] = Value;
			sign->SetPrefabSignData(signData);
		
			return;
		}
		else if(auto station = Cast<AFGBuildableRailroadStation>(Object))
		{
			station->GetStationIdentifier()->SetStationName(FText::FromString(Value));
			return;
		}
	
		GenericSet(REFLECTION_ARGS, Value);
	}

	static void SetColor(REFLECTION_PARAMS, FLinearColor Value)
	{
		if(auto sign = Cast<AFGBuildableWidgetSign>(Object))
		{

			FPrefabSignData signData;
			sign->GetSignPrefabData(signData);
		
			if(SourceName == "TextColor")
			{
				if(signData.ForegroundColor == Value) return;
				signData.ForegroundColor = Value;
			}
			else if(SourceName == "BackgroundColor")
			{
				if(signData.BackgroundColor == Value) return;
				signData.BackgroundColor = Value;
			}
			else if(SourceName == "AuxColor")
			{
				if(signData.AuxiliaryColor == Value) return;
				signData.AuxiliaryColor = Value;
			}

			sign->SetPrefabSignData(signData);
			return;
		}
		GenericSet(REFLECTION_ARGS, Value);
	}

	static void SetRecipe(REFLECTION_PARAMS, TSubclassOf<UFGRecipe> Value)
	{
		if(!Value.GetDefaultObject()) return;
	
		GenericSet(REFLECTION_ARGS, Value);
	}

	template<typename T>
	static void SetUnmanaged(REFLECTION_PARAMS, T Value) { GenericSet(REFLECTION_ARGS, Value); }

	
	
	template <typename T>
	static T FromPropertyValue(REFLECTION_PARAMS, T DefaultValue)
	{
		if(!IsValid(Object)) return DefaultValue;
		
		auto Val = Object->GetClass()->FindPropertyByName(SourceName);
		if(!Val) return DefaultValue;
		return *Val->ContainerPtrToValuePtr<T>(Object);
	}

	static double FromNumericPropertyValue(REFLECTION_PARAMS, double DefaultValue)
	{
		if(!IsValid(Object)) return DefaultValue;
		
		auto Val = CastField<FNumericProperty>(Object->GetClass()->FindPropertyByName(SourceName));
		if(!Val) return DefaultValue;
		if(Val->IsInteger()) return Val->GetSignedIntPropertyValue(Val->ContainerPtrToValuePtr<void>(Object));
		return Val->GetFloatingPointPropertyValue(Val->ContainerPtrToValuePtr<void>(Object));
	}

	template<typename T>
	static T GenericProcess(REFLECTION_PARAMS, T DefaultValue = T())
	{
		if(!IsValid(Object)) return DefaultValue;
	
		if(FromProperty)
			return FromPropertyValue(REFLECTION_ARGS, DefaultValue);
	
		if(Object->GetClass()->ImplementsInterface(IDynamicValuePasser::UClassType::StaticClass()))
		{
			auto ValueBase = IDynamicValuePasser::Execute_GetValue(Object, SourceName.ToString());
			return FromPropertyValue(ValueBase, "Value", true, DefaultValue);
		}

		struct{T RetVal;} Params{DefaultValue};
		if(!ProcessFunction(Object, SourceName, &Params)) return FromPropertyValue(REFLECTION_ARGS, DefaultValue);
		return Params.RetVal;
	}

	static double NumericProcess(REFLECTION_PARAMS, double DefaultValue)
	{
		//Explicit return value check because UE5 now uses double, but some code still uses float so we have to account for that,
		//Not to mention int returns as well, i am so fucking done with this code.
		if(!Object) return DefaultValue;

		if(FromProperty)
			return FromNumericPropertyValue(REFLECTION_ARGS, DefaultValue);

		if(Object->GetClass()->ImplementsInterface(IDynamicValuePasser::UClassType::StaticClass()))
		{
			auto ValueBase = IDynamicValuePasser::Execute_GetValue(Object, SourceName.ToString());
			return FromNumericPropertyValue(ValueBase, "Value", true, DefaultValue);
		}
		
		auto Function = Object->FindFunction(SourceName);
		if(!Function) return FromNumericPropertyValue(REFLECTION_ARGS, DefaultValue);
		
		auto FuncProperty = Function->ChildProperties;
		if(!FuncProperty) return FromNumericPropertyValue(REFLECTION_ARGS, DefaultValue);
		
		if(FuncProperty->IsA<FIntProperty>()) return GenericProcess<int>(REFLECTION_ARGS, DefaultValue);
		else if(FuncProperty->IsA<FFloatProperty>()) return GenericProcess<float>(REFLECTION_ARGS, DefaultValue);
		else return GenericProcess(REFLECTION_ARGS, DefaultValue);
	}

	template<typename T>
	static void GenericSet(REFLECTION_PARAMS, T Value)
	{
		if(FromProperty)
		{
			auto Val = Object->GetClass()->FindPropertyByName(SourceName);
			if(Val) *Val->ContainerPtrToValuePtr<T>(Object) = Value;
			return;
		}
		
		struct{T SetVal;} Params{Value};
		ProcessFunction(Object, SourceName, &Params);
	}


	static bool ProcessFunction(UObject* Object, FName SourceName, void* Params)
	{
		if(IsValid(Object))
		{
			if(auto Function = Object->FindFunction(SourceName))
			{
				Object->ProcessEvent(Function, Params);
				return true;
			}
		}
		return false;
	}
};
