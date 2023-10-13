// 

#pragma once

#include "CoreMinimal.h"
#include "Behaviour/FGWiremodBuildable.h"
#include "NumberIsInRange.generated.h"

UCLASS()
class FICSITWIREMOD_API ANumberIsInRange : public AFGWiremodBuildable
{
	GENERATED_BODY()

public:
	virtual void ServerProcess_Implementation(double DeltaTime) override
    	{
    		Out = UKismetMathLibrary::InRange_FloatFloat(
    			GetConnection(0).GetFloat(),
    			GetConnection(1).GetFloat(),
    			GetConnection(2).GetFloat(),
    			GetConnection(4).GetBool(),
    			GetConnection(5).GetBool()
    			);
    	}
    
    	virtual void GetLifetimeReplicatedProps( TArray<FLifetimeProperty>& OutLifetimeProps ) const override
    	{
    		Super::GetLifetimeReplicatedProps(OutLifetimeProps);
    
    		DOREPLIFETIME(ANumberIsInRange, Out);
    	}
    
    
    	UPROPERTY(Replicated, SaveGame, VisibleInstanceOnly)
    	bool Out;
};
