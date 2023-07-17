// 

#pragma once

#include "CoreMinimal.h"
#include "Behaviour/FGWiremodBuildable.h"
#include "VectorEquals.generated.h"

UCLASS()
class FICSITWIREMOD_API AVectorEquals : public AFGWiremodBuildable
{
	GENERATED_BODY()

public:
	virtual void Process_Implementation(double DeltaTime) override
	{
		auto V1 = GetConnection(0).GetVector();
		auto V2 = GetConnection(1).GetVector();
		const auto Tolerance = GetConnection(2).GetFloat();

		Out = V1.Equals(V2, Tolerance);
	}

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override
	{
		Super::GetLifetimeReplicatedProps(OutLifetimeProps);

		DOREPLIFETIME(AVectorEquals, Out)
	}

	UPROPERTY(Replicated, SaveGame)
	bool Out;
};
