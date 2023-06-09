// 

#pragma once

#include "CoreMinimal.h"
#include "Behaviour/FGWiremodBuildable.h"
#include "ReverseString.generated.h"

UCLASS()
class FICSITWIREMOD_API AReverseString : public AFGWiremodBuildable
{
	GENERATED_BODY()

public:
	virtual void Process_Implementation(float DeltaTime) override
	{
		Out = GetConnection(0).GetString().Reverse();
	}

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override
	{
		Super::GetLifetimeReplicatedProps(OutLifetimeProps);

		DOREPLIFETIME(AReverseString, Out)
	}

	UPROPERTY(Replicated, SaveGame)
	FString Out;
};
