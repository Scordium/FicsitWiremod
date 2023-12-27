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
#include "FGInventoryComponent.h"
#include "FGPowerCircuit.h"
#include "FGRecipe.h"
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
	UFUNCTION(BlueprintPure, meta=(CompactNodeTitle="->")) static UCCDynamicValueBase* MakeBool(UObject* WorldContext, bool Value);
	UFUNCTION(BlueprintPure, meta=(CompactNodeTitle="->")) static UCCDynamicValueBase* MakeFloat(UObject* WorldContext, double Value);
	UFUNCTION(BlueprintPure, meta=(CompactNodeTitle="->")) static UCCDynamicValueBase* MakeInt(UObject* WorldContext, int Value);
	UFUNCTION(BlueprintPure, meta=(CompactNodeTitle="->")) static UCCDynamicValueBase* MakeString(UObject* WorldContext, FString Value);
	UFUNCTION(BlueprintPure, meta=(CompactNodeTitle="->")) static UCCDynamicValueBase* MakeVector(UObject* WorldContext, FVector Value);
	UFUNCTION(BlueprintPure, meta=(CompactNodeTitle="->")) static UCCDynamicValueBase* MakeInventory(UObject* WorldContext, UFGInventoryComponent* Value);
	UFUNCTION(BlueprintPure, meta=(CompactNodeTitle="->")) static UCCDynamicValueBase* MakePowerGrid(UObject* WorldContext, UFGPowerCircuit* Value);
	UFUNCTION(BlueprintPure, meta=(CompactNodeTitle="->")) static UCCDynamicValueBase* MakeEntity(UObject* WorldContext, AActor* Value);
	UFUNCTION(BlueprintPure, meta=(CompactNodeTitle="->")) static UCCDynamicValueBase* MakeRecipe(UObject* WorldContext, TSubclassOf<UFGRecipe> Value);
	UFUNCTION(BlueprintPure, meta=(CompactNodeTitle="->")) static UCCDynamicValueBase* MakeColor(UObject* WorldContext, FLinearColor Value);
	UFUNCTION(BlueprintPure, meta=(CompactNodeTitle="->")) static UCCDynamicValueBase* MakeStack(UObject* WorldContext, FInventoryStack Value);
	UFUNCTION(BlueprintPure, meta=(CompactNodeTitle="->")) static UCCDynamicValueBase* MakeItemAmount(UObject* WorldContext, FItemAmount Value);
	UFUNCTION(BlueprintPure, meta=(CompactNodeTitle="->")) static UCCDynamicValueBase* MakeBoolArray(UObject* WorldContext, TArray<bool> Value);
	UFUNCTION(BlueprintPure, meta=(CompactNodeTitle="->")) static UCCDynamicValueBase* MakeFloatArray(UObject* WorldContext, TArray<double> Value);
	UFUNCTION(BlueprintPure, meta=(CompactNodeTitle="->")) static UCCDynamicValueBase* MakeStringArray(UObject* WorldContext, TArray<FString> Value);
	UFUNCTION(BlueprintPure, meta=(CompactNodeTitle="->")) static UCCDynamicValueBase* MakeVectorArray(UObject* WorldContext, TArray<FVector> Value);
	UFUNCTION(BlueprintPure, meta=(CompactNodeTitle="->")) static UCCDynamicValueBase* MakeInventoryArray(UObject* WorldContext, TArray<UFGInventoryComponent*> Value);
	UFUNCTION(BlueprintPure, meta=(CompactNodeTitle="->")) static UCCDynamicValueBase* MakePowerGridArray(UObject* WorldContext, TArray<UFGPowerCircuit*> Value);
	UFUNCTION(BlueprintPure, meta=(CompactNodeTitle="->")) static UCCDynamicValueBase* MakeEntityArray(UObject* WorldContext, TArray<AActor*> Value);
	UFUNCTION(BlueprintPure, meta=(CompactNodeTitle="->")) static UCCDynamicValueBase* MakeRecipeArray(UObject* WorldContext, TArray<TSubclassOf<UFGRecipe>> Value);
	UFUNCTION(BlueprintPure, meta=(CompactNodeTitle="->")) static UCCDynamicValueBase* MakeColorArray(UObject* WorldContext, TArray<FLinearColor> Value);
	UFUNCTION(BlueprintPure, meta=(CompactNodeTitle="->")) static UCCDynamicValueBase* MakeStackArray(UObject* WorldContext, TArray<FInventoryStack> Value);
	UFUNCTION(BlueprintPure, meta=(CompactNodeTitle="->")) static UCCDynamicValueBase* MakeItemAmountArray(UObject* WorldContext, TArray<FItemAmount> Value);

	UFUNCTION(BlueprintPure, meta=(CompactNodeTitle="->")) static bool ToBool(UCCDynamicValueBase* Base);
	UFUNCTION(BlueprintPure, meta=(CompactNodeTitle="->")) static double ToFloat(UCCDynamicValueBase* Base);
	UFUNCTION(BlueprintPure, meta=(CompactNodeTitle="->")) static int ToInt(UCCDynamicValueBase* Base);
	UFUNCTION(BlueprintPure, meta=(CompactNodeTitle="->")) static FString ToString(UCCDynamicValueBase* Base);
	UFUNCTION(BlueprintPure, meta=(CompactNodeTitle="->")) static FVector ToVector(UCCDynamicValueBase* Base);
	UFUNCTION(BlueprintPure, meta=(CompactNodeTitle="->")) static UFGInventoryComponent* ToInventory(UCCDynamicValueBase* Base);
	UFUNCTION(BlueprintPure, meta=(CompactNodeTitle="->")) static UFGPowerCircuit* ToCircuit(UCCDynamicValueBase* Base);
	UFUNCTION(BlueprintPure, meta=(CompactNodeTitle="->")) static AActor* ToEntity(UCCDynamicValueBase* Base);
	UFUNCTION(BlueprintPure, meta=(CompactNodeTitle="->")) static TSubclassOf<UFGRecipe> ToRecipe(UCCDynamicValueBase* Base);
	UFUNCTION(BlueprintPure, meta=(CompactNodeTitle="->")) static FLinearColor ToColor(UCCDynamicValueBase* Base);
	UFUNCTION(BlueprintPure, meta=(CompactNodeTitle="->")) static FInventoryStack ToStack(UCCDynamicValueBase* Base);
	UFUNCTION(BlueprintPure, meta=(CompactNodeTitle="->")) static FItemAmount ToItemAmount(UCCDynamicValueBase* Base);
	UFUNCTION(BlueprintPure, meta=(CompactNodeTitle="->")) static FCustomStruct ToCustomStruct(UCCDynamicValueBase* Base);
	UFUNCTION(BlueprintPure, meta=(CompactNodeTitle="->")) static TArray<bool> ToBoolArray(UCCDynamicValueBase* Base);
	UFUNCTION(BlueprintPure, meta=(CompactNodeTitle="->")) static TArray<double> ToFloatArray(UCCDynamicValueBase* Base);
	UFUNCTION(BlueprintPure, meta=(CompactNodeTitle="->")) static TArray<FString> ToStringArray(UCCDynamicValueBase* Base);
	UFUNCTION(BlueprintPure, meta=(CompactNodeTitle="->")) static TArray<FVector> ToVectorArray(UCCDynamicValueBase* Base);
	UFUNCTION(BlueprintPure, meta=(CompactNodeTitle="->")) static TArray<UFGInventoryComponent*> ToInventoryArray(UCCDynamicValueBase* Base);
	UFUNCTION(BlueprintPure, meta=(CompactNodeTitle="->")) static TArray<UFGPowerCircuit*> ToCircuitArray(UCCDynamicValueBase* Base);
	UFUNCTION(BlueprintPure, meta=(CompactNodeTitle="->")) static TArray<AActor*> ToEntityArray(UCCDynamicValueBase* Base);
	UFUNCTION(BlueprintPure, meta=(CompactNodeTitle="->")) static TArray<TSubclassOf<UFGRecipe>> ToRecipeArray(UCCDynamicValueBase* Base);
	UFUNCTION(BlueprintPure, meta=(CompactNodeTitle="->")) static TArray<FLinearColor> ToColorArray(UCCDynamicValueBase* Base);
	UFUNCTION(BlueprintPure, meta=(CompactNodeTitle="->")) static TArray<FInventoryStack> ToStackArray(UCCDynamicValueBase* Base);
	UFUNCTION(BlueprintPure, meta=(CompactNodeTitle="->")) static TArray<FItemAmount> ToItemAmountArray(UCCDynamicValueBase* Base);
	UFUNCTION(BlueprintPure, meta=(CompactNodeTitle="->")) static TArray<FCustomStruct> ToCustomStructArray(UCCDynamicValueBase* Base);


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
		case Unknown: return nullptr;
		default:
			auto TypeString = UEnum::GetValueAsString<EConnectionType>(Type);
			ACircuitryLogger::DispatchErrorEvent("Unknown connection type: " + TypeString + ". Failed to create dynamic value.");
			return nullptr;
		}
	}

	UFUNCTION(BlueprintCallable)
	static UCCDynamicValueBase* FromValue(const FConnectionData& Data, UObject* WorldContext)
	{
		auto Out = FromType(Data.ConnectionType, WorldContext ? WorldContext : Data.Object);
		if(Out) Out->SetValue(Data.Object, Data.FunctionName, Data.FromProperty);
		return Out;
	}
};
