// 

#pragma once

#include "CoreMinimal.h"
#include "Behaviour/FGWiremodBuildable.h"
#include "NormalizeNumber.generated.h"

UCLASS()
class FICSITWIREMOD_API ANormalizeNumber : public AFGWiremodBuildable
{
	GENERATED_BODY()

public:
	virtual void Process_Implementation(float DeltaTime) override
	{
		Out = UKismetMathLibrary::NormalizeToRange(WM_GetFloat(0), WM_GetFloat(1), WM_GetFloat(2));
	}

	virtual void GetLifetimeReplicatedProps( TArray<FLifetimeProperty>& OutLifetimeProps ) const override
	{
		Super::GetLifetimeReplicatedProps(OutLifetimeProps);

		DOREPLIFETIME(ANormalizeNumber, Out);
	}
	
	UPROPERTY(Replicated, VisibleInstanceOnly)
	float Out;
};
