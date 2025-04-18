// 

#pragma once

#include "CoreMinimal.h"
#include "FGBuildable.h"
#include "CircuitryWireSupport.generated.h"

UCLASS()
class FICSITWIREMOD_API ACircuitryWireSupport : public AFGBuildable
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	FVector GetSnapPositionWorldSpace();
};
