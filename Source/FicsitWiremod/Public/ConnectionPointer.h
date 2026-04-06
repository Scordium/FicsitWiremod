#pragma once

#include "CoreMinimal.h"
#include "ConnectionPointer.generated.h"

USTRUCT(Blueprintable, BlueprintType)
struct FConnectionPointer
{
	GENERATED_BODY()

public:
	FConnectionPointer() {  }

	FConnectionPointer(UObject* Target, FName SourceName, bool IsProperty)
	{
		this->Target = Target;
		this->SourceName = SourceName;
		this->IsProperty = IsProperty;
	}

	UPROPERTY()
	UObject* Target = nullptr;

	UPROPERTY()
	FName SourceName = FName();

	UPROPERTY()
	bool IsProperty = false;
	
};