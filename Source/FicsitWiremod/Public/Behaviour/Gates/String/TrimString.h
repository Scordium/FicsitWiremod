// 

#pragma once

#include "CoreMinimal.h"
#include "Behaviour/FGWiremodBuildable.h"
#include "TrimString.generated.h"

UCLASS()
class FICSITWIREMOD_API ATrimString : public AFGWiremodBuildable
{
	GENERATED_BODY()

public:
	virtual void ServerProcess_Implementation(double DeltaTime) override
	{
		Out = GetConnection(0).GetString();
		Out.TrimStartAndEndInline();
	}
	
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override
	{
		Super::GetLifetimeReplicatedProps(OutLifetimeProps);

		DOREPLIFETIME(ATrimString, Out)
	}

	UPROPERTY(Replicated, SaveGame)
	FString Out;
};
