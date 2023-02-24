// 

#pragma once

#include "CoreMinimal.h"
#include "Behaviour/FGWiremodBuildable.h"
#include "NumberMax.generated.h"

UCLASS()
class FICSITWIREMOD_API ANumberMax : public AFGWiremodBuildable
{
	GENERATED_BODY()

public:
	virtual void Process_Implementation(float DeltaTime) override
	{
		Out = FMath::Max(WM_GetFloat(0), WM_GetFloat(1));
	}

	virtual void GetLifetimeReplicatedProps( TArray<FLifetimeProperty>& OutLifetimeProps ) const override
	{
		Super::GetLifetimeReplicatedProps(OutLifetimeProps);

		DOREPLIFETIME(ANumberMax, Out);
	}


	UPROPERTY(Replicated, VisibleInstanceOnly)
	float Out;
};
