// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "FilterRule.generated.h"

UENUM(Blueprintable, BlueprintType)
enum ECircuitryFilterMultiRuleType
{
	CFRT_NONE,
	CFRT_AND,
	CFRT_OR
};

USTRUCT(Blueprintable, BlueprintType)
struct FCircuitryFilterRule
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite)
	bool RuleUsed = true;
	
};

