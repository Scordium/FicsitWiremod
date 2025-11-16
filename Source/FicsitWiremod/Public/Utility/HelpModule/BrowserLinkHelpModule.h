// 

#pragma once

#include "CoreMinimal.h"
#include "BaseHelpModule.h"

#if PLATFORM_WINDOWS
#include "Windows/WindowsPlatformProcess.h"
#elif PLATFORM_UNIX || PLATFORM_LINUX
#include "Linux/LinuxPlatformProcess.h"
#endif

#include "BrowserLinkHelpModule.generated.h"

/**
 * 
 */
UCLASS(DefaultToInstanced, EditInlineNew, BlueprintType)
class FICSITWIREMOD_API UBrowserLinkHelpModule : public UBaseHelpModule
{
	GENERATED_BODY()
public:

	UPROPERTY(EditAnywhere, Category = "Settings", BlueprintReadWrite, meta=(ExposeOnSpawn = true))
	FString Url;
	
	virtual void Execute_Implementation() override {FPlatformProcess::LaunchURL(*Url, NULL, NULL);}
};
