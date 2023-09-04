// 

#pragma once

#include "CoreMinimal.h"
#include "SignComponentVariableName.generated.h"

UCLASS(BlueprintType, Blueprintable)
class USignComponentVariableName : public UObject
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText Name;

	UFUNCTION(BlueprintPure)
	FText GetName() { return Name; }
};
