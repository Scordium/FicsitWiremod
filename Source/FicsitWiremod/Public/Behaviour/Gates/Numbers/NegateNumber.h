// 

#pragma once

#include "CoreMinimal.h"
#include "Behaviour/FGWiremodBuildable.h"
#include "NegateNumber.generated.h"

UCLASS()
class FICSITWIREMOD_API ANegateNumber : public AFGWiremodBuildable
{
	GENERATED_BODY()
    
public:
	virtual void ServerProcess_Implementation(double DeltaTime) override
	{
		Out = -GetConnection(0).GetFloat();
	}
    
	virtual void GetLifetimeReplicatedProps( TArray<FLifetimeProperty>& OutLifetimeProps ) const override
	{
		Super::GetLifetimeReplicatedProps(OutLifetimeProps);
    
		DOREPLIFETIME(ANegateNumber, Out);
	}
    
    
	UPROPERTY(Replicated, SaveGame, VisibleInstanceOnly)
	double Out;
};
