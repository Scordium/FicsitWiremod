// 

#pragma once

#include "CoreMinimal.h"
#include "Behaviour/FGWiremodBuildable.h"
#include "ACos_Rad.generated.h"

UCLASS()
class FICSITWIREMOD_API AACos_Rad : public AFGWiremodBuildable
{
	GENERATED_BODY()

public:
	virtual void Process_Implementation(float DeltaTime) override
	{
		Out = UKismetMathLibrary::Acos(WM_GetFloat(0));
	}

	virtual void GetLifetimeReplicatedProps( TArray<FLifetimeProperty>& OutLifetimeProps ) const override
	{
		Super::GetLifetimeReplicatedProps(OutLifetimeProps);

		DOREPLIFETIME(AACos_Rad, Out);
	}


	UPROPERTY(Replicated, VisibleInstanceOnly)
	float Out;
};
