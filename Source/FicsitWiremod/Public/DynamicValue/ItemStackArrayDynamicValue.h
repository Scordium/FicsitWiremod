// Copyright Coffee Stain Studios. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "DynamicValueBase.h"
#include "ItemStackArrayDynamicValue.generated.h"

/**
 * 
 */
UCLASS(Blueprintable,NonTransient)
class FICSITWIREMOD_API UItemStackArrayDynamicValue : public UDynamicValueBase
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite, SaveGame,Replicated)
	TArray<FInventoryStack> Value;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps)
	{
		Super::GetLifetimeReplicatedProps(OutLifetimeProps);

		DOREPLIFETIME(UItemStackArrayDynamicValue, Value)
	}

	virtual bool IsSupportedForNetworking() const override{return true;}
};
