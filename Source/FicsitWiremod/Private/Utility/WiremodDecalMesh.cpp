// 


#include "Utility/WiremodDecalMesh.h"

#include "WiremodUtils.h"
#include "Kismet/KismetMaterialLibrary.h"
#include "Utility/WiremodGameWorldModule.h"


// Sets default values for this component's properties
UWiremodDecalMesh::UWiremodDecalMesh() : Super()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;
	
	SetRelativeLocation(FVector(0, 0, 8.5));
	SetRelativeScale3D(FVector(0.35, 0.35, 0.35));
	SetStaticMesh(UWiremodGameWorldModule::GateDecalMesh);
	SetMaterial(0, UWiremodGameWorldModule::GateDecalMaterial);
	
	// ...
}

void UWiremodDecalMesh::BeginPlay()
{
	Super::BeginPlay();
	auto Buildable = Cast<AFGBuildable>(this->GetAttachmentRootActor());
	if(!Buildable) return;
	
	auto Texture = UWiremodUtils::GetBuildableTexture(Buildable);
	if(!Texture || Texture == UWiremodGameWorldModule::DecalDefaultTexture) this->DestroyComponent();
	else
	{
		auto DynamicMaterial = UKismetMaterialLibrary::CreateDynamicMaterialInstance(GetWorld(), this->GetMaterial(0));
		DynamicMaterial->SetTextureParameterValue(FName("Texture"), Texture);
		this->SetMaterial(0, DynamicMaterial);
	}
}

