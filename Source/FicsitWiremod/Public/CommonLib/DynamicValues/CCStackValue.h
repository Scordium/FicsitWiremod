// 

#pragma once

#include "CoreMinimal.h"
#include "CCDynamicValueBase.h"
#include "FGInventoryComponent.h"
#include "CCStackValue.generated.h"

/**
 * 
 */
UCLASS(Blueprintable,BlueprintType)
class FICSITWIREMOD_API UCCStackValue : public UCCDynamicValueBase
{
	GENERATED_BODY()

public:

	UCCStackValue() : Super(Stack){}

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override
	{
		Super::GetLifetimeReplicatedProps(OutLifetimeProps);

		DOREPLIFETIME(UCCStackValue, Value)
	}

	virtual void SetValue(const FValueReflectionSource& Source) override
	{
		if(!Source.Object) return;
		if(Source.Object->GetClass()->ImplementsInterface(IDynamicValuePasser::UClassType::StaticClass()))
			if(auto SameType = Cast<UCCStackValue>(IDynamicValuePasser::Execute_GetValue(Source.Object, Source.SourceName.ToString())))
			{
				Value = SameType->Value;
				return;
			}
		
		Source.SetByAddress(Value);
	}

	virtual bool Equals(UCCDynamicValueBase* Other) override
	{
		if(this == Other) return true;

		if(auto OtherSource = Cast<UCCStackValue>(Other))
			return OtherSource->Value.Item.GetItemClass() == Value.Item.GetItemClass() && OtherSource->Value.NumItems == Value.NumItems;

		return false;
	}

	virtual FString ToString() override { return FString::FromInt(Value.NumItems) + " " + UFGItemDescriptor::GetItemName(Value.Item.GetItemClass()).ToString(); }
	
	UPROPERTY(Replicated, SaveGame, BlueprintReadWrite)
	FInventoryStack Value;
};
