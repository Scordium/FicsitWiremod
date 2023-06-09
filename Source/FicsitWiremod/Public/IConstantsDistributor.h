// Copyright Coffee Stain Studios. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "CommonLib/BackwardsCompatibilityHandler.h"
#include "CommonLib/DynamicValues/CCDynamicValueBase.h"
#include "UObject/Interface.h"
#include "IConstantsDistributor.generated.h"


// This class does not need to be modified.
UINTERFACE(MinimalAPI, Blueprintable)
class UIConstantsDistributor : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class FICSITWIREMOD_API IIConstantsDistributor
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	UCCDynamicValueBase* GetValue(const FString& ValueName);
};
