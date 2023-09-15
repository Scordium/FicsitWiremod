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
	virtual void ServerProcess_Implementation(double DeltaTime) override
	{
		Out = UKismetMathLibrary::RadiansToDegrees(GetConnection(0).GetFloat());
	}
        
	virtual void GetLifetimeReplicatedProps( TArray<FLifetimeProperty>& OutLifetimeProps ) const override
	{
		Super::GetLifetimeReplicatedProps(OutLifetimeProps);
        
		DOREPLIFETIME(ARadiansToDegrees, Out);
	}
	
	UPROPERTY(Replicated, VisibleInstanceOnly)
	double Out;
};
