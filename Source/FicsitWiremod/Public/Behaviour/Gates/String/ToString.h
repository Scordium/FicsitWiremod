// 

#pragma once

#include "CoreMinimal.h"
#include "Behaviour/FGWiremodBuildable.h"
#include "CommonLib/DynamicValues/CCDynamicValueUtils.h"
#include "ToString.generated.h"

UCLASS()
class FICSITWIREMOD_API AToString : public AFGWiremodBuildable
{
	GENERATED_BODY()

public:

	virtual void ServerProcess_Implementation(double DeltaTime) override
	{
		ValueParserCache = UCCDynamicValueUtils::FromValue(GetConnection(0), ValueParserCache);
		if(ValueParserCache) Out = ValueParserCache->ToString();
		else Out = "";
	}


	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override
	{
		Super::GetLifetimeReplicatedProps(OutLifetimeProps);

		DOREPLIFETIME(AToString, Out)
	}
	
	UPROPERTY(Replicated, SaveGame)
	FString Out;


	//Utility property so that we don't have to create a new object each time we need to read the value,
	//because the value type will *not* change for 99.9% of the time this gate will function.
	UPROPERTY()
	UCCDynamicValueBase* ValueParserCache;
};
