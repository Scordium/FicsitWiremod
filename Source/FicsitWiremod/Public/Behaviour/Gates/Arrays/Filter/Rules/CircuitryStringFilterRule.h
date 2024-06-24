#pragma once
#include "FilterRule.h"

#include "CircuitryStringFilterRule.generated.h"

UENUM(Blueprintable, BlueprintType)
enum EStringFilterRuleType
{
	SFRT_StartsWith,
	SFRT_EndsWith,
	SFRT_Contains,
	SFRT_NotContains,
	SFRT_Equal,
	SFRT_NotEqual,
	SFRT_ShorterThan,
	SFRT_ExactLength,
	SFRT_NotExactLength,
	SFRT_LongerThan
};

USTRUCT(Blueprintable, BlueprintType)
struct FCircuitryStringFilterRule : public FCircuitryFilterRule
{
	GENERATED_BODY()

public:

	UPROPERTY(BlueprintReadWrite)
	FString Value = "";

	UPROPERTY(BlueprintReadWrite)
	TEnumAsByte<EStringFilterRuleType> RuleType = SFRT_Contains;

	UPROPERTY(BlueprintReadWrite)
	bool CaseSensitive = false;

	bool CheckFilterMatch(const FString& Other, bool IfUnused = false) const
	{
		if(!RuleUsed) return IfUnused;

		const auto CaseSensitivity = CaseSensitive ? ESearchCase::CaseSensitive : ESearchCase::IgnoreCase;

		switch (RuleType)
		{
		case SFRT_StartsWith: return Other.StartsWith(Value, CaseSensitivity);
		case SFRT_EndsWith: return Other.EndsWith(Value, CaseSensitivity);
		case SFRT_Contains: return Other.Contains(Value, CaseSensitivity);
		case SFRT_NotContains: return !Other.Contains(Value, CaseSensitivity);
		case SFRT_Equal: return Other.Equals(Value, CaseSensitivity);
		case SFRT_NotEqual: return !Other.Equals(Value, CaseSensitivity);
		case SFRT_ShorterThan: return Other.Len() < Value.Len();
		case SFRT_ExactLength: return Other.Len() == Value.Len();
		case SFRT_NotExactLength: return Other.Len() != Value.Len();
		case SFRT_LongerThan: return Other.Len() > Value.Len();
		default: return false;
		}
	}
};
