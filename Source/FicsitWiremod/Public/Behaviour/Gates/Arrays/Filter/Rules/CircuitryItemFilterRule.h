#pragma once
#include "CircuitryNumberFilterRule.h"
#include "FGInventoryComponent.h"
#include "FilterRule.h"
#include "ItemAmount.h"
#include "Resources/FGItemDescriptor.h"

#include "CircuitryItemFilterRule.generated.h"

USTRUCT(Blueprintable, BlueprintType)
struct FCircuitryItemDescriptorFilterRule : public FCircuitryFilterRule
{
	GENERATED_BODY()

public:

	UPROPERTY(BlueprintReadWrite)
	TArray<TSubclassOf<UFGItemDescriptor>> Value = TArray<TSubclassOf<UFGItemDescriptor>>();

	/*
	 * Inverse mode means that this rule should not pass if value array contains the provided input value
	 */
	UPROPERTY(BlueprintReadWrite)
	bool InverseMode = false;

	bool CheckFilterMatch(TSubclassOf<UFGItemDescriptor> Other, bool IfUnused = false) const
	{
		if(!RuleUsed) return IfUnused;
		
		bool Out = Value.Contains(Other);
		if(InverseMode) Out = !Out;

		return Out;
	}

	void FilterValues(TArray<TSubclassOf<UFGItemDescriptor>>& Values)
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

	bool operator()(const TSubclassOf<UFGItemDescriptor>& Other) const { return CheckFilterMatch(Other, true); }
};


USTRUCT(Blueprintable, BlueprintType)
struct FCircuitryItemFilterRule : public FCircuitryFilterRule
{
	GENERATED_BODY()

public:

	UPROPERTY(BlueprintReadWrite)
	FCircuitryItemDescriptorFilterRule DescriptorFilter;

	UPROPERTY(BlueprintReadWrite)
	FCircuitryNumberFilterRule AmountFilter;

	UPROPERTY(BlueprintReadWrite)
	TEnumAsByte<ECircuitryFilterMultiRuleType> FilterType = CFRT_OR;

	bool Predicate_Amount(const FItemAmount& Other) const { return CheckFilterMatch(Other.ItemClass, Other.Amount, true); }

	bool Predicate_Stack(const FInventoryStack& Other) const { return CheckFilterMatch(Other.Item.GetItemClass(), Other.NumItems, true); }

	void FilterValues(TArray<FItemAmount>& Values)
	{
		if (!RuleUsed) return;
		if (Values.IsEmpty()) return;
		
		int CurrentIndex = 0;
		while (CurrentIndex < Values.Num())
		{
			auto MatchesFilter = CheckFilterMatch(Values[CurrentIndex].ItemClass, Values[CurrentIndex].Amount);
			
			if (MatchesFilter)
				CurrentIndex++;
			else
				Values.RemoveAt(CurrentIndex);
		}
	}

	void FilterValues(TArray<FInventoryStack>& Values)
	{
		if (!RuleUsed) return;
		if (Values.IsEmpty()) return;
		
		int CurrentIndex = 0;
		while (CurrentIndex < Values.Num())
		{
			auto MatchesFilter = CheckFilterMatch(Values[CurrentIndex].Item.GetItemClass(), Values[CurrentIndex].NumItems);
			
			if (MatchesFilter)
				CurrentIndex++;
			else
				Values.RemoveAt(CurrentIndex);
		}
	}
	
private:

	bool CheckFilterMatch(TSubclassOf<UFGItemDescriptor> Desc, int Amount, bool IfUnused = false) const
	{
		if(!RuleUsed) return IfUnused;

		switch (FilterType)
		{
		case CFRT_OR: return DescriptorFilter.CheckFilterMatch(Desc) || AmountFilter.CheckFilterMatch(Amount);
		case CFRT_AND: return DescriptorFilter.CheckFilterMatch(Desc, true) && AmountFilter.CheckFilterMatch(Amount, true);
		default: return false;
		}
	}
};
