// 

#pragma once

#include "CoreMinimal.h"
#include "WiremodReflection.h"
#include "UObject/Interface.h"
#include "WiremodInterface.generated.h"

UENUM(BlueprintType)
enum EConnectionDirection
{
	Input,
	Output
};

// This class does not need to be modified.
UINTERFACE()
class UWiremodInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class FICSITWIREMOD_API IWiremodInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void OnInputConnected(FNewConnectionData Data, int Index, UObject* Setter);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void OnInputDisconnected(int Index, UObject* Setter);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	TArray<FBuildingConnection> GetAvailableConnections(EConnectionDirection direction = Output);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	TSubclassOf<UUserWidget> GetCompactWidget();
};
