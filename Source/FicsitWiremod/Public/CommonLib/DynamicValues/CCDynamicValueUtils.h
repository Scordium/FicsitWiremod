// 

#pragma once

#include "CoreMinimal.h"
#include "CCBoolArrayValue.h"
#include "CCBoolValue.h"
#include "CCCircuitArrayValue.h"
#include "CCCircuitValue.h"
#include "CCColorArrayValue.h"
#include "CCColorValue.h"
#include "CCCustomStructArrayValue.h"
#include "CCCustomStructValue.h"
#include "CCDynamicValueBase.h"
#include "CCEntityArrayValue.h"
#include "CCEntityValue.h"
#include "CCIntegerValue.h"
#include "CCInventoryArrayValue.h"
#include "CCInventoryValue.h"
#include "CCItemAmountArrayValue.h"
#include "CCItemAmountValue.h"
#include "CCNumberArrayValue.h"
#include "CCNumberValue.h"
#include "CCRecipeArrayValue.h"
#include "CCRecipeValue.h"
#include "CCStackArrayValue.h"
#include "CCStackValue.h"
#include "CCStringArrayValue.h"
#include "CCStringValue.h"
#include "CCVectorArrayValue.h"
#include "CCVectorValue.h"
#include "FGInventoryComponent.h"
#include "FGPowerCircuit.h"
#include "FGRecipe.h"
#include "WiremodUtils.h"
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
	UFUNCTION(BlueprintPure, meta=(CompactNodeTitle="->")) static UCCDynamicValueBase* MakeFloat(UObject* WorldContext, float Value);
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
	UFUNCTION(BlueprintPure, meta=(CompactNodeTitle="->")) static UCCDynamicValueBase* MakeFloatArray(UObject* WorldContext, TArray<float> Value);
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
	UFUNCTION(BlueprintPure, meta=(CompactNodeTitle="->")) static float ToFloat(UCCDynamicValueBase* Base);
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
	UFUNCTION(BlueprintPure, meta=(CompactNodeTitle="->")) static TArray<float> ToFloatArray(UCCDynamicValueBase* Base);
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
		
		if(auto Out = Cast<UCCDynamicValueBase>(WorldContext);
			Out && UConnectionTypeFunctions::IsValidConnectionPair(Out->ConnectionType, Type)) return Out;
		
		auto Level = WorldContext->GetWorld()->PersistentLevel;
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
		default: return nullptr;
		}
	}

	UFUNCTION(BlueprintCallable)
	static UCCDynamicValueBase* FromValue(const FConnectionData& Data, UObject* WorldContext)
	{
		auto Out = FromType(Data.ConnectionType, WorldContext);
		if(Out) Out->SetValue(Data);
		return Out;
	}
};
