﻿// 

#pragma once

#include "CoreMinimal.h"
#include "Behaviour/FGWiremodBuildable.h"
#include "NumberPower.generated.h"

UCLASS()
class FICSITWIREMOD_API ANumberPower : public AFGWiremodBuildable
{
	GENERATED_BODY()

public:
	virtual void ServerProcess_Implementation(double DeltaTime) override
	{
		Out = FMath::Pow(GetConnection(0).GetFloat(), GetConnection(1).GetFloat());
	}

	virtual void GetLifetimeReplicatedProps( TArray<FLifetimeProperty>& OutLifetimeProps ) const override
	{
		Super::GetLifetimeReplicatedProps(OutLifetimeProps);

		DOREPLIFETIME(ANumberPower, Out);
	}
	
	UPROPERTY(Replicated, SaveGame, VisibleInstanceOnly)
	double Out;
};
