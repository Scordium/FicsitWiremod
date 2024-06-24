// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CircuitryArrayFilterBase.h"
#include "Behaviour/Gates/Arrays/Filter/Rules/CircuitryStringFilterRule.h"
#include "CircuitryStringArrayFilter.generated.h"

/**
 * 
 */
UCLASS()
class FICSITWIREMOD_API UCircuitryStringArrayFilter : public UCircuitryArrayFilterBase
{
	GENERATED_BODY()

public:
	UCircuitryStringArrayFilter() : Super(String){}
	
	virtual bool FromJson(const FCircuitryArrayFilterData& FilterData) override { return DeserializeArrayFilter(FilterData, Data.StaticStruct(), &Data); }
	
	virtual FCircuitryArrayFilterData ToJson() override { return SerializeArrayFilter(Data.StaticStruct(), &Data); }

	UPROPERTY(BlueprintReadWrite, SaveGame, Replicated)
	FCircuitryStringFilterRule Data;
	
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override
	{
		Super::GetLifetimeReplicatedProps(OutLifetimeProps);
		DOREPLIFETIME(ThisClass, Data);
	}

	TArray<FString> FilterValues(const TArray<FString>& In) const { return In.FilterByPredicate([this](const FString& Value) { return Data.CheckFilterMatch(Value, true); }); }
};
