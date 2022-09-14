// Copyright Coffee Stain Studios. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "FGRemoteCallObject.h"
#include "WiremodRemoteCallObject.generated.h"

/**
 * 
 */
UCLASS()
class FICSITWIREMOD_API UWiremodRemoteCallObject : public UFGRemoteCallObject
{
	GENERATED_BODY()
	
	UFUNCTION(Server, WithValidation, Reliable, BlueprintCallable)
    void DoSomething(const FString& SomeData);

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps);

public:
	UPROPERTY(Replicated)
    bool bDummy = true;
};
