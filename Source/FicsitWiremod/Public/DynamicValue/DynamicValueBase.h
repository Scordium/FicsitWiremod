// Copyright Coffee Stain Studios. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "WiremodReflection.h"
#include "UObject/Object.h"
#include "DynamicValueBase.generated.h"

/**
 * 
 */
UCLASS(Blueprintable, NonTransient)
class FICSITWIREMOD_API UDynamicValueBase : public UObject
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite, SaveGame, Replicated)
	TEnumAsByte<EConnectionType> ValueType;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps)
	{
		Super::GetLifetimeReplicatedProps(OutLifetimeProps);

		DOREPLIFETIME(UDynamicValueBase, ValueType)
	}

	virtual bool IsSupportedForNetworking() const override{return true;}
	
	
};
