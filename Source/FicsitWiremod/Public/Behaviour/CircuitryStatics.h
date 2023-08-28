// 

#pragma once

#include "CoreMinimal.h"
#include "FGDecorationTemplate.h"
#include "AssetRegistry/AssetRegistryModule.h"
#include "Module/GameInstanceModule.h"
#include "CircuitryStatics.generated.h"

/**
 * 
 */
UCLASS()
class FICSITWIREMOD_API UCircuitryStatics : public UGameInstanceModule
{
	GENERATED_BODY()
	
public:

	static UStaticMesh* GetGateMesh(){ return Self ? Self->GateMesh : LoadObject<UStaticMesh>(NULL, *FString("/FicsitWiremod/Assets/Models/WiremodChipBase.WiremodChipBase")); }
	static UStaticMesh* GetGateDecalMesh() { return Self ? Self->GateDecalMesh : LoadObject<UStaticMesh>(NULL, *FString("/Engine/BasicShapes/Plane.Plane")); }
	static UMaterial* GetGateDecalMaterial() { return Self ? Self->GateDecalMaterial : LoadObject<UMaterial>(NULL, *FString("/FicsitWiremod/Assets/Materials/WiremodBuildableDecalMaterial.WiremodBuildableDecalMaterial")); }
	static UTexture* GetDefaultDecalTexture() { return Self ? Self->DecalDefaultTexture : LoadObject<UTexture>(NULL, *FString("/FicsitWiremod/Assets/Icons/WiremodIcon.WiremodIcon")); }
	static TSubclassOf<AFGDecorationTemplate> GetDefaultDecoratorClass()
	{
		if(Self) return Self->Decorator;
		
		return TSubclassOf<AFGDecorationTemplate>();
	}

protected:
	inline static UCircuitryStatics* Self;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UStaticMesh* GateMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UStaticMesh* GateDecalMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UMaterial* GateDecalMaterial;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UTexture* DecalDefaultTexture;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<AFGDecorationTemplate> Decorator;
	
	UFUNCTION(BlueprintCallable)
	void SetSelf() { Self = this; }
};
