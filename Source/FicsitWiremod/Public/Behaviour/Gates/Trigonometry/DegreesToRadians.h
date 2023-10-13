// 

#pragma once

#include "CoreMinimal.h"
#include "Behaviour/FGWiremodBuildable.h"
#include "DegreesToRadians.generated.h"

UCLASS()
class FICSITWIREMOD_API ADegreesToRadians : public AFGWiremodBuildable
{
	GENERATED_BODY()
        
public:
	virtual void ServerProcess_Implementation(double DeltaTime) override
	{
		Out = UKismetMathLibrary::DegreesToRadians(GetConnection(0).GetFloat());
	}
        
	virtual void GetLifetimeReplicatedProps( TArray<FLifetimeProperty>& OutLifetimeProps ) const override
	{
		Super::GetLifetimeReplicatedProps(OutLifetimeProps);
        
		DOREPLIFETIME(ADegreesToRadians, Out);
	}
        
        
	UPROPERTY(Replicated, SaveGame, VisibleInstanceOnly)
	double Out;
};
