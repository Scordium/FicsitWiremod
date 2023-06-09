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
	virtual void Process_Implementation(float DeltaTime) override
	{
		float A = GetConnection(0).GetFloat();;
		float B = GetConnection(1).GetFloat();;
		float Alpha = GetConnection(2).GetFloat();;
		
		Out = FMath::Lerp(A, B, Alpha);
	}
	
	virtual void GetLifetimeReplicatedProps( TArray<FLifetimeProperty>& OutLifetimeProps ) const override
	{
		Super::GetLifetimeReplicatedProps(OutLifetimeProps);
		DOREPLIFETIME(ALinearInterpolation, Out);
	}

	UPROPERTY(Replicated, VisibleInstanceOnly)
	float Out;
};
