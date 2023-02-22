// 

#pragma once

#include "CoreMinimal.h"
#include "Behaviour/FGWiremodBuildable.h"
#include "DegreesToRadians.generated.h"

UCLASS()
class FICSITWIREMOD_API ADegreesToRadians : public AFGWiremodBuildable
{
	GENERATED_BODY()
        
public:
	virtual void Process_Implementation(float DeltaTime) override
	{
		Out = UKismetMathLibrary::DegreesToRadians(WM_GetFloat(0));
	}
        
	virtual void GetLifetimeReplicatedProps( TArray<FLifetimeProperty>& OutLifetimeProps ) const override
	{
		Super::GetLifetimeReplicatedProps(OutLifetimeProps);
        
		DOREPLIFETIME(ADegreesToRadians, Out);
	}
        
        
	UPROPERTY(Replicated, VisibleInstanceOnly)
	float Out;
};
