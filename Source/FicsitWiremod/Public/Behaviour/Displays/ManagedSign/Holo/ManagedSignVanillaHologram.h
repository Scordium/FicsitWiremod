// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Buildables/FGBuildableWidgetSign.h"
#include "Hologram/FGBuildableHologram.h"
#include "ManagedSignVanillaHologram.generated.h"


UCLASS()
class FICSITWIREMOD_API AManagedSignVanillaHologram : public AFGBuildableHologram
{
	GENERATED_BODY()

public:
	AManagedSignVanillaHologram()
	{
		mNeedsValidFloor = false;
	}
	
	virtual void BeginPlay() override;

	virtual void SetHologramLocationAndRotation(const FHitResult& HitResult) override;

	virtual bool TryUpgrade(const FHitResult& HitResult) override;

	virtual bool IsValidHitResult(const FHitResult& HitResult) const override { return HitResult.GetActor() != nullptr; }

	virtual bool IsValidHitActor(AActor* HitActor) const override { return HitActor != nullptr; }
	
	virtual int32 GetRotationStep() const override { return 5; }

	virtual AActor* GetUpgradedActor() const override { return UpgradedActor; }


	UPROPERTY() FVector2D SizeSelf = FVector2D::ZeroVector;
	UPROPERTY() double OffsetSelf = 0;
	UPROPERTY() TSubclassOf<AFGBuildableWidgetSign> VanillaVersion;

	UPROPERTY() AFGBuildableWidgetSign* UpgradedActor = nullptr;
};
