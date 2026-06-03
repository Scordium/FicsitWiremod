#pragma once

#include "CoreMinimal.h"
#include "CommonLib/ConnectionType.h"
#include "CommonLib/ReflectionUtilities.h"
#include "Kismet/KismetSystemLibrary.h"
#include "ValueStringResolverBase.generated.h"

UCLASS(Blueprintable, BlueprintType)
class UValueStringResolverBase : public UObject
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	FString ResolveString(const FConnectionPointer& Pointer, EConnectionType Type) const;
	virtual FString ResolveString_Implementation(const FConnectionPointer& Pointer, EConnectionType Type) const
	{
		switch (Type)
		{
		case Unknown: return "?";
		case Boolean: return UReflectionUtilities::GetBool(Pointer) ? "true" : "false";
		case Number: return FString::SanitizeFloat(UReflectionUtilities::GetFloat(Pointer));
		case String: return UReflectionUtilities::GetString(Pointer);
		case Vector: return UReflectionUtilities::GetVector(Pointer).ToCompactString();
		case Inventory:
			{
				auto inv = UReflectionUtilities::GetInventory(Pointer);
				if(!inv) return "Invalid Inv.";

				TArray<FInventoryStack> Stacks;
				inv->GetInventoryStacks(Stacks);
				
				return CC_INT(Stacks.Num()) + "/" + CC_INT(inv->GetSizeLinear()) + " slots occupied";
			};
		case PowerGrid: return "?";
		case Entity:
			{
				auto Entity = UReflectionUtilities::GetEntity(Pointer);
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
				auto Recipe = UReflectionUtilities::GetRecipe(Pointer);
				return ::IsValid(Recipe) ? UFGRecipe::GetRecipeName(Recipe).ToString() : FString();
			}
		case Color: return UReflectionUtilities::GetColor(Pointer).ToString();
		case ArrayOfBoolean: return "[" + CC_INT(UReflectionUtilities::GetBoolArray(Pointer).Num()) + " bools]";
		case ArrayOfNumber: return "[" + CC_INT(UReflectionUtilities::GetFloatArray(Pointer).Num()) + " numbers]";
		case ArrayOfString: return "[" + CC_INT(UReflectionUtilities::GetStringArray(Pointer).Num()) + " strings]";
		case ArrayOfVector: return "[" + CC_INT(UReflectionUtilities::GetVectorArray(Pointer).Num()) + " vectors]";
		case ArrayOfEntity: return "[" + CC_INT(UReflectionUtilities::GetEntityArray(Pointer).Num()) + " entities]";
		case ArrayOfColor: return "[" + CC_INT(UReflectionUtilities::GetColorArray(Pointer).Num()) + " colors]";
		case ArrayOfInventory: return "[" + CC_INT(UReflectionUtilities::GetInventoryArray(Pointer).Num()) + " inventories]";
		case ArrayOfPowerGrid: return "[" + CC_INT(UReflectionUtilities::GetCircuitArray(Pointer).Num()) + " power grids]";
		case ArrayOfStack: return "[" + CC_INT(UReflectionUtilities::GetStackArray(Pointer).Num()) + " item stacks]";
		case ArrayOfRecipe: return "[" + CC_INT(UReflectionUtilities::GetRecipeArray(Pointer).Num()) + " elements]";
		case Stack:
			{
				auto Stack = UReflectionUtilities::GetStack(Pointer);
				return CC_INT(Stack.NumItems) + " " + UFGItemDescriptor::GetItemName(Stack.Item.GetItemClass()).ToString();
			}
		case Integer: return CC_INT(UReflectionUtilities::GetFloat(Pointer));
		case Any: return "?";
		case AnyArray: return "?";
		case AnyNonArray: return "?";
		case ItemAmount:
			{
				auto Item = UReflectionUtilities::GetItemAmount(Pointer);
				return CC_INT(Item.Amount) + " of " + UFGItemDescriptor::GetItemName(Item.ItemClass).ToString();
			}
		case ArrayOfItemAmount: return "[" + CC_INT(UReflectionUtilities::GetItemAmountArray(Pointer).Num()) + " item amounts]";
		case CustomStruct:
			{
				auto Val = UReflectionUtilities::GetCustomStruct(Pointer);
				return  Val.Name + " [" + CC_INT(Val.Values.Num()) + " values]";
			}

		case PixelImage:
			{
				auto Value = UReflectionUtilities::GetPixelImage(Pointer);
				return CC_INT(Value.Width) + "x" + CC_INT(Value.Height);
			}

		case ArrayOfPixelImage: return "[" + CC_INT(UReflectionUtilities::GetPixelImageArray(Pointer).Num()) + " pixel arts]";
			
		case Texture:
			if (auto Texture = UReflectionUtilities::GetTexture(Pointer)) return Texture->GetClass()->GetName();
			else return "null or invalid";
			
		case ArrayOfTexture: return "[" + CC_INT(UReflectionUtilities::GetTextureArray(Pointer).Num()) + " textures]";
		case SplitterRule:
			{
				auto Val = UReflectionUtilities::GetSplitterRule(Pointer);
				auto Name = FStringFormatArg(Val.ItemClass == nullptr ? "N/A" : UFGItemDescriptor::GetItemName(Val.ItemClass).ToString());
				auto Index = FStringFormatArg(Val.OutputIndex);
				auto Args = FStringFormatOrderedArguments(TArray{Name, Index});
				return FString::Format(*FString("{0} [{1}]"), Args);
			}
		case ArrayOfSplitterRule: return "[" + CC_INT(UReflectionUtilities::GetSplitterRuleArray(Pointer).Num()) + " rules]";
		case ItemDescriptor: return UFGItemDescriptor::GetItemName(UReflectionUtilities::GetItemDescriptor(Pointer, TSubclassOf<UFGItemDescriptor>())).ToString();
		case ArrayOfItemDescriptor: return "[" + CC_INT(UReflectionUtilities::GetItemDescriptorArray(Pointer).Num()) + " descriptors]";
		case TrainStop:
			{
				auto Station = UReflectionUtilities::GetTimeTableStop(Pointer).Station;
				return Station ? Station->GetStationName().ToString() : "N/A";
			}
		case ArrayOfTrainStop: return "[" + CC_INT(UReflectionUtilities::GetTimeTableStopArray(Pointer).Num()) + " stops]";
		case ElevatorFloor:
			{
				auto Floor = UReflectionUtilities::GetElevatorFloor(Pointer);
				return Floor.FloorName + " (" + CC_INT(Floor.Height) + "m)";
			}
		case ArrayOfElevatorFloor: return "[" + CC_INT(UReflectionUtilities::GetElevatorFloorArray(Pointer).Num()) + " floors]";
		default:
			auto TypeString = UEnum::GetValueAsString<EConnectionType>(Type);
			ACircuitryLogger::DispatchErrorEvent("Failed to find switch case for EConnectionType::" + TypeString + " in function RESOLVE_STRING. Returning default value instead...");
			return "?";
		}
	}
};
