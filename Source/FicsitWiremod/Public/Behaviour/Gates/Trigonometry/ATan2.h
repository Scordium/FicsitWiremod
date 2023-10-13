// 

#pragma once

#include "CoreMinimal.h"
#include "Behaviour/FGWiremodBuildable.h"
#include "Behaviour/MultistateWiremodBuildable.h"
#include "ATan2.generated.h"

UCLASS()
class FICSITWIREMOD_API AATan2 : public AMultistateWiremodBuildable
{
	GENERATED_BODY()

public:
	virtual void ServerProcess_Implementation(double DeltaTime) override
	{
		auto const Y = GetConnection(0).GetFloat();
		auto const X = GetConnection(1).GetFloat();

		// 0 - Degrees
		if(CurrentStateIndex == 0) Out = UKismetMathLibrary::DegAtan2(Y, X);
		// 1 - Radians
		else Out = UKismetMathLibrary::Atan2(Y, X);
	}

	virtual void GetLifetimeReplicatedProps( TArray<FLifetimeProperty>& OutLifetimeProps ) const override
	{
		Super::GetLifetimeReplicatedProps(OutLifetimeProps);

		DOREPLIFETIME(AATan2, Out);
	}


	UPROPERTY(Replicated, VisibleInstanceOnly)
	double Out;
};
