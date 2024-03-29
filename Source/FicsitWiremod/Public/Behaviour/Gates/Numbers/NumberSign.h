﻿// 

#pragma once

#include "CoreMinimal.h"
#include "Behaviour/FGWiremodBuildable.h"
#include "NumberSign.generated.h"

UCLASS()
class FICSITWIREMOD_API ANumberSign : public AFGWiremodBuildable
{
	GENERATED_BODY()

public:
	virtual void ServerProcess_Implementation(double DeltaTime) override
	{
		Out = FMath::Sign(GetConnection(0).GetFloat());
	}

	virtual void GetLifetimeReplicatedProps( TArray<FLifetimeProperty>& OutLifetimeProps ) const override
	{
		Super::GetLifetimeReplicatedProps(OutLifetimeProps);

		DOREPLIFETIME(ANumberSign, Out);
	}
	
	UPROPERTY(Replicated, SaveGame, VisibleInstanceOnly)
	double Out;
};
