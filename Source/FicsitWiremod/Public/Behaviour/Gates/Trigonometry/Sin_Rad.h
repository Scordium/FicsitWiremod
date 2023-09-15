// 

#pragma once

#include "CoreMinimal.h"
#include "Behaviour/FGWiremodBuildable.h"
#include "Sin_Rad.generated.h"

UCLASS()
class FICSITWIREMOD_API ASin_Rad : public AFGWiremodBuildable
{
	GENERATED_BODY()
        
public:
	virtual void ServerProcess_Implementation(double DeltaTime) override
	{
		Out = UKismetMathLibrary::Sin(GetConnection(0).GetFloat());
	}
        
	virtual void GetLifetimeReplicatedProps( TArray<FLifetimeProperty>& OutLifetimeProps ) const override
	{
		Super::GetLifetimeReplicatedProps(OutLifetimeProps);
        
		DOREPLIFETIME(ASin_Rad, Out);
	}
	
	UPROPERTY(Replicated, VisibleInstanceOnly)
	double Out;
};
