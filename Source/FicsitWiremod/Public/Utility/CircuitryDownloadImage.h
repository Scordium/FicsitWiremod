// 

#pragma once

#include "CoreMinimal.h"
#include "HttpModule.h"
#include "CommonLib/TextureUtilities.h"
#include "Engine/DataTable.h"
#include "ModLoading/ModLoadingLibrary.h"
#include "Subsystem/ModSubsystem.h"
#include "CircuitryDownloadImage.generated.h"

DECLARE_DYNAMIC_DELEGATE_TwoParams(FCircuitryDownloadImageDelegate, UTexture*, Texture, bool, Success);
DECLARE_MULTICAST_DELEGATE_ThreeParams(FCircuitryImageDownloadTaskFinished, const FString& Url, UTexture* Texture, bool Success);
/**
 * 
 */
UCLASS()
class FICSITWIREMOD_API UCircuitryDownloadImage : public UObject
{
	GENERATED_BODY()

public:
	void StartDownload(const FString& URL);

	void HandleImageDownload(FHttpRequestPtr HttpRequest, FHttpResponsePtr HttpResponse, bool bSucceeded);

	void OnFail()
	{
		RemoveFromRoot();
		Finished.Broadcast(Url, nullptr, false);
	}
public:
	FString Url;
	FCircuitryImageDownloadTaskFinished Finished;
};

USTRUCT()
struct FCircuitryImageDownloadTask
{
	GENERATED_BODY()

	UPROPERTY()
	bool Finished = false;

	UPROPERTY()
	bool Success = false;

	UPROPERTY()
	FDateTime FinishedAt = FDateTime::MinValue();

	UPROPERTY()
	FSoftObjectPath Texture = nullptr;
	
	TArray<FCircuitryDownloadImageDelegate> Delegates;

	void AddDelegate(const FCircuitryDownloadImageDelegate& Delegate)
	{
		if(Finished) Delegate.ExecuteIfBound(Cast<UTexture>(Texture.TryLoad()), Success);
		else Delegates.Add(Delegate);
	}

	void RemoveDelegatesForObject(const UObject* Object)
	{
		for (const FCircuitryDownloadImageDelegate& Delegate : TArray(Delegates))
		{
			if (Delegate.GetUObject() == Object) Delegates.Remove(Delegate);
		} 
	}

	void OnDownloadFinished(UTexture* ResultTexture, bool DownloadSuccess)
	{
		Finished = true;
		Success = DownloadSuccess;
		FinishedAt = FDateTime::Now();
		Texture = ResultTexture;

		for(auto& Delegate : Delegates) Delegate.ExecuteIfBound(ResultTexture, DownloadSuccess);

		Delegates.Empty();
	}
};



UCLASS(BlueprintType, Blueprintable)
class UCircuitryImageObject : public UObject
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadOnly) FTextureAssetData Data;


	UFUNCTION(BlueprintCallable)
	bool DoesFitFilter(const FText& Text, bool Modded, bool Vanilla, bool Buildables, bool Items, bool Other, bool UI, bool Materials, bool Customizer, bool Fauna, ETextureResolutionLevel ResolutionLevel)
	{
		auto StringText = Text.ToString();

		if(Data.ResolutionLevel != ResolutionLevel && ResolutionLevel != TRL_Unset && Data.ResolutionLevel != TRL_Unset) return false;

		if(Data.AssetSource == Mod && !Modded) return false;
		if(Data.AssetSource == Game && !Vanilla) return false;
		
		if(Data.Category == TAC_Item && !Items) return false;
		if(Data.Category == TAC_Buildable && !Buildables) return false;
		if(Data.Category == TAC_Interface && !UI) return false;
		if(Data.Category == TAC_Rendering && !Materials) return false;
		if(Data.Category == TAC_Customizer && !Customizer) return false;
		if(Data.Category == TAC_Fauna && !Fauna) return false;
		if(Data.Category == TAC_Other && !Other) return false;

		if(StringText.Len() > 0)
		{
			bool TextMatch = false;
			
			if(Data.OwnerPlugin.Contains(StringText)) TextMatch = true;
			else if(Data.Texture.GetAssetName().Contains(StringText)) TextMatch = true;
			else if(Data.ReadableName.ToString().Contains(StringText)) TextMatch = true;
			else
			{
				FString ModName;
				if(Data.OwnerPlugin == "Game" || Data.OwnerPlugin == "FactoryGame") ModName = "Satisfactory";
				else
				{
					FModInfo ModInfo;
					GetWorld()->GetGameInstance()->GetSubsystem<UModLoadingLibrary>()->GetLoadedModInfo(Data.OwnerPlugin, ModInfo);

					ModName = ModInfo.FriendlyName;
				}

				if(ModName.Contains(StringText)) TextMatch = true;
			}

			if(!TextMatch) return false;
		}
		
		return true;
	}

	UFUNCTION(BlueprintPure)
	UTexture2D* GetTexture()
	{
		return Cast<UTexture2D>(Data.Texture.TryLoad());
	}
};

UCLASS()
class ACircuitryImageStorage : public AModSubsystem
{
	GENERATED_BODY()

	ACircuitryImageStorage()
	{
		ReplicationPolicy = ESubsystemReplicationPolicy::SpawnOnClient;
		Self = this;
	}

public:

	static inline ACircuitryImageStorage* Self;

	virtual void BeginPlay() override
	{
		Super::BeginPlay();

		RefreshTextureAssetStorage();
	}

	UFUNCTION(BlueprintCallable)
	void RefreshTextureAssetStorage()
	{
		TextureAssets.Empty();
		auto Assets = UTextureUtilities::GetAllTextureAssets(false, false, true);
		
		for(auto& Asset : Assets)
		{
			if(!Asset.Texture.IsValid()) continue;
			
			auto DataRow = TextureAssetDescriptorTable->FindRow<FTextureAssetMetadata>(Asset.Texture.GetAssetFName(), "", false);
			if(DataRow)
			{
				if(DataRow->Ignored) continue;
				Asset.WithMetadata(*DataRow);
			}

			auto AssetCacheObject = NewObject<UCircuitryImageObject>(this);
			AssetCacheObject->Data = Asset;
			TextureAssets.Add(AssetCacheObject);
		}
	}
	
	UFUNCTION(BlueprintCallable)
	void DownloadImage(const FString& Url, FCircuitryDownloadImageDelegate OnFinish, bool AllowCache = true)
	{
		//Delete existing awaited tasks for this object to prevent system caching the wrong image in case an older task takes longer to finish than current one.
		for (auto& Task : Cache)
		{
			Task.Value.RemoveDelegatesForObject(OnFinish.GetUObject());
		}
		
		
		if(AllowCache)
		{
			if(auto CachedTask = Cache.Find(Url))
			{
				CachedTask->AddDelegate(OnFinish);
				return;
			}
		}
		
		//If the url starts with "asset://" then we should return whatever we find in that path
		if(Url.StartsWith("asset://"))
		{
			FString AssetPath = Url;
			AssetPath.ReplaceInline(*FString("asset://"), *FString());
			const auto Texture = Cast<UTexture>(FSoftObjectPath(AssetPath).TryLoad());

			OnFinish.ExecuteIfBound(Texture, Texture != nullptr);
			
			return;
		}
		//However for backwards compatibility we should check for old path that didn't contain that string.
		//If we don't find anything then it's probably just a url without a protocol specified, so we shouldn't return null.
		else if(!Url.Contains("://"))
		{
			const auto Texture = Cast<UTexture>(FSoftObjectPath(Url).TryLoad());
			if(Texture)
			{
				OnFinish.ExecuteIfBound(Texture, true);
				return;
			}
		}
		
		UCircuitryDownloadImage* DownloadTask = NewObject<UCircuitryDownloadImage>();
		auto CacheObject = FCircuitryImageDownloadTask();
		CacheObject.AddDelegate(OnFinish);
		DownloadTask->Finished.AddUObject(this, &ACircuitryImageStorage::OnDownloadFinished);
		
		Cache.Add(Url, CacheObject);
		
		DownloadTask->StartDownload(Url);
	}

	void OnDownloadFinished(const FString& Url, UTexture* ResultTexture, bool DownloadSuccess)
	{
		auto CachedTask = Cache.Find(Url);
		if(!CachedTask) return;
		CachedTask->OnDownloadFinished(ResultTexture, DownloadSuccess);
	}

	UFUNCTION(BlueprintCallable)
	void ClearUnfinishedCache()
	{
		for(auto& CacheEntry : TMap(Cache))
		{
			if(!CacheEntry.Value.Success)
			{
				CacheEntry.Value.Delegates.Empty();
				Cache.Remove(CacheEntry.Key);
			}
		}
	}

	UFUNCTION(BlueprintCallable)
	void ClearOldCache(int OlderThan = 300)
	{
		for(auto& CacheEntry : TMap(Cache))
		{
			auto TimeSinceFinished = (FDateTime::Now() - CacheEntry.Value.FinishedAt).GetTotalSeconds();
			auto IsOld = CacheEntry.Value.Finished && TimeSinceFinished > OlderThan;
			
			if(!CacheEntry.Value.Success || IsOld)
			{
				CacheEntry.Value.Delegates.Empty();
				Cache.Remove(CacheEntry.Key);
			}
		}
	}

	UFUNCTION(BlueprintCallable)
	void ClearAllCache()
	{
		for(auto& CacheEntry : Cache) CacheEntry.Value.Delegates.Empty();

		Cache.Empty();
	}

	UFUNCTION(BlueprintCallable)
	void ReattemptUnfinishedDownloads()
	{
		for(auto& CacheEntry : Cache)
		{
			if(!CacheEntry.Value.Finished)
			{
				UCircuitryDownloadImage* DownloadTask = NewObject<UCircuitryDownloadImage>();
				DownloadTask->Finished.AddUObject(this, &ACircuitryImageStorage::OnDownloadFinished);
				DownloadTask->StartDownload(CacheEntry.Key);
			}
		}
	}

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UDataTable* TextureAssetDescriptorTable;
	
	UPROPERTY()
	TMap<FString, FCircuitryImageDownloadTask> Cache;

	UPROPERTY(BlueprintReadOnly)
	TArray<UCircuitryImageObject*> TextureAssets;
};
