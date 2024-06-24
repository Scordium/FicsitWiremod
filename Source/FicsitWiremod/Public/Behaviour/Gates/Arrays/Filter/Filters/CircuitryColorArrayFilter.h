// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CircuitryArrayFilterBase.h"
#include "Behaviour/Gates/Arrays/Filter/Rules/CircuitryColorFilterRule.h"
#include "CircuitryColorArrayFilter.generated.h"

/**
 * 
 */
UCLASS()
class FICSITWIREMOD_API UCircuitryColorArrayFilter : public UCircuitryArrayFilterBase
{
	GENERATED_BODY()

public:
	UCircuitryColorArrayFilter() : Super(Color){}
	
	virtual bool FromJson(const FCircuitryArrayFilterData& FilterData) override { return DeserializeArrayFilter(FilterData, Data.StaticStruct(), &Data); }
	
	virtual FCircuitryArrayFilterData ToJson() override { return SerializeArrayFilter(Data.StaticStruct(), &Data); }

	UPROPERTY(BlueprintReadWrite, SaveGame, Replicated)
	FCircuitryColorFilterRule Data;
	
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override
	{
		Super::GetLifetimeReplicatedProps(OutLifetimeProps);
		DOREPLIFETIME(ThisClass, Data);
	}

	TArray<FLinearColor> FilterValues(const TArray<FLinearColor>& In) const { return In.FilterByPredicate([this](const FLinearColor& Value) { return Data.CheckFilterMatch(Value, true); }); }
};
