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
	virtual void Process_Implementation(float DeltaTime) override
	{
		auto Image = GetConnection(0).GetPixelImage();
		auto Scale = GetConnection(1).GetVector(FVector(1)) / 5; // Dividing by 5 to make the widget smaller 
		auto Offset = GetConnection(2).GetVector();
		ApplySettings(Scale, Offset);

		if(Image == Cache) return;

		Cache = Image;
		TextureCache = Image.MakeTexture();
		SetTexture(TextureCache);
	}

	UFUNCTION(BlueprintImplementableEvent)
	void SetTexture(UTexture2D* Texture);

	UFUNCTION(BlueprintImplementableEvent)
	void ApplySettings(const FVector& Scale, const FVector& Offset);

	UPROPERTY()
	FPixelScreenData Cache;

	UPROPERTY()
	UTexture2D* TextureCache;
};
