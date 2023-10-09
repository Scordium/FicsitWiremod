// 

#pragma once

#include "CoreMinimal.h"
#include "FGIconLibrary.h"
#include "FGInventoryComponent.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Resources/FGItemDescriptor.h"
#include "TextureUtilities.generated.h"

/**
 * 
 */
UCLASS()
class FICSITWIREMOD_API UTextureUtilities : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintPure)
	static UTexture2D* GetTextureFromIconId(int Id)
	{
		if(!IconToTextureMap.Contains(Id))
		{
			auto IconLib = UFGIconLibrary::Get();
			auto Texture = IconLib->GetTextureFromIconID(Id);
			
			TextureToIconMap.Add(Texture, Id);
			IconToTextureMap.Add(Id, Texture);
		}

		return IconToTextureMap[Id].Get();
	}

	UFUNCTION(BlueprintPure)
	static int32 GetIconFromTexture(UTexture2D* Texture)
	{
		if(!Texture) return -1;

		if(!TextureToIconMap.Contains(Texture))
		{
			auto IconLib = UFGIconLibrary::Get();
			auto Id = IconLib->GetIconIDForTexture(Texture);
			
			TextureToIconMap.Add(Texture, Id);
			IconToTextureMap.Add(Id, Texture);
		}

		return TextureToIconMap[Texture];
	}

	UFUNCTION(BlueprintCallable)
	static int32 GetIconFromDescriptor(TSubclassOf<UFGItemDescriptor> ItemDescriptor)
	{
		if(!ItemDescriptor) return -1;
		return GetIconFromTexture(UFGItemDescriptor::GetBigIcon(ItemDescriptor));
	}

	UFUNCTION(BlueprintCallable)
	static int32 GetIconFromStack(FInventoryStack Stack)
	{
		return GetIconFromDescriptor(Stack.Item.GetItemClass());
	}

	UFUNCTION(BlueprintCallable)
	static int32 GetIconFromItemAmount(FItemAmount ItemAmount)
	{
		return GetIconFromDescriptor(ItemAmount.ItemClass);
	}

	UFUNCTION(BlueprintPure)
	static FVector2D GetTextureSize(UTexture* Texture)
	{
		if(!Texture) return FVector2D::ZeroVector;
		return FVector2D(Texture->GetSurfaceWidth(), Texture->GetSurfaceHeight());
	}


	static inline TMap<TSoftObjectPtr<UTexture2D>, int32> TextureToIconMap;
	static inline TMap<int32, TSoftObjectPtr<UTexture2D>> IconToTextureMap;
};
