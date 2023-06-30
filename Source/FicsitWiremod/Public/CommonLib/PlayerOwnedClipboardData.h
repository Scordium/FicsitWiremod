// 

#pragma once

#include "CoreMinimal.h"
#include "FGFactoryClipboard.h"
#include "PlayerOwnedClipboardData.generated.h"

/**
 * 
 */
UCLASS()
class FICSITWIREMOD_API UPlayerOwnedClipboardData : public UFGFactoryClipboardSettings
{
	GENERATED_BODY()

public:
	UPROPERTY()
	APlayerController* Player;
};
