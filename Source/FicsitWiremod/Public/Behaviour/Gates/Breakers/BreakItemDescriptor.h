// 

#pragma once

#include "CoreMinimal.h"
#include "FGWiremodBuildable.h"
#include "Runtime/CoreUObject/Public/Templates/SubclassOf.h"

#include "BreakItemDescriptor.generated.h"

#define GET_CACHE_CHECKED(ret) if(DescriptorCache == nullptr) return ret;\
	auto CDO = DescriptorCache.GetDefaultObject();

UCLASS()
class FICSITWIREMOD_API ABreakItemDescriptor : public AFGWiremodBuildable
{
	GENERATED_BODY()

public:
	virtual void ServerProcess_Implementation(double DeltaTime) override
	{
		DescriptorCache = GetConnection(0).GetItemDescriptor();
	}

	
	UFUNCTION()
	FString GetItemName()
	{
		GET_CACHE_CHECKED("");
		return CDO->mDisplayName.ToString();
	}

	UFUNCTION()
	FString GetItemDesc()
	{
		GET_CACHE_CHECKED("");
		return CDO->mDescription.ToString();
	}

	UFUNCTION()
	double GetItemAmountSize()
	{
		GET_CACHE_CHECKED(-1);

		return UFGItemDescriptor::GetStackSize(DescriptorCache);
	}

	UFUNCTION()
	bool GetCanDiscard()
	{
		return UFGItemDescriptor::CanBeDiscarded(DescriptorCache);
	}

	UFUNCTION()
	int GetItemType()
	{
		GET_CACHE_CHECKED(-1);
		
		return (int) CDO->Form();
	}

	UFUNCTION()
	bool GetIsSolid()
	{
		GET_CACHE_CHECKED(false);

		return CDO->Form() == EResourceForm::RF_SOLID;
	}

	UFUNCTION()
	bool GetIsLiquid()
	{
		GET_CACHE_CHECKED(false);

		return CDO->Form() == EResourceForm::RF_LIQUID;
	}

	UFUNCTION()
	bool GetIsGas()
	{
		GET_CACHE_CHECKED(false);

		return CDO->Form() == EResourceForm::RF_GAS;
	}

	UFUNCTION()
	UTexture* GetItemIcon()
	{
		GET_CACHE_CHECKED(nullptr);

		return UFGItemDescriptor::GetBigIcon(DescriptorCache);
	}

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override
	{
		Super::GetLifetimeReplicatedProps(OutLifetimeProps);

		DOREPLIFETIME(ABreakItemDescriptor, DescriptorCache);
	}

	UPROPERTY(Replicated, SaveGame)
	TSubclassOf<UFGItemDescriptor> DescriptorCache;
};
