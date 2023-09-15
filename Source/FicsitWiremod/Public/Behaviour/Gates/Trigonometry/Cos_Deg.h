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
	virtual void ServerProcess_Implementation(double DeltaTime) override
	{
		Out = UKismetMathLibrary::DegCos(GetConnection(0).GetFloat());
	}
        
	virtual void GetLifetimeReplicatedProps( TArray<FLifetimeProperty>& OutLifetimeProps ) const override
	{
		Super::GetLifetimeReplicatedProps(OutLifetimeProps);
        
		DOREPLIFETIME(ACos_Deg, Out);
	}
        
        
	UPROPERTY(Replicated, VisibleInstanceOnly)
	double Out;
};
