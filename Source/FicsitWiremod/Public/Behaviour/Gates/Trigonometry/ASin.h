// 

#pragma once

#include "CoreMinimal.h"
#include "Behaviour/FGWiremodBuildable.h"
#include "Behaviour/MultistateWiremodBuildable.h"
#include "ASin.generated.h"

UCLASS()
class FICSITWIREMOD_API AASin : public AMultistateWiremodBuildable
{
	GENERATED_BODY()

public:
	virtual void ServerProcess_Implementation(double DeltaTime) override
	{
		// 0 - Degrees
		if(CurrentStateIndex == 0) Out = Out = UKismetMathLibrary::DegAsin(GetConnection(0).GetFloat());
		// 1 - Radians
		else Out = UKismetMathLibrary::Acos(GetConnection(0).GetFloat());
	}

	virtual void GetLifetimeReplicatedProps( TArray<FLifetimeProperty>& OutLifetimeProps ) const override
	{
		Super::GetLifetimeReplicatedProps(OutLifetimeProps);

		DOREPLIFETIME(AASin, Out);
	}
	
	UPROPERTY(Replicated, VisibleInstanceOnly)
	double Out;
};
