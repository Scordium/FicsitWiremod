// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FGActorRepresentationManager.h"
#include "FGMapMarkerRepresentation.h"
#include "Behaviour/FGWiremodBuildable.h"
#include "Engine/Texture2DDynamic.h"
#include "CircuitryMapMarker.generated.h"

UCLASS()
class FICSITWIREMOD_API ACircuitryMapMarker : public AFGWiremodBuildable, public IFGActorRepresentationInterface
{
	GENERATED_BODY()

public:
	virtual void BeginPlay() override
	{
		Super::BeginPlay();
		AddAsRepresentation();
	}

	virtual void ServerProcess_Implementation(double DeltaTime) override
	{
		UpdateRepresentation();
	}

	virtual void ClientProcess_Implementation(double DeltaTime) override
	{
		auto TextureValue = GetConnection(2).GetTexture();
		
		if(TextureValue && TextureCache)
		{
			if(TextureCache == TextureValue) return;
			if(TextureCache->GetResource() == TextureValue->GetResource()) return;
		
			if(auto Texture = Cast<UTexture2D>(TextureValue))
				TextureCache = Texture;
			else if(auto TextureDynamic = Cast<UTexture2DDynamic>(TextureValue))
			{
				if(TextureCache == GetTexture()) TextureCache = UTexture2D::CreateTransient(TextureDynamic->SizeX, TextureDynamic->SizeY);
				TextureCache->SetResource(TextureDynamic->GetResource());
			}
		}
		else TextureCache = GetTexture();
	}

	UFUNCTION()
	virtual bool AddAsRepresentation() override
	{
		auto Manager = AFGActorRepresentationManager::Get(this);
		if(!Manager) return false;

		Representation = Manager->CreateAndAddNewRepresentation(this, false, UFGMapMarkerRepresentation::StaticClass());
		return true;
	}
	
	UFUNCTION()
	virtual bool UpdateRepresentation() override
	{
		auto Manager = AFGActorRepresentationManager::Get(this);
		if(!Manager) return false;

		Manager->UpdateRepresentation(Representation);
		
		return true;
	}
	
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
	virtual FVector GetRealActorLocation() override { return GetConnection(0).GetVector(GetActorLocation()); }
	UFUNCTION()
	virtual FRotator GetRealActorRotation() override { return GetConnection(1).GetVector(GetActorRotation().Vector()).Rotation(); }
	UFUNCTION()
	virtual class UTexture2D* GetActorRepresentationTexture() override { return TextureCache; }
	UFUNCTION()
	virtual FText GetActorRepresentationText() override { return FText::FromString(GetConnection(3).GetString("Circuitry Beacon")); }
	UFUNCTION()
	virtual void SetActorRepresentationText( const FText& newText ) override {};
	UFUNCTION()
	virtual FLinearColor GetActorRepresentationColor() override { return GetConnection(4).GetColor(); }
	UFUNCTION()
	virtual void SetActorRepresentationColor( FLinearColor newColor ) override {}
	UFUNCTION()
	virtual ERepresentationType GetActorRepresentationType() override { return (ERepresentationType)(int)GetConnection(5).GetFloat(); }
	UFUNCTION()
	virtual bool GetActorShouldShowInCompass() override { return GetConnection(6).GetBool(true); }
	UFUNCTION()
	virtual bool GetActorShouldShowOnMap() override { return GetConnection(7).GetBool(true); }
	UFUNCTION()
	virtual EFogOfWarRevealType GetActorFogOfWarRevealType() override { return EFogOfWarRevealType::FOWRT_None; }
	UFUNCTION()
	virtual float GetActorFogOfWarRevealRadius() override { return 10; }
	UFUNCTION()
	virtual ECompassViewDistance GetActorCompassViewDistance() override { return (ECompassViewDistance)GetConnection(8).GetFloat(4); }
	UFUNCTION()
	virtual void SetActorCompassViewDistance( ECompassViewDistance compassViewDistance ) override {}

	UPROPERTY()
	UTexture2D* TextureCache;

	UPROPERTY()
	bool UsesTransient;

	UPROPERTY()
	UFGActorRepresentation* Representation;
};
