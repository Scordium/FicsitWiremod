// 

#pragma once

#include "CoreMinimal.h"
#include "CCDynamicValueBase.h"
#include "CommonLib/ReflectionUtilities.h"
#include "CCIntegerValue.generated.h"

/**
 * 
 */
UCLASS()
class FICSITWIREMOD_API UCCIntegerValue : public UCCDynamicValueBase
{
	GENERATED_BODY()


public:

	UCCIntegerValue() : Super(Integer){}

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override
	{
		Super::GetLifetimeReplicatedProps(OutLifetimeProps);

		DOREPLIFETIME(UCCIntegerValue, Value)
	}

	virtual void FromConnectionValue(UObject* Object, FName SourceName, bool FromProperty) override
	{
		if(!Object) return;
		if(Object->GetClass()->ImplementsInterface(IDynamicValuePasser::UClassType::StaticClass()))
			if(auto SameType = Cast<UCCIntegerValue>(IDynamicValuePasser::Execute_GetValue(Object, SourceName.ToString())))
			{
				Value = SameType->Value;
				return;
			}
		
		Value = UReflectionUtilities::GetFloat(REFLECTION_ARGS, Value);
	}

	virtual bool Equals(UCCDynamicValueBase* Other) override
	{
		if(this == Other) return true;

		if(auto OtherSource = Cast<UCCIntegerValue>(Other))
			return OtherSource->Value == Value;

		return false;
	}

	virtual FString ToString() override { return FString::FromInt(Value); }

	virtual bool FromWrapperValue(const FDynamicValueStringWrapper& Wrapper) override
	{
		Value = FCString::Atoi(*Wrapper.Value);
		return true;
	}

	virtual FDynamicValueStringWrapper ToWrapperValue() override
	{
		return FDynamicValueStringWrapper(ConnectionType, ToString());
	}
	
	UPROPERTY(Replicated, SaveGame, BlueprintReadWrite)
	int Value;
};
