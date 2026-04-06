// 

#pragma once

#include "CoreMinimal.h"
#include "ConnectionPointer.h"
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
	
	static bool GetBool(const FConnectionPointer& Pointer, bool DefaultValue = false) 
	{
		if(Pointer.SourceName == "WM_DOORCONTROL_FUNC")
			return UReflectionExternalFunctions::GetDoorIsLocked(Pointer.Target);
		else if(Pointer.SourceName == "WM_DRONE_DOCKED")
			return UReflectionExternalFunctions::GetDroneIsInStation(Pointer.Target);
		else if(auto Panel = Cast<AFGBuildableLightsControlPanel>(Pointer.Target); Panel && Pointer.SourceName == "IsTimeOfDayAware")
			return Panel->GetLightControlData().IsTimeOfDayAware;
		else
			return GenericProcess(Pointer,  DefaultValue);
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

		GenericSet(Pointer, Value);
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
			return GenericProcess<FString, FStrProperty>(Pointer,  DefaultValue);
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
		else GenericSet(Pointer, Value);
	}
	
	static FVector GetVector(const FConnectionPointer& Pointer, FVector DefaultValue = FVector::ZeroVector) { return GenericProcess<FVector, FStructProperty>(Pointer, DefaultValue);}
	static void SetVector(const FConnectionPointer& Pointer, FVector Value){ GenericSet(Pointer, Value); }
	
	static UFGInventoryComponent* GetInventory(const FConnectionPointer& Pointer) { return GenericProcess<UFGInventoryComponent*, FObjectPropertyBase>(Pointer); }
	static void SetInventory(const FConnectionPointer& Pointer, UFGInventoryComponent* Value) { GenericSet(Pointer, Value); }
	
	static UFGPowerCircuit* GetCircuit(const FConnectionPointer& Pointer) { return GenericProcess<UFGPowerCircuit*, FObjectPropertyBase>(Pointer); }
	static void SetCircuit(const FConnectionPointer& Pointer, UFGPowerCircuit* Value) { GenericSet(Pointer, Value); }
	
	static AActor* GetEntity(const FConnectionPointer& Pointer) 
	{
		if(Pointer.SourceName == "Self")
			return Cast<AActor>(Pointer.Target);
		else
			return GenericProcess<AActor*, FObjectPropertyBase>(Pointer, nullptr);
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
		else GenericSet(Pointer, Value);
	}
	
	static TSubclassOf<UFGRecipe> GetRecipe(const FConnectionPointer& Pointer) { return GenericProcess(Pointer,  TSubclassOf<UFGRecipe>()); }
	static void SetRecipe(const FConnectionPointer& Pointer, TSubclassOf<UFGRecipe> Value)
	{
		if(!Value) return;
		GenericSet(Pointer, Value);
	}
	
	static FLinearColor GetColor(const FConnectionPointer& Pointer, FLinearColor DefaultValue = FLinearColor::Black) { return GenericProcess<FLinearColor, FStructProperty>(Pointer,  DefaultValue); }
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
		GenericSet(Pointer, Value);
	}
	
	static FInventoryStack GetStack(const FConnectionPointer& Pointer) { return GenericProcess<FInventoryStack, FStructProperty>(Pointer); }
	static void SetStack(const FConnectionPointer& Pointer, FInventoryStack Value) { GenericSet(Pointer, Value); }
	
	static FItemAmount GetItemAmount(const FConnectionPointer& Pointer) { return GenericProcess<FItemAmount, FStructProperty>(Pointer); }
	static void SetItemAmount(const FConnectionPointer& Pointer, FItemAmount Value) { GenericSet(Pointer, Value); }
	
	static UTexture* GetTexture(const FConnectionPointer& Pointer) { return GenericProcess<UTexture*>(Pointer); }
	static void SetTexture(const FConnectionPointer& Pointer, UTexture* Value) { GenericSet(Pointer, Value); }
	
	static FSplitterSortRule GetSplitterRule(const FConnectionPointer& Pointer) { return GenericProcess<FSplitterSortRule, FStructProperty>(Pointer); }
	static void SetSplitterRule(const FConnectionPointer& Pointer, FSplitterSortRule Value) { GenericSet(Pointer, Value); }

	static TSubclassOf<UFGItemDescriptor> GetItemDescriptor(const FConnectionPointer& Pointer, TSubclassOf<UFGItemDescriptor> DefaultValue) { return GenericProcess(Pointer, DefaultValue); }
	static void SetItemDescriptor(const FConnectionPointer& Pointer, TSubclassOf<UFGItemDescriptor> Value) { GenericSet(Pointer, Value); }

	static FTimeTableStopData GetTimeTableStop(const FConnectionPointer& Pointer) { return GenericProcess<FTimeTableStopData, FStructProperty>(Pointer); }
	static void SetTimeTableStop(const FConnectionPointer& Pointer, FTimeTableStopData Value) { GenericSet(Pointer, Value); }

	static UFGPowerInfoComponent* GetPowerInfo(UObject* Object) { return UReflectionExternalFunctions::GetPowerInfo(Object); }

	static FElevatorFloorStopInfo GetElevatorFloor(const FConnectionPointer& Pointer) { return GenericProcess<FElevatorFloorStopInfo, FStructProperty>(Pointer); }
	static void SetElevatorFloor(const FConnectionPointer& Pointer, FElevatorFloorStopInfo Value) { GenericSet(Pointer, Value); }

	static FPixelScreenData GetPixelImage(const FConnectionPointer& Pointer) { return GenericProcess<FPixelScreenData, FStructProperty>(Pointer); }
	static void SetPixelImage(const FConnectionPointer& Pointer, FPixelScreenData Value) { GenericSet(Pointer, Value); }

	static FCustomStruct GetCustomStruct(const FConnectionPointer& Pointer) { return GenericProcess<FCustomStruct, FStructProperty>(Pointer); }
	static void SetCustomStruct(const FConnectionPointer& Pointer, FCustomStruct Value) { GenericSet(Pointer, Value); }

	static TArray<bool> GetBoolArray(const FConnectionPointer& Pointer) { return GenericProcess<TArray<bool>, FArrayProperty>(Pointer); }
	static void SetBoolArray(const FConnectionPointer& Pointer, TArray<bool> Value) { GenericSet(Pointer, Value); }
	
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
		
		
		return GenericProcess<TArray<double>, FArrayProperty>(Pointer);
	}
	static void SetFloatArray(const FConnectionPointer& Pointer, TArray<double> Value) { GenericSet(Pointer, Value); }
	
	static TArray<FString> GetStringArray(const FConnectionPointer& Pointer) { return GenericProcess<TArray<FString>, FArrayProperty>(Pointer); }
	static void SetStringArray(const FConnectionPointer& Pointer, const TArray<FString>& Value) { GenericSet(Pointer, Value); }
	
	static TArray<FVector> GetVectorArray(const FConnectionPointer& Pointer) { return GenericProcess<TArray<FVector>, FArrayProperty>(Pointer); }
	static void SetVectorArray(const FConnectionPointer& Pointer, const TArray<FVector>& Value) { GenericSet(Pointer, Value); }
	
	static TArray<UFGInventoryComponent*> GetInventoryArray(const FConnectionPointer& Pointer) { return GenericProcess<TArray<UFGInventoryComponent*>, FArrayProperty>(Pointer); }
	static void SetInventoryArray(const FConnectionPointer& Pointer, const TArray<UFGInventoryComponent*>& Value) { GenericSet(Pointer, Value); }
	
	static TArray<UFGPowerCircuit*> GetCircuitArray(const FConnectionPointer& Pointer) { return GenericProcess<TArray<UFGPowerCircuit*>, FArrayProperty>(Pointer); }
	static void SetCircuitArray(const FConnectionPointer& Pointer, const TArray<UFGPowerCircuit*>& Value) { GenericSet(Pointer, Value); }
	
	static TArray<AActor*> GetEntityArray(const FConnectionPointer& Pointer) { return GenericProcess<TArray<AActor*>, FArrayProperty>(Pointer); }
	static void SetEntityArray(const FConnectionPointer& Pointer, const TArray<AActor*>& Value) { GenericSet(Pointer, Value); }
	
	static TArray<TSubclassOf<UFGRecipe>> GetRecipeArray(const FConnectionPointer& Pointer) { return GenericProcess<TArray<TSubclassOf<UFGRecipe>>, FArrayProperty>(Pointer); }
	static void SetRecipeArray(const FConnectionPointer& Pointer, const TArray<TSubclassOf<UFGRecipe>>& Value) { GenericSet(Pointer, Value); }
	
	static TArray<FLinearColor> GetColorArray(const FConnectionPointer& Pointer) { return GenericProcess<TArray<FLinearColor>, FArrayProperty>(Pointer); }
	static void SetColorArray(const FConnectionPointer& Pointer, const TArray<FLinearColor>& Value) { GenericSet(Pointer, Value); }
	
	static TArray<FInventoryStack> GetStackArray(const FConnectionPointer& Pointer) { return GenericProcess<TArray<FInventoryStack>, FArrayProperty>(Pointer); }
	static void SetStackArray(const FConnectionPointer& Pointer, const TArray<FInventoryStack>& Value) { GenericSet(Pointer, Value); }
	
	static TArray<FItemAmount> GetItemAmountArray(const FConnectionPointer& Pointer) { return GenericProcess<TArray<FItemAmount>, FArrayProperty>(Pointer); }
	static void SetItemAmountArray(const FConnectionPointer& Pointer, const TArray<FItemAmount>& Value) { GenericSet(Pointer, Value); }
	
	static TArray<UTexture*> GetTextureArray(const FConnectionPointer& Pointer) { return GenericProcess<TArray<UTexture*>, FArrayProperty>(Pointer); }
	static void SetTextureArray(const FConnectionPointer& Pointer, const TArray<UTexture*>& Value) { GenericSet(Pointer, Value); }
	
	static TArray<FSplitterSortRule> GetSplitterRuleArray(const FConnectionPointer& Pointer) { return GenericProcess<TArray<FSplitterSortRule>, FArrayProperty>(Pointer); }
	static void SetSplitterRuleArray(const FConnectionPointer& Pointer, const TArray<FSplitterSortRule>& Value) { GenericSet(Pointer, Value); }

	static TArray<TSubclassOf<UFGItemDescriptor>> GetItemDescriptorArray(const FConnectionPointer& Pointer) { return GenericProcess<TArray<TSubclassOf<UFGItemDescriptor>>, FArrayProperty>(Pointer); }
	static void SetItemDescriptorArray(const FConnectionPointer& Pointer, const TArray<TSubclassOf<UFGItemDescriptor>>& Value) { GenericSet(Pointer, Value); }

	static TArray<FTimeTableStopData> GetTimeTableStopArray(const FConnectionPointer& Pointer) { return GenericProcess<TArray<FTimeTableStopData>>(Pointer); }
	static void SetTimeTableStopArray(const FConnectionPointer& Pointer, const TArray<FTimeTableStopData>& Value) { GenericSet(Pointer, Value); }

	static TArray<FElevatorFloorStopInfo> GetElevatorFloorArray(const FConnectionPointer& Pointer) { return GenericProcess<TArray<FElevatorFloorStopInfo>>(Pointer); }
	static void SetElevatorFloorArray(const FConnectionPointer& Pointer, const TArray<FElevatorFloorStopInfo>& Value) { GenericSet(Pointer, Value); }

	static TArray<FPixelScreenData> GetPixelImageArray(const FConnectionPointer& Pointer) { return GenericProcess<TArray<FPixelScreenData>>(Pointer); }
	static void SetPixelImageArray(const FConnectionPointer& Pointer, const TArray<FPixelScreenData>& Value) { GenericSet(Pointer, Value); }

	static TArray<FCustomStruct> GetCustomStructArray(const FConnectionPointer& Pointer) { return GenericProcess<TArray<FCustomStruct>>(Pointer); }
	static void SetCustomStructArray(const FConnectionPointer& Pointer, const TArray<FCustomStruct>& Value) { GenericSet(Pointer, Value); }

	template<typename T>
	static T GetUnmanaged(const FConnectionPointer& Pointer, T DefaultValue = T()){ return GenericProcess(Pointer, DefaultValue); }

	template<typename T>
	static void SetUnmanaged(const FConnectionPointer& Pointer, T Value) { GenericSet(Pointer, Value); }
	
	template <typename T, typename PropType>
	static T FromPropertyValue(const FConnectionPointer& Pointer, T DefaultValue)
	{
		if(!IsValid(Pointer.Target)) return DefaultValue;
		
		auto Val = Pointer.Target->GetClass()->FindPropertyByName(Pointer.SourceName);
		if(!Val) return DefaultValue;
		if(!Val->IsA<PropType>()) return DefaultValue;
		
		auto ValuePointer = Val->ContainerPtrToValuePtr<T>(Pointer.Target);
		
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

	template<typename T, typename PropType = FProperty>
	static T GenericProcess(const FConnectionPointer& Pointer, T DefaultValue = T())
	{
		if(!IsValid(Pointer.Target)) return DefaultValue;
	
		if(Pointer.IsProperty)
			return FromPropertyValue<T, PropType>(Pointer, DefaultValue);
	
		if(Pointer.Target->GetClass()->ImplementsInterface(IDynamicValuePasser::UClassType::StaticClass()))
		{
			auto ValueBase = IDynamicValuePasser::Execute_GetValue(Pointer.Target, Pointer.SourceName.ToString());
			return FromPropertyValue<T, PropType>(FConnectionPointer(ValueBase, "Value", true), DefaultValue);
		}

		struct{T RetVal;} Params{DefaultValue};
		if(!ProcessFunction<PropType>(Pointer, &Params)) return FromPropertyValue<T, PropType>(Pointer, DefaultValue);
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
		if(FuncProperty->IsA<FIntProperty>()) return GenericProcess<int, FIntProperty>(Pointer, DefaultValue);
		else if(FuncProperty->IsA<FEnumProperty>()) return GenericProcess<int, FEnumProperty>(Pointer, DefaultValue);
		else if(FuncProperty->IsA<FFloatProperty>()) return GenericProcess<float, FFloatProperty>(Pointer, DefaultValue);
		else if (FuncProperty->IsA<FDoubleProperty>()) return GenericProcess<double, FDoubleProperty>(Pointer, DefaultValue);
		else return DefaultValue;
	}

	template<typename T>
	static void GenericSet(const FConnectionPointer& Pointer, T Value)
	{
		if(Pointer.IsProperty)
		{
			auto Val = Pointer.Target->GetClass()->FindPropertyByName(Pointer.SourceName);
			if(Val) *Val->ContainerPtrToValuePtr<T>(Pointer.Target) = Value;
			return;
		}
		
		struct{T SetVal;} Params{Value};
		ProcessFunction(Pointer, &Params);
		
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
			ProcessFunction<FIntProperty>(Pointer, &params);
		}
		else if(FuncProperty->IsA<FEnumProperty>())
		{
			struct { int Value; } params{(int) Value};
			ProcessFunction<FEnumProperty>(Pointer, &params);
		}
		else if(FuncProperty->IsA<FFloatProperty>())
		{
			struct { float Value; } params{(float) Value};
			ProcessFunction<FFloatProperty>(Pointer, &params);
		}
		else
		{
			struct { double Value; } params{Value};
			ProcessFunction<FDoubleProperty>(Pointer, &params);
		}
	}


	template<typename PropType = FProperty>
	static bool ProcessFunction(const FConnectionPointer& Pointer, void* Params)
	{
		if(IsValid(Pointer.Target))
		{
			UFunction* Function = Pointer.Target->FindFunction(Pointer.SourceName);;
			
			if(Function)
			{
				auto ReturnProp = Function->GetReturnProperty();
				if(ReturnProp && !ReturnProp->IsA<PropType>()) return false;
				
				Pointer.Target->ProcessEvent(Function, Params);
				return true;
			}
		}
		return false;
	}
};
