// Copyright Coffee Stain Studios. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "DynamicValueBase.h"
#include "NumberDynamicValue.generated.h"

/**
 * 
 */
UCLASS(Blueprintable,NonTransient)
class FICSITWIREMOD_API UNumberDynamicValue : public UDynamicValueBase
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite, SaveGame,Replicated)
	float Value;


	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps)
	{
		Super::GetLifetimeReplicatedProps(OutLifetimeProps);

		DOREPLIFETIME(UNumberDynamicValue, Value)
	}

	virtual bool IsSupportedForNetworking() const override{return true;}
};
