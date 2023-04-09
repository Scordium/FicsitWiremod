// 

#pragma once

#include "CoreMinimal.h"
#include "Behaviour/WiremodInterface.h"
#include "Blueprint/UserWidget.h"
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

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	void CreateConnectionsList(EConnectionDirection Direction, EConnectionType AllowedConnectionType, UObject* Building);

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	void ShowBuildingDisqualifier(const FText& Text);

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	void ShowWireInfo(AConnectionWireBase* Wire);

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	void ClearUI();

	UFUNCTION(BlueprintPure, BlueprintImplementableEvent)
	FString GetConnectionName();

	UFUNCTION(BlueprintPure, BlueprintImplementableEvent)
	FNewConnectionData GetConnectionData();

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	void ScrollListDown(EConnectionType Type, bool ScrollInputs);

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	void ScrollListUp(EConnectionType Type, bool ScrollInputs);

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite)
	int CurrentIndex;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite)
	int MaxIndex;
};
