#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "MiscellaneousUtilities.generated.h"

UCLASS()
class FICSITWIREMOD_API UMiscellaneousUtilities : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable)
	static int32 SetBitValue(const int32 Value, const int32 Bit, const bool BitValue) { return (Value & ~(1 << Bit)) | (BitValue << Bit); }

	UFUNCTION(BlueprintPure)
	static bool GetBitValue(const int32 Value, const int32 Bit){ return (Value >> Bit) & 1; }

	UFUNCTION(BlueprintCallable)
	static int64 SetBitValue_Long(const int64 Value, const int64 Bit, const bool BitValue) { return (Value & ~(1 << Bit)) | (BitValue << Bit); }

	UFUNCTION(BlueprintPure)
	static bool GetBitValue_Long(const int64 Value, const int64 Bit){ return (Value >> Bit) & 1; }
};