// 

#pragma once

#include "CoreMinimal.h"
#include "ImageUtils.h"
#include "Behaviour/FGWiremodBuildable.h"
#include "TextureRenderer.generated.h"

UCLASS()
class FICSITWIREMOD_API ATextureRenderer : public AFGWiremodBuildable
{
	GENERATED_BODY()

public:
	virtual void ClientProcess_Implementation(double DeltaTime) override
	{
		//Fix to prevent crashes due to the old implementation that used pixel art data instead of texture.
		auto ImageInput = GetConnection(0);
		if(ImageInput.ConnectionType.GetValue() == EConnectionType::PixelImage)
		{
			OnInputDisconnected_Internal(0);
			return;
		}
		
		auto Image = ImageInput.GetTexture();
		auto Scale = GetConnection(1).GetVector(FVector(1)) / 5; // Dividing by 5 to make the widget smaller 
		auto Offset = GetConnection(2).GetVector();
		auto Rotation = GetConnection(3).GetVector() + FVector(-90, 90, 0); //Offset to the widget component's "front facing" rotation.
		ApplySettings(Scale, Offset, Rotation);

		if(Image == ImageCache) return;
		
		ImageCache = Image;
		SetTexture(ImageCache);
	}

	UFUNCTION(BlueprintImplementableEvent)
	void SetTexture(UTexture* Texture);

	UFUNCTION(BlueprintImplementableEvent)
	void ApplySettings(const FVector& Scale, const FVector& Offset, const FVector& Rotation);

	UPROPERTY()
	UTexture* ImageCache;
};
