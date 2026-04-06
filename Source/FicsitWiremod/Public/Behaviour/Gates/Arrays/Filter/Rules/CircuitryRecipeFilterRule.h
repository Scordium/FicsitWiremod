#pragma once

#include "CircuitryItemFilterRule.h"
#include "FilterRule.h"
#include "FGRecipe.h"
#include "Runtime/CoreUObject/Public/Templates/SubclassOf.h" 

#include "CircuitryRecipeFilterRule.generated.h"

USTRUCT(Blueprintable, BlueprintType)
struct FCircuitryRecipeFilterRule : public FCircuitryFilterRule
{
	GENERATED_BODY()

public:

	UPROPERTY(BlueprintReadWrite)
	FCircuitryItemFilterRule IngredientsFilter;

	UPROPERTY(BlueprintReadWrite)
	FCircuitryItemFilterRule ProductsFilter;

	UPROPERTY(BlueprintReadWrite)
	TEnumAsByte<ECircuitryFilterMultiRuleType> FilterType = CFRT_OR;

	UPROPERTY(BlueprintReadWrite)
	TEnumAsByte<ECircuitryFilterArrayMatchType> IngredientsFilterType = CFAMT_ANY;

	UPROPERTY(BlueprintReadWrite)
	TEnumAsByte<ECircuitryFilterArrayMatchType> ProductsFilterType = CFAMT_ANY;

	bool CheckFilterMatch(TSubclassOf<UFGRecipe> Recipe, bool IfUnused = false) const
	{
		if (!RuleUsed) return IfUnused;

		switch (FilterType)
		{
			case CFRT_OR: return IngredientsMatch(Recipe) || ProductsMatch(Recipe);
			case CFRT_AND: return IngredientsMatch(Recipe) && ProductsMatch(Recipe);
			default: return false;
		}
	}

	void FilterValues(TArray<TSubclassOf<UFGRecipe>>& Values)
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

	/*I should just implement array utilities myself...*/

	bool IngredientsMatch(TSubclassOf<UFGRecipe> Recipe) const
	{
		auto Ingredients = UFGRecipe::GetIngredients(Recipe);

		switch (IngredientsFilterType)
		{
		default:
		case CFAMT_ANY: return Ingredients.ContainsByPredicate([this](const FItemAmount& Item) -> bool { return IngredientsFilter.Predicate_Amount(Item); } );
		case CFAMT_NONE: return !Ingredients.ContainsByPredicate([this](const FItemAmount& Item) -> bool { return IngredientsFilter.Predicate_Amount(Item); } );
			//Adding a comment here so if someone reads this they don't have a stroke -
			//We're checking for ANY value that doesn't match the predicate,
			//if it exists then the Contains func will return true,  we invert it to return false (found an item that doesn't match)
		case CFAMT_ALL: return !Ingredients.ContainsByPredicate([this](const FItemAmount& Item) -> bool { return !IngredientsFilter.Predicate_Amount(Item); } );
		}
	}

	bool ProductsMatch(TSubclassOf<UFGRecipe> Recipe) const
	{
		auto Products = UFGRecipe::GetProducts(Recipe);
		switch (ProductsFilterType)
		{
		default:
		case CFAMT_ANY: return Products.ContainsByPredicate([this](const FItemAmount& Item) -> bool { return ProductsFilter.Predicate_Amount(Item); } );
		case CFAMT_NONE: return !Products.ContainsByPredicate([this](const FItemAmount& Item) -> bool { return ProductsFilter.Predicate_Amount(Item); } );
			//Adding a comment here so if someone reads this they don't have a stroke -
			//We're checking for ANY value that doesn't match the predicate,
			//if it exists then the Contains func will return true,  we invert it to return false (found an item that doesn't match)
		case CFAMT_ALL: return !Products.ContainsByPredicate([this](const FItemAmount& Item) -> bool { return !ProductsFilter.Predicate_Amount(Item); } );
		}
	}
};
