// Copyright Coffee Stain Studios. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "DynamicValueBase.h"
#include "UObject/Object.h"
#include "InventoryArrayDynamicValue.generated.h"

/**
 * 
 */
UCLASS(Blueprintable,NonTransient)
class FICSITWIREMOD_API UInventoryArrayDynamicValue : public UDynamicValueBase
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite, SaveGame, Replicated)
	TArray<UFGInventoryComponent*> Value;


	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps)
	{
		Super::GetLifetimeReplicatedProps(OutLifetimeProps);

		DOREPLIFETIME(UInventoryArrayDynamicValue, Value)
	}

	virtual bool IsSupportedForNetworking() const override{return true;}
};
