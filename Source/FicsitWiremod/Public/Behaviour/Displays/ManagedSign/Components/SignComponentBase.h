﻿// 

#pragma once

#include "CoreMinimal.h"
#include "SignBindingsProvider.h"
#include "SignComponentDescriptor.h"
#include "SignComponentVariablesWindow.h"
#include "Behaviour/FGWiremodBuildable.h"
#include "Blueprint/UserWidget.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "CommonLib/VectorUtils.h"
#include "Components/CanvasPanelSlot.h"
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

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	void RefreshComponentList();

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	void ObserveSignComponent(UUserWidget* Component, bool AddToObservedList = false);

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	void StopObservingSignComponent(UUserWidget* Component);

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	int GetNumObservedComponents() const;

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	void BroadcastPositionDelta(const FVector2D& Delta);
};

DECLARE_DYNAMIC_DELEGATE_OneParam(FUpdateEditorVariableValue, const FSignComponentVariableData&, Data);

UCLASS(BlueprintType, Blueprintable)
class FICSITWIREMOD_API USignEditorComponentBase : public UUserWidget
{
	GENERATED_BODY()
	
public:

	//For some reason UE refuses to tick with blueprint function, so i have to do this shit...
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override
	{
		CustomTick(MyGeometry, InDeltaTime);

		if(CanMoveComponent())
		{
			const double CanvasScale = IManagedSignEditorWindow::Execute_GetCanvasScale(Parent.GetObject());
			const double CanvasGridSize = IManagedSignEditorWindow::Execute_GetCanvasGridSize(Parent.GetObject());
			const auto MousePosition = UWidgetLayoutLibrary::GetMousePositionOnViewport(this);
			
			auto GridMousePos = UVectorUtils::GridSnap2D(MousePosition / CanvasScale - ViewportOffset, CanvasGridSize);


			if(IManagedSignEditorWindow::Execute_GetNumObservedComponents(Parent.GetObject()) > 1)
				IManagedSignEditorWindow::Execute_BroadcastPositionDelta(Parent.GetObject(), GridMousePos - GetComponentPosition());
			else if(GridMousePos != GetComponentPosition()) SetPosition(GridMousePos);
		}
	}

	UFUNCTION(BlueprintImplementableEvent)
	bool CanMoveComponent() const;

	UFUNCTION()
	FVector2D GetComponentPosition() const { return Cast<UCanvasPanelSlot>(Slot)->GetPosition(); }

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	void SetPosition(const FVector2D& Pos, bool UpdateInEditor = true);

	UFUNCTION(BlueprintCallable)
	void NudgeComponent(const FVector2D& Axis, bool UpdateInEditor = true)
	{
		const auto NudgeAmount = Axis * IManagedSignEditorWindow::Execute_GetCanvasGridSize(Parent.GetObject());
		
		SetPosition(GetComponentPosition() + NudgeAmount, UpdateInEditor);
	}
	
	UFUNCTION(BlueprintCallable)
	void DeltaMovePosition(const FVector2D& Pos, bool UpdateInEditor = true) { SetPosition(GetComponentPosition() + Pos, UpdateInEditor); }

	UFUNCTION(BlueprintCallable)
	void UpdateMouseViewportOffset()
	{
		const auto MousePosition = UWidgetLayoutLibrary::GetMousePositionOnViewport(this);
		const double CanvasScale = IManagedSignEditorWindow::Execute_GetCanvasScale(Parent.GetObject());

		ViewportOffset = MousePosition / CanvasScale - GetComponentPosition();
	}
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FSignComponentData ComponentData;
	
	UFUNCTION(BlueprintPure, BlueprintNativeEvent)
	FText GetComponentName();
	FText GetComponentName_Implementation()
	{
		return ComponentData.Metadata.ComponentName.IsEmptyOrWhitespace()
		? ComponentData.ComponentDescriptor.GetDefaultObject()->DisplayName
		: ComponentData.Metadata.ComponentName;
	}

	UFUNCTION(BlueprintCallable)
	void SetComponentName(const FText& NewName) { ComponentData.Metadata.ComponentName = NewName; }

	UFUNCTION(BlueprintCallable)
	void SetComponentCategory(const FText& NewCategory)
	{
		ComponentData.Metadata.Category = NewCategory.ToString();
		IManagedSignEditorWindow::Execute_RefreshComponentList(Parent.GetObject());
	}
	
	UFUNCTION(BlueprintPure, BlueprintNativeEvent)
	FSignComponentData CompileComponent();
	FSignComponentData CompileComponent_Implementation() { return ComponentData; }
	
	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FOnComponentFocusChanged OnComponentFocusChanged;

	UPROPERTY(BlueprintReadWrite, meta=(ExposeOnSpawn="true"))
	TScriptInterface<IManagedSignEditorWindow> Parent;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void OnComponentTargeted();
	void OnComponentTargeted_Implementation()
	{
		OnComponentFocusChanged.Broadcast(this, true);
		if(const auto VariablesWindow = IManagedSignEditorWindow::Execute_GetVariablesEditor(Parent.GetObject()))
		{
			VariablesWindow->OnVariableUpdated.AddUniqueDynamic(this, &USignEditorComponentBase::HandleVariableUpdate);
			VariablesWindow->OnVariableMetadataUpdated.AddUniqueDynamic(this, &USignEditorComponentBase::HandleMetadataUpdate);
		}
	}

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void OnComponentUntargeted();
	void OnComponentUntargeted_Implementation()
	{
		OnComponentFocusChanged.Broadcast(this, false);
		if(const auto VariablesWindow = IManagedSignEditorWindow::Execute_GetVariablesEditor(Parent.GetObject()))
		{
			VariablesWindow->OnVariableUpdated.RemoveAll(this);
			VariablesWindow->OnVariableMetadataUpdated.RemoveAll(this);
		}
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
	void LoadData(const FSignComponentData& Data)
	{
		ComponentData.ComponentDescriptor = Data.ComponentDescriptor;
		ComponentData.Metadata = Data.Metadata;
		for(auto& NewVar : Data.Variables)
		{
			HandleVariableUpdate(NewVar);
			for(auto& NewVarMetadata : NewVar.MetaData)
			{
				UpdateVariableMetadata(NewVar.Name, NewVarMetadata.Name, NewVarMetadata.Value);
			}
		}
	}

	/*
	 * Gets called when this component pushes its variables to configuration window
	 */
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	void InstantiateVariableEditors();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void DestroyComponent();
	void DestroyComponent_Implementation()
	{
		OnComponentFocusChanged.Broadcast(this, false);
		RemoveFromParent();
		IManagedSignEditorWindow::Execute_StopObservingSignComponent(Parent.GetObject(), this);
		IManagedSignEditorWindow::Execute_RefreshComponentList(Parent.GetObject());
	}
	
protected:
	UFUNCTION(BlueprintImplementableEvent)
	void CustomTick(const FGeometry& MyGeometry, double DeltaTime);

	UFUNCTION(BlueprintCallable)
	void SubscribeToVariableUpdates(TSubclassOf<USignComponentVariableName> Variable, FUpdateEditorVariableValue Event) { EditorVariableBindings.Add(Variable, Event); }

	UFUNCTION(BlueprintCallable)
	void ForceRefreshVariableBinding(TSubclassOf<USignComponentVariableName> Variable)
	{
		for(auto& Var : ComponentData.Variables)
		{
			if(Var.Name == Variable)
			{
				if(auto Delegate = EditorVariableBindings.Find(Variable)) Delegate->ExecuteIfBound(Var);
				return;
			}
		}
	}
	
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

	UFUNCTION()
	void HandleMetadataUpdate(TSubclassOf<USignComponentVariableName> VariableName, const FSignComponentVariableMetaData& MetaData)
	{
		UpdateVariableMetadata(VariableName, MetaData.Name, MetaData.Value, false);
	}
	
	UFUNCTION(BlueprintCallable)
	void UpdateVariableValue(TSubclassOf<USignComponentVariableName> VariableName, const FString& NewValue, bool UpdateVariablesEditor = true)
	{
		if(UpdateVariablesEditor)
		{
			if(const auto VariablesWindow = IManagedSignEditorWindow::Execute_GetVariablesEditor(Parent.GetObject()))
				VariablesWindow->UpdateComponentValue(VariableName, NewValue);
		}
		
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
	void UpdateVariableMetadata(TSubclassOf<USignComponentVariableName> VariableName, FName MetadataName, const FString& NewValue, bool UpdateVariablesEditor = true);
	void UpdateVariableMetadata_Implementation(TSubclassOf<USignComponentVariableName> VariableName, FName MetadataName, const FString& NewValue, bool UpdateVariablesEditor = true)
	{
		if(UpdateVariablesEditor)
		{
			if(const auto VariablesWindow = IManagedSignEditorWindow::Execute_GetVariablesEditor(Parent.GetObject()))
				VariablesWindow->UpdateComponentMetaValue(VariableName, MetadataName, NewValue);
		}

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

	UFUNCTION(BlueprintCallable)
	bool GetVariable(TSubclassOf<USignComponentVariableName> VariableName, FSignComponentVariableData& Variable)
	{
		for(auto& Var : ComponentData.Variables)
		{
			if(Var.Name == VariableName)
			{
				Variable = Var;
				return true;
			}
		}
		
		return false;
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

	UPROPERTY()
	FVector2D ViewportOffset;
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
	bool GetVariable(TSubclassOf<USignComponentVariableName> Name, FSignComponentVariableData& Out)
	{
		for(auto& Var : Variables)
		{
			if(Var.Name == Name)
			{
				Out = Var;
				return true;
			}
		}

		return false;
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

