#include "Behaviour/Displays/ManagedSign/ManagedSign.h"

void AManagedSign::OnSignDataChanged_Implementation(const FManagedSignData& NewData)
{
	for (int i = 0; i < NewData.Connections.Num() && i < InputConnections.Num(); ++i)
	{
		auto CurrentConnection = GetConnection(i);
		if (!CurrentConnection.IsValid()) continue;
		
		auto& NewConnection = NewData.Connections[i];
		if (UConnectionTypeFunctions::IsValidConnectionPair(NewConnection.Type, CurrentConnection.ConnectionType) == false)
		{
			OnInputDisconnected_Internal(i);
		}
	}
	
	GenerateSignWidget(NewData);
}

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
