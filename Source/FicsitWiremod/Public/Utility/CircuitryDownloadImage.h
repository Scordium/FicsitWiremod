// 

#pragma once

#include "CoreMinimal.h"
#include "HttpModule.h"
#include "Kismet/BlueprintAsyncActionBase.h"
#include "CircuitryDownloadImage.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FCircuitryDownloadImageDelegate, UTexture*, Texture);
/**
 * 
 */
UCLASS()
class FICSITWIREMOD_API UCircuitryDownloadImage : public UBlueprintAsyncActionBase
{
	GENERATED_BODY()
	
	UFUNCTION(BlueprintCallable, meta=( BlueprintInternalUseOnly="true" ))
	static UCircuitryDownloadImage* StartDownloadImage(FString URL, UTexture* FallbackValue);

	void StartDownload(FString URL, UTexture* FallbackValue);

	void HandleImageDownload(FHttpRequestPtr HttpRequest, FHttpResponsePtr HttpResponse, bool bSucceeded);

	void OnFail()
	{
		RemoveFromRoot();
		Finished.Broadcast(TextureFallbackValue);
	}
public:
	inline static TMap<FString, UTexture*> CachedDownloads;
	
	UPROPERTY(BlueprintAssignable)
	FCircuitryDownloadImageDelegate Finished;

	UPROPERTY()
	UTexture* TextureFallbackValue;
};

UCLASS()
class UCircuitryImageDownloaderFunctions : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable)
	static bool GetImageFromCache(FString URL, UTexture*& OutTexture)
	{
		if(auto Image = UCircuitryDownloadImage::CachedDownloads.Find(URL))
		{
			OutTexture = *Image;
			return true;
		}

		return false;
	}
};
