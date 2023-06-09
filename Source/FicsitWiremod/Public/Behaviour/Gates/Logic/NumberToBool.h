// 

#pragma once

#include "CoreMinimal.h"
#include "Behaviour/FGWiremodBuildable.h"
#include "NumberToBool.generated.h"

UCLASS()
class FICSITWIREMOD_API ANumberToBool : public AFGWiremodBuildable
{
	GENERATED_BODY()

public:
	virtual void Process_Implementation(float DeltaTime) override
	{
		Out = GetConnection(0).GetFloat() >= 1;
	}

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override
	{
		Super::GetLifetimeReplicatedProps(OutLifetimeProps);

		DOREPLIFETIME(ANumberToBool, Out)
	}
	
	UPROPERTY(Replicated, SaveGame)
	bool Out;
};
