// 

#pragma once

#include "CoreMinimal.h"
#include "FGResourceSettings.h"
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
	
	UFUNCTION()
	int GetItemAmount() const
	{
		return ItemAmount.Amount;
	}

	UFUNCTION()
	FString GetItemName()
	{
		VerifyCache();
		if(CDOCache) return CDOCache->mDisplayName.ToString();
		return "";
	}

	UFUNCTION()
	FString GetItemDesc()
	{
		VerifyCache();
		if(CDOCache) return CDOCache->mDescription.ToString();
		return "";
	}

	UFUNCTION()
	double GetItemAmountSize()
	{
		VerifyCache();
		if(CDOCache) return UFGResourceSettings::Get()->GetStackSizeFromEnum(CDOCache->mStackSize);
		return -1;
	}

	UFUNCTION()
	bool GetCanDiscard()
	{
		VerifyCache();
		if(CDOCache) return CDOCache->mCanBeDiscarded;
		return false;
	}

	UFUNCTION()
	int GetIconId()
	{
		VerifyCache();
		if(CDOCache) return UTextureUtilities::GetIconFromTexture(CDOCache->GetBigIconFromInstance());
		return -1;
	}

	UFUNCTION()
	int GetItemType()
	{
		VerifyCache();
		if(CDOCache) return (int) CDOCache->Form();
		return -1;
	}

	UFUNCTION()
	bool GetIsSolid()
	{
		VerifyCache();
		if(CDOCache) return CDOCache->Form() == EResourceForm::RF_SOLID;
		return false;
	}

	UFUNCTION()
	bool GetIsLiquid()
	{
		VerifyCache();
		if(CDOCache) return CDOCache->Form() == EResourceForm::RF_LIQUID;
		return false;
	}

	UFUNCTION()
	bool GetIsGas()
	{
		VerifyCache();
		if(CDOCache) return CDOCache->Form() == EResourceForm::RF_GAS;
		return false;
	}

	UFUNCTION()
	UTexture* GetItemIcon()
	{
		VerifyCache();
		if(CDOCache) return CDOCache->GetBigIconFromInstance();
		return nullptr;
	}

	UFUNCTION()
	TSubclassOf<UFGItemDescriptor> GetItemDescriptor()
	{
		return ItemAmount.ItemClass;
	}

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override
	{
		Super::GetLifetimeReplicatedProps(OutLifetimeProps);

		DOREPLIFETIME(ABreakItemAmount, ItemAmount);
	}

	void VerifyCache()
	{
		if(!CDOCache || CDOCache->GetClass() != ItemAmount.ItemClass)
			CDOCache = ItemAmount.ItemClass.GetDefaultObject();
	}


	UPROPERTY(Replicated, SaveGame, VisibleInstanceOnly)
	FItemAmount ItemAmount;
	
	UPROPERTY()
	UFGItemDescriptor* CDOCache;
};
