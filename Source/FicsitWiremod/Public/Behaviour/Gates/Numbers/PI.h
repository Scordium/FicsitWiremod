// 

#pragma once

#include "CoreMinimal.h"
#include "Behaviour/FGWiremodBuildable.h"
#include "PI.generated.h"

UCLASS()
class FICSITWIREMOD_API API : public AFGWiremodBuildable
{
	GENERATED_BODY()

	UFUNCTION()
	float Out() {return PI;}
};
