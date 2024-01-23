// 

#pragma once

#include "CoreMinimal.h"
#include "FGSaveInterface.h"
#include "CommonLib/ConnectionType.h"
#include "Net/UnrealNetwork.h"
#include "UObject/Object.h"
#include "CCDynamicValueBase.generated.h"
/**
 * 
 */

USTRUCT(Blueprintable, BlueprintType)
struct FDynamicValueStringWrapper
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite)
	TEnumAsByte<EConnectionType> Type = Unknown;

	UPROPERTY(BlueprintReadWrite)
	FString Value = "";

	FDynamicValueStringWrapper(){}
	FDynamicValueStringWrapper(const EConnectionType InType) : Type(InType) {}
	FDynamicValueStringWrapper(const EConnectionType InType, const FString& InValue) : Type(InType), Value(InValue) {}
};

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
	virtual void FromConnectionValue(UObject* Object, FName SourceName, bool FromProperty)
	{
		DispatchNotImplemented("SET_DYNAMIC_VALUE");
	}

	UFUNCTION(BlueprintPure)
	virtual FString ToString()
	{
		DispatchNotImplemented("TO_STRING");
		return "";
	}

	UFUNCTION(BlueprintPure)
	virtual TArray<FString> ToStringArray()
	{
		DispatchNotImplemented("TO_STRING_ARRAY");
		return TArray<FString>();
	}

	UFUNCTION(BlueprintPure)
	virtual FDynamicValueStringWrapper ToWrapperValue()
	{
		DispatchNotImplemented("TO_WRAPPER_VALUE");
		return FDynamicValueStringWrapper(ConnectionType);
	}

	UFUNCTION(BlueprintPure)
	virtual bool FromWrapperValue(const FDynamicValueStringWrapper& Wrapper)
	{
		DispatchNotImplemented("FROM_WRAPPER_VALUE");
		return false;
	}
	
	virtual bool IsSupportedForNetworking() const override { return true; }
	virtual bool ShouldSave_Implementation() const override { return true; }
	
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override
	{
		Super::GetLifetimeReplicatedProps(OutLifetimeProps);

		DOREPLIFETIME(UCCDynamicValueBase, ConnectionType)
	}

	bool IsOfType(EConnectionType Type) const { return Type == ConnectionType; }
	
	UPROPERTY(Replicated, SaveGame, BlueprintReadWrite)
	TEnumAsByte<EConnectionType> ConnectionType = Unknown;

	bool operator ==(UCCDynamicValueBase* Other) { return Equals(Other); }
	bool operator !=(UCCDynamicValueBase* Other) { return !Equals(Other); }
	virtual bool Equals(UCCDynamicValueBase* Other){ return this == Other; }

	void DispatchNotImplemented(const FString& FuncName) const { ACircuitryLogger::DispatchErrorEvent("Function " + FuncName + " is not implemented for class " + GetClass()->GetName()); }
};

