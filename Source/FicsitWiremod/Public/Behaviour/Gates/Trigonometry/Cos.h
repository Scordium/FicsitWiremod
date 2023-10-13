// 

#pragma once

#include "CoreMinimal.h"
#include "Behaviour/FGWiremodBuildable.h"
#include "Behaviour/MultistateWiremodBuildable.h"
#include "Cos.generated.h"

UCLASS()
class FICSITWIREMOD_API ACos : public AMultistateWiremodBuildable
{
	GENERATED_BODY()
        
public:
	virtual void ServerProcess_Implementation(double DeltaTime) override
	{
		// 0 - Degrees
		if(CurrentStateIndex == 0) Out = UKismetMathLibrary::DegCos(GetConnection(0).GetFloat());
		// 1 - Radians
		else Out = UKismetMathLibrary::Cos(GetConnection(0).GetFloat());
	}
        
	virtual void GetLifetimeReplicatedProps( TArray<FLifetimeProperty>& OutLifetimeProps ) const override
	{
		Super::GetLifetimeReplicatedProps(OutLifetimeProps);
        
		DOREPLIFETIME(ACos, Out);
	}
        
        
	UPROPERTY(Replicated, SaveGame, VisibleInstanceOnly)
	double Out;
};
