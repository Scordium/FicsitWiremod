// 

#pragma once

#include "CoreMinimal.h"
#include "Behaviour/FGWiremodBuildable.h"
#include "NumberAverage_Array.generated.h"

UCLASS()
class FICSITWIREMOD_API ANumberAverage_Array : public AFGWiremodBuildable
{
	GENERATED_BODY()

public:
	virtual void Process_Implementation(float DeltaTime) override
	{
		auto Array = GetConnection(0).GetFloatArray();
		float Result = 0;

		for(float Element : Array)
			Result += Element;

		Out = Result / Array.Num();
	}

	virtual void GetLifetimeReplicatedProps( TArray<FLifetimeProperty>& OutLifetimeProps ) const override
	{
		Super::GetLifetimeReplicatedProps(OutLifetimeProps);

		DOREPLIFETIME(ANumberAverage_Array, Out);
	}
	
	UPROPERTY(Replicated, VisibleInstanceOnly)
	float Out;
};
