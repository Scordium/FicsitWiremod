// Copyright Coffee Stain Studios. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "DynamicValueBase.h"
#include "UObject/Object.h"
#include "EntityDynamicValue.generated.h"

/**
 * 
 */
UCLASS(Blueprintable, NonTransient)
class FICSITWIREMOD_API UEntityDynamicValue : public UDynamicValueBase
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite, SaveGame,Replicated)
	AActor* Value;


	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps)
	{
		Super::GetLifetimeReplicatedProps(OutLifetimeProps);

		DOREPLIFETIME(UEntityDynamicValue, Value)
	}

	virtual bool IsSupportedForNetworking() const override{return true;}
};
