#include "Behaviour/Miscellaneous/CircuitryMapMarker.h"



void UFGCircutryMapMarkerRepresentation::UpdateRepresentationCompassMaterial(class UMaterialInstanceDynamic* compassMaterialInstance, APlayerController* ownerPlayerController) const
{
	auto Actor = Cast<ACircuitryMapMarker>(mRealActor);
	if (!Actor) return;
	
	compassMaterialInstance->SetTextureParameterValue(FName("Icon"), Actor->TextureCache);
	//compassMaterialInstance->SetTextureParameterValue(FName("Border"), Actor->TextureCache);
	//compassMaterialInstance->SetTextureParameterValue(FName("InnerBackground"), Actor->TextureCache);
	if (Actor->TextureCache) ACircuitryLogger::DispatchWarningEvent(Actor->TextureCache->GetName());
}
