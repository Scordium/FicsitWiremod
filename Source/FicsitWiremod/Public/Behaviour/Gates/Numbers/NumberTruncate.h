// 

#pragma once

#include "CoreMinimal.h"
#include "Behaviour/FGWiremodBuildable.h"
#include "NumberTruncate.generated.h"

UCLASS()
class FICSITWIREMOD_API ANumberTruncate : public AFGWiremodBuildable
{
	GENERATED_BODY()

public:
	virtual void Process_Implementation(float DeltaTime) override
	{
		Out = WM_GetInt(0);
	}

	virtual void GetLifetimeReplicatedProps( TArray<FLifetimeProperty>& OutLifetimeProps ) const override
	{
		Super::GetLifetimeReplicatedProps(OutLifetimeProps);

		DOREPLIFETIME(ANumberTruncate, Out);
	}
	
	UPROPERTY(Replicated, VisibleInstanceOnly)
	int Out;
};
