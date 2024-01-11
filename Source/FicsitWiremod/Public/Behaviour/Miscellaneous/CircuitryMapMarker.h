// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FGActorRepresentationManager.h"
#include "FGMapMarkerRepresentation.h"
#include "Behaviour/FGWiremodBuildable.h"
#include "CircuitryMapMarker.generated.h"

UCLASS()
class FICSITWIREMOD_API ACircuitryMapMarker : public AFGWiremodBuildable, public IFGActorRepresentationInterface
{
	GENERATED_BODY()

public:
	virtual void ServerProcess_Implementation(double DeltaTime) override
	{
		
	}

	UFUNCTION()
	virtual bool AddAsRepresentation() override
	{
		auto Manager = AFGActorRepresentationManager::Get(this);
		if(!Manager) return false;

		Manager->CreateAndAddNewRepresentation(this, false, UFGMapMarkerRepresentation::StaticClass());
		return true;
	}
	UFUNCTION()
	virtual bool UpdateRepresentation() override { return true; }
	
	UFUNCTION()
	virtual bool RemoveAsRepresentation() override
	{
		auto Manager = AFGActorRepresentationManager::Get(this);
		if(!Manager) return false;

		Manager->RemoveRepresentationOfActor(this);
		return true;
	}
	
	UFUNCTION()
	virtual bool IsActorStatic() override { return false; }
	UFUNCTION()
	virtual FVector GetRealActorLocation() override { return GetActorLocation(); }
	UFUNCTION()
	virtual FRotator GetRealActorRotation() override { return GetActorRotation(); }
	UFUNCTION()
	virtual class UTexture2D* GetActorRepresentationTexture() override { return UFGItemDescriptor::GetBigIcon(GetBuiltWithDescriptor()); }
	UFUNCTION()
	virtual FText GetActorRepresentationText() override { return FText::FromString("Test text"); }
	UFUNCTION()
	virtual void SetActorRepresentationText( const FText& newText ) override {};
	UFUNCTION()
	virtual FLinearColor GetActorRepresentationColor() override { return FLinearColor::MakeRandomColor(); }
	UFUNCTION()
	virtual void SetActorRepresentationColor( FLinearColor newColor ) override {}
	UFUNCTION()
	virtual ERepresentationType GetActorRepresentationType() override { return ERepresentationType::RT_MapMarker; }
	UFUNCTION()
	virtual bool GetActorShouldShowInCompass() override { return true; }
	UFUNCTION()
	virtual bool GetActorShouldShowOnMap() override { return true; }
	UFUNCTION()
	virtual EFogOfWarRevealType GetActorFogOfWarRevealType() override { return EFogOfWarRevealType::FOWRT_None; }
	UFUNCTION()
	virtual float GetActorFogOfWarRevealRadius() override { return 10; }
	UFUNCTION()
	virtual ECompassViewDistance GetActorCompassViewDistance() override { return ECompassViewDistance::CVD_Always; }
	UFUNCTION()
	virtual void SetActorCompassViewDistance( ECompassViewDistance compassViewDistance ) override {}
};
