// 

#pragma once

#include "CoreMinimal.h"
#include "CCDynamicValueBase.h"
#include "ItemAmount.h"
#include "CCItemAmountValue.generated.h"

/**
 * 
 */
UCLASS(Blueprintable,BlueprintType)
class FICSITWIREMOD_API UCCItemAmountValue : public UCCDynamicValueBase
{
	GENERATED_BODY()

public:
	UCCItemAmountValue() : Super(ItemAmount){}

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override
	{
		Super::GetLifetimeReplicatedProps(OutLifetimeProps);

		DOREPLIFETIME(UCCItemAmountValue, Value)
	}

	virtual void SetValue(const FValueReflectionSource& Source) override
	{
		if(!Source.Object) return;
		if(Source.Object->GetClass()->ImplementsInterface(IDynamicValuePasser::UClassType::StaticClass()))
			if(auto SameType = Cast<UCCItemAmountValue>(IDynamicValuePasser::Execute_GetValue(Source.Object, Source.SourceName.ToString())))
			{
				Value = SameType->Value;
				return;
			}
		
		Source.SetByAddress(Value);
	}

	virtual bool Equals(UCCDynamicValueBase* Other) override
	{
		if(this == Other) return true;

		if(auto OtherSource = Cast<UCCItemAmountValue>(Other))
			return OtherSource->Value.Amount == Value.Amount && OtherSource->Value.ItemClass == Value.ItemClass;

		return false;
	}

	virtual FString ToString() override { return FString::FromInt(Value.Amount) + " of " + UFGItemDescriptor::GetItemName(Value.ItemClass).ToString();}
	
	UPROPERTY(Replicated, SaveGame, BlueprintReadWrite)
	FItemAmount Value;
};
