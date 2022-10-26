// Copyright Coffee Stain Studios. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "DynamicValueBase.h"
#include "BoolArrayDynamicValue.generated.h"

/**
 * 
 */
UCLASS(Blueprintable, NonTransient)
class FICSITWIREMOD_API UBoolArrayDynamicValue : public UDynamicValueBase
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite, SaveGame, Replicated)
	TArray<bool> Value;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps)
	{
		Super::GetLifetimeReplicatedProps(OutLifetimeProps);

		DOREPLIFETIME(UBoolArrayDynamicValue, Value)
	}

	virtual bool IsSupportedForNetworking() const override{return true;}
};
