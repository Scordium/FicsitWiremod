#pragma once

#include "CoreMinimal.h"
#include "ConnectionType.h"
#include "ReflectionUtilities.h"
#include "Kismet/KismetSystemLibrary.h"
#include "ValueStringResolverBase.generated.h"

UCLASS(Blueprintable, BlueprintType)
class UValueStringResolverBase : public UObject
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	FString ResolveString(UObject* Object, FName SourceName, EConnectionType Type, bool FromProperty) const;
	virtual FString ResolveString_Implementation(UObject* Object, FName SourceName, EConnectionType Type, bool FromProperty) const
	{
		switch (Type)
		{
		case Unknown: return "?";
		case Boolean: return UReflectionUtilities::GetBool(REFLECTION_ARGS) ? "true" : "false";
		case Number: return FString::SanitizeFloat(UReflectionUtilities::GetFloat(REFLECTION_ARGS));
		case String: return UReflectionUtilities::GetString(REFLECTION_ARGS);
		case Vector: return UReflectionUtilities::GetVector(REFLECTION_ARGS).ToCompactString();
		case Inventory:
			{
				auto inv = UReflectionUtilities::GetInventory(REFLECTION_ARGS);
				if(!inv) return "Invalid Inv.";

				TArray<FInventoryStack> Stacks;
				inv->GetInventoryStacks(Stacks);
				
				return CC_INT(Stacks.Num()) + "/" + CC_INT(inv->GetSizeLinear()) + " slots occupied";
			};
		case PowerGrid: return "?";
		case Entity:
			{
				auto Entity = UReflectionUtilities::GetEntity(REFLECTION_ARGS);
				auto ObjectName = UKismetSystemLibrary::GetObjectName(Entity);
				
				if(auto Player = Cast<AFGCharacterPlayer>(Entity))
				{
					//Check if the player state is valid. If the player is offline it will be null and crash if not handled properly
					if(auto State = Player->GetPlayerState()) return ObjectName + "(Player " + State->GetPlayerName() + ")";
					return ObjectName + "(Offline player)";
				}
				return ObjectName;
			}
		case Recipe:
			{
				auto Recipe = UReflectionUtilities::GetRecipe(REFLECTION_ARGS);
				return ::IsValid(Recipe) ? UFGRecipe::GetRecipeName(Recipe).ToString() : FString();
			}
		case Color: return UReflectionUtilities::GetColor(REFLECTION_ARGS).ToString();
		case ArrayOfBoolean: return "[" + CC_INT(UReflectionUtilities::GetBoolArray(REFLECTION_ARGS).Num()) + " bools]";
		case ArrayOfNumber: return "[" + CC_INT(UReflectionUtilities::GetFloatArray(REFLECTION_ARGS).Num()) + " numbers]";
		case ArrayOfString: return "[" + CC_INT(UReflectionUtilities::GetStringArray(REFLECTION_ARGS).Num()) + " strings]";
		case ArrayOfVector: return "[" + CC_INT(UReflectionUtilities::GetVectorArray(REFLECTION_ARGS).Num()) + " vectors]";
		case ArrayOfEntity: return "[" + CC_INT(UReflectionUtilities::GetEntityArray(REFLECTION_ARGS).Num()) + " entities]";
		case ArrayOfColor: return "[" + CC_INT(UReflectionUtilities::GetColorArray(REFLECTION_ARGS).Num()) + " colors]";
		case ArrayOfInventory: return "[" + CC_INT(UReflectionUtilities::GetInventoryArray(REFLECTION_ARGS).Num()) + " inventories]";
		case ArrayOfPowerGrid: return "[" + CC_INT(UReflectionUtilities::GetCircuitArray(REFLECTION_ARGS).Num()) + " power grids]";
		case ArrayOfStack: return "[" + CC_INT(UReflectionUtilities::GetStackArray(REFLECTION_ARGS).Num()) + " item stacks]";
		case ArrayOfRecipe: return "[" + CC_INT(UReflectionUtilities::GetRecipeArray(REFLECTION_ARGS).Num()) + " elements]";
		case Stack:
			{
				auto Stack = UReflectionUtilities::GetStack(REFLECTION_ARGS);
				return CC_INT(Stack.NumItems) + " " + UFGItemDescriptor::GetItemName(Stack.Item.GetItemClass()).ToString();
			}
		case Integer: return CC_INT(UReflectionUtilities::GetFloat(REFLECTION_ARGS));
		case Any: return "?";
		case AnyArray: return "?";
		case AnyNonArray: return "?";
		case ItemAmount:
			{
				auto Item = UReflectionUtilities::GetItemAmount(REFLECTION_ARGS);
				return CC_INT(Item.Amount) + " of " + UFGItemDescriptor::GetItemName(Item.ItemClass).ToString();
			}
		case ArrayOfItemAmount: return "[" + CC_INT(UReflectionUtilities::GetItemAmountArray(REFLECTION_ARGS).Num()) + " item amounts]";
		case CustomStruct:
			{
				auto Val = UReflectionUtilities::GetCustomStruct(REFLECTION_ARGS);
				return  Val.Name + " [" + CC_INT(Val.Values.Num()) + " values]";
			}

		case PixelImage:
			{
				auto Value = UReflectionUtilities::GetPixelImage(REFLECTION_ARGS);
				return CC_INT(Value.Width) + "x" + CC_INT(Value.Height);
			}

		case ArrayOfPixelImage: return "[" + CC_INT(UReflectionUtilities::GetPixelImageArray(REFLECTION_ARGS).Num()) + " pixel arts]";
			
		case Texture:
			if (auto Texture = UReflectionUtilities::GetTexture(REFLECTION_ARGS)) return Texture->GetClass()->GetName();
			else return "null or invalid";
			
		case ArrayOfTexture: return "[" + CC_INT(UReflectionUtilities::GetTextureArray(REFLECTION_ARGS).Num()) + " textures]";
		case SplitterRule:
			{
				auto Val = UReflectionUtilities::GetSplitterRule(REFLECTION_ARGS);
				auto Name = FStringFormatArg(Val.ItemClass == nullptr ? "N/A" : UFGItemDescriptor::GetItemName(Val.ItemClass).ToString());
				auto Index = FStringFormatArg(Val.OutputIndex);
				auto Args = FStringFormatOrderedArguments(TArray{Name, Index});
				return FString::Format(*FString("{0} [{1}]"), Args);
			}
		case ArrayOfSplitterRule: return "[" + CC_INT(UReflectionUtilities::GetSplitterRuleArray(REFLECTION_ARGS).Num()) + " rules]";
		case ItemDescriptor: return UFGItemDescriptor::GetItemName(UReflectionUtilities::GetItemDescriptor(REFLECTION_ARGS, TSubclassOf<UFGItemDescriptor>())).ToString();
		case ArrayOfItemDescriptor: return "[" + CC_INT(UReflectionUtilities::GetItemDescriptorArray(REFLECTION_ARGS).Num()) + " descriptors]";
		case TrainStop:
			{
				auto Station = UReflectionUtilities::GetTimeTableStop(REFLECTION_ARGS).Station;
				return Station ? Station->GetStationName().ToString() : "N/A";
			}
		case ArrayOfTrainStop: return "[" + CC_INT(UReflectionUtilities::GetTimeTableStopArray(REFLECTION_ARGS).Num()) + " stops]";
		case ElevatorFloor:
			{
				auto Floor = UReflectionUtilities::GetElevatorFloor(REFLECTION_ARGS);
				return Floor.FloorName + " (" + CC_INT(Floor.Height) + "m)";
			}
		case ArrayOfElevatorFloor: return "[" + CC_INT(UReflectionUtilities::GetElevatorFloorArray(REFLECTION_ARGS).Num()) + " floors]";
		default:
			auto TypeString = UEnum::GetValueAsString<EConnectionType>(Type);
			ACircuitryLogger::DispatchErrorEvent("Failed to find switch case for EConnectionType::" + TypeString + " in function RESOLVE_STRING. Returning default value instead...");
			return "?";
		}
	}
};
