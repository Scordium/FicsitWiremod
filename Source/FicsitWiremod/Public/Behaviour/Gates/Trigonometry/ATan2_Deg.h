// 

#pragma once

#include "CoreMinimal.h"
#include "Behaviour/FGWiremodBuildable.h"
#include "ATan2_Deg.generated.h"

UCLASS()
class FICSITWIREMOD_API AATan2_Deg : public AFGWiremodBuildable
{
	GENERATED_BODY()

public:
	virtual void Process_Implementation(double DeltaTime) override
	{
		Out = UKismetMathLibrary::DegAtan2(GetConnection(0).GetFloat(), GetConnection(1).GetFloat());
	}

	virtual void GetLifetimeReplicatedProps( TArray<FLifetimeProperty>& OutLifetimeProps ) const override
	{
		Super::GetLifetimeReplicatedProps(OutLifetimeProps);

		DOREPLIFETIME(AATan2_Deg, Out);
	}


	UPROPERTY(Replicated, VisibleInstanceOnly)
	double Out;
};
