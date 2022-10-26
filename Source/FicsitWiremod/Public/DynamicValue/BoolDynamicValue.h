// Copyright Coffee Stain Studios. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "DynamicValueBase.h"
#include "UObject/Object.h"
#include "BoolDynamicValue.generated.h"

/**
 * 
 */
UCLASS(Blueprintable, NonTransient)
class FICSITWIREMOD_API UBoolDynamicValue : public UDynamicValueBase
{
	GENERATED_BODY()
	
public:
	UPROPERTY(BlueprintReadWrite, SaveGame, Replicated)
	bool Value;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps)
	{
		Super::GetLifetimeReplicatedProps(OutLifetimeProps);

		DOREPLIFETIME(UBoolDynamicValue, Value)
	}
	virtual bool IsSupportedForNetworking() const override{return true;}
};
