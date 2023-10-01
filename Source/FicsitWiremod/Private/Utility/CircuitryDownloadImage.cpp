// 


#include "Utility/CircuitryDownloadImage.h"

#include "IImageWrapper.h"
#include "IImageWrapperModule.h"
#include "Engine/Texture2DDynamic.h"
#include "Interfaces/IHttpResponse.h"


UCircuitryDownloadImage* UCircuitryDownloadImage::StartDownloadImage(FString URL, FCircuitryDownloadImageDelegate OnFinish, bool AllowCache)
{
	UCircuitryDownloadImage* DownloadTask = NewObject<UCircuitryDownloadImage>();
	DownloadTask->Finished = OnFinish;
	DownloadTask->StartDownload(URL, AllowCache);

	return DownloadTask;
	
}

void UCircuitryDownloadImage::StartDownload(FString URL, bool AllowCache)
{
	if(URL.Len() == 0)
	{
		OnFail();
		return;
	}

	if(AllowCache)
		if(auto Cache = CachedDownloads.Find(URL); Cache && *Cache)
		{
			Finished.ExecuteIfBound(*Cache, true);
			return;
		}
	
	TSharedRef<IHttpRequest, ESPMode::ThreadSafe> HttpRequest = FHttpModule::Get().CreateRequest();

	HttpRequest->OnProcessRequestComplete().BindUObject(this, &UCircuitryDownloadImage::HandleImageDownload);
	HttpRequest->SetURL(URL);
	HttpRequest->SetVerb(TEXT("GET"));
	if(!HttpRequest->ProcessRequest()) OnFail();
	
}

void UCircuitryDownloadImage::HandleImageDownload(FHttpRequestPtr HttpRequest, FHttpResponsePtr HttpResponse, bool bSucceeded)
{
#if !UE_SERVER

	RemoveFromRoot();

	if ( bSucceeded && HttpResponse.IsValid() && HttpResponse->GetContentLength() > 0 )
	{
		IImageWrapperModule& ImageWrapperModule = FModuleManager::LoadModuleChecked<IImageWrapperModule>(FName("ImageWrapper"));
		TSharedPtr<IImageWrapper> ImageWrappers[3] =
		{
			ImageWrapperModule.CreateImageWrapper(EImageFormat::PNG),
			ImageWrapperModule.CreateImageWrapper(EImageFormat::JPEG),
			ImageWrapperModule.CreateImageWrapper(EImageFormat::BMP),
		};

		for ( auto ImageWrapper : ImageWrappers )
		{
			if ( ImageWrapper.IsValid() && ImageWrapper->SetCompressed(HttpResponse->GetContent().GetData(), HttpResponse->GetContentLength()) )
			{
				TArray64<uint8> RawData;
				const ERGBFormat InFormat = ERGBFormat::BGRA;
				if ( ImageWrapper->GetRaw(InFormat, 8, RawData) )
				{
					if ( UTexture2DDynamic* Texture = UTexture2DDynamic::Create(ImageWrapper->GetWidth(), ImageWrapper->GetHeight()) )
					{
						Texture->SRGB = true;
						Texture->UpdateResource();

						FTexture2DDynamicResource* TextureResource = static_cast<FTexture2DDynamicResource*>(Texture->GetResource());
						if (TextureResource)
						{
							ENQUEUE_RENDER_COMMAND(FWriteRawDataToTexture)(
								[TextureResource, RawData = MoveTemp(RawData)](FRHICommandListImmediate& RHICmdList)
								{
									TextureResource->WriteRawToTexture_RenderThread(RawData);
								});
						}
						CachedDownloads.Add(HttpRequest->GetURL(), Texture);
						Finished.ExecuteIfBound(Texture, true);					
						return;
					}
				}
			}
		}
	}

	Finished.ExecuteIfBound(nullptr, false);

#endif
}
