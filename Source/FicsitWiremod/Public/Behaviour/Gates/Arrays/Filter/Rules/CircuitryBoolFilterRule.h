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

	void FilterValues(TArray<bool>& Values)
	{
		if (!RuleUsed) return;
		if (Values.IsEmpty()) return;
		
		int CurrentIndex = 0;
		while (CurrentIndex < Values.Num())
		{
			auto MatchesFilter = CheckFilterMatch(Values[CurrentIndex]);
			
			if (MatchesFilter)
				CurrentIndex++;
			else
				Values.RemoveAt(CurrentIndex);
		}
	}
};