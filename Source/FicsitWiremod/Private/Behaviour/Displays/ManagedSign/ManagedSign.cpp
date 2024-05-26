#include "Behaviour/Displays/ManagedSign/ManagedSign.h"

void USignComponentsCategory::AddItem(const FString& ItemCategory, USignEditorComponentBase* Component, int NestingLevel)
{
	FString CategoryThis = ItemCategory;
	FString	CategoryNext;

	bool IsValidCategoryString = (!ItemCategory.Contains("|") && ItemCategory.Len() > 0) || ItemCategory.Split("|", &CategoryThis, &CategoryNext);
	
	if(NestingLevel < 5 && IsValidCategoryString)
	{
		CategoryThis.TrimStartAndEndInline();
		CategoryNext.TrimStartAndEndInline();
		
		auto ThisLevelCategory = Subcategories.FindOrAdd(CategoryThis, NewObject<USignComponentsCategory>(this));
		ThisLevelCategory->CategoryName = CategoryThis;

		ThisLevelCategory->AddItem(CategoryNext, Component, NestingLevel + 1);
	}
	else Components.Add(Component);
}
