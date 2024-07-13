// 

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ContextMenuItemWidget.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnContextMenuItemSelected, UUserWidget*, Item);
DECLARE_DYNAMIC_DELEGATE_OneParam(FOnContextMenuItemClicked, UUserWidget*, Item);
/**
 * 
 */
UCLASS(Blueprintable, BlueprintType, Abstract)
class FICSITWIREMOD_API UContextMenuItemWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite, meta=(ExposeOnSpawn="true"))
	FText DisplayedText;

	UPROPERTY(BlueprintReadWrite, meta=(ExposeOnSpawn="true"))
	FText Tooltip;

	UPROPERTY(BlueprintReadWrite, meta=(ExposeOnSpawn="true"))
	FText Keybind;
	/*
	 * Whether this item is clickable on its own, or is only used to contain other items.
	 */
	UPROPERTY(BlueprintReadWrite, meta=(ExposeOnSpawn="true"))
	bool IsClickable = true;
	
	UPROPERTY(BlueprintReadWrite, meta=(ExposeOnSpawn="true"))
	TArray<UContextMenuItemWidget*> Children;
	
	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FOnContextMenuItemSelected OnSelected;

	UFUNCTION(BlueprintCallable)
	void BindMenuContainerToSelectionEvent(const FOnContextMenuItemClicked& Event)
	{
		OnSelected.Add(Event);
		for(auto Child : Children)
		{
			if(Child) Child->BindMenuContainerToSelectionEvent(Event);
		}
	}

protected:

	UFUNCTION(BlueprintCallable)
	void CallClickedEvent(){ OnSelected.Broadcast(this); }
};
