// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CircuitryArrayFilterBase.h"
#include "Behaviour/Gates/Arrays/Filter/Rules/CircuitryVectorFilterRule.h"
#include "CircuitryVectorArrayFilter.generated.h"

/**
 * 
 */
UCLASS()
class FICSITWIREMOD_API UCircuitryVectorArrayFilter : public UCircuitryArrayFilterBase
{
	GENERATED_BODY()

public:
	UCircuitryVectorArrayFilter() : Super(Vector){}
	
	virtual bool FromJson(const FCircuitryArrayFilterData& FilterData) override { return DeserializeArrayFilter(FilterData, Data.StaticStruct(), &Data); }
	
	virtual FCircuitryArrayFilterData ToJson() override { return SerializeArrayFilter(Data.StaticStruct(), &Data); }

	UPROPERTY(BlueprintReadWrite, SaveGame, Replicated)
	FCircuitryVectorFilterRule Data;
	
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override
	{
		Super::GetLifetimeReplicatedProps(OutLifetimeProps);
		DOREPLIFETIME(ThisClass, Data);
	}

	TArray<FVector> FilterValues(const TArray<FVector>& In) const
	{
		return In.FilterByPredicate([this](const FVector& InVector) { return Data.CheckFilterMatch(InVector, true); });
	}
};
