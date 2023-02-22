// 

#pragma once

#include "CoreMinimal.h"
#include "Behaviour/FGWiremodBuildable.h"
#include "RadiansToDegrees.generated.h"

UCLASS()
class FICSITWIREMOD_API ARadiansToDegrees : public AFGWiremodBuildable
{
	GENERATED_BODY()
        
public:
	virtual void Process_Implementation(float DeltaTime) override
	{
		Out = UKismetMathLibrary::RadiansToDegrees(WM_GetFloat(0));
	}
        
	virtual void GetLifetimeReplicatedProps( TArray<FLifetimeProperty>& OutLifetimeProps ) const override
	{
		Super::GetLifetimeReplicatedProps(OutLifetimeProps);
        
		DOREPLIFETIME(ARadiansToDegrees, Out);
	}
	
	UPROPERTY(Replicated, VisibleInstanceOnly)
	float Out;
};
