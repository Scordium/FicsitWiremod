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
	virtual void ServerProcess_Implementation(double DeltaTime) override
	{
		Out = FMath::FRandRange(GetConnection(0).GetFloat(), GetConnection(1).GetFloat(1));
	}
    
	virtual void GetLifetimeReplicatedProps( TArray<FLifetimeProperty>& OutLifetimeProps ) const override
	{
		Super::GetLifetimeReplicatedProps(OutLifetimeProps);
    
		DOREPLIFETIME(ARandomNumber, Out);
	}
    	
	UPROPERTY(Replicated, SaveGame, VisibleInstanceOnly)
	double Out;
};
