// 

#pragma once

#include "CoreMinimal.h"
#include "Behaviour/FGWiremodBuildable.h"
#include "Pixel.generated.h"

UCLASS()
class FICSITWIREMOD_API APixel : public AFGWiremodBuildable
{
	GENERATED_BODY()

public:

	virtual void Process_Implementation(float DeltaTime) override
	{
		const auto Color = GetConnection(0).GetColor();
		const auto Emission = GetConnection(1).GetFloat();

		if(Color != ColorCache || Emission != EmissionCache)
		{
			ColorCache = Color;
			EmissionCache = Emission;

			SetColorAndEmission(Color, Emission);
		}
	}

	UFUNCTION(BlueprintImplementableEvent)
	void SetColorAndEmission(FLinearColor Color, float Emission);
	
	UPROPERTY()
	FLinearColor ColorCache;

	UPROPERTY()
	float EmissionCache;
};

UCLASS()
class FICSITWIREMOD_API ABoolPixel : public AFGWiremodBuildable
{
	GENERATED_BODY()

public:

	virtual void Process_Implementation(float DeltaTime) override
	{
		auto Value = GetConnection(0).GetBool();
		auto Color = Value
		? GetConnection(3).GetColor(FLinearColor::Green)
		: GetConnection(2).GetColor(FLinearColor::Red);
		
		const auto Emission = GetConnection(1).GetFloat(1);

		if(Color != ColorCache || Emission != EmissionCache)
		{
			ColorCache = Color;
			EmissionCache = Emission;

			SetColorAndEmission(Color, Emission);
		}
	}

	UFUNCTION(BlueprintImplementableEvent)
	void SetColorAndEmission(FLinearColor Color, float Emission);
	
	UPROPERTY()
	FLinearColor ColorCache;

	UPROPERTY()
	float EmissionCache;
};
