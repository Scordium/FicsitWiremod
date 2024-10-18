// 

#pragma once

#include "CoreMinimal.h"
#include "../../CommonLib/DynamicValues/CCDynamicValueBase.h"
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
		auto Connection = GetConnection(0);

		if(CachedValue == nullptr) CachedValue = UCCDynamicValueUtils::FromValue(GetConnection(0), CachedValue);
		else Out = !CachedValue->Equals(Connection.Object, Connection.FunctionName, Connection.FromProperty);

		if (Out) CachedValue->FromConnectionValue(Connection.Object, Connection.FunctionName, Connection.FromProperty);
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
