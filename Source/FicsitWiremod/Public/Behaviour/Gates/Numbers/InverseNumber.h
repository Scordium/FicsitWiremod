// 

#pragma once

#include "CoreMinimal.h"
#include "Behaviour/FGWiremodBuildable.h"
#include "InverseNumber.generated.h"

UCLASS()
class FICSITWIREMOD_API AInverseNumber : public AFGWiremodBuildable
{
	GENERATED_BODY()
    
public:
	virtual void ServerProcess_Implementation(double DeltaTime) override
	{
		Out = 1.0 / GetConnection(0).GetFloat();;
	}
    
	virtual void GetLifetimeReplicatedProps( TArray<FLifetimeProperty>& OutLifetimeProps ) const override
	{
		Super::GetLifetimeReplicatedProps(OutLifetimeProps);
    
		DOREPLIFETIME(AInverseNumber, Out);
	}
	
	UPROPERTY(Replicated, VisibleInstanceOnly)
	double Out;
};
