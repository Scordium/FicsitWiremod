// 

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "ReflectionHelpers.generated.h"


USTRUCT(BlueprintType)
struct FValueReflectionSource
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite, SaveGame)
	UObject* Object = nullptr;

	UPROPERTY(BlueprintReadWrite, SaveGame)
	FName SourceName;

	UPROPERTY(BlueprintReadWrite, SaveGame)
	bool SourceIsProperty = false;

	FValueReflectionSource(){}

	FValueReflectionSource(UObject* Object, FName SourceName, bool FromProperty)
	{
		this->Object = Object;
		this->SourceName = SourceName;
		this->SourceIsProperty = FromProperty;
	}

	void ProcessFunction(void* Params) const
	{
		if(IsValid(Object))
		{
			if(auto Function = Object->FindFunction(SourceName))
				Object->ProcessEvent(Function, Params);
		}
	}

	template <typename T>
	T FromPropertyValue(T DefaultValue) const
	{
		if(!Object) return DefaultValue;
		
		auto Val = Object->GetClass()->FindPropertyByName(SourceName);
		if(!Val) return DefaultValue;
		return *Val->ContainerPtrToValuePtr<T>(Object);
	}

	template<typename T>
	T GetValue(T DefaultValue) const
	{
		if(SourceIsProperty)
			return FromPropertyValue(DefaultValue);

		struct{T RetVal;} Params{DefaultValue};
		ProcessFunction(&Params);
		return Params.RetVal;
	}

	template<typename T>
	void SetByAddress(T& Output) const
	{
		if(SourceIsProperty)
		{
			Output = FromPropertyValue(Output);
			return;
		}
		
		struct{T RetVal;} Params{};
		ProcessFunction(&Params);
		Output = Params.RetVal;
	}
};


/**
 * 
 */
UCLASS()
class FICSITWIREMOD_API UReflectionHelpers : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()


	
};
