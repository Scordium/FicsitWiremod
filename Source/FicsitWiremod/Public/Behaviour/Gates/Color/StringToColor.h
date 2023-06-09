// 

#pragma once

#include "CoreMinimal.h"
#include "Behaviour/FGWiremodBuildable.h"
#include "StringToColor.generated.h"

UCLASS()
class FICSITWIREMOD_API AStringToColor : public AFGWiremodBuildable
{
	GENERATED_BODY()

public:
	virtual void Process_Implementation(float DeltaTime) override
	{
		Out.InitFromString(GetConnection(0).GetString());
	}

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override
	{
		Super::GetLifetimeReplicatedProps(OutLifetimeProps);

		DOREPLIFETIME(AStringToColor, Out)
	}

	UPROPERTY(Replicated, SaveGame)
	FLinearColor Out;
};
