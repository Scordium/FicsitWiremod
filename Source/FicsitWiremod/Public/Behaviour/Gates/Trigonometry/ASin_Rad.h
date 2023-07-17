// 

#pragma once

#include "CoreMinimal.h"
#include "Behaviour/FGWiremodBuildable.h"
#include "ASin_Rad.generated.h"

UCLASS()
class FICSITWIREMOD_API AASin_Rad : public AFGWiremodBuildable
{
	GENERATED_BODY()
    
public:
	virtual void Process_Implementation(double DeltaTime) override
	{
		Out = UKismetMathLibrary::Asin(GetConnection(0).GetFloat());
	}
    
	virtual void GetLifetimeReplicatedProps( TArray<FLifetimeProperty>& OutLifetimeProps ) const override
	{
		Super::GetLifetimeReplicatedProps(OutLifetimeProps);
    
		DOREPLIFETIME(AASin_Rad, Out);
	}
    
    
	UPROPERTY(Replicated, VisibleInstanceOnly)
	double Out;
};
