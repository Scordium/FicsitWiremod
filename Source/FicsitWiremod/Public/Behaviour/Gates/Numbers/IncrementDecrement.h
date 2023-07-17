// 

#pragma once

#include "CoreMinimal.h"
#include "Behaviour/FGWiremodBuildable.h"
#include "IncrementDecrement.generated.h"

#define DF_DO_INCREMENT GetConnection(0).GetBool()
#define DF_DO_DECREMENT GetConnection(1).GetBool()
#define DF_DO_RESET_VALUE GetConnection(2).GetBool()
#define DF_DEFAULT_VALUE GetConnection(3).GetFloat(0.f)
#define DF_INCREMENT_VALUE GetConnection(4).GetFloat(1.f)
#define DF_DECREMENT_VALUE GetConnection(5).GetFloat(1.f)

UCLASS()
class FICSITWIREMOD_API AIncrementDecrement : public AFGWiremodBuildable
{
	GENERATED_BODY()

public:
	virtual void Process_Implementation(double DeltaTime) override
	{
		if(DF_DO_RESET_VALUE) Out = DF_DEFAULT_VALUE;
		else
		{
			if(DF_DO_INCREMENT) Out += DF_INCREMENT_VALUE;
			if(DF_DO_DECREMENT) Out -= DF_DECREMENT_VALUE;	
		}
	}

	virtual void GetLifetimeReplicatedProps( TArray<FLifetimeProperty>& OutLifetimeProps ) const override
	{
		Super::GetLifetimeReplicatedProps(OutLifetimeProps);

		DOREPLIFETIME(AIncrementDecrement, Out);
	}


	UPROPERTY(Replicated, VisibleInstanceOnly, SaveGame)
	double Out;
};
