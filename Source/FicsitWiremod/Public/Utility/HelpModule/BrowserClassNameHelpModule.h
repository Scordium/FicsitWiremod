#pragma once

#include "BaseHelpModule.h"

#if PLATFORM_WINDOWS
#include "Windows/WindowsPlatformProcess.h"
#elif PLATFORM_UNIX || PLATFORM_LINUX
#include "Linux/LinuxPlatformProcess.h"
#endif

#include "CoreMinimal.h"
#include "CircuitryStatics.h"
#include "BrowserClassNameHelpModule.generated.h"


UCLASS(Blueprintable, BlueprintType)
class UBrowserClassNameHelpModule : public UBaseHelpModule
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, Category = "Settings", BlueprintReadWrite, meta=(ExposeOnSpawn = true))
	TSubclassOf<UObject> PresetClass;

	const FString HelpUrlBase = UCircuitryStatics::CircuitryWiki + "/docs/";

	virtual void Execute_Implementation() override
	{
		if (PresetClass == nullptr) return;
		
		FString Url = HelpUrlBase + PresetClass.Get()->GetName();
		FPlatformProcess::LaunchURL(*Url, NULL, NULL);
	}
	
	virtual void ExecuteOnClass_Implementation(TSubclassOf<UObject> Class) override
	{
		if (Class == nullptr) return;
		
		FString Url = HelpUrlBase + Class.Get()->GetName();
		
		FPlatformProcess::LaunchURL(*Url, NULL, NULL);
	}

	virtual void ExecuteOnObject_Implementation(UObject* Object) override
	{
		if (Object != nullptr) ExecuteOnClass(Object->GetClass());
	}
};
