// 

#pragma once

#include "CoreMinimal.h"
#include "FGRemoteCallObject.h"
#include "CCDynamicValueRCO.generated.h"

/**
 * 
 */
UCLASS(Blueprintable,BlueprintType)
class FICSITWIREMOD_API UCCDynamicValueRCO : public UFGRemoteCallObject
{
	GENERATED_BODY()


public:

	

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override
	{
		Super::GetLifetimeReplicatedProps(OutLifetimeProps);
		DOREPLIFETIME(UCCDynamicValueRCO, RandomPropertyBecauseDocsSaySo)
	}

	UPROPERTY(Replicated)
	int RandomPropertyBecauseDocsSaySo;
};
