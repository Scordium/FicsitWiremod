// 

#pragma once

#include "CoreMinimal.h"
#include "BaseHelpModule.h"
#include "BrowserLinkHelpModule.generated.h"

/**
 * 
 */
UCLASS(DefaultToInstanced, EditInlineNew, BlueprintType)
class FICSITWIREMOD_API UBrowserLinkHelpModule : public UBaseHelpModule
{
	GENERATED_BODY()
public:

	UPROPERTY(EditAnywhere, Category = "Settings", meta=(ExposeOnSpawn = true))
	FString Url;
	
	virtual void Execute_Implementation() override {FPlatformProcess::LaunchURL(*Url, NULL, NULL);}
};
