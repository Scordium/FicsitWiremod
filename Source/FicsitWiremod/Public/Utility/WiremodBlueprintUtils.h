// 

#pragma once

#include "CoreMinimal.h"
#include "Behaviour/WiremodInterface.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "WiremodBlueprintUtils.generated.h"

/**
 * 
 */
UCLASS()
class FICSITWIREMOD_API UWiremodBlueprintUtils : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintPure)
	bool IsWiremod(UObject* Object)
	{
		if(!Object) return false;

		return Object->GetClass()->Implements<UWiremodInterface>();
	}
	
	UFUNCTION(BlueprintCallable)
	void GetCanConnect(UObject* Object, UObject* Actor)
	{
		
	}
};
