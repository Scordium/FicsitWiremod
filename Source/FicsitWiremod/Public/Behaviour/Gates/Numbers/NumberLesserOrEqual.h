// 

#pragma once

#include "CoreMinimal.h"
#include "Behaviour/FGWiremodBuildable.h"
#include "NumberLesserOrEqual.generated.h"

UCLASS()
class FICSITWIREMOD_API ANumberLesserOrEqual : public AFGWiremodBuildable
{
	GENERATED_BODY()

public:
	virtual void Process_Implementation(double DeltaTime) override
	{
		Out = GetConnection(0).GetFloat() <= GetConnection(1).GetFloat();
	}

	virtual void GetLifetimeReplicatedProps( TArray<FLifetimeProperty>& OutLifetimeProps ) const override
	{
		Super::GetLifetimeReplicatedProps(OutLifetimeProps);

		DOREPLIFETIME(ANumberLesserOrEqual, Out);
	}


	UPROPERTY(Replicated, VisibleInstanceOnly)
	bool Out;
};
