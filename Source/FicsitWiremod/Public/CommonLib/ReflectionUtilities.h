// 

#pragma once

#include "CoreMinimal.h"
#include "ConnectionPointer.h"
#include "Behaviour/Displays/CCImageData.h"
#include "CustomStruct.h"
#include "Buildables/FGBuildableConveyorMonitor.h"
#include "Buildables/FGBuildablePortalBase.h"
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
#include "FGRailroadTimeTable.h"
#include "ReflectionUtilities.generated.h"
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

UCLASS()
class FICSITWIREMOD_API UReflectionUtilities : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
public:
	
	static bool GetBool(const FConnectionPointer& Pointer, bool DefaultValue = false) 
	{
		if(Pointer.SourceName == "WM_DOORCONTROL_FUNC")
			return UReflectionExternalFunctions::GetDoorIsLocked(Pointer.Target);
		else if(Pointer.SourceName == "WM_DRONE_DOCKED")
			return UReflectionExternalFunctions::GetDroneIsInStation(Pointer.Target);
		else if(auto Panel = Cast<AFGBuildableLightsControlPanel>(Pointer.Target); Panel && Pointer.SourceName == "IsTimeOfDayAware")
			return Panel->GetLightControlData().IsTimeOfDayAware;
		else
			return GenericProcess<bool, FBoolProperty>(Pointer, nullptr, DefaultValue);
	}

	static void SetBool(const FConnectionPointer& Pointer, bool Value)
	{
		if(Pointer.SourceName.ToString().StartsWith("WM_") && Value)
		{
			if(Pointer.SourceName == "WM_FLUSHTANK_FUNC") UReflectionExternalFunctions::FlushTank(Pointer.Target);
			else if(Pointer.SourceName == "WM_FLUSHNET_FUNC") UReflectionExternalFunctions::FlushNetwork(Pointer.Target);
			else if(Pointer.SourceName == "WM_ON_USE_FUNC") UReflectionExternalFunctions::ExecuteOnUse(Pointer.Target);
		}
		else if(Pointer.SourceName.ToString().StartsWith("WMCL_"))
		{
			if(Pointer.SourceName == "WMCL_DOORCONTROL_FUNC") UReflectionExternalFunctions::ChangeDoorState(Pointer.Target, Value);
			else if(Pointer.SourceName == "WMCL_RAILSIGNAL_STOP") UReflectionExternalFunctions::ChangeRailroadSignalState(Pointer.Target, Value);
			else if(Pointer.SourceName == "WMCL_FF_DOGGODOOR") UReflectionExternalFunctions::ChangeDoggoHouseDoorState(Pointer.Target, Value);
		}
	
		if(auto panel = Cast<AFGBuildableLightsControlPanel>(Pointer.Target))
		{
		
			if(Pointer.SourceName == "IsLightEnabled")
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

		GenericSet<FBoolProperty>(Pointer, Value, nullptr);
	}
	
	static double GetFloat(const FConnectionPointer& Pointer, double DefaultValue = 0) 
	{
		if(auto panel = Cast<AFGBuildableLightsControlPanel>(Pointer.Target))
		{
			//Control panels store their data in struct, so to get the values we have to manually disassemble the struct.
			if(Pointer.SourceName == "ColorSlotIndex")
				return panel->GetLightControlData().ColorSlotIndex;
			else
				return panel->GetLightControlData().Intensity;
		}
		else if(auto Sign = Cast<AFGBuildableWidgetSign>(Pointer.Target))
			return UReflectionExternalFunctions::GetSignValue(Sign, Pointer.SourceName.ToString(), DefaultValue);
		else if(auto FluidTank = Cast<AFGBuildablePipeReservoir>(Pointer.Target))
		{
			if(Pointer.SourceName == "Content")
				return FluidTank->GetFluidBox()->Content;
			else
				return FluidTank->GetFluidBox()->MaxContent;
		}

		return NumericProcess(Pointer, DefaultValue);
	}

	static void SetFloat(const FConnectionPointer& Pointer, double Value)
	{
		if(Pointer.SourceName == "WM_RAILSWITCH_FUNC")
		{
			if(auto RailSwitch = Cast<AFGBuildableRailroadSwitchControl>(Pointer.Target))
			{
				//If switch is not in the position that we want, switch it to the next one.
				if(RailSwitch->GetSwitchPosition() != trunc(Value)) RailSwitch->ToggleSwitchPosition();
				return;
			}
		}
		else if(auto Panel = Cast<AFGBuildableLightsControlPanel>(Pointer.Target))
		{
			auto PanelData = Panel->GetLightControlData();

			if(Pointer.SourceName == "ColorSlotIndex")
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
		else if (auto sign = Cast<AFGBuildableWidgetSign>(Pointer.Target))
		{
			FPrefabSignData signData;
			sign->GetSignPrefabData(signData);
		
			if(Pointer.SourceName == "Emissive")
			{
				if(signData.Emissive == Value) return;
				signData.Emissive = Value;
			}
			else if(Pointer.SourceName == "Glossiness")
			{
				if(signData.Glossiness == Value) return;
				signData.Glossiness = Value;
			}
			else
			{
				if(signData.IconElementData.Contains(Pointer.SourceName.ToString()))
					if(signData.IconElementData[Pointer.SourceName.ToString()] == trunc(Value)) return;
			
				signData.IconElementData[Pointer.SourceName.ToString()] = trunc(Value);
			}

			sign->SetPrefabSignData(signData);
			return;
		}
		else if(Pointer.SourceName == "SetPendingPotential")
		{
			if(auto Building = Cast<AFGBuildableFactory>(Pointer.Target))
			{
				if (Building->GetPendingPotential() == Value) return;
			}
		}

		NumericSet(Pointer, Value);
	}
	
	static FString GetString(const FConnectionPointer& Pointer, FString DefaultValue = "") 
	{
		if(auto Sign = Cast<AFGBuildableWidgetSign>(Pointer.Target))
			return UReflectionExternalFunctions::GetSignText(Sign, Pointer.SourceName.ToString(), DefaultValue);
		else if(auto station = Cast<AFGBuildableRailroadStation>(Pointer.Target))
			return station->GetStationIdentifier()->GetStationName().ToString();
		else if (auto Portal = Cast<AFGBuildablePortalBase>(Pointer.Target))
			return Portal->GetPortalName().ToString();
		else
			return GenericProcess<FString, FStrProperty>(Pointer, nullptr, DefaultValue);
	}

	static void SetString(const FConnectionPointer& Pointer, FString Value)
	{
		if(auto sign = Cast<AFGBuildableWidgetSign>(Pointer.Target))
		{
			FPrefabSignData signData;

			sign->GetSignPrefabData(signData);
		
			if(signData.TextElementData.Contains(Pointer.SourceName.ToString()))
				if(signData.TextElementData[Pointer.SourceName.ToString()].Equals(Value)) return;
		
			signData.TextElementData[Pointer.SourceName.ToString()] = Value;
			sign->SetPrefabSignData(signData);
		}
		else if(auto station = Cast<AFGBuildableRailroadStation>(Pointer.Target))
		{
			if (auto StationIdentifier = station->GetStationIdentifier())
				StationIdentifier->SetStationName(FText::FromString(Value));
		}
		else if (auto Portal = Cast<AFGBuildablePortalBase>(Pointer.Target))
			Portal->SetPortalName(FText::FromString(Value));
		else GenericSet<FStrProperty>(Pointer, Value, nullptr);
	}
	
	static FVector GetVector(const FConnectionPointer& Pointer, FVector DefaultValue = FVector::ZeroVector) { return GenericProcess<FVector, FStructProperty>(Pointer, nullptr, DefaultValue);}
	static void SetVector(const FConnectionPointer& Pointer, FVector Value){ GenericSet<FStructProperty>(Pointer, Value, nullptr); }
	
	static UFGInventoryComponent* GetInventory(const FConnectionPointer& Pointer) { return GenericProcess<UFGInventoryComponent*, FObjectPropertyBase>(Pointer, UFGInventoryComponent::StaticClass(), nullptr); }
	static void SetInventory(const FConnectionPointer& Pointer, UFGInventoryComponent* Value) { GenericSet(Pointer, Value, UFGInventoryComponent::StaticClass()); }
	
	static UFGPowerCircuit* GetCircuit(const FConnectionPointer& Pointer) { return GenericProcess<UFGPowerCircuit*, FObjectPropertyBase>(Pointer, UFGPowerCircuit::StaticClass(), nullptr); }
	static void SetCircuit(const FConnectionPointer& Pointer, UFGPowerCircuit* Value) { GenericSet(Pointer, Value, UFGPowerCircuit::StaticClass()); }
	
	static AActor* GetEntity(const FConnectionPointer& Pointer) 
	{
		if(Pointer.SourceName == "Self")
			return Cast<AActor>(Pointer.Target);
		else
			return GenericProcess<AActor*, FObjectPropertyBase>(Pointer, AActor::StaticClass(), nullptr);
	}
	static void SetEntity(const FConnectionPointer& Pointer, AActor* Value)
	{
		if (auto Portal = Cast<AFGBuildablePortalBase>(Pointer.Target))
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
		else GenericSet(Pointer, Value, AActor::StaticClass());
	}
	
	static TSubclassOf<UFGRecipe> GetRecipe(const FConnectionPointer& Pointer) { return GenericProcess(Pointer, UFGRecipe::StaticClass(),  TSubclassOf<UFGRecipe>()); }
	static void SetRecipe(const FConnectionPointer& Pointer, TSubclassOf<UFGRecipe> Value)
	{
		if(!Value) return;
		GenericSet(Pointer, Value, UFGRecipe::StaticClass());
	}
	
	static FLinearColor GetColor(const FConnectionPointer& Pointer, FLinearColor DefaultValue = FLinearColor::Black) { return GenericProcess<FLinearColor, FStructProperty>(Pointer, nullptr, DefaultValue); }
	static void SetColor(const FConnectionPointer& Pointer, FLinearColor Value)
	{
		if(auto sign = Cast<AFGBuildableWidgetSign>(Pointer.Target))
		{

			FPrefabSignData signData;
			sign->GetSignPrefabData(signData);
		
			if(Pointer.SourceName == "TextColor")
			{
				if(signData.ForegroundColor == Value) return;
				signData.ForegroundColor = Value;
			}
			else if(Pointer.SourceName == "BackgroundColor")
			{
				if(signData.BackgroundColor == Value) return;
				signData.BackgroundColor = Value;
			}
			else if(Pointer.SourceName == "AuxColor")
			{
				if(signData.AuxiliaryColor == Value) return;
				signData.AuxiliaryColor = Value;
			}

			sign->SetPrefabSignData(signData);
			return;
		}
		GenericSet<FStructProperty>(Pointer, Value, nullptr);
	}
	
	static FInventoryStack GetStack(const FConnectionPointer& Pointer) { return GenericProcess<FInventoryStack>(Pointer, FInventoryStack::StaticStruct()); }
	static void SetStack(const FConnectionPointer& Pointer, FInventoryStack Value) { GenericSet(Pointer, Value, FInventoryStack::StaticStruct()); }
	
	static FItemAmount GetItemAmount(const FConnectionPointer& Pointer) { return GenericProcess<FItemAmount>(Pointer, FItemAmount::StaticStruct()); }
	static void SetItemAmount(const FConnectionPointer& Pointer, FItemAmount Value) { GenericSet(Pointer, Value, FItemAmount::StaticStruct()); }
	
	static UTexture* GetTexture(const FConnectionPointer& Pointer) { return GenericProcess<UTexture*, FObjectPropertyBase>(Pointer, UTexture::StaticClass(), nullptr); }
	static void SetTexture(const FConnectionPointer& Pointer, UTexture* Value) { GenericSet(Pointer, Value, UTexture::StaticClass()); }
	
	static FSplitterSortRule GetSplitterRule(const FConnectionPointer& Pointer) { return GenericProcess<FSplitterSortRule>(Pointer, FSplitterSortRule::StaticStruct()); }
	static void SetSplitterRule(const FConnectionPointer& Pointer, FSplitterSortRule Value) { GenericSet(Pointer, Value, FSplitterSortRule::StaticStruct()); }

	static TSubclassOf<UFGItemDescriptor> GetItemDescriptor(const FConnectionPointer& Pointer, TSubclassOf<UFGItemDescriptor> DefaultValue) { return GenericProcess<TSubclassOf<UFGItemDescriptor>, FProperty>(Pointer, UFGItemDescriptor::StaticClass(), DefaultValue); }
	static void SetItemDescriptor(const FConnectionPointer& Pointer, TSubclassOf<UFGItemDescriptor> Value) { GenericSet(Pointer, Value, UFGItemDescriptor::StaticClass()); }

	static FTimeTableStop GetTimeTableStop(const FConnectionPointer& Pointer) { return GenericProcess<FTimeTableStop>(Pointer, FTimeTableStop::StaticStruct()); }
	static void SetTimeTableStop(const FConnectionPointer& Pointer, FTimeTableStop Value) { GenericSet(Pointer, Value, FTimeTableStop::StaticStruct()); }

	static UFGPowerInfoComponent* GetPowerInfo(UObject* Object) { return UReflectionExternalFunctions::GetPowerInfo(Object); }

	static FElevatorFloorStopInfo GetElevatorFloor(const FConnectionPointer& Pointer) { return GenericProcess<FElevatorFloorStopInfo>(Pointer, FElevatorFloorStopInfo::StaticStruct()); }
	static void SetElevatorFloor(const FConnectionPointer& Pointer, FElevatorFloorStopInfo Value) { GenericSet(Pointer, Value, FElevatorFloorStopInfo::StaticStruct()); }

	static FPixelScreenData GetPixelImage(const FConnectionPointer& Pointer) { return GenericProcess<FPixelScreenData>(Pointer, FPixelScreenData::StaticStruct()); }
	static void SetPixelImage(const FConnectionPointer& Pointer, FPixelScreenData Value) { GenericSet(Pointer, Value, FPixelScreenData::StaticStruct()); }

	static FCustomStruct GetCustomStruct(const FConnectionPointer& Pointer) { return GenericProcess<FCustomStruct>(Pointer, FCustomStruct::StaticStruct()); }
	static void SetCustomStruct(const FConnectionPointer& Pointer, FCustomStruct Value) { GenericSet(Pointer, Value, FCustomStruct::StaticStruct()); }

	static TArray<bool> GetBoolArray(const FConnectionPointer& Pointer) { return GenericProcess<TArray<bool>, FBoolProperty>(Pointer, nullptr); }
	static void SetBoolArray(const FConnectionPointer& Pointer, TArray<bool> Value) { GenericSet<FBoolProperty>(Pointer, Value, nullptr); }
	
	static TArray<double> GetFloatArray(const FConnectionPointer& Pointer)
	{
		const FName ConveyorMonitorAverage = FName("CC_MONITORAVG");
		if (auto ConveyorMonitor = Cast<AFGBuildableConveyorMonitor>(Pointer.Target); ConveyorMonitor && Pointer.SourceName == ConveyorMonitorAverage)
		{
			TArray<double> AverageGraph;
			for (const FItemMonitorData& Data : ConveyorMonitor->GetAverageDataForUIRepresentation())
			{
				AverageGraph.Add(Data.FloatLocalAverage);
			}
			
			return AverageGraph;
		}
		
		
		return GenericProcess<TArray<double>, FDoubleProperty>(Pointer, nullptr);
	}
	static void SetFloatArray(const FConnectionPointer& Pointer, TArray<double> Value) { GenericSet<FDoubleProperty>(Pointer, Value, nullptr); }
	
	static TArray<FString> GetStringArray(const FConnectionPointer& Pointer) { return GenericProcess<TArray<FString>, FStrProperty>(Pointer, nullptr); }
	static void SetStringArray(const FConnectionPointer& Pointer, const TArray<FString>& Value) { GenericSet<FStrProperty>(Pointer, Value, nullptr); }
	
	static TArray<FVector> GetVectorArray(const FConnectionPointer& Pointer) { return GenericProcess<TArray<FVector>, FStructProperty>(Pointer, nullptr); }
	static void SetVectorArray(const FConnectionPointer& Pointer, const TArray<FVector>& Value) { GenericSet<FStructProperty>(Pointer, Value, nullptr); }
	
	static TArray<UFGInventoryComponent*> GetInventoryArray(const FConnectionPointer& Pointer) { return GenericProcess<TArray<UFGInventoryComponent*>>(Pointer, UFGInventoryComponent::StaticClass()); }
	static void SetInventoryArray(const FConnectionPointer& Pointer, const TArray<UFGInventoryComponent*>& Value) { GenericSet(Pointer, Value, UFGInventoryComponent::StaticClass()); }
	
	static TArray<UFGPowerCircuit*> GetCircuitArray(const FConnectionPointer& Pointer) { return GenericProcess<TArray<UFGPowerCircuit*>>(Pointer, UFGPowerCircuit::StaticClass()); }
	static void SetCircuitArray(const FConnectionPointer& Pointer, const TArray<UFGPowerCircuit*>& Value) { GenericSet(Pointer, Value, UFGPowerCircuit::StaticClass()); }
	
	static TArray<AActor*> GetEntityArray(const FConnectionPointer& Pointer) { return GenericProcess<TArray<AActor*>>(Pointer, AActor::StaticClass()); }
	static void SetEntityArray(const FConnectionPointer& Pointer, const TArray<AActor*>& Value) { GenericSet(Pointer, Value, AActor::StaticClass()); }
	
	static TArray<TSubclassOf<UFGRecipe>> GetRecipeArray(const FConnectionPointer& Pointer) { return GenericProcess<TArray<TSubclassOf<UFGRecipe>>>(Pointer, UFGRecipe::StaticClass()); }
	static void SetRecipeArray(const FConnectionPointer& Pointer, const TArray<TSubclassOf<UFGRecipe>>& Value) { GenericSet(Pointer, Value, UFGRecipe::StaticClass()); }
	
	static TArray<FLinearColor> GetColorArray(const FConnectionPointer& Pointer) { return GenericProcess<TArray<FLinearColor>, FStructProperty>(Pointer, nullptr); }
	static void SetColorArray(const FConnectionPointer& Pointer, const TArray<FLinearColor>& Value) { GenericSet<FStructProperty>(Pointer, Value, nullptr); }
	
	static TArray<FInventoryStack> GetStackArray(const FConnectionPointer& Pointer) { return GenericProcess<TArray<FInventoryStack>>(Pointer, FInventoryStack::StaticStruct()); }
	static void SetStackArray(const FConnectionPointer& Pointer, const TArray<FInventoryStack>& Value) { GenericSet(Pointer, Value, FInventoryStack::StaticStruct()); }
	
	static TArray<FItemAmount> GetItemAmountArray(const FConnectionPointer& Pointer) { return GenericProcess<TArray<FItemAmount>>(Pointer, FItemAmount::StaticStruct()); }
	static void SetItemAmountArray(const FConnectionPointer& Pointer, const TArray<FItemAmount>& Value) { GenericSet(Pointer, Value, FItemAmount::StaticStruct()); }
	
	static TArray<UTexture*> GetTextureArray(const FConnectionPointer& Pointer) { return GenericProcess<TArray<UTexture*>>(Pointer, UTexture::StaticClass()); }
	static void SetTextureArray(const FConnectionPointer& Pointer, const TArray<UTexture*>& Value) { GenericSet(Pointer, Value, UTexture::StaticClass()); }
	
	static TArray<FSplitterSortRule> GetSplitterRuleArray(const FConnectionPointer& Pointer) { return GenericProcess<TArray<FSplitterSortRule>>(Pointer, FSplitterSortRule::StaticStruct()); }
	static void SetSplitterRuleArray(const FConnectionPointer& Pointer, const TArray<FSplitterSortRule>& Value) { GenericSet(Pointer, Value, FSplitterSortRule::StaticStruct()); }

	static TArray<TSubclassOf<UFGItemDescriptor>> GetItemDescriptorArray(const FConnectionPointer& Pointer) { return GenericProcess<TArray<TSubclassOf<UFGItemDescriptor>>>(Pointer, UFGItemDescriptor::StaticClass()); }
	static void SetItemDescriptorArray(const FConnectionPointer& Pointer, const TArray<TSubclassOf<UFGItemDescriptor>>& Value) { GenericSet(Pointer, Value, UFGItemDescriptor::StaticClass()); }

	static TArray<FTimeTableStop> GetTimeTableStopArray(const FConnectionPointer& Pointer) { return GenericProcess<TArray<FTimeTableStop>>(Pointer, FTimeTableStop::StaticStruct()); }
	static void SetTimeTableStopArray(const FConnectionPointer& Pointer, const TArray<FTimeTableStop>& Value) { GenericSet(Pointer, Value, FTimeTableStop::StaticStruct()); }

	static TArray<FElevatorFloorStopInfo> GetElevatorFloorArray(const FConnectionPointer& Pointer) { return GenericProcess<TArray<FElevatorFloorStopInfo>>(Pointer, FElevatorFloorStopInfo::StaticStruct()); }
	static void SetElevatorFloorArray(const FConnectionPointer& Pointer, const TArray<FElevatorFloorStopInfo>& Value) { GenericSet(Pointer, Value, FElevatorFloorStopInfo::StaticStruct()); }

	static TArray<FPixelScreenData> GetPixelImageArray(const FConnectionPointer& Pointer) { return GenericProcess<TArray<FPixelScreenData>>(Pointer, FPixelScreenData::StaticStruct()); }
	static void SetPixelImageArray(const FConnectionPointer& Pointer, const TArray<FPixelScreenData>& Value) { GenericSet(Pointer, Value, FPixelScreenData::StaticStruct()); }

	static TArray<FCustomStruct> GetCustomStructArray(const FConnectionPointer& Pointer) { return GenericProcess<TArray<FCustomStruct>>(Pointer, FCustomStruct::StaticStruct()); }
	static void SetCustomStructArray(const FConnectionPointer& Pointer, const TArray<FCustomStruct>& Value) { GenericSet(Pointer, Value, FCustomStruct::StaticStruct()); }

	template<typename ReturnType> static ReturnType GetUnmanaged(const FConnectionPointer& Pointer, ReturnType DefaultValue = ReturnType()){ return GenericProcess(Pointer, nullptr, DefaultValue); }
	template<typename ValueType> static void SetUnmanaged(const FConnectionPointer& Pointer, ValueType Value) { GenericSet(Pointer, Value, nullptr); }
	
	template <typename ReturnType, typename PropType>
	static ReturnType FromPropertyValue(const FConnectionPointer& Pointer, ReturnType DefaultValue, UStruct* StructTypeClass)
	{
		if(!IsValid(Pointer.Target)) return DefaultValue;
		
		auto Val = Pointer.Target->GetClass()->FindPropertyByName(Pointer.SourceName);
		if(!Val) return DefaultValue;
		if (!CheckType<PropType>(Val, StructTypeClass)) return DefaultValue;
		
		auto ValuePointer = Val->ContainerPtrToValuePtr<ReturnType>(Pointer.Target);
		
		return *ValuePointer;
	}

	static double FromNumericPropertyValue(const FConnectionPointer& Pointer, double DefaultValue)
	{
		if(!IsValid(Pointer.Target)) return DefaultValue;
		
		auto Val = CastField<FNumericProperty>(Pointer.Target->GetClass()->FindPropertyByName(Pointer.SourceName));
		if(!Val) return DefaultValue;
		if(Val->IsInteger()) return Val->GetSignedIntPropertyValue(Val->ContainerPtrToValuePtr<void>(Pointer.Target));
		return Val->GetFloatingPointPropertyValue(Val->ContainerPtrToValuePtr<void>(Pointer.Target));
	}

	template<typename ReturnType, typename PropType = FProperty>
	static ReturnType GenericProcess(const FConnectionPointer& Pointer, UStruct* ScriptStruct, ReturnType DefaultValue = ReturnType())
	{
		if(!IsValid(Pointer.Target)) return DefaultValue;
	
		if(Pointer.IsProperty)
			return FromPropertyValue<ReturnType, PropType>(Pointer, DefaultValue, ScriptStruct);
	
		if(Pointer.Target->GetClass()->ImplementsInterface(IDynamicValuePasser::UClassType::StaticClass()))
		{
			auto ValueBase = IDynamicValuePasser::Execute_GetValue(Pointer.Target, Pointer.SourceName.ToString());
			auto ValuePointer = FConnectionPointer(ValueBase, "Value", true);
			return FromPropertyValue<ReturnType, PropType>(ValuePointer, DefaultValue, ScriptStruct);
		}
		
		struct{ReturnType RetVal;} Params{DefaultValue};
		bool IsSuccessfulFunctionCall = ProcessFunction<PropType>(Pointer, &Params, ScriptStruct);
		if(!IsSuccessfulFunctionCall) return FromPropertyValue<ReturnType, PropType>(Pointer, DefaultValue, ScriptStruct);
		return Params.RetVal;
	}

	static double NumericProcess(const FConnectionPointer& Pointer, double DefaultValue)
	{
		if(!Pointer.Target) return DefaultValue;

		if(Pointer.IsProperty)
			return FromNumericPropertyValue(Pointer, DefaultValue);

		if(Pointer.Target->GetClass()->ImplementsInterface(IDynamicValuePasser::UClassType::StaticClass()))
		{
			auto ValueBase = IDynamicValuePasser::Execute_GetValue(Pointer.Target, Pointer.SourceName.ToString());
			return FromNumericPropertyValue(FConnectionPointer(ValueBase, "Value", true), DefaultValue);
		}
		
		auto Function = Pointer.Target->FindFunction(Pointer.SourceName);
		if(!Function) return FromNumericPropertyValue(Pointer, DefaultValue);
		
		auto FuncProperty = Function->ChildProperties;
		if(!FuncProperty) return FromNumericPropertyValue(Pointer, DefaultValue);

		//Explicit return value check because UE5 now uses double, but some code still uses float so we have to account for that,
		//Not to mention int returns as well, i am so fucking done with this code.
		if(FuncProperty->IsA<FIntProperty>()) return GenericProcess<int, FIntProperty>(Pointer, nullptr, DefaultValue);
		else if(FuncProperty->IsA<FEnumProperty>()) return GenericProcess<int, FEnumProperty>(Pointer, nullptr, DefaultValue);
		else if(FuncProperty->IsA<FFloatProperty>()) return GenericProcess<float, FFloatProperty>(Pointer, nullptr, DefaultValue);
		else if (FuncProperty->IsA<FDoubleProperty>()) return GenericProcess<double, FDoubleProperty>(Pointer, nullptr, DefaultValue);
		else return DefaultValue;
	}

	template<typename PropertyType = FProperty, typename ValueType>
	static void GenericSet(const FConnectionPointer& Pointer, ValueType Value, UStruct* StructTypeClass)
	{
		if(Pointer.IsProperty)
		{
			auto Val = Pointer.Target->GetClass()->FindPropertyByName(Pointer.SourceName);
			if(Val) *Val->ContainerPtrToValuePtr<ValueType>(Pointer.Target) = Value;
			return;
		}
		
		struct{ValueType SetVal;} Params{Value};
		ProcessFunction<PropertyType>(Pointer, &Params, StructTypeClass);
		
	}

	static void NumericSet(const FConnectionPointer& Pointer, double Value)
	{
		if(Pointer.IsProperty)
		{
			auto Val = CastField<FNumericProperty>(Pointer.Target->GetClass()->FindPropertyByName(Pointer.SourceName));
			if(!Val) return;
			if(Val->IsInteger()) Val->SetIntPropertyValue(Val->ContainerPtrToValuePtr<void>(Pointer.Target), (int64) Value);
			else Val->SetFloatingPointPropertyValue(Val->ContainerPtrToValuePtr<void>(Pointer.Target), Value);
		}

		auto Function = Pointer.Target->FindFunction(Pointer.SourceName);
		if(!Function) return;
		
		auto FuncProperty = Function->ChildProperties;
		if(!FuncProperty) return;

		if(FuncProperty->IsA<FIntProperty>())
		{
			struct { int Value; } params{(int) Value};
			ProcessFunction<FIntProperty>(Pointer, &params, nullptr);
		}
		else if(FuncProperty->IsA<FEnumProperty>())
		{
			struct { int Value; } params{(int) Value};
			ProcessFunction<FEnumProperty>(Pointer, &params, nullptr);
		}
		else if(FuncProperty->IsA<FFloatProperty>())
		{
			struct { float Value; } params{(float) Value};
			ProcessFunction<FFloatProperty>(Pointer, &params, nullptr);
		}
		else
		{
			struct { double Value; } params{Value};
			ProcessFunction<FDoubleProperty>(Pointer, &params, nullptr);
		}
	}


	template<typename PropType>
	static bool ProcessFunction(const FConnectionPointer& Pointer, void* Params, UStruct* StructTypeClass)
	{
		if(!IsValid(Pointer.Target)) return false;
		
		UFunction* Function = Pointer.Target->FindFunction(Pointer.SourceName);
		if (!Function) return false;
		if(!CheckType<PropType>(Function->GetReturnProperty(), StructTypeClass)) return false;

		Pointer.Target->ProcessEvent(Function, Params);
		return true;
	}
	
	template<typename PropType>
	static bool CheckType(FProperty* Property, UStruct* StructTypeClass)
	{
		if (!Property) return true;

		if (Property->IsA<FArrayProperty>())
			Property = ((FArrayProperty*) Property)->Inner;
		
		
		if (Property->IsA<FStructProperty>())
		{
			//If the provided struct type is null then we skip this check and assume the type is correct.
			if (StructTypeClass == nullptr) return true;
			auto StructClass = ((FStructProperty*) Property)->Struct;

			if (StructClass == StructTypeClass) return true;
			
			return StructClass.IsA((UClass*)StructTypeClass);
		}

		return Property->IsA<PropType>();
	}
};
