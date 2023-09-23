// 

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "CCImageData.generated.h"

USTRUCT(BlueprintType)
struct FPixelScreenRow
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite, SaveGame)
	TArray<FLinearColor> RowData;

	FPixelScreenRow(){}
	FPixelScreenRow(int Width, FLinearColor Color)
	{
		for(int i = 0; i < Width; i++) RowData.Add(Color);
	}


	bool operator==(const FPixelScreenRow& Other) const
	{
		return RowData == Other.RowData;
	}
};


USTRUCT(BlueprintType)
struct FPixelScreenData
{
	GENERATED_BODY()


	UPROPERTY(BlueprintReadWrite, SaveGame)
	TArray<FPixelScreenRow> Data;

	UPROPERTY(BlueprintReadWrite, SaveGame)
	FLinearColor BackgroundColor = FLinearColor::Transparent;

	UPROPERTY(BlueprintReadWrite, SaveGame)
	int Width = 0;

	UPROPERTY(BlueprintReadWrite, SaveGame)
	int Height = 0;

	FPixelScreenData(){}
	FPixelScreenData(int Width, int Height, FLinearColor BackgroundColor)
	{
		for(int i = 0; i < Height; i++)
		{
			Data.Add(FPixelScreenRow(Width, BackgroundColor));
		}

		this->BackgroundColor = BackgroundColor;
		this->Width = Width;
		this->Height = Height;
	}

	int GetSizeInBytes() const
	{
		return Height * Width * sizeof(FLinearColor);
	}

	double GetSizeInMegabytes() const
	{
		return GetSizeInBytes() / 1000000.0f;
	}

	bool IsValid() const
	{
		return Width && Height;
	}

	TArray<TArray<FLinearColor>> MakeGrid() const
	{
		TArray<TArray<FLinearColor>> Out;

		//We need to reverse the array so that texture mipmap fills correctly. Otherwise the texture will appear upside down.
		for(int i = Data.Num() - 1; i >= 0; i--) Out.Add(Data[i].RowData);
		
		return Out;
	}

	bool operator ==(const FPixelScreenData& Other) const
	{
		if(Width != Other.Width || Height != Other.Height) return false;
		return Data == Other.Data;
	}

	UTexture2D* MakeTexture() const
	{
		if(!IsValid()) return nullptr;
		
		
		auto GridData = MakeGrid();
		auto Texture = UTexture2D::CreateTransient(
		Width, 
		Height
		);

		Texture->CreateResource();
		// Lock the texture so it can be modified
		uint8* MipData = static_cast<uint8*>(Texture->GetPlatformData()->Mips[0].BulkData.Lock(LOCK_READ_WRITE));

		// Create base mip.
		uint8* DestPtr = NULL;
		for( int32 y=0; y<Height; y++ )
		{
			DestPtr = &MipData[(Height - 1 - y) * Width * sizeof(FColor)];
			for( int32 x=0; x<Width; x++ )
			{
				auto Pixel = GridData[y][x].ToFColor(false);
				*DestPtr++ = Pixel.B;
				*DestPtr++ = Pixel.G;
				*DestPtr++ = Pixel.R;
				*DestPtr++ = Pixel.A;
			}
		}

		// Unlock the texture
		Texture->GetPlatformData()->Mips[0].BulkData.Unlock();

		//Configure texture for pixel art style rendering
		Texture->MipLoadOptions = ETextureMipLoadOptions::OnlyFirstMip;
		Texture->LODGroup = TextureGroup::TEXTUREGROUP_UI;
		Texture->CompressionSettings = TC_Alpha;
		Texture->Filter = TF_Nearest;
		
		Texture->UpdateResource();
		
		return Texture;
	}
};


UCLASS()
class UPixelScreenBlueprintFunctions : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:


	UFUNCTION(BlueprintPure, meta=(CompactNodeTitle="GET"))
	static FPixelScreenRow GetRow(const FPixelScreenData& Data, int Row)
	{
		if(Data.Data.IsValidIndex(Row))
			return Data.Data[Row];

		GetTypeHash(&Data);

		return FPixelScreenRow();
	}

	UFUNCTION(BlueprintPure)
	static double GetSizeInMegabytes(const FPixelScreenData& Data){ return Data.GetSizeInMegabytes(); }

	UFUNCTION(BlueprintPure)
	static int GetSizeInBytes(const FPixelScreenData& Data){ return Data.GetSizeInBytes(); }

	UFUNCTION(BlueprintCallable)
	static FPixelScreenData MakePixelScreenFromData(int Width, int Height, FLinearColor BackgroundColor)
	{
		return FPixelScreenData(Width, Height, BackgroundColor);
	}

	UFUNCTION(BlueprintCallable)
	static FPixelScreenData MakePixelScreenFromFile(FString FileName)
	{
		//TODO
		return FPixelScreenData();
	}

	UFUNCTION(BlueprintCallable)
	static UTexture2D* ToTexture(const FPixelScreenData& Data)
	{
		return Data.MakeTexture();
	}

	UFUNCTION(BlueprintCallable)
	static bool IsTextureReadyForExport(UTexture* Texture)
	{
		if (!Texture || !Texture->GetResource() || !Texture->GetResource()->TextureRHI)
		{
			return false;
		}

		return true;
	}

	UFUNCTION(BlueprintPure)
	static bool IsValidImage(const FPixelScreenData& Data) { return Data.IsValid(); }

	UFUNCTION(BlueprintPure, meta=(CompactNodeTitle="=="))
	static bool Equals(const FPixelScreenData& Data1, const FPixelScreenData& Data2) { return Data1 == Data2; }
};
