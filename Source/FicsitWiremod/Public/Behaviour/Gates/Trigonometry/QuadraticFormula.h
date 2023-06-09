// 

#pragma once

#include "CoreMinimal.h"
#include "Behaviour/FGWiremodBuildable.h"
#include "QuadraticFormula.generated.h"

UCLASS()
class FICSITWIREMOD_API AQuadraticFormula : public AFGWiremodBuildable
{
	GENERATED_BODY()

public:
	virtual void GetLifetimeReplicatedProps( TArray<FLifetimeProperty>& OutLifetimeProps ) const override
	{
		Super::GetLifetimeReplicatedProps(OutLifetimeProps);
        
		DOREPLIFETIME(AQuadraticFormula, OutPos);
		DOREPLIFETIME(AQuadraticFormula, OutNeg);
	}

	virtual void Process_Implementation(float DeltaTime) override
	{
		float a = GetConnection(0).GetFloat();;
		float b = GetConnection(1).GetFloat();;
		float c = GetConnection(2).GetFloat();;

		if(a == 0 || b == 0 || c == 0) return;

		float D = b*b - 4*a*c;
		if(D == 0) return;

		OutPos = (-b + sqrtf(D)) / (2*a);
		OutNeg = (-b - sqrtf(D)) / (2*a);
	}


	UPROPERTY(Replicated, VisibleInstanceOnly)
	float OutPos;

	UPROPERTY(Replicated, VisibleInstanceOnly)
	float OutNeg;
};
