// 

#pragma once

#include "CoreMinimal.h"
#include "Behaviour/FGWiremodBuildable.h"
#include "VectorToColor.generated.h"

UCLASS()
class FICSITWIREMOD_API AVectorToColor : public AFGWiremodBuildable
{
	GENERATED_BODY()

public:
	virtual void ServerProcess_Implementation(double DeltaTime) override
	{
		auto Vector = GetConnection(0).GetVector();
		auto Alpha = GetConnection(1).GetFloat();

		Out = FLinearColor(Vector);
		Out.A = Alpha;
	}

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override
	{
		Super::GetLifetimeReplicatedProps(OutLifetimeProps);

		DOREPLIFETIME(AVectorToColor, Out)
	}

	UPROPERTY(Replicated, SaveGame)
	FLinearColor Out;
};
