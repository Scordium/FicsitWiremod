// 

#pragma once

#include "CoreMinimal.h"
#include "Behaviour/FGWiremodBuildable.h"
#include "NumberMin.generated.h"

UCLASS()
class FICSITWIREMOD_API ANumberMin : public AFGWiremodBuildable
{
	GENERATED_BODY()

public:
	virtual void Process_Implementation(float DeltaTime) override
	{
		Out = FMath::Min(WM_GetFloat(0), WM_GetFloat(1));
	}

	virtual void GetLifetimeReplicatedProps( TArray<FLifetimeProperty>& OutLifetimeProps ) const override
	{
		Super::GetLifetimeReplicatedProps(OutLifetimeProps);

		DOREPLIFETIME(ANumberMin, Out);
	}


	UPROPERTY(Replicated, VisibleInstanceOnly)
	float Out;
};
