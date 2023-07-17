// 

#pragma once

#include "CoreMinimal.h"
#include "Behaviour/FGWiremodBuildable.h"
#include "CommonLib/DynamicValues/CCArrayValueBase.h"
#include "CommonLib/DynamicValues/CCDynamicValueUtils.h"
#include "ArrayContains.generated.h"

UCLASS()
class FICSITWIREMOD_API AArrayContains : public AFGWiremodBuildable
{
	GENERATED_BODY()

public:
	virtual void Process_Implementation(double DeltaTime) override
	{
		auto Connection = GetConnection(0);
		ArrayCache = UCCDynamicValueUtils::FromValue(Connection, ArrayCache);

		if(auto Array = Cast<UCCArrayValueBase>(ArrayCache))
			Out = Array->Contains(GetConnection(1));
		else Out = false;

		SetInputType(1, UConnectionTypeFunctions::ArrayToBase(Connection.ConnectionType));
	}

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override
	{
		Super::GetLifetimeReplicatedProps(OutLifetimeProps);

		DOREPLIFETIME(AArrayContains, Out)
	}

	UPROPERTY()
	UCCDynamicValueBase* ArrayCache;

	UPROPERTY(Replicated, SaveGame)
	bool Out;
};
