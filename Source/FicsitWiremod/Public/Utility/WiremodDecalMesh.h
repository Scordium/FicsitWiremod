// 

#pragma once

#include "CoreMinimal.h"
#include "WiremodGameWorldModule.h"
#include "WiremodUtils.h"
#include "Behaviour/CircuitryStatics.h"
#include "Buildables/FGBuildable.h"
#include "Components/StaticMeshComponent.h"
#include "Kismet/KismetMaterialLibrary.h"
#include "WiremodDecalMesh.generated.h"


UCLASS(ClassGroup=(Rendering, Common), EditInlineNew, Blueprintable, BlueprintType, meta=(BlueprintSpawnableComponent))
class FICSITWIREMOD_API UWiremodDecalMesh : public UStaticMeshComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UWiremodDecalMesh();

	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable)
	void UpdateTexture()
	{
		const auto Buildable = Cast<AFGBuildable>(this->GetAttachmentRootActor());
		if(!Buildable) return;

		const auto Texture = UWiremodUtils::GetTexture(Buildable);
		if(!Texture || Texture == UCircuitryStatics::GetDefaultDecalTexture()) SetTexture(nullptr);
		else SetTexture(Texture);
	}

	void SetTexture(UTexture2D* Texture)
	{
		auto Material = GetDynamicMaterial();
		if(!Texture)
		{
			Material->SetScalarParameterValue(FName("OpacityMultiplier"), 0);
		}
		else
		{
			Material->SetTextureParameterValue(FName("Texture"), Texture);
			Material->SetScalarParameterValue(FName("OpacityMultiplier"), 1);
		}
		this->SetMaterial(0, Material);
	}

	UMaterialInstanceDynamic* GetDynamicMaterial()
	{
		auto Material = this->GetMaterial(0);
		if(auto DynamicInstance = Cast<UMaterialInstanceDynamic>(Material)) return DynamicInstance;
		else return UKismetMaterialLibrary::CreateDynamicMaterialInstance(GetWorld(), Material);
	}
	
};
