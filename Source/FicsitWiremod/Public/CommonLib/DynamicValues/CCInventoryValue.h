// 

#pragma once

#include "CoreMinimal.h"
#include "CCDynamicValueBase.h"
#include "FGInventoryComponent.h"
#include "CCInventoryValue.generated.h"

/**
 * 
 */
UCLASS(Blueprintable,BlueprintType)
class FICSITWIREMOD_API UCCInventoryValue : public UCCDynamicValueBase
{
	GENERATED_BODY()

public:
	UCCInventoryValue() : Super(Inventory){}

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override
	{
		Super::GetLifetimeReplicatedProps(OutLifetimeProps);

		DOREPLIFETIME(UCCInventoryValue, Value)
	}

	virtual void SetValue(const FValueReflectionSource& Source) override
	{
		if(!Source.Object) return;
		if(Source.Object->GetClass()->ImplementsInterface(IDynamicValuePasser::UClassType::StaticClass()))
			if(auto SameType = Cast<UCCInventoryValue>(IDynamicValuePasser::Execute_GetValue(Source.Object, Source.SourceName.ToString())))
			{
				Value = SameType->Value;
				return;
			}
		
		Source.SetByAddress(Value);
	}

	virtual bool Equals(UCCDynamicValueBase* Other) override
	{
		if(this == Other) return true;

		if(auto OtherSource = Cast<UCCInventoryValue>(Other))
			return OtherSource->Value == Value;

		return false;
	}

	virtual FString ToString() override
	{
		if(!Value) return "Invalid Inv.";

		TArray<FInventoryStack> Stacks;
		Value->GetInventoryStacks(Stacks);
				
		return FString::FromInt(Stacks.Num()) + "/" + FString::FromInt(Value->GetSizeLinear()) + " slots occupied";
	}
	
	UPROPERTY(Replicated, SaveGame, BlueprintReadWrite)
	UFGInventoryComponent* Value;
};
