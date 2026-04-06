// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ConnectionType.h"
#include "UObject/Object.h"
#include "FilterRule.generated.h"

UENUM(Blueprintable, BlueprintType)
enum ECircuitryFilterMultiRuleType
{
	CFRT_NONE,
	CFRT_AND,
	CFRT_OR
};

UENUM(Blueprintable, BlueprintType)
enum ECircuitryFilterArrayMatchType
{
	CFAMT_ANY,
	CFAMT_ALL,
	CFAMT_NONE
};

USTRUCT(Blueprintable, BlueprintType)
struct FCircuitryFilterRule
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite)
	bool RuleUsed = true;
	
};

USTRUCT(Blueprintable, BlueprintType)
struct FCircuitryArrayFilterData
{
	GENERATED_BODY()

public:

	UPROPERTY(BlueprintReadWrite, SaveGame)
	TEnumAsByte<EConnectionType> FilterType;

	UPROPERTY(BlueprintReadWrite, SaveGame)
	FString JsonDataString;

	bool operator==(const FCircuitryArrayFilterData& Other) const
	{
		bool TypeMatch = FilterType == Other.FilterType;
		if(FilterType == ItemAmount || FilterType == Stack) TypeMatch = Other.FilterType == ItemAmount || Other.FilterType == Stack;
		
		return TypeMatch && JsonDataString.Equals(Other.JsonDataString, ESearchCase::CaseSensitive);
	}

	bool operator!=(const FCircuitryArrayFilterData& Other) const { return !(*this == Other); }
};

