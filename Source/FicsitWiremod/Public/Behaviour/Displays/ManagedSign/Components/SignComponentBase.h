// 

#pragma once

#include "CoreMinimal.h"
#include "SignComponentDescriptor.h"
#include "Behaviour/FGWiremodBuildable.h"
#include "Blueprint/UserWidget.h"
#include "SignComponentBase.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnComponentFocusChanged, UUserWidget*, Component, bool, Focused);



UCLASS(BlueprintType, Blueprintable)
class FICSITWIREMOD_API USignEditorComponentBase : public UUserWidget
{
	GENERATED_BODY()
	
public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(ExposeOnSpawn="true"))
	TArray<FSignComponentVariableData> Variables;

	UFUNCTION(BlueprintPure, BlueprintImplementableEvent)
	FSignComponentData CompileComponent();
	
	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FOnComponentFocusChanged OnComponentFocusChanged;

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void OnComponentTargeted();

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void OnComponentUntargeted();

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	void InstantiateVariableEditors(UWidget* VariableEditorsContainer);

protected:

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void UpdateVariableValue(TSubclassOf<USignComponentVariableName> Name, const FString& Value);
	void UpdateVariableValue_Implementation(TSubclassOf<USignComponentVariableName> Name, const FString& Value)
	{
		for(auto& Var : Variables)
		{
			if(Var.Name == Name) Var.Data = Value;
		}
	}

	UFUNCTION(BlueprintCallable)
	FString GetVariableValue(TSubclassOf<USignComponentVariableName> Name, bool& Success)
	{
		for(auto& Var : Variables)
		{
			// ಠ_ಠ
			Success = Var.Name == Name;
			if(Success) return Var.Data;
		}

		return FString();
	}

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void InitializeComponent();
};
/**
 * 
 */
UCLASS(Blueprintable, BlueprintType)
class FICSITWIREMOD_API USignComponentBase : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	void InitializeComponent();
	
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	void OnUpdate(AFGWiremodBuildable* Sign);
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(ExposeOnSpawn="true"))
	TArray<FSignComponentVariableData> Variables;

	UFUNCTION(BlueprintCallable)
	FString GetVariableValue(TSubclassOf<USignComponentVariableName> Name, bool& Success)
	{
		for(auto& Var : Variables)
		{
			// ಠ_ಠ
			Success = Var.Name == Name;
			if(Success) return Var.Data;
		}

		return FString();
	}
};
