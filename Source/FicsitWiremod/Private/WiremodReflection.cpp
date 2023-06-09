// Copyright Coffee Stain Studios. All Rights Reserved.


#include "WiremodReflection.h"
#include "FGTrainStationIdentifier.h"
#include "IConstantsDistributor.h"
#include "ReflectionExternalFunctions.h"
#include "Buildables/FGBuildableLightsControlPanel.h"
#include "Buildables/FGBuildableRailroadStation.h"
#include "Buildables/FGBuildableRailroadSwitchControl.h"

template <typename T>
static T FromPropertyValue(const FConnectionData& Data, T DefaultValue)
{
	if(!Data.Object) return DefaultValue;
		
	auto Val = Data.Object->GetClass()->FindPropertyByName(Data.FunctionName);
	if(!Val) return DefaultValue;
	return *Val->ContainerPtrToValuePtr<T>(Data.Object);
}

template<typename T>
static T GenericProcess(const FConnectionData& Data, T DefaultValue)
{
	if(!Data.Object) return DefaultValue;
	
	if(Data.FromProperty)
		return FromPropertyValue(Data, DefaultValue);
	
	if(Data.Object->GetClass()->ImplementsInterface(IDynamicValuePasser::UClassType::StaticClass()))
	{
		auto ValueBase = IDynamicValuePasser::Execute_GetValue(Data.Object, Data.FunctionName.ToString());
		return FromPropertyValue(FConnectionData(ValueBase, "Value"), DefaultValue);
	}

	struct{T RetVal;} Params{DefaultValue};
	if(!Data.ProcessFunction(&Params)) return FromPropertyValue(Data, DefaultValue);
	return Params.RetVal;
}

template<typename ValueType>
static void GenericSet(const FConnectionData& Data, ValueType Value)
{
	struct{ValueType RetVal;} Params{Value};
	Data.ProcessFunction(&Params);
}

bool FConnectionData::GetBool(bool DefaultValue) const
{
	if(FunctionName == "WM_DOORCONTROL_FUNC")
		return UReflectionExternalFunctions::GetDoorIsLocked(Object);
	else if(auto Panel = Cast<AFGBuildableLightsControlPanel>(Object); Panel && FunctionName == "IsTimeOfDayAware")
		return Panel->GetLightControlData().IsTimeOfDayAware;
	else
		return GenericProcess(*this, DefaultValue);
}

float FConnectionData::GetFloat(float DefaultValue) const
{
	if(auto panel = Cast<AFGBuildableLightsControlPanel>(Object))
	{
		//Control panels store their data in struct, so to get the values we have to manually disassemble the struct.
		if(FunctionName == "ColorSlotIndex")
			return panel->GetLightControlData().ColorSlotIndex;
		else
			return panel->GetLightControlData().Intensity;
	}
	else if(auto Sign = Cast<AFGBuildableWidgetSign>(Object))
		return UReflectionExternalFunctions::GetSignIconId(Sign, FunctionName.ToString(), DefaultValue);
	else if(auto FluidTank = Cast<AFGBuildablePipeReservoir>(Object))
	{
		if(FunctionName == "Content")
			return FluidTank->GetFluidBox()->Content;
		else
			return FluidTank->GetFluidBox()->MaxContent;
	}

	return ConnectionType == Integer ? GenericProcess<int>(*this, DefaultValue) : GenericProcess(*this, DefaultValue);
}

FString FConnectionData::GetString(FString DefaultValue) const
{
	if(auto Sign = Cast<AFGBuildableWidgetSign>(Object))
		return UReflectionExternalFunctions::GetSignText(Sign, FunctionName.ToString(), DefaultValue);
	else if(auto station = Cast<AFGBuildableRailroadStation>(Object))
		return station->GetStationIdentifier()->GetStationName().ToString();
	else
		return GenericProcess(*this, DefaultValue);
}

FVector FConnectionData::GetVector(FVector DefaultValue) const{ return GenericProcess(*this, DefaultValue); }
UFGInventoryComponent* FConnectionData::GetInventory() const{ return GenericProcess<UFGInventoryComponent*>(*this, nullptr); }
UFGPowerCircuit* FConnectionData::GetCircuit() const{ return GenericProcess<UFGPowerCircuit*>(*this, nullptr); }
AActor* FConnectionData::GetEntity() const
{
	if(FunctionName == "Self")
		return Cast<AActor>(Object);
	else
		return GenericProcess<AActor*>(*this, nullptr);
}
TSubclassOf<UFGRecipe> FConnectionData::GetRecipe() const
{
	TSubclassOf<UFGRecipe> Out = GenericProcess(*this, TSubclassOf<UFGRecipe>());
	return Out.GetDefaultObject() ? Out.GetDefaultObject()->GetClass() : TSubclassOf<UFGRecipe>();
}
FLinearColor FConnectionData::GetColor(FLinearColor DefaultValue) const{ return GenericProcess(*this, DefaultValue); }
FInventoryStack FConnectionData::GetStack() const{ return GenericProcess(*this, FInventoryStack()); }
FItemAmount FConnectionData::GetItemAmount() const{ return GenericProcess(*this, FItemAmount()); }
FCustomStruct FConnectionData::GetCustomStruct() const{ return GenericProcess(*this, FCustomStruct()); }
TArray<bool> FConnectionData::GetBoolArray() const{ return GenericProcess(*this, TArray<bool>()); }
TArray<float> FConnectionData::GetFloatArray() const{ return GenericProcess(*this, TArray<float>()); }
TArray<FString> FConnectionData::GetStringArray() const{ return GenericProcess(*this, TArray<FString>()); }
TArray<FVector> FConnectionData::GetVectorArray() const{ return GenericProcess(*this, TArray<FVector>()); }
TArray<UFGInventoryComponent*> FConnectionData::GetInventoryArray() const{ return GenericProcess(*this, TArray<UFGInventoryComponent*>()); }
TArray<UFGPowerCircuit*> FConnectionData::GetCircuitArray() const{ return GenericProcess(*this, TArray<UFGPowerCircuit*>()); }
TArray<AActor*> FConnectionData::GetEntityArray() const{ return GenericProcess(*this, TArray<AActor*>()); }
TArray<TSubclassOf<UFGRecipe>> FConnectionData::GetRecipeArray() const{ return GenericProcess(*this, TArray<TSubclassOf<UFGRecipe>>()); }
TArray<FLinearColor> FConnectionData::GetColorArray() const{ return GenericProcess(*this, TArray<FLinearColor>()); }
TArray<FInventoryStack> FConnectionData::GetStackArray() const{ return GenericProcess(*this, TArray<FInventoryStack>()); }
TArray<FItemAmount> FConnectionData::GetItemAmountArray() const{ return GenericProcess(*this, TArray<FItemAmount>()); }
TArray<FCustomStruct> FConnectionData::GetCustomStructArray() const{ return GenericProcess(*this, TArray<FCustomStruct>()); }

void FConnectionData::SetBool(bool Value) const
{
	if(FromProperty)
	{
		if(auto Property = Cast<FBoolProperty>(Object->GetClass()->FindPropertyByName(FunctionName)))
		{
			Property->SetPropertyValue_InContainer(Object, Value);
			return;
		}
	}

	
	if(FunctionName.ToString().StartsWith("WM_") && Value)
	{
		if(FunctionName == "WM_FLUSHTANK_FUNC") UReflectionExternalFunctions::FlushTank(Object);
		else if(FunctionName == "WM_FLUSHNET_FUNC") UReflectionExternalFunctions::FlushNetwork(Object);
		else if(FunctionName == "WM_ON_USE_FUNC") UReflectionExternalFunctions::ExecuteOnUse(Object);
	}
	else if(FunctionName.ToString().StartsWith("WMCL_"))
	{
		if(FunctionName == "WMCL_DOORCONTROL_FUNC") UReflectionExternalFunctions::ChangeDoorState(Object, Value);
		else if(FunctionName == "WMCL_RAILSIGNAL_STOP") UReflectionExternalFunctions::ChangeRailroadSignalState(Object, Value);
		else if(FunctionName == "WMCL_FF_DOGGODOOR") UReflectionExternalFunctions::ChangeDoggoHouseDoorState(Object, Value);
	}
	
	if(auto panel = Cast<AFGBuildableLightsControlPanel>(Object))
	{
		
		if(FunctionName == "IsLightEnabled")
		{
			if(panel->IsLightEnabled() == Value) return;
			
			panel->SetLightEnabled(Value);
			auto affected = panel->GetControlledBuildables(AFGBuildableLightSource::StaticClass());
			for (auto light : affected)
			{
				Cast<AFGBuildableLightSource>(light)->SetLightEnabled(Value);
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

	GenericSet(*this, Value);
}

void FConnectionData::SetFloat(float Value) const
{
	if(FromProperty)
	{
		if(ConnectionType == Integer)
		{
			if(auto Property = Cast<FIntProperty>(Object->GetClass()->FindPropertyByName(FunctionName)))
			{
				Property->SetPropertyValue_InContainer(Object, Value);
				return;
			}	
		}
		else
		{
			if(auto Property = Cast<FFloatProperty>(Object->GetClass()->FindPropertyByName(FunctionName)))
			{
				Property->SetPropertyValue_InContainer(Object, Value);
				return;
			}
		}
	}

	
	if(FunctionName == "WM_RAILSWITCH_FUNC")
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

		if(FunctionName == "ColorSlotIndex")
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
		
		if(FunctionName == "Emissive")
		{
			if(signData.Emissive == Value) return;
			signData.Emissive = Value;
		}
		else if(FunctionName == "Glossiness")
		{
			if(signData.Glossiness == Value) return;
			signData.Glossiness = Value;
		}
		else
		{
			if(signData.IconElementData.Contains(FunctionName.ToString()))
				if(signData.IconElementData[FunctionName.ToString()] == trunc(Value)) return;
			
			signData.IconElementData[FunctionName.ToString()] = trunc(Value);
		}

		sign->SetPrefabSignData(signData);
		return;
	}
	else if(FunctionName == "SetPendingPotential")
	{
		if(auto Building = Cast<AFGBuildableFactory>(Object))
		{
			if (Building->GetPendingPotential() == Value) return;
		}
	}

	if(ConnectionType == Integer)
		GenericSet<int>(*this, Value);
	else
		GenericSet(*this, Value);
}

void FConnectionData::SetString(FString Value) const
{
	if(FromProperty)
	{
		if(auto Property = Cast<FStrProperty>(Object->GetClass()->FindPropertyByName(FunctionName)))
		{
			Property->SetPropertyValue_InContainer(Object, Value);
			return;
		}
	}
	
	if(auto sign = Cast<AFGBuildableWidgetSign>(Object))
	{
		FPrefabSignData signData;

		sign->GetSignPrefabData(signData);
		
		if(signData.TextElementData.Contains(FunctionName.ToString()))
			if(signData.TextElementData[FunctionName.ToString()] == Value) return;
		
		signData.TextElementData[FunctionName.ToString()] = Value;
		sign->SetPrefabSignData(signData);
		
		return;
	}
	else if(auto station = Cast<AFGBuildableRailroadStation>(Object))
	{
		station->GetStationIdentifier()->SetStationName(FText::FromString(Value));
		return;
	}
	
	GenericSet(*this, Value);
}

void FConnectionData::SetColor(FLinearColor Value) const
{
	if(FromProperty)
	{
		if(auto Property = Cast<FStructProperty>(Object->GetClass()->FindPropertyByName(FunctionName)))
		{
			auto StructValue = Property->ContainerPtrToValuePtr<FLinearColor>(Object);
			StructValue->R = Value.R;
			StructValue->G = Value.G;
			StructValue->B = Value.B;
			StructValue->A = Value.A;
			return;
		}
	}
	
	if(auto sign = Cast<AFGBuildableWidgetSign>(Object))
	{

		FPrefabSignData signData;
		sign->GetSignPrefabData(signData);
		
		if(FunctionName == "TextColor")
		{
			if(signData.ForegroundColor == Value) return;
			signData.ForegroundColor = Value;
		}
		else if(FunctionName == "BackgroundColor")
		{
			if(signData.BackgroundColor == Value) return;
			signData.BackgroundColor = Value;
		}
		else if(FunctionName == "AuxColor")
		{
			if(signData.AuxiliaryColor == Value) return;
			signData.AuxiliaryColor = Value;
		}

		sign->SetPrefabSignData(signData);
		return;
	}
	GenericSet(*this, Value);
}

void FConnectionData::SetRecipe(TSubclassOf<UFGRecipe> Value) const
{
	if(!Value.GetDefaultObject()) return;
	
	GenericSet(*this, Value);
}

bool FConnectionData::ProcessFunction(void* Params) const
{
	if(IsValid())
	{
		if(auto Function = Object->FindFunction(FunctionName))
		{
			Object->ProcessEvent(Function, Params);
			return true;
		}
	}
	return false;
}

FString FConnectionData::GetStringifiedValue() const
	{
		switch (ConnectionType)
		{
		case Unknown: return "?";
		case Boolean: return GetBool() ? "true" : "false";
		case Number: return FString::SanitizeFloat(GetFloat());
		case String: return GetString();
		case Vector: return GetVector().ToCompactString();
		case Inventory:
			{
				auto inv = GetInventory();
				if(!inv) return "Invalid Inv.";

				TArray<FInventoryStack> Stacks;
				inv->GetInventoryStacks(Stacks);
				
				return FString::FromInt(Stacks.Num()) + "/" + FString::FromInt(inv->GetSizeLinear()) + " slots occupied";
			};
		case PowerGrid: return "?";
		case Entity:
			{
				auto Entity = GetEntity();
				auto ObjectName = UKismetSystemLibrary::GetObjectName(Entity);
				
				if(auto Player = Cast<AFGCharacterPlayer>(Entity))
				{
					//Check if the player state is valid. If the player is offline it will be null and crash if not handled properly
					if(auto State = Player->GetPlayerState()) return ObjectName + "(Player " + State->GetPlayerName() + ")";
					return UKismetSystemLibrary::GetObjectName(Player) + "(Offline player)";
				}
				return UKismetSystemLibrary::GetObjectName(Entity);
			}
		case Recipe:
			{
				auto Recipe = GetRecipe();
				return ::IsValid(Recipe) ? UFGRecipe::GetRecipeName(Recipe).ToString() : FString();
			}
		case Color: return GetColor().ToString();
		case ArrayOfBoolean: return "[" + FString::FromInt(GetBoolArray().Num()) + " elements]";
		case ArrayOfNumber: return "[" + FString::FromInt(GetFloatArray().Num()) + " elements]";
		case ArrayOfString: return "[" + FString::FromInt(GetStringArray().Num()) + " elements]";
		case ArrayOfVector: return "[" + FString::FromInt(GetVectorArray().Num()) + " elements]";
		case ArrayOfEntity: return "[" + FString::FromInt(GetEntityArray().Num()) + " elements]";
		case ArrayOfColor: return "[" + FString::FromInt(GetColorArray().Num()) + " elements]";
		case ArrayOfInventory: return "[" + FString::FromInt(GetInventoryArray().Num()) + " elements]";
		case ArrayOfPowerGrid: return "[" + FString::FromInt(GetCircuitArray().Num()) + " elements]";
		case ArrayOfStack: return "[" + FString::FromInt(GetStackArray().Num()) + " elements]";
		case ArrayOfRecipe: return "[" + FString::FromInt(GetRecipeArray().Num()) + " elements]";
		case Stack:
			{
				auto Stack = GetStack();
				return FString::FromInt(Stack.NumItems) + " " + UFGItemDescriptor::GetItemName(Stack.Item.GetItemClass()).ToString();
			}
		case Integer: return FString::FromInt(GetFloat());
		case Any: return "?";
		case AnyArray: return "?";
		case AnyNonArray: return "?";
		case ItemAmount:
			{
				auto Item = GetItemAmount();
				return FString::FromInt(Item.Amount) + " of " + UFGItemDescriptor::GetItemName(Item.ItemClass).ToString();
			}
		case ArrayOfItemAmount: return "[" + FString::FromInt(GetItemAmountArray().Num()) + " elements]";
		case CustomStruct:
			{
				auto Val = GetCustomStruct();
				return  Val.Name + " [" + FString::FromInt(Val.Values.Num()) + " values]";
			}
		default:
			UE_LOG(LogTemp, Error, TEXT("Failed to find switch case for EConnectionType::%d in function GET_STRINGIFIED_VALUE. Returning default value instead..."), (int)ConnectionType);
			return "?";
		}
	}
