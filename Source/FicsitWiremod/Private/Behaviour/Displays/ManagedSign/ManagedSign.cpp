#include "Behaviour/Displays/ManagedSign/ManagedSign.h"

void USignComponentsCategory::AddItem(const FString& ItemCategory, USignEditorComponentBase* Component, int NestingLevel)
{
	FString CategoryThis = ItemCategory.TrimStartAndEnd();
	FString	CategoryNext;
	
	if(NestingLevel < 5 && ItemCategory.Split("|", &CategoryThis, &CategoryNext))
	{
		CategoryThis.TrimStartAndEndInline();
		CategoryNext.TrimStartAndEndInline();
		
		auto ThisLevelCategory = Subcategories.FindOrAdd(CategoryThis, NewObject<USignComponentsCategory>(this));
		ThisLevelCategory->CategoryName = CategoryThis;

		ThisLevelCategory->AddItem(CategoryNext, Component, NestingLevel + 1);
	}
	else if(NestingLevel < 5 && ItemCategory.Len() > 0)
	{
		auto ThisLevelCategory = Subcategories.FindOrAdd(ItemCategory, NewObject<USignComponentsCategory>(this));
		ThisLevelCategory->CategoryName = ItemCategory;
		ThisLevelCategory->Components.Add(Component);
	}
	else Components.Add(Component);
}
