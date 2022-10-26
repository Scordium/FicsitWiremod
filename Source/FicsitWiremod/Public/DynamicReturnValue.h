// Copyright Coffee Stain Studios. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "DynamicValue/DynamicValueBase.h"
#include "UObject/Interface.h"
#include "DynamicReturnValue.generated.h"

// This class does not need to be modified.
UINTERFACE()
class UDynamicReturnValue : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class FICSITWIREMOD_API IDynamicReturnValue
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	UDynamicValueBase* GetValue(const FString& ValueName);
};
