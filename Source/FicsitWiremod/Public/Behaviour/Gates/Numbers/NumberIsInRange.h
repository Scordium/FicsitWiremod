// 

#pragma once

#include "CoreMinimal.h"
#include "Behaviour/FGWiremodBuildable.h"
#include "NumberIsInRange.generated.h"

UCLASS()
class FICSITWIREMOD_API ANumberIsInRange : public AFGWiremodBuildable
{
	GENERATED_BODY()

public:
	virtual void Process_Implementation(float DeltaTime) override
    	{
    		Out = UKismetMathLibrary::InRange_FloatFloat(
    			WM_GetFloat(0),
    			WM_GetFloat(1),
    			WM_GetFloat(2),
    			WM_GetBool(4),
    			WM_GetBool(5)
    			);
    	}
    
    	virtual void GetLifetimeReplicatedProps( TArray<FLifetimeProperty>& OutLifetimeProps ) const override
    	{
    		Super::GetLifetimeReplicatedProps(OutLifetimeProps);
    
    		DOREPLIFETIME(ANumberIsInRange, Out);
    	}
    
    
    	UPROPERTY(Replicated, VisibleInstanceOnly)
    	bool Out;
};
