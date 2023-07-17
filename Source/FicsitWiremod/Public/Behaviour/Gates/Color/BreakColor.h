// 

#pragma once

#include "CoreMinimal.h"
#include "Behaviour/FGWiremodBuildable.h"
#include "BreakColor.generated.h"

UCLASS()
class FICSITWIREMOD_API ABreakColor : public AFGWiremodBuildable
{
	GENERATED_BODY()

public:
	virtual void Process_Implementation(double DeltaTime) override
	{
		auto Color = GetConnection(0).GetColor().LinearRGBToHSV();
		
		Out_H = Color.R;
		Out_S = Color.G;
		Out_V = Color.B;
		Out_A = Color.A;
	}


	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override
	{
		Super::GetLifetimeReplicatedProps(OutLifetimeProps);

		DOREPLIFETIME(ABreakColor, Out_H)
		DOREPLIFETIME(ABreakColor, Out_S)
		DOREPLIFETIME(ABreakColor, Out_V)
		DOREPLIFETIME(ABreakColor, Out_A)
	}

	UPROPERTY(Replicated, SaveGame)
	double Out_H;

	UPROPERTY(Replicated, SaveGame)
	double Out_S;

	UPROPERTY(Replicated, SaveGame)
	double Out_V;

	UPROPERTY(Replicated, SaveGame)
	double Out_A;
};


UCLASS()
class FICSITWIREMOD_API ABreakColorRGBA : public AFGWiremodBuildable
{
	GENERATED_BODY()

public:
	virtual void Process_Implementation(double DeltaTime) override
	{
		auto Color = GetConnection(0).GetColor();
		
		Out_R = Color.R;
		Out_G = Color.G;
		Out_B = Color.B;
		Out_A = Color.A;
	}


	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override
	{
		Super::GetLifetimeReplicatedProps(OutLifetimeProps);

		DOREPLIFETIME(ABreakColorRGBA, Out_R)
		DOREPLIFETIME(ABreakColorRGBA, Out_G)
		DOREPLIFETIME(ABreakColorRGBA, Out_B)
		DOREPLIFETIME(ABreakColorRGBA, Out_A)
	}

	UPROPERTY(Replicated, SaveGame)
	double Out_R;

	UPROPERTY(Replicated, SaveGame)
	double Out_G;

	UPROPERTY(Replicated, SaveGame)
	double Out_B;

	UPROPERTY(Replicated, SaveGame)
	double Out_A;
};
