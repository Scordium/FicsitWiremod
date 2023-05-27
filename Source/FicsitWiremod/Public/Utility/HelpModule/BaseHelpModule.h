// 

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "BaseHelpModule.generated.h"

/**
 * 
 */
UCLASS(BlueprintType, EditInlineNew, DefaultToInstanced)
class FICSITWIREMOD_API UBaseHelpModule : public UObject
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintNativeEvent)
	void Execute();
	virtual void Execute_Implementation(){};
};
