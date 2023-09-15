// 

#pragma once

#include "CoreMinimal.h"
#include "Behaviour/FGWiremodBuildable.h"
#include "ATan_Rad.generated.h"

UCLASS()
class FICSITWIREMOD_API AATan_Rad : public AFGWiremodBuildable
{
	GENERATED_BODY()
    
public:
	virtual void ServerProcess_Implementation(double DeltaTime) override
	{
		Out = UKismetMathLibrary::Atan(GetConnection(0).GetFloat());
	}
    
	virtual void GetLifetimeReplicatedProps( TArray<FLifetimeProperty>& OutLifetimeProps ) const override
	{
		Super::GetLifetimeReplicatedProps(OutLifetimeProps);
    
		DOREPLIFETIME(AATan_Rad, Out);
	}
	
	UPROPERTY(Replicated, VisibleInstanceOnly)
	double Out;
};
