// 

#pragma once

#include "CoreMinimal.h"
#include "FGSaveInterface.h"
#include "CommonLib/ConnectionType.h"
#include "CommonLib/ReflectionHelpers.h"
#include "UObject/Object.h"
#include "CCDynamicValueBase.generated.h"
/**
 * 
 */
UCLASS(Blueprintable, BlueprintType, Abstract)
class FICSITWIREMOD_API UCCDynamicValueBase : public UObject, public IFGSaveInterface
{
	GENERATED_BODY()

public:

	UCCDynamicValueBase(){}

	explicit UCCDynamicValueBase(EConnectionType Type)
	{
		ConnectionType = Type;
	}

	UFUNCTION(BlueprintCallable)
	virtual void SetValue(const FValueReflectionSource& Source) { checkf(0, TEXT("Function SET_DYNAMIC_VALUE is not implemented for this class!")) }

	UFUNCTION(BlueprintPure)
	virtual FString ToString(){ checkf(0, TEXT("Function TO_STRING is not implemented for this class!")) return ""; }

	UFUNCTION(BlueprintPure)
	virtual TArray<FString> ToStringArray() { checkf(0, TEXT("Function TO_STRING_ARRAY is not implemented for this class!")) return TArray<FString>(); }
	
	virtual bool IsSupportedForNetworking() const override { return true; }
	virtual bool ShouldSave_Implementation() const override { return true; }
	
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override
	{
		Super::GetLifetimeReplicatedProps(OutLifetimeProps);

		DOREPLIFETIME(UCCDynamicValueBase, ConnectionType)
	}
	
	UPROPERTY(Replicated, SaveGame, BlueprintReadWrite)
	TEnumAsByte<EConnectionType> ConnectionType = Unknown;

	bool operator ==(UCCDynamicValueBase* Other) { return Equals(Other); }
	bool operator !=(UCCDynamicValueBase* Other) { return !Equals(Other); }
	virtual bool Equals(UCCDynamicValueBase* Other){ return this == Other; }
};


UINTERFACE(BlueprintType, Blueprintable)
class UDynamicValuePasser : public UInterface
{
	GENERATED_BODY()
};

class IDynamicValuePasser
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	UCCDynamicValueBase* GetValue(const FString& ValueName);
};
