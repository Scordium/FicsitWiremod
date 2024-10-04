// 

#pragma once

#include "CoreMinimal.h"
#include "FGGameInstance.h"
#include "FGGlobalSettings.h"
#include "FGIconLibrary.h"
#include "FGInventoryComponent.h"
#include "WiremodUtils.h"
#include "AssetRegistry/AssetRegistryModule.h"
#include "Components/Image.h"
#include "Engine/DataTable.h"
#include "Engine/Texture2DDynamic.h"
#include "Engine/TextureRenderTarget2D.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Resources/FGItemDescriptor.h"
#include "Slate/WidgetRenderer.h"
#include "TextureUtilities.generated.h"

UENUM(Blueprintable, BlueprintType)
enum ETextureAssetSource
{
	Engine,
	Game,
	Mod,
	Material,
	Other
};

UENUM(Blueprintable, BlueprintType)
enum ETextureAssetCategory
{
	TAC_Item UMETA(DisplayName="Item"),
	TAC_Buildable UMETA(DisplayName="Buildable icon"),
	TAC_Interface UMETA(DisplayName="UI"),
	TAC_Rendering UMETA(DisplayName="Rendering/Material texture"),
	TAC_Other UMETA(DisplayName="Other"),
	TAC_Customizer UMETA(DisplayName="Customizer"),
	TAC_Fauna UMETA(DisplayName="Fauna")
};

UENUM(Blueprintable, BlueprintType)
enum ETextureResolutionLevel
{
	TRL_Unset,
	TRL_Low,
	TRL_High
};

USTRUCT()
struct FTextureAssetMetadata : public FTableRowBase
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	bool Ignored;
	
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	FText ReadableName;
	
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	TEnumAsByte<ETextureAssetSource> Source;
	
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	TEnumAsByte<ETextureAssetCategory> Category;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
	TEnumAsByte<ETextureResolutionLevel> ResolutionLevel = TRL_Unset;
};

USTRUCT(Blueprintable, BlueprintType)
struct FTextureAssetData
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly)
	FString OwnerPlugin;

	UPROPERTY(BlueprintReadOnly)
	FSoftObjectPath Texture;

	UPROPERTY(BlueprintReadWrite)
	FText ReadableName = FText::FromString("N/A");

	UPROPERTY(BlueprintReadWrite)
	TEnumAsByte<ETextureAssetSource> AssetSource = Other;
	
	UPROPERTY(BlueprintReadWrite)
	TEnumAsByte<ETextureAssetCategory> Category = TAC_Other;

	UPROPERTY(BlueprintReadWrite)
	TEnumAsByte<ETextureResolutionLevel> ResolutionLevel = TRL_Unset;

	FTextureAssetData(){}

	FTextureAssetData(const FString& Owner, const FSoftObjectPath& TextureAsset) : OwnerPlugin(Owner), Texture(TextureAsset)
	{
		ReadableName = FText::FromName(TextureAsset.GetAssetFName());
	}

	void WithMetadata(const FTextureAssetMetadata& Metadata)
	{
		if(!Metadata.ReadableName.IsEmpty()) ReadableName = Metadata.ReadableName;
		AssetSource = Metadata.Source;
		Category = Metadata.Category;
		ResolutionLevel = Metadata.ResolutionLevel;
	}
};
/**
 * 
 */
UCLASS()
class FICSITWIREMOD_API UTextureUtilities : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable)
	static TArray<FTextureAssetData> GetAllTextureAssets(bool IncludeEngineContent = false, bool IncludeMaterialData = false, bool IncludeModContent = true)
	{
		IAssetRegistry* AssetRegistry = IAssetRegistry::Get();
		TArray<FTextureAssetData> Out;

		if(AssetRegistry)
		{
			TArray<FAssetData> AssetsData;
			AssetRegistry->GetAssetsByClass(FTopLevelAssetPath("/Script/Engine.Texture2D"), AssetsData);
		
			for(auto& AssetData : AssetsData)
			{
				const auto AssetPath = AssetData.GetSoftObjectPath();
				auto AssetOwner = UWiremodUtils::GetModReferenceFromObjectPath(AssetPath);
				auto OutputData = FTextureAssetData(AssetOwner, AssetData.GetSoftObjectPath());
				
				//Check for engine content
				bool IsEngineContent = EngineAssetPaths.Contains(AssetOwner);
				if(IsEngineContent)
				{
					if(!IncludeEngineContent) continue;
					OutputData.AssetSource = Engine;
				}
				//Check for modded content
				else if(AssetOwner != "Game")
				{
					if(!IncludeModContent) continue;
					OutputData.AssetSource = Mod;
				}
				else if(AssetOwner == "Game")
				{
					//Check for normals, albedo, reflection, etc. textures
					//In other words textures used for materials rather than UI and stuff
					bool IsMaterialTexture = AssetPath.GetAssetPathString().Contains("/Textures") || AssetPath.GetAssetPathString().Contains("/Texture");
					if(IsMaterialTexture)
					{
						if(!IncludeMaterialData) continue;
						OutputData.AssetSource = Material;
					}
					else OutputData.AssetSource = Game;
				}
				
				Out.Add(OutputData);
			}
		}
		return Out;
	}

	UFUNCTION(BlueprintPure)
	static FVector2D GetTextureSize(UTexture* Texture)
	{
		if(!Texture) return FVector2D::ZeroVector;
		return FVector2D(Texture->GetSurfaceWidth(), Texture->GetSurfaceHeight());
	}

	UFUNCTION(BlueprintCallable)
	static void SetImageBrushUniversal(UImage* Image, UTexture* Texture, bool MatchSize = true)
	{
		if(!Image || !Texture) return;

		if(auto Tex2D = Cast<UTexture2D>(Texture))
			Image->SetBrushFromTexture(Tex2D, MatchSize);
		else if(auto Tex2DDynamic = Cast<UTexture2DDynamic>(Texture))
			Image->SetBrushFromTextureDynamic(Tex2DDynamic);
	}

	UFUNCTION(BlueprintCallable)
	static UTexture2D* CreateTexture(const FVector2D& Size) { return UTexture2D::CreateTransient(Size.X, Size.Y); }

	static inline TArray<FString> EngineAssetPaths =
	{
		"Engine",
		"FortniteMain",
		"Wwise",
		"SpeedTreeImporter",
		"Paper2D",
		"Niagara",
		"Interchange"
	};
};
