// Copyright Coffee Stain Studios. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "WiremodReflection.h"
#include "UObject/Interface.h"
#include "IConstantsDistributor.generated.h"

USTRUCT(BlueprintType)
struct FDynamicValue
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame)
	TEnumAsByte<EConnectionType> ConnectionType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame)
	bool StoredBool;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame)
	float StoredFloat;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame)
	FString StoredString;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame)
	FVector StoredVector;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame)
	FLinearColor StoredColor;
};


// This class does not need to be modified.
UINTERFACE(MinimalAPI, Blueprintable)
class UIConstantsDistributor : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class FICSITWIREMOD_API IIConstantsDistributor
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	FDynamicValue GetValue(const FString& ValueName);
	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
};
