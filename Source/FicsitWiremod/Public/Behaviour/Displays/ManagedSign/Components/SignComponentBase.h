// 

#pragma once

#include "CoreMinimal.h"
#include "SignBindingsProvider.h"
#include "SignComponentDescriptor.h"
#include "SignComponentVariablesWindow.h"
#include "Behaviour/FGWiremodBuildable.h"
#include "Blueprint/UserWidget.h"
#include "SignComponentBase.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnComponentFocusChanged, UUserWidget*, Component, bool, Focused);

UINTERFACE(Blueprintable, BlueprintType)
class UManagedSignEditorWindow : public UInterface
{
	GENERATED_BODY()
};

class IManagedSignEditorWindow
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	FVector2D GetCanvasSize();

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	double GetCanvasScale();

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	TScriptInterface<ISignComponentVariableBindingsProvider> GetBindingsProvider();

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	double GetCanvasGridSize();

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	USignComponentVariablesWindow* GetVariablesEditor();

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	bool CanOpenContextMenu();

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	void DuplicateComponent(const FSignComponentData& ComponentData);

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	TArray<UUserWidget*> GetAllComponentsOfClass(TSubclassOf<USignComponentDescriptor> ComponentClass);
};

DECLARE_DYNAMIC_DELEGATE_OneParam(FUpdateEditorVariableValue, const FSignComponentVariableData&, Data);

UCLASS(BlueprintType, Blueprintable)
class FICSITWIREMOD_API USignEditorComponentBase : public UUserWidget
{
	GENERATED_BODY()
	
public:

	//For some reason UE refuses to tick with blueprint function, so i have to do this shit...
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override { CustomTick(MyGeometry, InDeltaTime); }

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(ExposeOnSpawn="true"))
	FSignComponentData ComponentData;
	
	UFUNCTION(BlueprintPure, BlueprintNativeEvent)
	FText GetComponentName();
	FText GetComponentName_Implementation()
	{
		return ComponentData.Metadata.ComponentName.ToString().Len() == 0
		? ComponentData.ComponentDescriptor.GetDefaultObject()->DisplayName
		: ComponentData.Metadata.ComponentName;
	}

	UFUNCTION(BlueprintCallable)
	void SetComponentName(const FText& NewName) { ComponentData.Metadata.ComponentName = NewName; }
	
	UFUNCTION(BlueprintPure, BlueprintNativeEvent)
	FSignComponentData CompileComponent();
	FSignComponentData CompileComponent_Implementation() { return ComponentData; }
	
	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FOnComponentFocusChanged OnComponentFocusChanged;

	UPROPERTY(BlueprintReadWrite, meta=(ExposeOnSpawn="true"))
	TScriptInterface<IManagedSignEditorWindow> Parent;

	/*
	 * Whether the user is currently focused on this component (or group, if this component is part of a group)
	 * TODO: Should this be deprecated? With new vars config window this is useless.
	 *
	 */
	UPROPERTY(BlueprintReadWrite)
	bool IsFocused;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void OnComponentTargeted();
	void OnComponentTargeted_Implementation()
	{
		IsFocused = true;
		if(const auto VariablesWindow = IManagedSignEditorWindow::Execute_GetVariablesEditor(Parent.GetObject())) VariablesWindow->OnVariableUpdated.AddDynamic(this, &USignEditorComponentBase::HandleVariableUpdate);
	}

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void OnComponentUntargeted();
	void OnComponentUntargeted_Implementation()
	{
		IsFocused = false;
		if(const auto VariablesWindow = IManagedSignEditorWindow::Execute_GetVariablesEditor(Parent.GetObject())) VariablesWindow->OnVariableUpdated.RemoveDynamic(this, &USignEditorComponentBase::HandleVariableUpdate);
	}
	
	/*
	 * Gets called when this component is created or loaded on the editor's canvas.
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void InitializeComponent();
	void InitializeComponent_Implementation(){ }

	/*
	 * Gets called after "Initialize Component" to apply all current variable values to the component
	 */
	UFUNCTION(BlueprintCallable)
	void LoadVariableValues(const TArray<FSignComponentVariableData>& VariablesData) { for(auto& NewVar : VariablesData) HandleVariableUpdate(NewVar); }

	/*
	 * Gets called when this component pushes its variables to configuration window
	 */
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	void InstantiateVariableEditors();
	
protected:
	
	UFUNCTION(BlueprintImplementableEvent)
	void CustomTick(const FGeometry& MyGeometry, double DeltaTime);

	UFUNCTION(BlueprintCallable)
	void SubscribeToVariableUpdates(TSubclassOf<USignComponentVariableName> Variable, FUpdateEditorVariableValue Event) { EditorVariableBindings.Add(Variable, Event); }

	UFUNCTION()
	void HandleVariableUpdate(const FSignComponentVariableData& Data)
	{
		for(auto& Var : ComponentData.Variables)
		{
			if(Var.Name == Data.Name)
			{
				Var = Data;
				if(auto Delegate = EditorVariableBindings.Find(Data.Name)) Delegate->ExecuteIfBound(Var);
				return;
			}
		}
	}
	
	UFUNCTION(BlueprintCallable)
	void UpdateVariableValue(TSubclassOf<USignComponentVariableName> VariableName, const FString& NewValue)
	{
		if(const auto VariablesWindow = IManagedSignEditorWindow::Execute_GetVariablesEditor(Parent.GetObject()))
			VariablesWindow->UpdateComponentValue(VariableName, NewValue);
		
		for(auto& Var : ComponentData.Variables)
		{
			if(Var.Name == VariableName)
			{
				if(!USignBindingsFunctions::IsBinding(Var.Data)) Var.Data = NewValue;
				else Var.DefaultValue = NewValue;

				if(auto Delegate = EditorVariableBindings.Find(VariableName)) Delegate->ExecuteIfBound(Var);
				return;
			}
		}
	}

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void UpdateVariableMetadata(TSubclassOf<USignComponentVariableName> VariableName, FName MetadataName, const FString& NewValue);
	void UpdateVariableMetadata_Implementation(TSubclassOf<USignComponentVariableName> VariableName, FName MetadataName, const FString& NewValue)
	{
		if(const auto VariablesWindow = IManagedSignEditorWindow::Execute_GetVariablesEditor(Parent.GetObject()))
			VariablesWindow->UpdateComponentMetaValue(VariableName, MetadataName, NewValue);

		for(auto& Var : ComponentData.Variables)
		{
			if(Var.Name == VariableName)
			{
				for(auto& MetaVar : Var.MetaData)
				{
					if(MetaVar.Name == MetadataName)
					{
						MetaVar.Value = NewValue;
						return;
					}
				}
				return;
			}
		}
	}

	UFUNCTION(BlueprintPure)
	FString GetVariableMetadataValue(TSubclassOf<USignComponentVariableName> VariableName, FName MetadataName, bool& Success)
	{
		for(auto& Var : ComponentData.Variables)
		{
			if(Var.Name == VariableName)
			{
				for(auto& VarMeta : Var.MetaData)
				{
					Success = VarMeta.Name == MetadataName;
					if(Success) return VarMeta.Value;
				}
			}
		}

		Success = false;
		return FString();
	}
	
private:

	UPROPERTY()
	TMap<TSubclassOf<USignComponentVariableName>, FUpdateEditorVariableValue> EditorVariableBindings;
};


DECLARE_DYNAMIC_DELEGATE_ThreeParams(FOnComponentVariableUpdate, FConnectionData, Data, const FString&, DefaultValue, const TArray<FSignComponentVariableMetaData>&, Meta);
DECLARE_DYNAMIC_DELEGATE_TwoParams(FOnRegisteredConstantVariable, FString, Data, const TArray<FSignComponentVariableMetaData>&, MetaData);

USTRUCT()
struct FVariableValueBindingData
{
	GENERATED_BODY()

public:
	UPROPERTY()
	FName BindingName;

	UPROPERTY()
	int InputIndex = -1;

	UPROPERTY()
	FOnComponentVariableUpdate Event;

	UPROPERTY()
	FString DefaultValue;

	UPROPERTY()
	TArray<FSignComponentVariableMetaData> MetaData;

	void Call(AFGWiremodBuildable* Sign);

	FVariableValueBindingData(){}
	
	FVariableValueBindingData(FName BindingName, const FOnComponentVariableUpdate& Event, const FString& DefaultValue, const TArray<FSignComponentVariableMetaData>& MetaData)
	{
		this->BindingName = BindingName;
		this->Event = Event;
		this->DefaultValue = DefaultValue;
		this->MetaData = MetaData;
	}
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
	
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void OnUpdate(AFGWiremodBuildable* Sign);
	void OnUpdate_Implementation(AFGWiremodBuildable* Sign)
	{
		for(auto& EventData : VariableToBuildableInputMap)
		{
			EventData.Value.Call(Sign);
		}
	}
	
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

	UFUNCTION(BlueprintPure)
	FString GetVariableMeta(TSubclassOf<USignComponentVariableName> Name, FName MetadataName, bool& Success)
	{
		for(auto& Var : Variables)
		{
			if(Var.Name == Name)
			{
				for(auto& Meta : Var.MetaData)
				{
					Success = Meta.Name == MetadataName;
					if(Success) return Meta.Value;
				}
			}
		}

		return FString();
	}

protected:

	UFUNCTION(BlueprintCallable, meta=(AutoCreateRefTerm="ConstantValueFallback"))
	void TryBindFunctionToUpdate(const FOnComponentVariableUpdate& Event, const FOnRegisteredConstantVariable& ConstantValueFallback, TSubclassOf<USignComponentVariableName> VariableName)
	{
		auto const BindingString = USignBindingsFunctions::GetBindingPrefixString();
		for(auto& Var : Variables)
		{
			if(Var.Name == VariableName)
			{

				//If the value is not an input binding, call the constant value fallback
				if(!Var.Data.StartsWith(BindingString))
				{
					ConstantValueFallback.ExecuteIfBound(Var.Data, Var.MetaData);
					return;
				}

				
				const auto BindingName = USignBindingsFunctions::GetBindingNameFromString(Var.Data);
				//Otherwise add it to the bindings array to be called later during updates
				VariableToBuildableInputMap.Add(VariableName, FVariableValueBindingData(BindingName, Event, Var.DefaultValue, Var.MetaData));
				return;
			}
		}
	}

	UPROPERTY()
	TMap<TSubclassOf<USignComponentVariableName>, FVariableValueBindingData> VariableToBuildableInputMap;
};

