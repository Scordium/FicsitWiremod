// Copyright Coffee Stain Studios. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "DynamicValueBase.h"
#include "UObject/Object.h"
#include "ColorDynamicValue.generated.h"

/**
 * 
 */
UCLASS(Blueprintable, NonTransient)
class FICSITWIREMOD_API UColorDynamicValue : public UDynamicValueBase
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite, SaveGame, Replicated)
	FLinearColor Value;


	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps)
	{
		Super::GetLifetimeReplicatedProps(OutLifetimeProps);

		DOREPLIFETIME(UColorDynamicValue, Value)
	}

	virtual bool IsSupportedForNetworking() const override{return true;}
};
