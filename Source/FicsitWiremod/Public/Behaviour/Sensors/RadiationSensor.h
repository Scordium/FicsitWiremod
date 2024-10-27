// 

#pragma once

#include "CoreMinimal.h"
#include "FGRadiationInterface.h"
#include "FGRadioactivitySubsystem.h"
#include "FGWiremodBuildable.h"
#include "RadiationSensor.generated.h"

UCLASS()
class FICSITWIREMOD_API ARadiationSensor : public AFGWiremodBuildable, public IFGRadiationInterface
{
	GENERATED_BODY()

	virtual void BeginPlay() override
	{
		Super::BeginPlay();

		if (HasAuthority())
		{
			if (auto GameState = Cast<AFGGameState>(UGameplayStatics::GetGameState(this)))
			{
				auto RadiationSystem = GameState->GetRadioactivitySubsystem();
				if (RadiationSystem) RadiationSystem->mPotentiallyAffectedActors.AddUnique(this);
			}
		} 
	}
	
	virtual void ReceiveRadiation_Implementation(float amount, float duration, FVector direction, TSubclassOf<UFGDamageType> damageType) override
	{
		RadiationAmount = amount > 0.0047 ? amount : 0;
	}

	virtual bool CanReceiveRadiation_Implementation() const override { return true; }

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override
	{
		Super::GetLifetimeReplicatedProps(OutLifetimeProps);

		DOREPLIFETIME(ARadiationSensor, RadiationAmount);
	}
	
	UPROPERTY(Replicated, SaveGame)
	double RadiationAmount;
};
