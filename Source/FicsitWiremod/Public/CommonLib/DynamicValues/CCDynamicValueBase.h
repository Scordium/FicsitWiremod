// 

#pragma once

#include "CoreMinimal.h"
#include "FGSaveInterface.h"
#include "JsonValue.h"
#include "UnrealNetwork.h"
#include "ConnectionPointer.h"
#include "CommonLib/ConnectionType.h"
#include "CCDynamicValueBase.generated.h"
/**
 * 
 */

#define ARRAY_SEPARATOR "\uFFFA"

#define DYNAMIC_FROMPOINTER(func) \
if(!Pointer.Target) return; \
if(Pointer.Target->GetClass()->ImplementsInterface(IDynamicValuePasser::UClassType::StaticClass())) \
	if(auto SameType = Cast<ThisClass>(IDynamicValuePasser::Execute_GetValue(Pointer.Target, Pointer.SourceName.ToString()))) \
	{ \
		Value = SameType->Value; \
		OnValueUpdate(); \
		return; \
	} \
Value = UReflectionUtilities::func(Pointer);\
OnValueUpdate();

#define DYNAMIC_FROMPOINTER_DEFAULT(func, default_val) \
if(!Pointer.Target) return; \
if(Pointer.Target->GetClass()->ImplementsInterface(IDynamicValuePasser::UClassType::StaticClass())) \
	if(auto SameType = Cast<ThisClass>(IDynamicValuePasser::Execute_GetValue(Pointer.Target, Pointer.SourceName.ToString()))) \
	{ \
		Value = SameType->Value; \
		OnValueUpdate(); \
		return; \
	} \
Value = UReflectionUtilities::func(Pointer, default_val);\
OnValueUpdate();

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

	bool operator==(const FDynamicValueStringWrapper& Other) const
	{
		return Type == Other.Type && Value == Other.Value;
	}

	bool operator!=(const FDynamicValueStringWrapper& Other) const { return !(*this == Other); }
};

UCLASS(Blueprintable, BlueprintType, Abstract)
class FICSITWIREMOD_API UCCDynamicValueBase : public UObject, public IFGSaveInterface
{
	GENERATED_BODY()

public:

	UCCDynamicValueBase() { DispatchCreated(); }

	UCCDynamicValueBase(EConnectionType Type)
	{
		ConnectionType = Type;
		DispatchCreated();
	}

	UFUNCTION(BlueprintCallable)
	virtual void FromConnectionValue(const FConnectionPointer& Pointer)
	{
		DispatchNotImplemented("SET_DYNAMIC_VALUE");
	}

	UFUNCTION(BlueprintPure)
	virtual FString ToString()
	{
		DispatchNotImplemented("TO_STRING");
		return "";
	}
	
	virtual TSharedPtr<FJsonValue> ToJson()
	{
		return MakeShareable(new FJsonValueNull());
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

	virtual void OnValueUpdate(){}
	
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
	virtual bool Equals(UCCDynamicValueBase* Other, bool ComparePointers = true){ return ComparePointers && this == Other; }
	virtual bool Equals(const FConnectionPointer& Pointer) { return false; }

	void DispatchNotImplemented(const FString& FuncName) const { ACircuitryLogger::DispatchErrorEvent("Function " + FuncName + " is not implemented for class " + GetClass()->GetName()); }

	void DispatchCreated(){ ACircuitryLogger::DispatchEvent("Dynamic value " + GetName() + " was created", ELogVerbosity::Display); }
};

UCLASS()
class UDynamicValueConstants : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintPure)
	static FString GetSeparatorSymbol() { return ARRAY_SEPARATOR; }
	
};

