// 

#pragma once

#include "CoreMinimal.h"
#include "HttpModule.h"
#include "Kismet/BlueprintAsyncActionBase.h"
#include "CircuitryDownloadImage.generated.h"

DECLARE_DYNAMIC_DELEGATE_TwoParams(FCircuitryDownloadImageDelegate, UTexture*, Texture, bool, Success);
/**
 * 
 */
UCLASS()
class FICSITWIREMOD_API UCircuitryDownloadImage : public UBlueprintAsyncActionBase
{
	GENERATED_BODY()
	
	UFUNCTION(BlueprintCallable, meta=( BlueprintInternalUseOnly="true" ))
	static UCircuitryDownloadImage* StartDownloadImage(FString URL, FCircuitryDownloadImageDelegate OnFinish, bool AllowCache = true);

	void StartDownload(FString URL, bool AllowCache);

	void HandleImageDownload(FHttpRequestPtr HttpRequest, FHttpResponsePtr HttpResponse, bool bSucceeded);

	void OnFail()
	{
		RemoveFromRoot();
		Finished.Execute(nullptr, false);
	}
public:
	FCircuitryDownloadImageDelegate Finished;
	inline static TMap<FString, UTexture*> CachedDownloads;
};

UCLASS()
class UCircuitryImageDownloaderFunctions : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable)
	static void AddImageToDownloaderCache(FString URL, UTexture* Texture){ UCircuitryDownloadImage::CachedDownloads.Add(URL, Texture); }
};
