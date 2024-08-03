// 

#pragma once

#include "CoreMinimal.h"
#include "Behaviour/FGWiremodBuildable.h"
#include "CommonLib/DynamicValues/CCDynamicValueUtils.h"
#include "ChangeDetector.generated.h"

UCLASS()
class FICSITWIREMOD_API AChangeDetector : public AFGWiremodBuildable
{
	GENERATED_BODY()

public:
	virtual void ServerProcess_Implementation(double DeltaTime) override
	{
		auto Value = UCCDynamicValueUtils::FromValue(GetConnection(0), CachedValue);

		if(CachedValue == nullptr) Out = false;
		else Out = !CachedValue->Equals(Value);

		CachedValue = Value;
	}

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override
	{
		Super::GetLifetimeReplicatedProps(OutLifetimeProps);

		DOREPLIFETIME(AChangeDetector, Out)
	}


	UPROPERTY(Replicated, SaveGame)
	bool Out;

	UPROPERTY(SaveGame)
	UCCDynamicValueBase* CachedValue;
};
