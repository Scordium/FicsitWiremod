// 

#pragma once

#include "CoreMinimal.h"
#include "FGInventoryComponent.h"
#include "Behaviour/FGWiremodBuildable.h"
#include "CommonLib/TextureUtilities.h"
#include "BreakItemStack.generated.h"

class UFGItemDescriptor;

UCLASS()
class FICSITWIREMOD_API ABreakItemStack : public AFGWiremodBuildable
{
	GENERATED_BODY()

public:
	virtual void ServerProcess_Implementation(double DeltaTime) override
	{
		Stack = GetConnection(0).GetStack();
	}

	UFUNCTION(BlueprintPure)
	int GetItemCount() const
	{
		return Stack.NumItems;
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
	double GetStackSize()
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
		return Stack.Item.GetItemClass();
	}

	void VerifyCache()
	{
		if(!CDOCache || CDOCache->GetClass() != Stack.Item.GetItemClass())
			CDOCache = Stack.Item.GetItemClass().GetDefaultObject();
	}
	
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override
	{
		Super::GetLifetimeReplicatedProps(OutLifetimeProps);

		DOREPLIFETIME(ABreakItemStack, Stack);
	}


	UPROPERTY(Replicated, SaveGame, VisibleInstanceOnly)
	FInventoryStack Stack;

	UPROPERTY()
	UFGItemDescriptor* CDOCache;
};
