// 

#pragma once

#include "CoreMinimal.h"
#include "CCImageData.h"
#include "CustomStruct.h"
#include "FGBuildableConveyorMonitor.h"
#include "FGBuildablePortalBase.h"
#include "FGElevatorTypes.h"
#include "FGInventoryComponent.h"
#include "FGPowerCircuit.h"
#include "FGRecipe.h"
#include "FGTrainStationIdentifier.h"
#include "ReflectionExternalFunctions.h"
#include "Buildables/FGBuildableLightsControlPanel.h"
#include "Buildables/FGBuildableRailroadStation.h"
#include "Buildables/FGBuildableRailroadSwitchControl.h"
#include "Buildables/FGBuildableSplitterSmart.h"
#include "Kismet/BlueprintFunctionLibrary.h"
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

	void* GetRawValuePointer(const FString& ValueName);
};


USTRUCT(BlueprintType)
struct FTimeTableStopData
{
	GENERATED_BODY()

	UPROPERTY()
	class AFGTrainStationIdentifier* Station = nullptr;

	/** Station Rules */
	UPROPERTY()
	FTrainDockingRuleSet DockingRuleSet;

	FTimeTableStopData(){}

	FTimeTableStopData(AFGTrainStationIdentifier* StationIdentifier, const FTrainDockingRuleSet& RuleSet)
	{
		Station = StationIdentifier;
		DockingRuleSet = RuleSet;
	}
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
		else if(SourceName == "WM_DRONE_DOCKED")
			return UReflectionExternalFunctions::GetDroneIsInStation(Object);
		else if(auto Panel = Cast<AFGBuildableLightsControlPanel>(Object); Panel && SourceName == "IsTimeOfDayAware")
			return Panel->GetLightControlData().IsTimeOfDayAware;
		else
			return GenericProcess(REFLECTION_ARGS,  DefaultValue);
	}

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

	static void SetFloat(REFLECTION_PARAMS, double Value)
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

		NumericSet(REFLECTION_ARGS, Value);
	}
	
	static FString GetString(REFLECTION_PARAMS, FString DefaultValue = "") 
	{
		if(auto Sign = Cast<AFGBuildableWidgetSign>(Object))
			return UReflectionExternalFunctions::GetSignText(Sign, SourceName.ToString(), DefaultValue);
		else if(auto station = Cast<AFGBuildableRailroadStation>(Object))
			return station->GetStationIdentifier()->GetStationName().ToString();
		else if (auto Portal = Cast<AFGBuildablePortalBase>(Object))
			return Portal->GetPortalName().ToString();
		else
			return GenericProcess<FString, FStrProperty>(REFLECTION_ARGS,  DefaultValue);
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
		}
		else if(auto station = Cast<AFGBuildableRailroadStation>(Object))
		{
			if (auto StationIdentifier = station->GetStationIdentifier())
				StationIdentifier->SetStationName(FText::FromString(Value));
		}
		else if (auto Portal = Cast<AFGBuildablePortalBase>(Object))
			Portal->SetPortalName(FText::FromString(Value));
		else GenericSet(REFLECTION_ARGS, Value);
	}
	
	static FVector GetVector(REFLECTION_PARAMS, FVector DefaultValue = FVector::ZeroVector) { return GenericProcess<FVector, FStructProperty>(REFLECTION_ARGS, DefaultValue);}
	static void SetVector(REFLECTION_PARAMS, FVector Value){ GenericSet(REFLECTION_ARGS, Value); }
	
	static UFGInventoryComponent* GetInventory(REFLECTION_PARAMS) { return GenericProcess<UFGInventoryComponent*, FObjectPropertyBase>(REFLECTION_ARGS); }
	static void SetInventory(REFLECTION_PARAMS, UFGInventoryComponent* Value) { GenericSet(REFLECTION_ARGS, Value); }
	
	static UFGPowerCircuit* GetCircuit(REFLECTION_PARAMS) { return GenericProcess<UFGPowerCircuit*, FObjectPropertyBase>(REFLECTION_ARGS); }
	static void SetCircuit(REFLECTION_PARAMS, UFGPowerCircuit* Value) { GenericSet(REFLECTION_ARGS, Value); }
	
	static AActor* GetEntity(REFLECTION_PARAMS) 
	{
		if(SourceName == "Self")
			return Cast<AActor>(Object);
		else
			return GenericProcess<AActor*, FObjectPropertyBase>(REFLECTION_ARGS, nullptr);
	}
	static void SetEntity(REFLECTION_PARAMS, AActor* Value)
	{
		if (auto Portal = Cast<AFGBuildablePortalBase>(Object))
		{
			if (auto OtherPortal = Cast<AFGBuildablePortalBase>(Value))
			{
				if(Portal->GetLinkedPortal() != OtherPortal)
				{
					Portal->DisconnectLinkedPortal();
					OtherPortal->DisconnectLinkedPortal();
				}
				Portal->MakeLinkToPortal(OtherPortal);
				OtherPortal->MakeLinkToPortal(Portal);
			}
		}
		else GenericSet(REFLECTION_ARGS, Value);
	}
	
	static TSubclassOf<UFGRecipe> GetRecipe(REFLECTION_PARAMS) { return GenericProcess(REFLECTION_ARGS,  TSubclassOf<UFGRecipe>()); }
	static void SetRecipe(REFLECTION_PARAMS, TSubclassOf<UFGRecipe> Value)
	{
		if(!Value) return;
		GenericSet(REFLECTION_ARGS, Value);
	}
	
	static FLinearColor GetColor(REFLECTION_PARAMS, FLinearColor DefaultValue = FLinearColor::Black) { return GenericProcess<FLinearColor, FStructProperty>(REFLECTION_ARGS,  DefaultValue); }
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
	
	static FInventoryStack GetStack(REFLECTION_PARAMS) { return GenericProcess<FInventoryStack, FStructProperty>(REFLECTION_ARGS); }
	static void SetStack(REFLECTION_PARAMS, FInventoryStack Value) { GenericSet(REFLECTION_ARGS, Value); }
	
	static FItemAmount GetItemAmount(REFLECTION_PARAMS) { return GenericProcess<FItemAmount, FStructProperty>(REFLECTION_ARGS); }
	static void SetItemAmount(REFLECTION_PARAMS, FItemAmount Value) { GenericSet(REFLECTION_ARGS, Value); }
	
	static UTexture* GetTexture(REFLECTION_PARAMS) { return GenericProcess<UTexture*>(REFLECTION_ARGS); }
	static void SetTexture(REFLECTION_PARAMS, UTexture* Value) { GenericSet(REFLECTION_ARGS, Value); }
	
	static FSplitterSortRule GetSplitterRule(REFLECTION_PARAMS) { return GenericProcess<FSplitterSortRule, FStructProperty>(REFLECTION_ARGS); }
	static void SetSplitterRule(REFLECTION_PARAMS, FSplitterSortRule Value) { GenericSet(REFLECTION_ARGS, Value); }

	static TSubclassOf<UFGItemDescriptor> GetItemDescriptor(REFLECTION_PARAMS, TSubclassOf<UFGItemDescriptor> DefaultValue) { return GenericProcess(REFLECTION_ARGS, DefaultValue); }
	static void SetItemDescriptor(REFLECTION_PARAMS, TSubclassOf<UFGItemDescriptor> Value) { GenericSet(REFLECTION_ARGS, Value); }

	static FTimeTableStopData GetTimeTableStop(REFLECTION_PARAMS) { return GenericProcess<FTimeTableStopData, FStructProperty>(REFLECTION_ARGS); }
	static void SetTimeTableStop(REFLECTION_PARAMS, FTimeTableStopData Value) { GenericSet(REFLECTION_ARGS, Value); }

	static UFGPowerInfoComponent* GetPowerInfo(UObject* Object) { return UReflectionExternalFunctions::GetPowerInfo(Object); }

	static FElevatorFloorStopInfo GetElevatorFloor(REFLECTION_PARAMS) { return GenericProcess<FElevatorFloorStopInfo, FStructProperty>(REFLECTION_ARGS); }
	static void SetElevatorFloor(REFLECTION_PARAMS, FElevatorFloorStopInfo Value) { GenericSet(REFLECTION_ARGS, Value); }

	static FPixelScreenData GetPixelImage(REFLECTION_PARAMS) { return GenericProcess<FPixelScreenData, FStructProperty>(REFLECTION_ARGS); }
	static void SetPixelImage(REFLECTION_PARAMS, FPixelScreenData Value) { GenericSet(REFLECTION_ARGS, Value); }

	static FCustomStruct GetCustomStruct(REFLECTION_PARAMS) { return GenericProcess<FCustomStruct, FStructProperty>(REFLECTION_ARGS); }
	static void SetCustomStruct(REFLECTION_PARAMS, FCustomStruct Value) { GenericSet(REFLECTION_ARGS, Value); }

	static TArray<bool> GetBoolArray(REFLECTION_PARAMS) { return GenericProcess<TArray<bool>, FArrayProperty>(REFLECTION_ARGS); }
	static void SetBoolArray(REFLECTION_PARAMS, TArray<bool> Value) { GenericSet(REFLECTION_ARGS, Value); }
	
	static TArray<double> GetFloatArray(REFLECTION_PARAMS)
	{
		const FName ConveyorMonitorAverage = FName("CC_MONITORAVG");
		if (auto ConveyorMonitor = Cast<AFGBuildableConveyorMonitor>(Object); ConveyorMonitor && SourceName == ConveyorMonitorAverage)
		{
			TArray<double> AverageGraph;
			for (const FItemMonitorData& Data : ConveyorMonitor->GetAverageDataForUIRepresentation())
			{
				AverageGraph.Add(Data.FloatLocalAverage);
			}
			
			return AverageGraph;
		}
		
		
		return GenericProcess<TArray<double>, FArrayProperty>(REFLECTION_ARGS);
	}
	static void SetFloatArray(REFLECTION_PARAMS, TArray<double> Value) { GenericSet(REFLECTION_ARGS, Value); }
	
	static TArray<FString> GetStringArray(REFLECTION_PARAMS) { return GenericProcess<TArray<FString>, FArrayProperty>(REFLECTION_ARGS); }
	static void SetStringArray(REFLECTION_PARAMS, const TArray<FString>& Value) { GenericSet(REFLECTION_ARGS, Value); }
	
	static TArray<FVector> GetVectorArray(REFLECTION_PARAMS) { return GenericProcess<TArray<FVector>, FArrayProperty>(REFLECTION_ARGS); }
	static void SetVectorArray(REFLECTION_PARAMS, const TArray<FVector>& Value) { GenericSet(REFLECTION_ARGS, Value); }
	
	static TArray<UFGInventoryComponent*> GetInventoryArray(REFLECTION_PARAMS) { return GenericProcess<TArray<UFGInventoryComponent*>, FArrayProperty>(REFLECTION_ARGS); }
	static void SetInventoryArray(REFLECTION_PARAMS, const TArray<UFGInventoryComponent*>& Value) { GenericSet(REFLECTION_ARGS, Value); }
	
	static TArray<UFGPowerCircuit*> GetCircuitArray(REFLECTION_PARAMS) { return GenericProcess<TArray<UFGPowerCircuit*>, FArrayProperty>(REFLECTION_ARGS); }
	static void SetCircuitArray(REFLECTION_PARAMS, const TArray<UFGPowerCircuit*>& Value) { GenericSet(REFLECTION_ARGS, Value); }
	
	static TArray<AActor*> GetEntityArray(REFLECTION_PARAMS) { return GenericProcess<TArray<AActor*>, FArrayProperty>(REFLECTION_ARGS); }
	static void SetEntityArray(REFLECTION_PARAMS, const TArray<AActor*>& Value) { GenericSet(REFLECTION_ARGS, Value); }
	
	static TArray<TSubclassOf<UFGRecipe>> GetRecipeArray(REFLECTION_PARAMS) { return GenericProcess<TArray<TSubclassOf<UFGRecipe>>, FArrayProperty>(REFLECTION_ARGS); }
	static void SetRecipeArray(REFLECTION_PARAMS, const TArray<TSubclassOf<UFGRecipe>>& Value) { GenericSet(REFLECTION_ARGS, Value); }
	
	static TArray<FLinearColor> GetColorArray(REFLECTION_PARAMS) { return GenericProcess<TArray<FLinearColor>, FArrayProperty>(REFLECTION_ARGS); }
	static void SetColorArray(REFLECTION_PARAMS, const TArray<FLinearColor>& Value) { GenericSet(REFLECTION_ARGS, Value); }
	
	static TArray<FInventoryStack> GetStackArray(REFLECTION_PARAMS) { return GenericProcess<TArray<FInventoryStack>, FArrayProperty>(REFLECTION_ARGS); }
	static void SetStackArray(REFLECTION_PARAMS, const TArray<FInventoryStack>& Value) { GenericSet(REFLECTION_ARGS, Value); }
	
	static TArray<FItemAmount> GetItemAmountArray(REFLECTION_PARAMS) { return GenericProcess<TArray<FItemAmount>, FArrayProperty>(REFLECTION_ARGS); }
	static void SetItemAmountArray(REFLECTION_PARAMS, const TArray<FItemAmount>& Value) { GenericSet(REFLECTION_ARGS, Value); }
	
	static TArray<UTexture*> GetTextureArray(REFLECTION_PARAMS) { return GenericProcess<TArray<UTexture*>, FArrayProperty>(REFLECTION_ARGS); }
	static void SetTextureArray(REFLECTION_PARAMS, const TArray<UTexture*>& Value) { GenericSet(REFLECTION_ARGS, Value); }
	
	static TArray<FSplitterSortRule> GetSplitterRuleArray(REFLECTION_PARAMS) { return GenericProcess<TArray<FSplitterSortRule>, FArrayProperty>(REFLECTION_ARGS); }
	static void SetSplitterRuleArray(REFLECTION_PARAMS, const TArray<FSplitterSortRule>& Value) { GenericSet(REFLECTION_ARGS, Value); }

	static TArray<TSubclassOf<UFGItemDescriptor>> GetItemDescriptorArray(REFLECTION_PARAMS) { return GenericProcess<TArray<TSubclassOf<UFGItemDescriptor>>, FArrayProperty>(REFLECTION_ARGS); }
	static void SetItemDescriptorArray(REFLECTION_PARAMS, const TArray<TSubclassOf<UFGItemDescriptor>>& Value) { GenericSet(REFLECTION_ARGS, Value); }

	static TArray<FTimeTableStopData> GetTimeTableStopArray(REFLECTION_PARAMS) { return GenericProcess<TArray<FTimeTableStopData>>(REFLECTION_ARGS); }
	static void SetTimeTableStopArray(REFLECTION_PARAMS, const TArray<FTimeTableStopData>& Value) { GenericSet(REFLECTION_ARGS, Value); }

	static TArray<FElevatorFloorStopInfo> GetElevatorFloorArray(REFLECTION_PARAMS) { return GenericProcess<TArray<FElevatorFloorStopInfo>>(REFLECTION_ARGS); }
	static void SetElevatorFloorArray(REFLECTION_PARAMS, const TArray<FElevatorFloorStopInfo>& Value) { GenericSet(REFLECTION_ARGS, Value); }

	static TArray<FPixelScreenData> GetPixelImageArray(REFLECTION_PARAMS) { return GenericProcess<TArray<FPixelScreenData>>(REFLECTION_ARGS); }
	static void SetPixelImageArray(REFLECTION_PARAMS, const TArray<FPixelScreenData>& Value) { GenericSet(REFLECTION_ARGS, Value); }

	static TArray<FCustomStruct> GetCustomStructArray(REFLECTION_PARAMS) { return GenericProcess<TArray<FCustomStruct>>(REFLECTION_ARGS); }
	static void SetCustomStructArray(REFLECTION_PARAMS, const TArray<FCustomStruct>& Value) { GenericSet(REFLECTION_ARGS, Value); }

	template<typename T>
	static T GetUnmanaged(REFLECTION_PARAMS, T DefaultValue = T()){ return GenericProcess(REFLECTION_ARGS, DefaultValue); }

	template<typename T>
	static void SetUnmanaged(REFLECTION_PARAMS, T Value) { GenericSet(REFLECTION_ARGS, Value); }
	
	template <typename T, typename PropType>
	static T FromPropertyValue(REFLECTION_PARAMS, T DefaultValue)
	{
		if(!IsValid(Object)) return DefaultValue;
		
		auto Val = Object->GetClass()->FindPropertyByName(SourceName);
		if(!Val) return DefaultValue;
		if(!Val->IsA<PropType>()) return DefaultValue;
		
		auto ValuePointer = Val->ContainerPtrToValuePtr<T>(Object);
		
		return *ValuePointer;
	}

	static double FromNumericPropertyValue(REFLECTION_PARAMS, double DefaultValue)
	{
		if(!IsValid(Object)) return DefaultValue;
		
		auto Val = CastField<FNumericProperty>(Object->GetClass()->FindPropertyByName(SourceName));
		if(!Val) return DefaultValue;
		if(Val->IsInteger()) return Val->GetSignedIntPropertyValue(Val->ContainerPtrToValuePtr<void>(Object));
		return Val->GetFloatingPointPropertyValue(Val->ContainerPtrToValuePtr<void>(Object));
	}

	template<typename T, typename PropType = FProperty>
	static T GenericProcess(REFLECTION_PARAMS, T DefaultValue = T())
	{
		if(!IsValid(Object)) return DefaultValue;
	
		if(FromProperty)
			return FromPropertyValue<T, PropType>(REFLECTION_ARGS, DefaultValue);
	
		if(Object->GetClass()->ImplementsInterface(IDynamicValuePasser::UClassType::StaticClass()))
		{
			auto ValueBase = IDynamicValuePasser::Execute_GetValue(Object, SourceName.ToString());
			return FromPropertyValue<T, PropType>(ValueBase, "Value", true, DefaultValue);
		}

		struct{T RetVal;} Params{DefaultValue};
		if(!ProcessFunction<PropType>(REFLECTION_ARGS, &Params)) return FromPropertyValue<T, PropType>(REFLECTION_ARGS, DefaultValue);
		return Params.RetVal;
	}

	static double NumericProcess(REFLECTION_PARAMS, double DefaultValue)
	{
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

		//Explicit return value check because UE5 now uses double, but some code still uses float so we have to account for that,
		//Not to mention int returns as well, i am so fucking done with this code.
		if(FuncProperty->IsA<FIntProperty>()) return GenericProcess<int, FIntProperty>(REFLECTION_ARGS, DefaultValue);
		else if(FuncProperty->IsA<FEnumProperty>()) return GenericProcess<int, FEnumProperty>(REFLECTION_ARGS, DefaultValue);
		else if(FuncProperty->IsA<FFloatProperty>()) return GenericProcess<float, FFloatProperty>(REFLECTION_ARGS, DefaultValue);
		else if (FuncProperty->IsA<FDoubleProperty>()) return GenericProcess<double, FDoubleProperty>(REFLECTION_ARGS, DefaultValue);
		else return DefaultValue;
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
		ProcessFunction(REFLECTION_ARGS, &Params);
		
	}

	static void NumericSet(REFLECTION_PARAMS, double Value)
	{
		if(FromProperty)
		{
			auto Val = CastField<FNumericProperty>(Object->GetClass()->FindPropertyByName(SourceName));
			if(!Val) return;
			if(Val->IsInteger()) Val->SetIntPropertyValue(Val->ContainerPtrToValuePtr<void>(Object), (int64) Value);
			else Val->SetFloatingPointPropertyValue(Val->ContainerPtrToValuePtr<void>(Object), Value);
		}

		auto Function = Object->FindFunction(SourceName);
		if(!Function) return;
		
		auto FuncProperty = Function->ChildProperties;
		if(!FuncProperty) return;

		if(FuncProperty->IsA<FIntProperty>())
		{
			struct { int Value; } params{(int) Value};
			ProcessFunction<FIntProperty>(REFLECTION_ARGS, &params);
		}
		else if(FuncProperty->IsA<FEnumProperty>())
		{
			struct { int Value; } params{(int) Value};
			ProcessFunction<FEnumProperty>(REFLECTION_ARGS, &params);
		}
		else if(FuncProperty->IsA<FFloatProperty>())
		{
			struct { float Value; } params{(float) Value};
			ProcessFunction<FFloatProperty>(REFLECTION_ARGS, &params);
		}
		else
		{
			struct { double Value; } params{Value};
			ProcessFunction<FDoubleProperty>(REFLECTION_ARGS, &params);
		}
	}


	template<typename PropType = FProperty>
	static bool ProcessFunction(REFLECTION_PARAMS, void* Params)
	{
		if(IsValid(Object))
		{
			UFunction* Function = Object->FindFunction(SourceName);;
			
			if(Function)
			{
				auto ReturnProp = Function->GetReturnProperty();
				if(ReturnProp && !ReturnProp->IsA<PropType>()) return false;
				
				Object->ProcessEvent(Function, Params);
				return true;
			}
		}
		return false;
	}
};
