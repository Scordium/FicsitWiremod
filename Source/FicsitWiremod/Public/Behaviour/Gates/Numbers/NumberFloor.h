// 

#pragma once

#include "CoreMinimal.h"
#include "Behaviour/FGWiremodBuildable.h"
#include "NumberFloor.generated.h"

UCLASS()
class FICSITWIREMOD_API ANumberFloor : public AFGWiremodBuildable
{
	GENERATED_BODY()

public:
	virtual void Process_Implementation(float DeltaTime) override
	{
		Out = FMath::FloorToInt(WM_GetFloat(0));
	}

	virtual void GetLifetimeReplicatedProps( TArray<FLifetimeProperty>& OutLifetimeProps ) const override
	{
		Super::GetLifetimeReplicatedProps(OutLifetimeProps);

		DOREPLIFETIME(ANumberFloor, Out);
	}
	
	UPROPERTY(Replicated, VisibleInstanceOnly)
	int Out;
};
