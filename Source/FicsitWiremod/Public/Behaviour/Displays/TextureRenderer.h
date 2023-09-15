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
		auto Image = GetConnection(0).GetPixelImage();
		auto Scale = GetConnection(1).GetVector(FVector(1)) / 5; // Dividing by 5 to make the widget smaller 
		auto Offset = GetConnection(2).GetVector();
		auto Rotation = GetConnection(3).GetVector() + FVector(-90, 90, 0); //Offset to the widget component's "front facing" rotation.
		ApplySettings(Scale, Offset, Rotation);

		if(Image == Cache) return;

		Cache = Image;
		TextureCache = Image.MakeTexture();
		SetTexture(TextureCache);
	}

	UFUNCTION(BlueprintImplementableEvent)
	void SetTexture(UTexture2D* Texture);

	UFUNCTION(BlueprintImplementableEvent)
	void ApplySettings(const FVector& Scale, const FVector& Offset, const FVector& Rotation);

	UPROPERTY()
	FPixelScreenData Cache;

	UPROPERTY()
	UTexture2D* TextureCache;
};
