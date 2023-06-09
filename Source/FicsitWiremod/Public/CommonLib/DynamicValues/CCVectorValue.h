// 

#pragma once

#include "CoreMinimal.h"
#include "CCDynamicValueBase.h"
#include "CCVectorValue.generated.h"

/**
 * 
 */
UCLASS(Blueprintable,BlueprintType)
class FICSITWIREMOD_API UCCVectorValue : public UCCDynamicValueBase
{
	GENERATED_BODY()

public:

	UCCVectorValue() : Super(Vector){}

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override
	{
		Super::GetLifetimeReplicatedProps(OutLifetimeProps);

		DOREPLIFETIME(UCCVectorValue, Value)
	}

	virtual void SetValue(const FValueReflectionSource& Source) override
	{
		if(!Source.Object) return;
		if(Source.Object->GetClass()->ImplementsInterface(IDynamicValuePasser::UClassType::StaticClass()))
			if(auto SameType = Cast<UCCVectorValue>(IDynamicValuePasser::Execute_GetValue(Source.Object, Source.SourceName.ToString())))
			{
				Value = SameType->Value;
				return;
			}
		
		Source.SetByAddress(Value);
	}

	virtual bool Equals(UCCDynamicValueBase* Other) override
	{
		if(this == Other) return true;

		if(auto OtherSource = Cast<UCCVectorValue>(Other))
			return OtherSource->Value == Value;

		return false;
	}

	virtual FString ToString() override { return Value.ToCompactString(); }
	
	UPROPERTY(Replicated, SaveGame, BlueprintReadWrite)
	FVector Value;
};
