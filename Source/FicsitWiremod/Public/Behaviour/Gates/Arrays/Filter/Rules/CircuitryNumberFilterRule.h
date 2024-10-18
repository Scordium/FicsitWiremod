#pragma once
#include "FilterRule.h"

#include "CircuitryNumberFilterRule.generated.h"

UENUM(Blueprintable, BlueprintType)
enum ENumberFilterRuleType
{
	NFRT_LessThan,
	NFRT_LessOrEqual,
	NFRT_Equal,
	NFRT_NotEqual,
	NFRT_GreaterOrEqual,
	NFRT_GreaterThan,
	NFRT_IsInRange,
	NFRT_IsNotInRange
};

USTRUCT(Blueprintable, BlueprintType)
struct FCircuitryNumberFilterRule : public FCircuitryFilterRule
{
	GENERATED_BODY()

public:

	UPROPERTY(BlueprintReadWrite)
	TEnumAsByte<ENumberFilterRuleType> FilterRuleType = NFRT_GreaterThan;

	UPROPERTY(BlueprintReadWrite)
	double Value = 0;

	UPROPERTY(BlueprintReadWrite)
	double AuxValue = 1;

	bool CheckFilterMatch(double CompareValue, bool IfUnused = false) const
	{
		if(!RuleUsed) return IfUnused;
		
		switch (FilterRuleType)
		{
		case NFRT_LessThan: return CompareValue < Value;
		case NFRT_LessOrEqual: return CompareValue <= Value;
		case NFRT_Equal: return CompareValue == Value;
		case NFRT_NotEqual: return CompareValue != Value;
		case NFRT_GreaterOrEqual: return CompareValue >= Value;
		case NFRT_GreaterThan: return CompareValue > Value;
		case NFRT_IsInRange: return CompareValue >= Value && CompareValue <= AuxValue;
		case NFRT_IsNotInRange: return CompareValue < Value || CompareValue > AuxValue;
		default: return false;
		}
	}

	bool operator()(const double& Other) const { return CheckFilterMatch(Other, true); }
};
