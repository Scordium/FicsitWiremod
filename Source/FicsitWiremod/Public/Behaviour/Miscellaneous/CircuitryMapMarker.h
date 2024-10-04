#pragma once

#include "CoreMinimal.h"
#include "FactoryGame.h"
#include "FGActorRepresentationManager.h"
#include "FGActorRepresentationInterface.h"
#include "FGActorRepresentation.h"
#include "../FGWiremodBuildable.h"
#include "../../../../../../../Source/FactoryGame/Public/FGActorRepresentationInterface.h"
#include "../../Utility/CircuitryLogger.h"
#include "Behaviour/FGWiremodBuildable.h"
#include "Engine/Texture2DDynamic.h"
#include "Runtime/CoreUObject/Public/UObject/UObjectGlobals.h"
#include "Runtime/Engine/Classes/Engine/Texture2D.h"
#include "Runtime/Engine/Classes/Engine/Texture2DDynamic.h"
#include "Runtime/Engine/Classes/GameFramework/PlayerController.h"
#include "Runtime/Engine/Classes/Kismet/KismetMaterialLibrary.h"
#include "Runtime/Engine/Classes/Materials/MaterialInstance.h"
#include "Runtime/Engine/Classes/Materials/MaterialInstanceDynamic.h"
#include "Runtime/Engine/Classes/Materials/MaterialInterface.h"
#include "CircuitryMapMarker.generated.h"


UCLASS()
class UFGCircutryMapMarkerRepresentation : public UFGActorRepresentation
{
	GENERATED_BODY()

public:

	virtual void UpdateRepresentationCompassMaterial(class UMaterialInstanceDynamic* compassMaterialInstance, APlayerController* ownerPlayerController) const override;
};

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

	virtual void Dismantle_Implementation() override
	{
		Super::Dismantle_Implementation();
		RemoveAsRepresentation();
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

		Representation = Manager->CreateAndAddNewRepresentation(this, false, UFGCircutryMapMarkerRepresentation::StaticClass());
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
	virtual void SetActorRepresentationText( const FText& newText ) override {}
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

	virtual UMaterialInterface* GetActorRepresentationCompassMaterial() override { return BaseMaterial; }

	UPROPERTY()
	UTexture2D* TextureCache;

	UPROPERTY()
	UFGActorRepresentation* Representation;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
	UMaterialInterface* BaseMaterial = LoadObject<UMaterialInterface>(nullptr, *FString("/Script/Engine.Material'/Game/FactoryGame/Interface/UI/Minimap/IconMaterials/MM_UI_CompassIcon.MM_UI_CompassIcon'"));
};
