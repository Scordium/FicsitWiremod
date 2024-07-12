// 

#pragma once

#include "CoreMinimal.h"
#include "Logging/LogVerbosity.h"
#include "Subsystem/ModSubsystem.h"
#include "CircuitryLogger.generated.h"

//DECLARE_LOG_CATEGORY_EXTERN(LogCircuitry, All, All);
DEFINE_LOG_CATEGORY_STATIC(LogCircuitry, All, All)

#define CC_LOG(Src, Args) FString::Format(*FString(Src), FStringFormatOrderedArguments(TArray<FStringFormatArg>{Args}))
#define CC_INT(Num) FString::FromInt(Num)

UCLASS()
class FICSITWIREMOD_API ACircuitryLogger : public AModSubsystem
{
	GENERATED_BODY()

public:

	static inline ACircuitryLogger* Self;
	
	ACircuitryLogger() : Super()
	{
		PrimaryActorTick.bCanEverTick = false;
		Self = this;
	}
	
	static void DispatchErrorEvent(const FString& ErrorText)
	{
		DispatchEvent(ErrorText, ELogVerbosity::Error);
		if(Self) Self->OnError(ErrorText);
	}
	
	static void DispatchWarningEvent(const FString& WarningText)
	{
		DispatchEvent(WarningText, ELogVerbosity::Warning);
	}
	
	static void DispatchEvent(const FString& Text, uint8 Verbosity)
	{
		FMsg::Logf(nullptr, 0, LogCircuitry.GetCategoryName(), (ELogVerbosity::Type)Verbosity, TEXT("%s"), *Text);
	}

	UFUNCTION(BlueprintImplementableEvent)
	void OnError(const FString& ErrorText);
};
