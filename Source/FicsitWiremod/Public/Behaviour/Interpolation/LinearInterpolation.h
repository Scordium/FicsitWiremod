// 

#pragma once

#include "CoreMinimal.h"
#include "Behaviour/FGWiremodBuildable.h"
#include "LinearInterpolation.generated.h"

UCLASS()
class FICSITWIREMOD_API ALinearInterpolation : public AFGWiremodBuildable
{
	GENERATED_BODY()

public:
	virtual void Process_Implementation(double DeltaTime) override
	{
		auto A = GetConnection(0).GetFloat();
		auto B = GetConnection(1).GetFloat();
		auto Alpha = GetConnection(2).GetFloat();
		
		Out = FMath::Lerp(A, B, Alpha);
	}
	
	virtual void GetLifetimeReplicatedProps( TArray<FLifetimeProperty>& OutLifetimeProps ) const override
	{
		Super::GetLifetimeReplicatedProps(OutLifetimeProps);
		DOREPLIFETIME(ALinearInterpolation, Out);
	}

	UPROPERTY(Replicated, VisibleInstanceOnly)
	double Out;
};
