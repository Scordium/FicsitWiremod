// 

#pragma once

#include "CoreMinimal.h"
#include "SignComponentVariableName.h"
#include "Blueprint/UserWidget.h"
#include "SignComponentVariableEditor.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnComponentVariableValueChanged, UUserWidget*, VariableEditorWidget, TSubclassOf<USignComponentVariableName>, Name, FString, NewValue);

UCLASS(Blueprintable, BlueprintType)
class FICSITWIREMOD_API USignComponentVariableEditor : public UUserWidget
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(ExposeOnSpawn="true"))
	TSubclassOf<USignComponentVariableName> Name;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(ExposeOnSpawn="true"))
	FString Data;

	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FOnComponentVariableValueChanged OnComponentVariableValueChanged;

	/* 
	 *	Call this after constructing the widget if some of your components (such as spin boxes) don't work properly
	 */
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void InitializeEditor(const FString& Value);

protected:

	UFUNCTION(BlueprintCallable)
	void CallChanged() { OnComponentVariableValueChanged.Broadcast(this, Name, Data); }

	UFUNCTION(BlueprintPure)
	FText GetName() { return Name.GetDefaultObject()->GetName(); }
};
