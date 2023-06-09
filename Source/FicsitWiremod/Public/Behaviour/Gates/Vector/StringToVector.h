// 

#pragma once

#include "CoreMinimal.h"
#include "Behaviour/FGWiremodBuildable.h"
#include "StringToVector.generated.h"

UCLASS()
class FICSITWIREMOD_API AStringToVector : public AFGWiremodBuildable
{
	GENERATED_BODY()

public:
	virtual void Process_Implementation(float DeltaTime) override
	{
		Success = Out.InitFromString(GetConnection(0).GetString());
	}

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override
	{
		Super::GetLifetimeReplicatedProps(OutLifetimeProps);

		DOREPLIFETIME(AStringToVector, Out)
		DOREPLIFETIME(AStringToVector, Success)
	}


	UPROPERTY(Replicated, SaveGame)
	FVector Out;

	UPROPERTY(Replicated, SaveGame)
	bool Success;
};
