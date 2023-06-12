// 


#include "..\..\..\Public\CommonLib\DynamicValues\CCDynamicValueUtils.h"

#include "CommonLib/DynamicValues/CCBoolArrayValue.h"
#include "CommonLib/DynamicValues/CCBoolValue.h"
#include "CommonLib/DynamicValues/CCCircuitArrayValue.h"
#include "CommonLib/DynamicValues/CCCircuitValue.h"
#include "CommonLib/DynamicValues/CCColorArrayValue.h"
#include "CommonLib/DynamicValues/CCColorValue.h"
#include "CommonLib/DynamicValues/CCCustomStructArrayValue.h"
#include "CommonLib/DynamicValues/CCEntityArrayValue.h"
#include "CommonLib/DynamicValues/CCEntityValue.h"
#include "CommonLib/DynamicValues/CCInventoryArrayValue.h"
#include "CommonLib/DynamicValues/CCInventoryValue.h"
#include "CommonLib/DynamicValues/CCItemAmountArrayValue.h"
#include "CommonLib/DynamicValues/CCItemAmountValue.h"
#include "CommonLib/DynamicValues/CCNumberArrayValue.h"
#include "CommonLib/DynamicValues/CCNumberValue.h"
#include "CommonLib/DynamicValues/CCRecipeArrayValue.h"
#include "CommonLib/DynamicValues/CCRecipeValue.h"
#include "CommonLib/DynamicValues/CCStackArrayValue.h"
#include "CommonLib/DynamicValues/CCStackValue.h"
#include "CommonLib/DynamicValues/CCStringArrayValue.h"
#include "CommonLib/DynamicValues/CCStringValue.h"
#include "CommonLib/DynamicValues/CCVectorArrayValue.h"
#include "CommonLib/DynamicValues/CCVectorValue.h"

UCCDynamicValueBase* UCCDynamicValueUtils::MakeBool(UObject* WorldContext, bool Value)
{
	if(auto Out = Cast<UCCBoolValue>(WorldContext))
	{
		Out->Value = Value;
		return Out;
	}

	auto Out = NewObject<UCCBoolValue>(WorldContext->GetWorld()->PersistentLevel);
	Out->Value = Value;
	return Out;
}

UCCDynamicValueBase* UCCDynamicValueUtils::MakeFloat(UObject* WorldContext, float Value)
{
	if(auto Out = Cast<UCCNumberValue>(WorldContext))
	{
		Out->Value = Value;
		return Out;
	}

	auto Out = NewObject<UCCNumberValue>(WorldContext->GetWorld()->PersistentLevel);
	Out->Value = Value;
	return Out;
}

UCCDynamicValueBase* UCCDynamicValueUtils::MakeInt(UObject* WorldContext, int Value)
{
		if(auto Out = Cast<UCCIntegerValue>(WorldContext))
    	{
    		Out->Value = Value;
    		return Out;
    	}
    
    	auto Out = NewObject<UCCIntegerValue>(WorldContext->GetWorld()->PersistentLevel);
    	Out->Value = Value;
    	return Out;
}

UCCDynamicValueBase* UCCDynamicValueUtils::MakeString(UObject* WorldContext, FString Value)
{
	if(auto Out = Cast<UCCStringValue>(WorldContext))
	{
		Out->Value = Value;
		return Out;
	}

	auto Out = NewObject<UCCStringValue>(WorldContext->GetWorld()->PersistentLevel);
	Out->Value = Value;
	return Out;
}

UCCDynamicValueBase* UCCDynamicValueUtils::MakeVector(UObject* WorldContext, FVector Value)
{
	if(auto Out = Cast<UCCVectorValue>(WorldContext))
	{
		Out->Value = Value;
		return Out;
	}

	auto Out = NewObject<UCCVectorValue>(WorldContext->GetWorld()->PersistentLevel);
	Out->Value = Value;
	return Out;
}

UCCDynamicValueBase* UCCDynamicValueUtils::MakeInventory(UObject* WorldContext, UFGInventoryComponent* Value)
{
	if(auto Out = Cast<UCCInventoryValue>(WorldContext))
	{
		Out->Value = Value;
		return Out;
	}

	auto Out = NewObject<UCCInventoryValue>(WorldContext->GetWorld()->PersistentLevel);
	Out->Value = Value;
	return Out;
}

UCCDynamicValueBase* UCCDynamicValueUtils::MakePowerGrid(UObject* WorldContext, UFGPowerCircuit* Value)
{
	if(auto Out = Cast<UCCCircuitValue>(WorldContext))
	{
		Out->Value = Value;
		return Out;
	}

	auto Out = NewObject<UCCCircuitValue>(WorldContext->GetWorld()->PersistentLevel);
	Out->Value = Value;
	return Out;
}

UCCDynamicValueBase* UCCDynamicValueUtils::MakeEntity(UObject* WorldContext, AActor* Value)
{
	if(auto Out = Cast<UCCEntityValue>(WorldContext))
	{
		Out->Value = Value;
		return Out;
	}

	auto Out = NewObject<UCCEntityValue>(WorldContext->GetWorld()->PersistentLevel);
	Out->Value = Value;
	return Out;
}

UCCDynamicValueBase* UCCDynamicValueUtils::MakeRecipe(UObject* WorldContext, TSubclassOf<UFGRecipe> Value)
{
	if(auto Out = Cast<UCCRecipeValue>(WorldContext))
	{
		Out->Value = Value;
		return Out;
	}

	auto Out = NewObject<UCCRecipeValue>(WorldContext->GetWorld()->PersistentLevel);
	Out->Value = Value;
	return Out;
}

UCCDynamicValueBase* UCCDynamicValueUtils::MakeColor(UObject* WorldContext, FLinearColor Value)
{
	if(auto Out = Cast<UCCColorValue>(WorldContext))
	{
		Out->Value = Value;
		return Out;
	}

	auto Out = NewObject<UCCColorValue>(WorldContext->GetWorld()->PersistentLevel);
	Out->Value = Value;
	return Out;
}

UCCDynamicValueBase* UCCDynamicValueUtils::MakeStack(UObject* WorldContext, FInventoryStack Value)
{
	if(auto Out = Cast<UCCStackValue>(WorldContext))
	{
		Out->Value = Value;
		return Out;
	}

	auto Out = NewObject<UCCStackValue>(WorldContext->GetWorld()->PersistentLevel);
	Out->Value = Value;
	return Out;
}

UCCDynamicValueBase* UCCDynamicValueUtils::MakeItemAmount(UObject* WorldContext, FItemAmount Value)
{
	if(auto Out = Cast<UCCItemAmountValue>(WorldContext))
	{
		Out->Value = Value;
		return Out;
	}

	auto Out = NewObject<UCCItemAmountValue>(WorldContext->GetWorld()->PersistentLevel);
	Out->Value = Value;
	return Out;
}

UCCDynamicValueBase* UCCDynamicValueUtils::MakeBoolArray(UObject* WorldContext, TArray<bool> Value)
{
	if(auto Out = Cast<UCCBoolArrayValue>(WorldContext))
	{
		Out->Value = Value;
		return Out;
	}

	auto Out = NewObject<UCCBoolArrayValue>(WorldContext->GetWorld()->PersistentLevel);
	Out->Value = Value;
	return Out;
}

UCCDynamicValueBase* UCCDynamicValueUtils::MakeFloatArray(UObject* WorldContext, TArray<float> Value)
{
	if(auto Out = Cast<UCCNumberArrayValue>(WorldContext))
	{
		Out->Value = Value;
		return Out;
	}

	auto Out = NewObject<UCCNumberArrayValue>(WorldContext->GetWorld()->PersistentLevel);
	Out->Value = Value;
	return Out;
}

UCCDynamicValueBase* UCCDynamicValueUtils::MakeStringArray(UObject* WorldContext, TArray<FString> Value)
{
	if(auto Out = Cast<UCCStringArrayValue>(WorldContext))
	{
		Out->Value = Value;
		return Out;
	}

	auto Out = NewObject<UCCStringArrayValue>(WorldContext->GetWorld()->PersistentLevel);
	Out->Value = Value;
	return Out;
}

UCCDynamicValueBase* UCCDynamicValueUtils::MakeVectorArray(UObject* WorldContext, TArray<FVector> Value)
{
	if(auto Out = Cast<UCCVectorArrayValue>(WorldContext))
	{
		Out->Value = Value;
		return Out;
	}

	auto Out = NewObject<UCCVectorArrayValue>(WorldContext->GetWorld()->PersistentLevel);
	Out->Value = Value;
	return Out;
}

UCCDynamicValueBase* UCCDynamicValueUtils::MakeInventoryArray(UObject* WorldContext, TArray<UFGInventoryComponent*> Value)
{
	if(auto Out = Cast<UCCInventoryArrayValue>(WorldContext))
	{
		Out->Value = Value;
		return Out;
	}

	auto Out = NewObject<UCCInventoryArrayValue>(WorldContext->GetWorld()->PersistentLevel);
	Out->Value = Value;
	return Out;
}

UCCDynamicValueBase* UCCDynamicValueUtils::MakePowerGridArray(UObject* WorldContext, TArray<UFGPowerCircuit*> Value)
{
	if(auto Out = Cast<UCCCircuitArrayValue>(WorldContext))
	{
		Out->Value = Value;
		return Out;
	}

	auto Out = NewObject<UCCCircuitArrayValue>(WorldContext->GetWorld()->PersistentLevel);
	Out->Value = Value;
	return Out;
}

UCCDynamicValueBase* UCCDynamicValueUtils::MakeEntityArray(UObject* WorldContext, TArray<AActor*> Value)
{
	if(auto Out = Cast<UCCEntityArrayValue>(WorldContext))
	{
		Out->Value = Value;
		return Out;
	}

	auto Out = NewObject<UCCEntityArrayValue>(WorldContext->GetWorld()->PersistentLevel);
	Out->Value = Value;
	return Out;
}

UCCDynamicValueBase* UCCDynamicValueUtils::MakeRecipeArray(UObject* WorldContext, TArray<TSubclassOf<UFGRecipe>> Value)
{
	if(auto Out = Cast<UCCRecipeArrayValue>(WorldContext))
	{
		Out->Value = Value;
		return Out;
	}

	auto Out = NewObject<UCCRecipeArrayValue>(WorldContext->GetWorld()->PersistentLevel);
	Out->Value = Value;
	return Out;
}

UCCDynamicValueBase* UCCDynamicValueUtils::MakeColorArray(UObject* WorldContext, TArray<FLinearColor> Value)
{
	if(auto Out = Cast<UCCColorArrayValue>(WorldContext))
	{
		Out->Value = Value;
		return Out;
	}

	auto Out = NewObject<UCCColorArrayValue>(WorldContext->GetWorld()->PersistentLevel);
	Out->Value = Value;
	return Out;
}

UCCDynamicValueBase* UCCDynamicValueUtils::MakeStackArray(UObject* WorldContext, TArray<FInventoryStack> Value)
{
	if(auto Out = Cast<UCCStackArrayValue>(WorldContext))
	{
		Out->Value = Value;
		return Out;
	}

	auto Out = NewObject<UCCStackArrayValue>(WorldContext->GetWorld()->PersistentLevel);
	Out->Value = Value;
	return Out;
}

UCCDynamicValueBase* UCCDynamicValueUtils::MakeItemAmountArray(UObject* WorldContext, TArray<FItemAmount> Value)
{
	if(auto Out = Cast<UCCItemAmountArrayValue>(WorldContext))
	{
		Out->Value = Value;
		return Out;
	}

	auto Out = NewObject<UCCItemAmountArrayValue>(WorldContext->GetWorld()->PersistentLevel);
	Out->Value = Value;
	return Out;
}

bool UCCDynamicValueUtils::ToBool(UCCDynamicValueBase* Base)
{
	if(auto Out = Cast<UCCBoolValue>(Base))
		return Out->Value;

	return bool();
}

float UCCDynamicValueUtils::ToFloat(UCCDynamicValueBase* Base)
{
	if(Base)
	{
		if(auto Float = Cast<UCCNumberValue>(Base)) return Float->Value;
		else if(auto Int = Cast<UCCIntegerValue>(Base)) return Int->Value;
	}

	return float();
}

int UCCDynamicValueUtils::ToInt(UCCDynamicValueBase* Base)
{
	if(Base)
	{
		if(auto Float = Cast<UCCNumberValue>(Base)) return Float->Value;
		else if(auto Int = Cast<UCCIntegerValue>(Base)) return Int->Value;
	}

	return int();
}

FString UCCDynamicValueUtils::ToString(UCCDynamicValueBase* Base)
{
	if(auto Out = Cast<UCCStringValue>(Base))
		return Out->Value;

	return FString();
}

FVector UCCDynamicValueUtils::ToVector(UCCDynamicValueBase* Base)
{
	if(auto Out = Cast<UCCVectorValue>(Base))
		return Out->Value;

	return FVector();
}

UFGInventoryComponent* UCCDynamicValueUtils::ToInventory(UCCDynamicValueBase* Base)
{
	if(auto Out = Cast<UCCInventoryValue>(Base))
		return Out->Value;

	return nullptr;
}

UFGPowerCircuit* UCCDynamicValueUtils::ToCircuit(UCCDynamicValueBase* Base)
{
	if(auto Out = Cast<UCCCircuitValue>(Base))
		return Out->Value;

	return nullptr;
}

AActor* UCCDynamicValueUtils::ToEntity(UCCDynamicValueBase* Base)
{
	if(auto Out = Cast<UCCEntityValue>(Base))
		return Out->Value;

	return nullptr;
}

TSubclassOf<UFGRecipe> UCCDynamicValueUtils::ToRecipe(UCCDynamicValueBase* Base)
{
	if(auto Out = Cast<UCCRecipeValue>(Base))
		return Out->Value;

	return TSubclassOf<UFGRecipe>();
}

FLinearColor UCCDynamicValueUtils::ToColor(UCCDynamicValueBase* Base)
{
	if(auto Out = Cast<UCCColorValue>(Base))
		return Out->Value;

	return FLinearColor();
}

FInventoryStack UCCDynamicValueUtils::ToStack(UCCDynamicValueBase* Base)
{
	if(auto Out = Cast<UCCStackValue>(Base))
		return Out->Value;

	return FInventoryStack();
}

FItemAmount UCCDynamicValueUtils::ToItemAmount(UCCDynamicValueBase* Base)
{
	if(auto Out = Cast<UCCItemAmountValue>(Base))
		return Out->Value;

	return FItemAmount();
}

FCustomStruct UCCDynamicValueUtils::ToCustomStruct(UCCDynamicValueBase* Base)
{
	if(auto Out = Cast<UCCCustomStructValue>(Base))
		return Out->Value;

	return FCustomStruct();
}

TArray<bool> UCCDynamicValueUtils::ToBoolArray(UCCDynamicValueBase* Base)
{
	if(auto Out = Cast<UCCBoolArrayValue>(Base))
		return Out->Value;

	return TArray<bool>();
}

TArray<float> UCCDynamicValueUtils::ToFloatArray(UCCDynamicValueBase* Base)
{
	if(auto Out = Cast<UCCNumberArrayValue>(Base))
		return Out->Value;

	return TArray<float>();
}

TArray<FString> UCCDynamicValueUtils::ToStringArray(UCCDynamicValueBase* Base)
{
	if(auto Out = Cast<UCCStringArrayValue>(Base))
		return Out->Value;

	return TArray<FString>();
}

TArray<FVector> UCCDynamicValueUtils::ToVectorArray(UCCDynamicValueBase* Base)
{
	if(auto Out = Cast<UCCVectorArrayValue>(Base))
		return Out->Value;

	return TArray<FVector>();
}

TArray<UFGInventoryComponent*> UCCDynamicValueUtils::ToInventoryArray(UCCDynamicValueBase* Base)
{
	if(auto Out = Cast<UCCInventoryArrayValue>(Base))
		return Out->Value;

	return TArray<UFGInventoryComponent*>();
}

TArray<UFGPowerCircuit*> UCCDynamicValueUtils::ToCircuitArray(UCCDynamicValueBase* Base)
{
	if(auto Out = Cast<UCCCircuitArrayValue>(Base))
		return Out->Value;

	return TArray<UFGPowerCircuit*>();
}

TArray<AActor*> UCCDynamicValueUtils::ToEntityArray(UCCDynamicValueBase* Base)
{
	if(auto Out = Cast<UCCEntityArrayValue>(Base))
		return Out->Value;

	return TArray<AActor*>();
}

TArray<TSubclassOf<UFGRecipe>> UCCDynamicValueUtils::ToRecipeArray(UCCDynamicValueBase* Base)
{
	if(auto Out = Cast<UCCRecipeArrayValue>(Base))
		return Out->Value;

	return TArray<TSubclassOf<UFGRecipe>>();
}

TArray<FLinearColor> UCCDynamicValueUtils::ToColorArray(UCCDynamicValueBase* Base)
{
	if(auto Out = Cast<UCCColorArrayValue>(Base))
		return Out->Value;

	return TArray<FLinearColor>();
}

TArray<FInventoryStack> UCCDynamicValueUtils::ToStackArray(UCCDynamicValueBase* Base)
{
	if(auto Out = Cast<UCCStackArrayValue>(Base))
		return Out->Value;

	return TArray<FInventoryStack>();
}

TArray<FItemAmount> UCCDynamicValueUtils::ToItemAmountArray(UCCDynamicValueBase* Base)
{
	if(auto Out = Cast<UCCItemAmountArrayValue>(Base))
		return Out->Value;

	return TArray<FItemAmount>();
}

TArray<FCustomStruct> UCCDynamicValueUtils::ToCustomStructArray(UCCDynamicValueBase* Base)
{
	if(auto Out = Cast<UCCCustomStructArrayValue>(Base))
		return Out->Value;

	return TArray<FCustomStruct>();
}
