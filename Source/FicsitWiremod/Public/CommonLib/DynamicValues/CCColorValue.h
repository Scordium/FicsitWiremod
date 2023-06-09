// 

#pragma once

#include "CoreMinimal.h"
#include "CCDynamicValueBase.h"
#include "CCColorValue.generated.h"

/**
 * 
 */
UCLASS(Blueprintable,BlueprintType)
class FICSITWIREMOD_API UCCColorValue : public UCCDynamicValueBase
{
	GENERATED_BODY()

public:
	UCCColorValue() : Super(Color){}

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override
	{
		Super::GetLifetimeReplicatedProps(OutLifetimeProps);

		DOREPLIFETIME(UCCColorValue, Value)
	}

	virtual void SetValue(const FValueReflectionSource& Source) override
	{
		if(!Source.Object) return;
		
		if(Source.Object->GetClass()->ImplementsInterface(IDynamicValuePasser::UClassType::StaticClass()))
			if(auto SameType = Cast<UCCColorValue>(IDynamicValuePasser::Execute_GetValue(Source.Object, Source.SourceName.ToString())))
			{
				Value = SameType->Value;
				return;
			}
		
		Source.SetByAddress(Value);
	}

	virtual bool Equals(UCCDynamicValueBase* Other) override
	{
		if(this == Other) return true;

		if(auto OtherSource = Cast<UCCColorValue>(Other))
			return OtherSource->Value == Value;

		return false;
	}

	virtual FString ToString() override { return Value.ToString(); }
	
	UPROPERTY(Replicated, SaveGame, BlueprintReadWrite)
	FLinearColor Value;
};
