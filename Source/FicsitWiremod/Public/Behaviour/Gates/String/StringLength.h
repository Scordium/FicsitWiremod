// 

#pragma once

#include "CoreMinimal.h"
#include "Behaviour/FGWiremodBuildable.h"
#include "StringLength.generated.h"

UCLASS()
class FICSITWIREMOD_API AStringLength : public AFGWiremodBuildable
{
	GENERATED_BODY()

public:

	virtual void Process_Implementation(double DeltaTime) override { Out = GetConnection(0).GetString().Len(); }
	
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override
	{
		Super::GetLifetimeReplicatedProps(OutLifetimeProps);

		DOREPLIFETIME(AStringLength, Out)
	}

	UPROPERTY(Replicated, SaveGame)
	int Out;
};
