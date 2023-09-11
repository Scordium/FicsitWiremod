// 

#pragma once

#include "CoreMinimal.h"
#include "SignComponentVariableName.h"
#include "Blueprint/UserWidget.h"
#include "CommonLib/ConnectionType.h"
#include "SignComponentVariableEditor.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnComponentVariableValueChanged, UUserWidget*, VariableEditorWidget, TSubclassOf<USignComponentVariableName>, Name, FString, NewValue);

USTRUCT(BlueprintType)
struct FManagedSignConnectionSettings
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame)
	FString Name;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame)
	TEnumAsByte<EConnectionType> Type;
};

UINTERFACE(BlueprintType, Blueprintable)
class USignComponentVariableBindingsProvider : public UInterface
{
	GENERATED_BODY()
};

class ISignComponentVariableBindingsProvider
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	TArray<FManagedSignConnectionSettings> GetConnectionBindings();
};

UCLASS(Blueprintable, BlueprintType)
class FICSITWIREMOD_API USignComponentVariableEditor : public UUserWidget
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(ExposeOnSpawn="true"))
	TScriptInterface<ISignComponentVariableBindingsProvider> BindingsProvider; 

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
	
	/*
	 * Broadcasts an update to the component and editor when called, can be called to update a displayed value or to simply refresh the current one
	 */
	UFUNCTION(BlueprintCallable)
	void CallChanged() { OnComponentVariableValueChanged.Broadcast(this, Name, Data); }

	UFUNCTION(BlueprintPure)
	FText GetName() { return Name.GetDefaultObject()->GetName(); }
};
