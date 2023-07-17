// 

#pragma once

#include "CoreMinimal.h"
#include "Behaviour/FGWiremodBuildable.h"
#include "CommonLib/DynamicValues/CCDynamicValueUtils.h"
#include "Equals.generated.h"

UCLASS()
class FICSITWIREMOD_API AEquals : public AFGWiremodBuildable
{
	GENERATED_BODY()

public:
	virtual void ServerProcess_Implementation(double DeltaTime) override
	{
		ElementACache = UCCDynamicValueUtils::FromValue(GetConnection(0), ElementACache);
		ElementBCache = UCCDynamicValueUtils::FromValue(GetConnection(1), ElementBCache);

		if(!ElementACache || !ElementBCache) Out = false;
		else Out = ElementACache->Equals(ElementBCache);
	}


	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override
	{
		Super::GetLifetimeReplicatedProps(OutLifetimeProps);

		DOREPLIFETIME(AEquals, Out);
	}

	UPROPERTY()
	UCCDynamicValueBase* ElementACache;

	UPROPERTY()
	UCCDynamicValueBase* ElementBCache;

	UPROPERTY(BlueprintReadOnly, SaveGame, Replicated)
	bool Out;
};
