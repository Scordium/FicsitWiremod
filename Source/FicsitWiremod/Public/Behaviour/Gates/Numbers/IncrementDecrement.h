// 

#pragma once

#include "CoreMinimal.h"
#include "Behaviour/FGWiremodBuildable.h"
#include "IncrementDecrement.generated.h"

#define DF_DO_INCREMENT WM_GetBool(0)
#define DF_DO_DECREMENT WM_GetBool(1)
#define DF_DO_RESET_VALUE WM_GetBool(2)
#define DF_DEFAULT_VALUE WM_GetFloat(3, 0.f)
#define DF_INCREMENT_VALUE WM_GetFloat(4, 1.f)
#define DF_DECREMENT_VALUE WM_GetFloat(5, 1.f)

UCLASS()
class FICSITWIREMOD_API AIncrementDecrement : public AFGWiremodBuildable
{
	GENERATED_BODY()

public:
	virtual void Process_Implementation(float DeltaTime) override
	{
		if(DF_DO_RESET_VALUE) Out = DF_DEFAULT_VALUE;
		else if(DF_DO_INCREMENT) Out += DF_INCREMENT_VALUE;
		else if(DF_DO_DECREMENT) Out -= DF_DECREMENT_VALUE;
	}

	virtual void GetLifetimeReplicatedProps( TArray<FLifetimeProperty>& OutLifetimeProps ) const override
	{
		Super::GetLifetimeReplicatedProps(OutLifetimeProps);

		DOREPLIFETIME(AIncrementDecrement, Out);
	}


	UPROPERTY(Replicated, VisibleInstanceOnly)
	float Out;
};
