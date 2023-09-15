// 

#pragma once

#include "CoreMinimal.h"
#include "Behaviour/FGWiremodBuildable.h"
#include "MakeColor.generated.h"

UCLASS()
class FICSITWIREMOD_API AMakeColor : public AFGWiremodBuildable
{
	GENERATED_BODY()

public:
	virtual void ServerProcess_Implementation(double DeltaTime) override
	{
		auto H = GetConnection(0).GetFloat();
		auto S = GetConnection(1).GetFloat();
		auto V = GetConnection(2).GetFloat();
		auto A = GetConnection(3).GetFloat(1);
		
		Out = FLinearColor(H, S, V, A).HSVToLinearRGB();
	}

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override
	{
		Super::GetLifetimeReplicatedProps(OutLifetimeProps);

		DOREPLIFETIME(AMakeColor, Out)
	}

	UPROPERTY(Replicated, SaveGame)
	FLinearColor Out;
};

UCLASS()
class FICSITWIREMOD_API AMakeColorRGBA : public AFGWiremodBuildable
{
	GENERATED_BODY()

public:
	virtual void ServerProcess_Implementation(double DeltaTime) override
	{
		Out.R = GetConnection(0).GetFloat();
		Out.G = GetConnection(1).GetFloat();
		Out.B = GetConnection(2).GetFloat();
		Out.A = GetConnection(3).GetFloat(1);
	}

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override
	{
		Super::GetLifetimeReplicatedProps(OutLifetimeProps);

		DOREPLIFETIME(AMakeColorRGBA, Out)
	}

	UPROPERTY(Replicated, SaveGame)
	FLinearColor Out;
};
