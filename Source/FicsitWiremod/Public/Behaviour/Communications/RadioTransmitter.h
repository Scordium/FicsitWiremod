// 

#pragma once

#include "CoreMinimal.h"
#include "Behaviour/FGWiremodBuildable.h"
#include "RadioTransmitter.generated.h"


// Empty class to prevent issues with radio receiver reading from invalid buildables
UCLASS()
class FICSITWIREMOD_API ARadioTransmitter : public AFGWiremodBuildable
{
	GENERATED_BODY()
};
