#pragma once

#include "CoreMinimal.h"
#include "CommonLib/ConnectionType.h"
#include "UObject/Object.h"
#include "JsonObjectConverter.h"
#include "Net/UnrealNetwork.h"
#include "CircuitryArrayFilterBase.generated.h"

USTRUCT(Blueprintable, BlueprintType)
struct FCircuitryArrayFilterData
{
	GENERATED_BODY()

public:

	UPROPERTY(BlueprintReadWrite)
	TEnumAsByte<EConnectionType> FilterType;

	UPROPERTY(BlueprintReadWrite)
	FString JsonDataString;

	bool operator==(const FCircuitryArrayFilterData& Other) const
	{
		bool TypeMatch = FilterType == Other.FilterType;
		if(FilterType == ItemAmount || FilterType == Stack) TypeMatch = Other.FilterType == ItemAmount || Other.FilterType == Stack;
		
		return TypeMatch && JsonDataString.Equals(Other.JsonDataString, ESearchCase::CaseSensitive);
	}
};


UCLASS(Abstract, BlueprintType, Blueprintable)
class FICSITWIREMOD_API UCircuitryArrayFilterBase : public UObject
{
	GENERATED_BODY()

public:

	UCircuitryArrayFilterBase(){}
	UCircuitryArrayFilterBase(EConnectionType Type) : FilterType(Type){ }

	UFUNCTION(BlueprintCallable)
	virtual FCircuitryArrayFilterData ToJson()
	{
		ACircuitryLogger::DispatchErrorEvent("TO_JSON is not implemented in " + GetClass()->GetName());
		return FCircuitryArrayFilterData();
	}

	UFUNCTION(BlueprintCallable)
	virtual bool FromJson(const FCircuitryArrayFilterData& FilterData)
	{
		ACircuitryLogger::DispatchErrorEvent("FROM_JSON is not implemented in " + GetClass()->GetName());
		return false;
	}

	bool DeserializeArrayFilter(const FCircuitryArrayFilterData& FilterData, const UScriptStruct* StructDescriptor, void* StructPtr)
	{
		if(VerifyType && FilterData.FilterType != FilterType) return false;
		
		TSharedPtr<FJsonObject> Object;
		if(!FJsonSerializer::Deserialize(TJsonReaderFactory<>::Create(FilterData.JsonDataString), Object)) return false;

		return FJsonObjectConverter::JsonObjectToUStruct(Object.ToSharedRef(), StructDescriptor, StructPtr);
	}

	FCircuitryArrayFilterData SerializeArrayFilter(const UScriptStruct* StructDescriptor, const void* StructPtr)
	{
		FCircuitryArrayFilterData Out;
		Out.FilterType = FilterType;
		
		auto Writer = TJsonWriterFactory<>::Create(&Out.JsonDataString, 0);
		TSharedRef<FJsonObject> Object = MakeShared<FJsonObject>();
		FJsonObjectConverter::UStructToJsonObject(StructDescriptor, StructPtr, Object);
		
		FJsonSerializer::Serialize(Object, Writer);
		return Out;
	}
	
	UPROPERTY(BlueprintReadWrite, SaveGame, Replicated)
	TEnumAsByte<EConnectionType> FilterType = Unknown;

	UPROPERTY(BlueprintReadWrite, SaveGame, Replicated)
	bool VerifyType = true;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override
	{
		Super::GetLifetimeReplicatedProps(OutLifetimeProps);
		DOREPLIFETIME(UCircuitryArrayFilterBase, FilterType);
	}
};
