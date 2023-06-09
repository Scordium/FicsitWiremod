// 

#pragma once

#include "CoreMinimal.h"
#include "CCDynamicValueBase.h"
#include "FGRecipe.h"
#include "CCRecipeValue.generated.h"

/**
 * 
 */
UCLASS(Blueprintable,BlueprintType)
class FICSITWIREMOD_API UCCRecipeValue : public UCCDynamicValueBase
{
	GENERATED_BODY()

public:
	UCCRecipeValue() : Super(Recipe){}

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override
	{
		Super::GetLifetimeReplicatedProps(OutLifetimeProps);

		DOREPLIFETIME(UCCRecipeValue, Value)
	}

	virtual void SetValue(const FValueReflectionSource& Source) override
	{
		if(!Source.Object) return;
		if(Source.Object->GetClass()->ImplementsInterface(IDynamicValuePasser::UClassType::StaticClass()))
			if(auto SameType = Cast<UCCRecipeValue>(IDynamicValuePasser::Execute_GetValue(Source.Object, Source.SourceName.ToString())))
			{
				Value = SameType->Value;
				return;
			}
		
		Source.SetByAddress(Value);
	}

	virtual bool Equals(UCCDynamicValueBase* Other) override
	{
		if(this == Other) return true;

		if(auto OtherSource = Cast<UCCRecipeValue>(Other))
			return OtherSource->Value == Value;

		return false;
	}

	virtual FString ToString() override { return ::IsValid(Value) ? UFGRecipe::GetRecipeName(Value).ToString() : FString("N/A");}
	
	UPROPERTY(Replicated, SaveGame, BlueprintReadWrite)
	TSubclassOf<UFGRecipe> Value;
};
