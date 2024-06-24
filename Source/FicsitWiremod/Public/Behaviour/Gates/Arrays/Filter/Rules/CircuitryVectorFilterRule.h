#pragma once
#include "CircuitryNumberFilterRule.h"

#include "CircuitryVectorFilterRule.generated.h"

USTRUCT(Blueprintable, BlueprintType)
struct FCircuitryVectorFilterRule : public FCircuitryFilterRule
{
	GENERATED_BODY()

public:

	UPROPERTY(BlueprintReadWrite)
	FCircuitryNumberFilterRule FilterX;

	UPROPERTY(BlueprintReadWrite)
	FCircuitryNumberFilterRule FilterY;

	UPROPERTY(BlueprintReadWrite)
	FCircuitryNumberFilterRule FilterZ;

	UPROPERTY(BlueprintReadWrite)
	TEnumAsByte<ECircuitryFilterMultiRuleType> FilterType = CFRT_OR;

	bool CheckFilterMatch(const FVector& Other, bool IfUnused = false) const
	{
		if(!RuleUsed) return IfUnused;

		switch (FilterType)
		{
		case CFRT_OR: return FilterX.CheckFilterMatch(Other.X) || FilterY.CheckFilterMatch(Other.Y) || FilterZ.CheckFilterMatch(Other.Z);
		case CFRT_AND: return FilterX.CheckFilterMatch(Other.X, true) && FilterY.CheckFilterMatch(Other.Y, true) && FilterZ.CheckFilterMatch(Other.Z, true);
		default: return false;
		}
	}

	bool operator()(const FVector& Other) const { return CheckFilterMatch(Other, true); }
};
