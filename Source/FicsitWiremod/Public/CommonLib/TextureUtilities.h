// 

#pragma once

#include "CoreMinimal.h"
#include "WiremodUtils.h"
#include "AssetRegistry/AssetRegistryModule.h"
#include "Components/Image.h"
#include "Engine/DataTable.h"
#include "Engine/Texture2DDynamic.h"
#include "Kismet/BlueprintFunctionLibrary.h"
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

	UFUNCTION(BlueprintCallable)
	static FString EncodeTextureAsBase64(UTexture2D* Texture, bool HeadlessEncoding = false)
	{
		if(!Texture) return "";

		if (!HeadlessEncoding)
		{
			if (Texture->MipGenSettings != TMGS_NoMipmaps || Texture->CompressionSettings != TC_EditorIcon)
			{
				Texture->MipGenSettings = TMGS_NoMipmaps;
				Texture->CompressionSettings = TC_EditorIcon;
				Texture->Modify();
				ACircuitryLogger::DispatchErrorEvent("Resave texture " + Texture->GetName());
				return "";
			}
		}
		
		
		bool ResourceCreated = false;
		if (Texture->GetResource() == NULL)
		{
			ResourceCreated = true;
			Texture->CreateResource();
		}

		uint8* FormatedImageData = static_cast<uint8*>( Texture->GetPlatformData()->Mips[0].BulkData.Lock(LOCK_READ_WRITE));
		FString Result;
		
		if (FormatedImageData)
		{
			const int Width = Texture->GetPlatformData()->Mips[0].SizeX;
			const int Height = Texture->GetPlatformData()->Mips[0].SizeY;
			const int Depth = Texture->GetPlatformData()->Mips[0].SizeZ;
			const EPixelFormat PixelFormat = Texture->GetPlatformData()->PixelFormat;
			const EGammaSpace GammaSpace = Texture->GetGammaSpace();
		
			TArray64<uint8> Data;
			ERawImageFormat::Type RawPixelFormat = ERawImageFormat::Type::Invalid;

			switch (PixelFormat)
			{
			case PF_R8G8B8A8:
			case PF_B8G8R8A8:
			case PF_DXT5:
					RawPixelFormat = ERawImageFormat::Type::BGRA8;
				break;

			default:
				ACircuitryLogger::DispatchErrorEvent("Unknown Pixel Format - " + UEnum::GetValueAsString(PixelFormat));
				break;
					
			}

			if (RawPixelFormat != ERawImageFormat::Type::Invalid)
			{
				FImageView View = FImageView(FormatedImageData, Width, Height, Depth, RawPixelFormat, GammaSpace);
				FImageUtils::CompressImage(Data, *FString("png"), View);
			
				Result = "data:image/png;base64," + FBase64::Encode(Data.GetData(), Data.Num());
			}
		}
		else
		{
			ACircuitryLogger::DispatchErrorEvent("Formatted image data is null. " + Texture->GetName());
			Result = "";
		}
		
		Texture->GetPlatformData()->Mips[0].BulkData.Unlock();
		if (ResourceCreated) Texture->ReleaseResource();

		return Result;
	}

	
};
