// 

#pragma once

#include "CoreMinimal.h"
#include "Behaviour/FGWiremodBuildable.h"
#include "RadiationSensor.generated.h"

UCLASS()
class FICSITWIREMOD_API ARadiationSensor : public AFGWiremodBuildable, public IFGRadiationInterface
{
	GENERATED_BODY()

	virtual void ReceiveRadiation_Implementation(float amount, float duration, FVector direction, TSubclassOf<UFGDamageType> damageType) override
	{
		RadiationAmount = amount;
	}

	virtual bool CanReceiveRadiation_Implementation() const override { return true; }

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override
	{
		Super::GetLifetimeReplicatedProps(OutLifetimeProps);

		DOREPLIFETIME(ARadiationSensor, RadiationAmount)
	}
	
	UPROPERTY(Replicated)
	double RadiationAmount;
};
