// 

#pragma once

#include "CoreMinimal.h"
#include "Behaviour/FGWiremodBuildable.h"
#include "NumberEquals.generated.h"

UCLASS()
class FICSITWIREMOD_API ANumberEquals : public AFGWiremodBuildable
{
	GENERATED_BODY()

public:
	virtual void Process_Implementation(float DeltaTime) override
	{
		Out = WM_GetFloat(0) == WM_GetFloat(1);
	}

	virtual void GetLifetimeReplicatedProps( TArray<FLifetimeProperty>& OutLifetimeProps ) const override
	{
		Super::GetLifetimeReplicatedProps(OutLifetimeProps);

		DOREPLIFETIME(ANumberEquals, Out);
	}


	UPROPERTY(Replicated, VisibleInstanceOnly)
	bool Out;
};
