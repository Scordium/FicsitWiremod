// 

#pragma once

#include "CoreMinimal.h"
#include "Behaviour/FGWiremodBuildable.h"
#include "AbsoluteNumber.generated.h"

UCLASS()
class FICSITWIREMOD_API AAbsoluteNumber : public AFGWiremodBuildable
{
	GENERATED_BODY()

public:
	virtual void Process_Implementation(float DeltaTime) override
	{
		Out = FMath::Abs(WM_GetFloat(0));
	}

	virtual void GetLifetimeReplicatedProps( TArray<FLifetimeProperty>& OutLifetimeProps ) const override
	{
		Super::GetLifetimeReplicatedProps(OutLifetimeProps);

		DOREPLIFETIME(AAbsoluteNumber, Out);
	}
	
	UPROPERTY(Replicated, VisibleInstanceOnly)
	float Out;
};
