// 

#pragma once

#include "CoreMinimal.h"
#include "CCDynamicValueBase.h"
#include "CommonLib/CustomStruct.h"
#include "CCCustomStructValue.generated.h"

/**
 * 
 */
UCLASS(Blueprintable,BlueprintType)
class FICSITWIREMOD_API UCCCustomStructValue : public UCCDynamicValueBase
{
	GENERATED_BODY()

public:
	UCCCustomStructValue() : Super(CustomStruct){}

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override
	{
		Super::GetLifetimeReplicatedProps(OutLifetimeProps);

		DOREPLIFETIME(UCCCustomStructValue, Value)
	}
	
	virtual void SetValue(const FValueReflectionSource& Source) override
	{
		if(!Source.Object) return;
		if(Source.Object->GetClass()->ImplementsInterface(IDynamicValuePasser::UClassType::StaticClass()))
			if(auto SameType = Cast<UCCCustomStructValue>(IDynamicValuePasser::Execute_GetValue(Source.Object, Source.SourceName.ToString())))
			{
				Value = SameType->Value;
				return;
			}
		
		Source.SetByAddress(Value);
	}

	virtual bool Equals(UCCDynamicValueBase* Other) override
	{
		if(this == Other) return true;

		if(auto OtherSource = Cast<UCCCustomStructValue>(Other))
			return OtherSource->Value == Value;

		return false;
	}

	virtual FString ToString() override { return Value.Name + "[" + FString::FromInt(Value.Values.Num()) + " values]"; }
	
	UPROPERTY(Replicated, SaveGame)
	FCustomStruct Value;
};
