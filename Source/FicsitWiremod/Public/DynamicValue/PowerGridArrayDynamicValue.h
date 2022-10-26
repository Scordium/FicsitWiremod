// Copyright Coffee Stain Studios. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "DynamicValueBase.h"
#include "FGPowerCircuit.h"
#include "PowerGridArrayDynamicValue.generated.h"

/**
 * 
 */
UCLASS(Blueprintable,NonTransient)
class FICSITWIREMOD_API UPowerGridArrayDynamicValue : public UDynamicValueBase
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite, SaveGame,Replicated)
	TArray<UFGPowerCircuit*> Value;


	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps)
	{
		Super::GetLifetimeReplicatedProps(OutLifetimeProps);

		DOREPLIFETIME(UPowerGridArrayDynamicValue, Value)
	}

	virtual bool IsSupportedForNetworking() const override{return true;}
};
