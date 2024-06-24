// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CircuitryArrayFilterBase.h"
#include "Behaviour/Gates/Arrays/Filter/Rules/CircuitryItemFilterRule.h"
#include "CircuitryItemArrayFilter.generated.h"

UCLASS()
class UCircuitryItemDescriptorArrayFilter : public UCircuitryArrayFilterBase
{
	GENERATED_BODY()

public:

	UCircuitryItemDescriptorArrayFilter() : Super(ItemDescriptor){}

	virtual bool FromJson(const FCircuitryArrayFilterData& FilterData) override { return DeserializeArrayFilter(FilterData, Data.StaticStruct(), &Data); }
	
	virtual FCircuitryArrayFilterData ToJson() override { return SerializeArrayFilter(Data.StaticStruct(), &Data); }

	UPROPERTY(BlueprintReadWrite, SaveGame, Replicated)
	FCircuitryItemDescriptorFilterRule Data;
	
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override
	{
		Super::GetLifetimeReplicatedProps(OutLifetimeProps);
		DOREPLIFETIME(ThisClass, Data);
	}

	TArray<TSubclassOf<UFGItemDescriptor>> FilterValues(const TArray<TSubclassOf<UFGItemDescriptor>>& In) const
	{
		return In.FilterByPredicate([this](TSubclassOf<UFGItemDescriptor> Item) { return Data.CheckFilterMatch(Item, true); });
	}
};

UCLASS()
class FICSITWIREMOD_API UCircuitryItemArrayFilter : public UCircuitryArrayFilterBase
{
	GENERATED_BODY()

public:
	UCircuitryItemArrayFilter() : Super(ItemAmount) { }
	
	virtual bool FromJson(const FCircuitryArrayFilterData& FilterData) override { return DeserializeArrayFilter(FilterData, Data.StaticStruct(), &Data); }
	
	virtual FCircuitryArrayFilterData ToJson() override { return SerializeArrayFilter(Data.StaticStruct(), &Data); }

	UPROPERTY(BlueprintReadWrite, SaveGame, Replicated)
	FCircuitryItemFilterRule Data;
	
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override
	{
		Super::GetLifetimeReplicatedProps(OutLifetimeProps);
		DOREPLIFETIME(ThisClass, Data);
	}

	TArray<FItemAmount> FilterValues(const TArray<FItemAmount>& In) const
	{
		return In.FilterByPredicate([this](const FItemAmount& Item) { return Data.Predicate_Amount(Item); });
	}
	TArray<FInventoryStack> FilterValues(const TArray<FInventoryStack>& In) const
	{
		return In.FilterByPredicate([this](const FInventoryStack& Stack){ return Data.Predicate_Stack(Stack); });
	}
};
