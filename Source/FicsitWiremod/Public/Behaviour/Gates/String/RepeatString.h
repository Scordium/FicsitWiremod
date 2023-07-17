// 

#pragma once

#include "CoreMinimal.h"
#include "Behaviour/FGWiremodBuildable.h"
#include "RepeatString.generated.h"

UCLASS()
class FICSITWIREMOD_API ARepeatString : public AFGWiremodBuildable
{
	GENERATED_BODY()

public:
	virtual void Process_Implementation(double DeltaTime) override
	{
		FString String = GetConnection(0).GetString();
		int RepeatCount = GetConnection(1).GetFloat();

		Out = "";
		for(; RepeatCount > 0; RepeatCount--) Out += String;
	}


	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override
	{
		Super::GetLifetimeReplicatedProps(OutLifetimeProps);

		DOREPLIFETIME(ARepeatString, Out)
	}

	UPROPERTY(Replicated, SaveGame)
	FString Out;
};
