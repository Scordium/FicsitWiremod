// 

#pragma once

#include "CoreMinimal.h"
#include "Behaviour/FGWiremodBuildable.h"
#include "HexToColor.generated.h"

UCLASS()
class FICSITWIREMOD_API AHexToColor : public AFGWiremodBuildable
{
	GENERATED_BODY()

public:
	virtual void ServerProcess_Implementation(double DeltaTime) override
	{
		Out = UFGBlueprintFunctionLibrary::HexToLinearColor(GetConnection(0).GetString());
	}


	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override
	{
		Super::GetLifetimeReplicatedProps(OutLifetimeProps);

		DOREPLIFETIME(AHexToColor, Out)
	}


	UPROPERTY(Replicated, SaveGame)
	FLinearColor Out;
};
