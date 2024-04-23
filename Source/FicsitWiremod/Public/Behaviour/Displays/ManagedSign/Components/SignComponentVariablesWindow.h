// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SignComponentDescriptor.h"
#include "Blueprint/UserWidget.h"
#include "SignComponentVariablesWindow.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnVariableUpdated, const FSignComponentVariableData&, Variable);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnVariableMetadataUpdated, TSubclassOf<USignComponentVariableName>, Variable, const FSignComponentVariableMetaData&, MetaData);

/**
 * 
 */
UCLASS(Blueprintable, BlueprintType)
class FICSITWIREMOD_API USignComponentVariablesWindow : public UUserWidget
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	USignComponentVariableEditorBase* AddVariableEditor(const FSignComponentVariableData& VariableData);

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	void AddVariableEditors(const TArray<FSignComponentVariableData>& Variables, bool ClearCurrentEditors = true);

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	void SetComponentControls(UUserWidget* ControlsWidget);

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	void AddComponentVariablesAndFilter(const TArray<FSignComponentData>& Components);

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	void UpdateComponentValue(TSubclassOf<USignComponentVariableName> Name, const FString& NewValue);

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	void UpdateComponentMetaValue(TSubclassOf<USignComponentVariableName> VariableName, FName MetadataName, const FString& NewValue);

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	void ClearWindow();

	UPROPERTY(BlueprintCallable, BlueprintAssignable)
	FOnVariableUpdated OnVariableUpdated;

	UPROPERTY(BlueprintCallable, BlueprintAssignable)
	FOnVariableMetadataUpdated OnVariableMetadataUpdated;
};
