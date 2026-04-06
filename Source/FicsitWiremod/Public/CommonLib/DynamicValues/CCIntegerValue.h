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

	virtual void FromConnectionValue(const FConnectionPointer& Pointer) override
	{
		DYNAMIC_FROMPOINTER_DEFAULT(GetFloat, Value)
	}

	virtual bool Equals(UCCDynamicValueBase* Other, bool ComparePointers = true) override
	{
		if(auto OtherSource = Cast<ThisClass>(Other))
			return OtherSource->Value == Value;
		
		return Super::Equals(Other, ComparePointers);
	}

	virtual bool Equals(const FConnectionPointer& Pointer) override
	{
		return UReflectionUtilities::GetFloat(Pointer) == Value;
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
