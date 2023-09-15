// 

#pragma once

#include "CoreMinimal.h"
#include "Behaviour/FGWiremodBuildable.h"
#include "ColorToHex.generated.h"

UCLASS()
class FICSITWIREMOD_API AColorToHex : public AFGWiremodBuildable
{
	GENERATED_BODY()

public:
	virtual void ServerProcess_Implementation(double DeltaTime) override
	{
		Out = UFGBlueprintFunctionLibrary::LinearColorToHex(GetConnection(0).GetColor());
	}


	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override
	{
		Super::GetLifetimeReplicatedProps(OutLifetimeProps);

		DOREPLIFETIME(AColorToHex, Out)
	}

	UPROPERTY(Replicated, SaveGame)
	FString Out;
};
