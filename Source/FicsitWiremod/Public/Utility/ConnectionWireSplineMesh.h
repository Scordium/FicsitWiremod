// 

#pragma once

#include "CoreMinimal.h"
#include "Components/SplineMeshComponent.h"
#include "ConnectionWireSplineMesh.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class FICSITWIREMOD_API UConnectionWireSplineMesh : public USplineMeshComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UConnectionWireSplineMesh();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
};
