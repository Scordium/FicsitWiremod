// 

#pragma once

#include "CoreMinimal.h"
#include "Behaviour/FGWiremodBuildable.h"
#include "Pixel.generated.h"


UCLASS()
class FICSITWIREMOD_API ABuildablePixelBase : public AFGWiremodBuildable
{
	GENERATED_BODY()

public:

	UPROPERTY(Replicated, BlueprintReadOnly, SaveGame)
	FVector Scale = FVector::OneVector;

	virtual void BeginPlay() override
	{
		Super::BeginPlay();

		SetActorScale3D(Scale);
	}

	virtual void ClientProcess_Implementation(double DeltaTime) override final
	{
		auto NewColor = GetColor();
		auto NewEmission = GetEmission();

		if(Color != NewColor || Emission != NewEmission)
		{
			Color = NewColor;
			Emission = NewEmission;
			UpdateColorAndEmission();
		}
	}

	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override
	{
		Super::GetLifetimeReplicatedProps(OutLifetimeProps);

		DOREPLIFETIME(ThisClass, Scale)
	}

	UFUNCTION(BlueprintImplementableEvent)
	void UpdateColorAndEmission();
	
	UPROPERTY(BlueprintReadOnly)
	FLinearColor Color;

	UPROPERTY(BlueprintReadOnly)
	double Emission;

protected:

	virtual FLinearColor GetColor(){ return FLinearColor::Black; }
	virtual double GetEmission() { return 0; }
};

UCLASS()
class FICSITWIREMOD_API APixel : public ABuildablePixelBase
{
	GENERATED_BODY()

public:
	virtual FLinearColor GetColor() override { return GetConnection(0).GetColor(); }
	virtual double GetEmission() override { return GetConnection(1).GetFloat(1); }
};

UCLASS()
class FICSITWIREMOD_API ABoolPixel : public ABuildablePixelBase
{
	GENERATED_BODY()

public:

	virtual FLinearColor GetColor() override
	{
		return GetConnection(0).GetBool()
		? GetConnection(3).GetColor(FLinearColor::Green)
		: GetConnection(2).GetColor(FLinearColor::Red);
	}
	virtual double GetEmission() override { return GetConnection(1).GetFloat(1); }
};
