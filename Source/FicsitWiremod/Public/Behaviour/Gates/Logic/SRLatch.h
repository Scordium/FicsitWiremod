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
	virtual void Process_Implementation(float DeltaTime) override
	{
		auto S = WM_GetBool(0);
		auto R = WM_GetBool(1);

		if(!(Out && OutInverted))
		{
			Out = false;
			OutInverted = true;
		}
		
		if(S & R) Out = OutInverted = false;
		else if (S)
		{
			Out = true;
			OutInverted = false;
		}
		else if (R)
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
	virtual void Process_Implementation(float DeltaTime) override
	{
		if(!WM_GetBool(2)) return;
		
		auto S = WM_GetBool(0);
		auto R = WM_GetBool(1);

		if(!(Out && OutInverted))
		{
			Out = false;
			OutInverted = true;
		}
		
		if(S & R) Out = OutInverted = false;
		else if (S)
		{
			Out = true;
			OutInverted = false;
		}
		else if (R)
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
