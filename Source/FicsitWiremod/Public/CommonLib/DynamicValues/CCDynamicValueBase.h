// 

#pragma once

#include "CoreMinimal.h"
#include "FGSaveInterface.h"
#include "CommonLib/ConnectionType.h"
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
	virtual void SetValue(UObject* Object, FName SourceName, bool FromProperty)
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

	void DispatchNotImplemented(FString FuncName) const { ACircuitryLogger::DispatchErrorEvent("Function " + FuncName + " is not implemented for class " + GetClass()->GetName()); }
};

