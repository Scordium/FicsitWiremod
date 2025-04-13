// 

#pragma once

#include "CoreMinimal.h"
#include "CCBoolValue.h"
#include "CCCircuitValue.h"
#include "CCColorValue.h"
#include "CCCustomStructValue.h"
#include "CCDynamicValueBase.h"
#include "CCEntityValue.h"
#include "CCIntegerValue.h"
#include "CCInventoryValue.h"
#include "CCItemAmountValue.h"
#include "CCItemDescriptorValue.h"
#include "CCNumberValue.h"
#include "CCPixelImageValue.h"
#include "CCRecipeValue.h"
#include "CCStackValue.h"
#include "CCStringValue.h"
#include "CCTextureValue.h"
#include "CCVectorValue.h"
#include "CCSplitterRuleValue.h"
#include "CCTimeTableStopValue.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "CCDynamicValueUtils.generated.h"

/**
 * 
 */
UCLASS(Blueprintable,BlueprintType)
class FICSITWIREMOD_API UCCDynamicValueUtils : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintPure, meta=(CompactNodeTitle="->")) static UCCDynamicValueBase* FromType(EConnectionType Type, UObject* WorldContext)
	{
		if(!WorldContext) return nullptr;
		if(auto Out = Cast<UCCDynamicValueBase>(WorldContext); Out && Out->IsOfType(Type)) return Out;
		
		auto Level = WorldContext->GetClass()->IsChildOf(AActor::StaticClass()) ? WorldContext : WorldContext->GetWorld()->PersistentLevel;
		switch (Type)
		{
		case Boolean: return NewObject<UCCBoolValue>(Level);
		case Color: return NewObject<UCCColorValue>(Level);
		case Entity: return NewObject<UCCEntityValue>(Level);
		case Inventory: return NewObject<UCCInventoryValue>(Level);
		case Integer: return NewObject<UCCIntegerValue>(Level);
		case Number: return NewObject<UCCNumberValue>(Level);
		case Recipe: return NewObject<UCCRecipeValue>(Level);
		case Stack: return NewObject<UCCStackValue>(Level);
		case String: return NewObject<UCCStringValue>(Level);
		case Vector: return NewObject<UCCVectorValue>(Level);
		case CustomStruct: return NewObject<UCCCustomStructValue>(Level);
		case ItemAmount: return NewObject<UCCItemAmountValue>(Level);
		case PowerGrid: return NewObject<UCCCircuitValue>(Level);
		case PixelImage: return NewObject<UCCPixelImageValue>(Level);
		case Texture: return NewObject<UCCTextureValue>(Level);
		case SplitterRule: return NewObject<UCCSplitterRuleValue>(Level);
		case ItemDescriptor: return NewObject<UCCItemDescriptorValue>(Level);
		case TrainStop: return NewObject<UCCTimeTableStopValue>(Level);
		case ArrayOfBoolean: return NewObject<UCCBoolArrayValue>(Level);
		case ArrayOfColor: return NewObject<UCCColorArrayValue>(Level);
		case ArrayOfEntity: return NewObject<UCCEntityArrayValue>(Level);
		case ArrayOfInventory: return NewObject<UCCInventoryArrayValue>(Level);
		case ArrayOfNumber: return NewObject<UCCNumberArrayValue>(Level);
		case ArrayOfRecipe: return NewObject<UCCRecipeArrayValue>(Level);
		case ArrayOfStack: return NewObject<UCCStackArrayValue>(Level);
		case ArrayOfString: return NewObject<UCCStringArrayValue>(Level);
		case ArrayOfVector: return NewObject<UCCVectorArrayValue>(Level);
		case ArrayOfItemAmount: return NewObject<UCCItemAmountArrayValue>(Level);
		case ArrayOfPowerGrid: return NewObject<UCCCircuitArrayValue>(Level);
		case ArrayOfCustomStruct: return NewObject<UCCCustomStructArrayValue>(Level);
		case ArrayOfPixelImage: return NewObject<UCCPixelImageArrayValue>(Level);
		case ArrayOfTexture: return NewObject<UCCTextureArrayValue>(Level);
		case ArrayOfSplitterRule: return NewObject<UCCSplitterRuleArrayValue>(Level);
		case ArrayOfItemDescriptor: return NewObject<UCCItemDescriptorArrayValue>(Level);
		case ArrayOfTrainStop: return NewObject<UCCTimeTableStopArrayValue>(Level);
		case Unknown: return nullptr;
		default:
			auto TypeString = UEnum::GetValueAsString<EConnectionType>(Type);
			ACircuitryLogger::DispatchErrorEvent("Unknown connection type: " + TypeString + ". Failed to create dynamic value.");
			return nullptr;
		}
	}

	template<class T>
	static T* FromType(EConnectionType Type, UObject* WorldContext)
	{
		

		return Cast<T>(FromType(Type, WorldContext));
	}

	UFUNCTION(BlueprintCallable)
	static UCCDynamicValueBase* FromValue(const FConnectionData& Data, UObject* WorldContext, bool AllowSelfReturn = true)
	{
		if (AllowSelfReturn)
			if (auto Dynamic = Cast<UCCDynamicValueBase>(Data.Object)) return Dynamic;
		
		auto Out = FromType(Data.ConnectionType, WorldContext ? WorldContext : Data.Object);
		if(Out) Out->FromConnectionValue(Data.Object, Data.FunctionName, Data.FromProperty);
		return Out;
	}

	UFUNCTION(BlueprintCallable)
	static UCCDynamicValueBase* FromWrapper(UObject* WorldContext, const FDynamicValueStringWrapper& Wrapper)
	{
		auto ValueObject = FromType(Wrapper.Type, WorldContext);
		if(ValueObject) ValueObject->FromWrapperValue(Wrapper);
		return ValueObject;
	}
};
