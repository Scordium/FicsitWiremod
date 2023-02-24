// 

#pragma once

#include "CoreMinimal.h"
#include "Behaviour/FGWiremodBuildable.h"
#include "ATan_Deg.generated.h"

UCLASS()
class FICSITWIREMOD_API AATan_Deg : public AFGWiremodBuildable
{
	GENERATED_BODY()
        
    public:
    	virtual void Process_Implementation(float DeltaTime) override
    	{
    		Out = UKismetMathLibrary::DegAtan(WM_GetFloat(0));
    	}
        
    	virtual void GetLifetimeReplicatedProps( TArray<FLifetimeProperty>& OutLifetimeProps ) const override
    	{
    		Super::GetLifetimeReplicatedProps(OutLifetimeProps);
        
    		DOREPLIFETIME(AATan_Deg, Out);
    	}
        
        
    	UPROPERTY(Replicated, VisibleInstanceOnly)
    	float Out;
};
