// 

#pragma once

#include "CoreMinimal.h"
#include "Components/StaticMeshComponent.h"
#include "WiremodDecalMesh.generated.h"


UCLASS(ClassGroup=(Rendering, Common), EditInlineNew, Blueprintable, BlueprintType, meta=(BlueprintSpawnableComponent))
class FICSITWIREMOD_API UWiremodDecalMesh : public UStaticMeshComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UWiremodDecalMesh();

	virtual void BeginPlay() override;
};
