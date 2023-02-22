// 

#pragma once

#include "CoreMinimal.h"
#include "Behaviour/FGWiremodBuildable.h"
#include "Cos_Deg.generated.h"

UCLASS()
class FICSITWIREMOD_API ACos_Deg : public AFGWiremodBuildable
{
	GENERATED_BODY()
        
public:
	virtual void Process_Implementation(float DeltaTime) override
	{
		Out = UKismetMathLibrary::DegCos(WM_GetFloat(0));
	}
        
	virtual void GetLifetimeReplicatedProps( TArray<FLifetimeProperty>& OutLifetimeProps ) const override
	{
		Super::GetLifetimeReplicatedProps(OutLifetimeProps);
        
		DOREPLIFETIME(ACos_Deg, Out);
	}
        
        
	UPROPERTY(Replicated, VisibleInstanceOnly)
	float Out;
};
