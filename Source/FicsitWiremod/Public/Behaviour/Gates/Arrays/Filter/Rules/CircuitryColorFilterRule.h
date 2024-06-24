#pragma once
#include "CircuitryNumberFilterRule.h"
#include "FilterRule.h"

#include "CircuitryColorFilterRule.generated.h"

USTRUCT(Blueprintable, BlueprintType)
struct FCircuitryColorFilterRule : public FCircuitryFilterRule
{
	GENERATED_BODY()

public:

	UPROPERTY(BlueprintReadWrite)
	FCircuitryNumberFilterRule FilterR;

	UPROPERTY(BlueprintReadWrite)
	FCircuitryNumberFilterRule FilterG;

	UPROPERTY(BlueprintReadWrite)
	FCircuitryNumberFilterRule FilterB;

	UPROPERTY(BlueprintReadWrite)
	FCircuitryNumberFilterRule FilterA;

	UPROPERTY(BlueprintReadWrite)
	TEnumAsByte<ECircuitryFilterMultiRuleType> FilterType = CFRT_OR;

	bool CheckFilterMatch(const FLinearColor& Other, bool IfUnused = false) const
	{
		if(!RuleUsed) return IfUnused;

		switch (FilterType)
		{
		case CFRT_OR: return FilterR.CheckFilterMatch(Other.R) || FilterG.CheckFilterMatch(Other.G) || FilterB.CheckFilterMatch(Other.B) || FilterA.CheckFilterMatch(Other.A);
		case CFRT_AND: return FilterR.CheckFilterMatch(Other.R, true) && FilterG.CheckFilterMatch(Other.G, true) && FilterB.CheckFilterMatch(Other.B, true) && FilterA.CheckFilterMatch(Other.A, true);
		default: return false;
		}
	}
};
