// 

#pragma once

#include "CoreMinimal.h"
#include "FGBuildableHologram.h"

#include "CircuitryBreadboardHologram.generated.h"


UENUM(Blueprintable, BlueprintType)
enum ECircuitryBreadboardHologramPlaceStep
{
	BBPS_Build,
	BBPS_Scale
};

UCLASS()
class FICSITWIREMOD_API ACircuitryBreadboardHologram : public AFGBuildableHologram
{
	GENERATED_BODY()

public:
	
	void BeginPlay();
	
	void ConfigureActor(AFGBuildable* inBuildable) const;
	
	void SetHologramLocationAndRotation(const FHitResult& HitResult);

	virtual bool IsValidHitActor(AActor* hitActor) const override { return hitActor != nullptr; }

	virtual bool IsValidHitResult(const FHitResult& hitResult) const override { return hitResult.GetActor() != nullptr; }

	virtual bool DoMultiStepPlacement(bool isInputFromARelease) override
	{
		if(CurrentStep == BBPS_Build)
		{
			CurrentStep = BBPS_Scale;
			return false;
		}
		else return true;
	}

	virtual bool CanTakeNextBuildStep() const override { return true; }

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	float SizeStep;

	UPROPERTY(BlueprintReadOnly)
	TEnumAsByte<ECircuitryBreadboardHologramPlaceStep> CurrentStep;

	UPROPERTY(BlueprintReadOnly)
	FVector2D Size;
	
private:

	FQuat CalculateRotation(const FVector& ImpactNormal) const
	{
		return FQuat::FindBetweenNormals(FVector::UpVector, ImpactNormal);
	}
	UPROPERTY() FVector StartPosition;
	UPROPERTY() FVector StartImpactNormal;
};
