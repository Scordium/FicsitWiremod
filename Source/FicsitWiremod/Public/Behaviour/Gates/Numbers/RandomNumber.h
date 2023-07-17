// 

#pragma once

#include "CoreMinimal.h"
#include "Behaviour/FGWiremodBuildable.h"
#include "RandomNumber.generated.h"

UCLASS()
class FICSITWIREMOD_API ARandomNumber : public AFGWiremodBuildable
{
	GENERATED_BODY()
    
public:
	virtual void Process_Implementation(double DeltaTime) override
	{
		Out = FMath::FRandRange(GetConnection(0).GetFloat(), GetConnection(1).GetFloat());
	}
    
	virtual void GetLifetimeReplicatedProps( TArray<FLifetimeProperty>& OutLifetimeProps ) const override
	{
		Super::GetLifetimeReplicatedProps(OutLifetimeProps);
    
		DOREPLIFETIME(ARandomNumber, Out);
	}
    	
	UPROPERTY(Replicated, VisibleInstanceOnly)
	double Out;
};
