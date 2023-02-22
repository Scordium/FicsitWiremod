// 

#pragma once

#include "CoreMinimal.h"
#include "ConnectionWireBase.h"
#include "Module/GameWorldModule.h"
#include "WiremodGameWorldModule.generated.h"

/**
 * 
 */
UCLASS(BlueprintType)
class FICSITWIREMOD_API UWiremodGameWorldModule : public UGameWorldModule
{
	GENERATED_BODY()
	
public:

	
	static inline UStaticMesh* WiremodGateMesh = LoadObject<UStaticMesh>(NULL, *FString("/FicsitWiremod/Assets/Models/WiremodChipBase.WiremodChipBase"));
	static inline UStaticMesh* GateDecalMesh = LoadObject<UStaticMesh>(NULL, *FString("/Engine/BasicShapes/Plane.Plane"));
	static inline UMaterial* GateDecalMaterial = LoadObject<UMaterial>(NULL, *FString("/FicsitWiremod/Assets/Materials/WiremodBuildableDecalMaterial.WiremodBuildableDecalMaterial"));
	static inline UTexture* DecalDefaultTexture = LoadObject<UTexture>(NULL, *FString("/FicsitWiremod/Assets/Icons/WiremodIcon.WiremodIcon"));
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<AConnectionWireBase> VisibleWireClass;
};
