// 

#pragma once

#include "CoreMinimal.h"
#include "Behaviour/FGWiremodBuildable.h"
#include "CommonLib/TextureUtilities.h"
#include "BreakItemAmount.generated.h"

UCLASS()
class FICSITWIREMOD_API ABreakItemAmount : public AFGWiremodBuildable
{
	GENERATED_BODY()

public:
	virtual void ServerProcess_Implementation(double DeltaTime) override
	{
		ItemAmount = GetConnection(0).GetItemAmount();
	}
	
	UFUNCTION(BlueprintPure)
	int GetItemAmount() const
	{
		return ItemAmount.Amount;
	}

	UFUNCTION(BlueprintPure)
	FString GetItemName() const
	{
		return UFGItemDescriptor::GetItemName(ItemAmount.ItemClass).ToString();
	}

	UFUNCTION(BlueprintPure)
	FString GetItemDesc() const
	{
		return UFGItemDescriptor::GetItemDescription(ItemAmount.ItemClass).ToString();
	}

	UFUNCTION(BlueprintPure)
	double GetItemAmountSize() const
	{
		return UFGItemDescriptor::GetStackSizeConverted(ItemAmount.ItemClass);
	}

	UFUNCTION(BlueprintPure)
	bool GetCanDiscard() const
	{
		return UFGItemDescriptor::CanBeDiscarded(ItemAmount.ItemClass);
	}

	UFUNCTION(BlueprintCallable)
	int GetIconId() const
	{
		return UTextureUtilities::GetIconFromItemAmount(ItemAmount);
	}

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override
	{
		Super::GetLifetimeReplicatedProps(OutLifetimeProps);

		DOREPLIFETIME(ABreakItemAmount, ItemAmount);
	}


	UPROPERTY(Replicated, VisibleInstanceOnly)
	FItemAmount ItemAmount;
};
