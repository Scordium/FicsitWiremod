// 

#pragma once

#include "CoreMinimal.h"
#include "Behaviour/FGWiremodBuildable.h"
#include "NumberToTime.generated.h"

UCLASS()
class FICSITWIREMOD_API ANumberToTime : public AFGWiremodBuildable
{
	GENERATED_BODY()

public:
	virtual void ServerProcess_Implementation(double DeltaTime) override
	{
		Out = UFGBlueprintFunctionLibrary::SecondsToTimeString(GetConnection(0).GetFloat());
	}


	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override
	{
		Super::GetLifetimeReplicatedProps(OutLifetimeProps);

		DOREPLIFETIME(ANumberToTime, Out)
	}

	UPROPERTY(Replicated, SaveGame)
	FString Out;
};
