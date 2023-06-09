// 

#pragma once

#include "CoreMinimal.h"
#include "Behaviour/FGWiremodBuildable.h"
#include "NumberRound.generated.h"

UCLASS()
class FICSITWIREMOD_API ANumberRound : public AFGWiremodBuildable
{
	GENERATED_BODY()
    
public:
	virtual void Process_Implementation(float DeltaTime) override
	{
		Out = FMath::RoundToInt(GetConnection(0).GetFloat());
	}
    
	virtual void GetLifetimeReplicatedProps( TArray<FLifetimeProperty>& OutLifetimeProps ) const override
	{
		Super::GetLifetimeReplicatedProps(OutLifetimeProps);
    
		DOREPLIFETIME(ANumberRound, Out);
	}
    	
	UPROPERTY(Replicated, VisibleInstanceOnly)
	int Out;
};
