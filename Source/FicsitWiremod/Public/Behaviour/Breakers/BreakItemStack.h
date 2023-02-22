// 

#pragma once

#include "CoreMinimal.h"
#include "Behaviour/FGWiremodBuildable.h"
#include "CommonLib/TextureUtilities.h"
#include "BreakItemStack.generated.h"

UCLASS()
class FICSITWIREMOD_API ABreakItemStack : public AFGWiremodBuildable
{
	GENERATED_BODY()

public:
	virtual void Process_Implementation(float DeltaTime) override
	{
		Stack = WM_GetStack(0);
	}

	UFUNCTION(BlueprintPure)
	int GetItemCount() const
	{
		return Stack.NumItems;
	}

	UFUNCTION(BlueprintPure)
	FString GetItemName() const
	{
		return UFGItemDescriptor::GetItemName(Stack.Item.GetItemClass()).ToString();
	}

	UFUNCTION(BlueprintPure)
	FString GetItemDesc() const
	{
		return UFGItemDescriptor::GetItemDescription(Stack.Item.GetItemClass()).ToString();
	}

	UFUNCTION(BlueprintPure)
	float GetStackSize() const
	{
		return UFGItemDescriptor::GetStackSizeConverted(Stack.Item.GetItemClass());
	}

	UFUNCTION(BlueprintPure)
	bool GetCanDiscard() const
	{
		return UFGItemDescriptor::CanBeDiscarded(Stack.Item.GetItemClass());
	}

	UFUNCTION(BlueprintCallable)
	int GetIconId() const
	{
		return UTextureUtilities::GetIconFromStack(Stack);
	}

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override
	{
		Super::GetLifetimeReplicatedProps(OutLifetimeProps);

		DOREPLIFETIME(ABreakItemStack, Stack);
	}


	UPROPERTY(Replicated, VisibleInstanceOnly)
	FInventoryStack Stack;
};
