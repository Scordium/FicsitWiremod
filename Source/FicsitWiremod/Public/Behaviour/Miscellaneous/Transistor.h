// 

#pragma once

#include "CoreMinimal.h"
#include "Behaviour/FGWiremodBuildable.h"
#include "CommonLib/DynamicValues/CCDynamicValueUtils.h"
#include "Engine/ActorChannel.h"
#include "Transistor.generated.h"

UCLASS()
class FICSITWIREMOD_API ATransistor : public AFGWiremodBuildable, public IDynamicValuePasser
{
	GENERATED_BODY()

public:
	virtual void ServerProcess_Implementation(double DeltaTime) override
	{
		if(GetConnection(2).GetBool()) Out = nullptr;
		else if(GetConnection(1).GetBool())
			Out = UCCDynamicValueUtils::FromValue(GetConnection(0), Out ? (UObject*)Out : (UObject*)this);

		SetOutputType(0, Out ? Out->ConnectionType.GetValue() : Unknown);
	}

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override
	{
		Super::GetLifetimeReplicatedProps(OutLifetimeProps);

		DOREPLIFETIME(ATransistor, Out)
	}

	virtual void GatherReplicatedObjects_Implementation(TArray<UObject*>& OutObjects) override
	{
		Super::GatherReplicatedObjects_Implementation(OutObjects);
		OutObjects.Add(Out);
	}


	virtual UObject* GetValue_Implementation(const class FString& ValueName) override{ return Out; }

	UPROPERTY(Replicated, SaveGame)
	UCCDynamicValueBase* Out;
};
