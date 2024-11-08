// 

#pragma once

#include "CoreMinimal.h"
#include "Behaviour/CircuitryInterface.h"
#include "Blueprint/UserWidget.h"
#include "Runtime/CoreUObject/Public/UObject/Interface.h"
#include "Runtime/UMG/Public/Blueprint/UserWidget.h"
#include "Utility/ConnectionWireBase.h"
#include "WiretoolWidget.generated.h"

/**
 * 
 */
UCLASS()
class FICSITWIREMOD_API UWiretoolWidget : public UUserWidget
{
	GENERATED_BODY()


public:

	
};

UINTERFACE()
class UWiringToolWidget : public UInterface
{
	GENERATED_BODY()
};

class IWiringToolWidget
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	void CreateConnectionsList(EConnectionDirection Direction, EConnectionType AllowedConnectionType, UObject* Building);

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	void ShowBuildingDisqualifier(const FText& Text);

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	void ClearUI();

	UFUNCTION(BlueprintImplementableEvent)
	FString GetConnectionName();

	UFUNCTION(BlueprintImplementableEvent)
	FConnectionData GetConnectionData();

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	void ScrollListDown(EConnectionType Type, bool ScrollInputs);

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	void ScrollListUp(EConnectionType Type, bool ScrollInputs);

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	int GetCurrentIndex();

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	int GetMaxIndex();
	
};
