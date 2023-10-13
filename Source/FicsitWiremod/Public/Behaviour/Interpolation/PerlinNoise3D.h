// 

#pragma once

#include "CoreMinimal.h"
#include "KismetAnimationLibrary.h"
#include "Behaviour/FGWiremodBuildable.h"
#include "PerlinNoise3D.generated.h"

UCLASS()
class FICSITWIREMOD_API APerlinNoise3D : public AFGWiremodBuildable
{
	GENERATED_BODY()

public:
	virtual void ServerProcess_Implementation(double DeltaTime) override
	{
		auto X = GetConnection(0).GetFloat();
		auto Y = GetConnection(1).GetFloat();
		auto Z = GetConnection(2).GetFloat();
		auto MinX = GetConnection(3).GetFloat();
		auto MaxX = GetConnection(4).GetFloat();
		auto MinY = GetConnection(5).GetFloat();
		auto MaxY = GetConnection(6).GetFloat();
		auto MinZ = GetConnection(7).GetFloat();
		auto MaxZ = GetConnection(8).GetFloat();
		
		Out = UKismetAnimationLibrary::K2_MakePerlinNoiseVectorAndRemap(X, Y, Z, MinX, MaxX, MinY, MaxY, MinZ, MaxZ);
	}

	virtual void GetLifetimeReplicatedProps( TArray<FLifetimeProperty>& OutLifetimeProps ) const override
	{
		Super::GetLifetimeReplicatedProps(OutLifetimeProps);
		DOREPLIFETIME(APerlinNoise3D, Out);
	}

	UPROPERTY(VisibleInstanceOnly, SaveGame, Replicated)
	FVector Out;
};
