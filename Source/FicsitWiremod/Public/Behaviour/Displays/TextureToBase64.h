#pragma once

#include "Behaviour/FGWiremodBuildable.h"
#include "Engine/TextureRenderTarget2D.h"
#include "TextureToBase64.generated.h"

UCLASS()
class ATextureToBase64 : public AFGWiremodBuildable
{
	GENERATED_BODY()

public:
	virtual void ServerProcess_Implementation(double DeltaTime) override
	{
		if (!GetConnection(1).GetBool())
		{
			HasProcessed = false;
			return;
		}

		if (HasProcessed) return;
		
		auto Texture = Cast<UTexture2D>(GetConnection(0).GetTexture());
		if (!Texture) return;

		HasProcessed = true;

		Out = UTextureUtilities::EncodeTextureAsBase64(Texture);
	}

	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override
	{
		Super::GetLifetimeReplicatedProps(OutLifetimeProps);

		DOREPLIFETIME(ThisClass, Out)
	}

	UPROPERTY(Replicated, SaveGame)
	FString Out;

	UPROPERTY()
	bool HasProcessed;
};
