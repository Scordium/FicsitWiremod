// 

#pragma once

#include "CoreMinimal.h"
#include "Behaviour/FGWiremodBuildable.h"
#include "SRLatch.generated.h"

UCLASS()
class FICSITWIREMOD_API ASRLatch : public AFGWiremodBuildable
{
	GENERATED_BODY()

public:
	virtual void ServerProcess_Implementation(double DeltaTime) override
	{
		auto S = GetConnection(0).GetBool();
		auto R = GetConnection(1).GetBool();
		auto IsInvalidState = !Out && !OutInverted;
		
		if(S & R) Out = OutInverted = false;
		else if (S)
		{
			Out = true;
			OutInverted = false;
		}
		else if (R || IsInvalidState)
		{
			Out = false;
			OutInverted = true;
		}
	}


	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override
	{
		Super::GetLifetimeReplicatedProps(OutLifetimeProps);

		DOREPLIFETIME(ASRLatch, Out)
		DOREPLIFETIME(ASRLatch, OutInverted)
	}


	UPROPERTY(Replicated, SaveGame)
	bool Out;

	UPROPERTY(Replicated, SaveGame)
	bool OutInverted;
};


UCLASS()
class FICSITWIREMOD_API ASRLatchWithEnable : public AFGWiremodBuildable
{
	GENERATED_BODY()

public:
	virtual void ServerProcess_Implementation(double DeltaTime) override
	{
		if(!GetConnection(2).GetBool()) return;
		
		auto S = GetConnection(0).GetBool();
		auto R = GetConnection(1).GetBool();
		auto IsInvalidState = !Out && !OutInverted;
		
		if(S & R) Out = OutInverted = false;
		else if (S)
		{
			Out = true;
			OutInverted = false;
		}
		else if (R || IsInvalidState)
		{
			Out = false;
			OutInverted = true;
		}
	}


	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override
	{
		Super::GetLifetimeReplicatedProps(OutLifetimeProps);

		DOREPLIFETIME(ASRLatchWithEnable, Out)
		DOREPLIFETIME(ASRLatchWithEnable, OutInverted)
	}


	UPROPERTY(Replicated, SaveGame)
	bool Out;

	UPROPERTY(Replicated, SaveGame)
	bool OutInverted;
};
