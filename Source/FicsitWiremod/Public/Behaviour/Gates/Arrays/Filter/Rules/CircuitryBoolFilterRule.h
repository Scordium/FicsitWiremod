#pragma once
#include "FilterRule.h"
#include "CircuitryBoolFilterRule.generated.h"

USTRUCT(Blueprintable, BlueprintType)
struct FCircuitryBoolFilterRule : public FCircuitryFilterRule
{
	GENERATED_BODY()

public:

	UPROPERTY(BlueprintReadWrite)
	bool Value = true;

	bool CheckFilterMatch(bool Other, bool IfUnused = false) const
	{	
		return RuleUsed ? Other == Value : IfUnused;
	}
};