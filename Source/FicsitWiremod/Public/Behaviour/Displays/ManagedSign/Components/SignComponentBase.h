// 

#pragma once

#include "CoreMinimal.h"
#include "SignComponentDescriptor.h"
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
	UPanelWidget* GetVariablesContainer();

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	void UpdateVariable(TSubclassOf<USignComponentVariableName> Name, const FString& NewValue);

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	void UpdateMetaData(TSubclassOf<USignComponentVariableName> Name, const TArray<FSignComponentVariableMetaData>& NewMetaData);

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	bool CanOpenContextMenu();

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	void DuplicateComponent(const FSignComponentData& ComponentData);

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	TArray<UUserWidget*> GetAllComponentsOfClass(TSubclassOf<USignComponentDescriptor> ComponentClass);
};

DECLARE_DYNAMIC_DELEGATE_OneParam(FUpdateEditorVariableValue, const FString&, NewValue);

UCLASS(BlueprintType, Blueprintable)
class FICSITWIREMOD_API USignEditorComponentBase : public UUserWidget
{
	GENERATED_BODY()
	
public:

	//For some reason UE refuses to tick with blueprint function, so i have to do this shit...
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override { CustomTick(MyGeometry, InDeltaTime); }

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(ExposeOnSpawn="true"))
	TSubclassOf<USignComponentDescriptor> ComponentDescriptor;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FSignComponentVariableData> Variables;

	UFUNCTION(BlueprintPure, BlueprintNativeEvent)
	FString GetComponentName();
	FString GetComponentName_Implementation(){ return ComponentDescriptor.GetDefaultObject()->DisplayName.ToString(); }
	
	UFUNCTION(BlueprintPure, BlueprintNativeEvent)
	FSignComponentData CompileComponent();
	FSignComponentData CompileComponent_Implementation() { return FSignComponentData(ComponentDescriptor, Variables); }
	
	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FOnComponentFocusChanged OnComponentFocusChanged;

	UPROPERTY(BlueprintReadWrite, meta=(ExposeOnSpawn="true"))
	TScriptInterface<IManagedSignEditorWindow> Parent;

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void OnComponentTargeted();

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void OnComponentUntargeted();

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	void InstantiateVariableEditors(UWidget* VariableEditorsContainer);

	UFUNCTION(BlueprintCallable)
	void LoadVariableValues(TArray<FSignComponentVariableData> SavedVariables)
	{
		for (auto SavedVariable : SavedVariables)
		{
			//This is a ridiculously ineffective implementation, but i don't think anyone will reach a scenario where this would introduce noticeable lag spike.
			UpdateVariableValue(SavedVariable.Name, SavedVariable.Data);
			for(auto Metadata : SavedVariable.MetaData) UpdateVariableMetadata(SavedVariable.Name, Metadata.Name, Metadata.Value);
		}
	}
	
protected:
	
	UFUNCTION(BlueprintImplementableEvent)
	void CustomTick(const FGeometry& MyGeometry, double DeltaTime);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void UpdateVariableValue(TSubclassOf<USignComponentVariableName> Name, const FString& Value);
	void UpdateVariableValue_Implementation(TSubclassOf<USignComponentVariableName> Name, const FString& Value)
	{
		for(auto& Var : Variables)
		{
			if(Var.Name == Name) Var.Data = Value;
		}

		auto Delegate = EditorVariableBindings.Find(Name);
		if(Delegate) Delegate->ExecuteIfBound(Value);
	}

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void UpdateVariableMetadata(TSubclassOf<USignComponentVariableName> Name, FName MetadataName, const FString& Value);
	void UpdateVariableMetadata_Implementation(TSubclassOf<USignComponentVariableName> Name, FName MetadataName, const FString& Value)
	{
		for(auto& Var : Variables)
		{
			if(Var.Name == Name)
			{
				for(auto& Meta : Var.MetaData)
				{
					if(Meta.Name == MetadataName)
					{
						Meta.Value = Value;
						return;
					}
				}
				Var.MetaData.Add(FSignComponentVariableMetaData(MetadataName, Value));
			}
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

	UFUNCTION(BlueprintPure)
	FString GetVariableMeta(TSubclassOf<USignComponentVariableName> Name, FName MetadataName, bool& Success)
	{
		for(auto& Var : Variables)
		{
			if(Var.Name == Name)
			{
				for(auto Meta : Var.MetaData)
				{
					Success = Meta.Name == MetadataName;
					if(Success) return Meta.Value;
				}
			}
		}

		return FString();
	}

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void InitializeComponent();

	UFUNCTION(BlueprintCallable)
	void BindVariableToDelegate(TSubclassOf<USignComponentVariableName> Variable, FUpdateEditorVariableValue Event)
	{
		EditorVariableBindings.Add(Variable, Event);
	}

private:

	UPROPERTY()
	TMap<TSubclassOf<USignComponentVariableName>, FUpdateEditorVariableValue> EditorVariableBindings;
};


DECLARE_DYNAMIC_DELEGATE_TwoParams(FOnComponentVariableUpdate, FConnectionData, Data, const TArray<FSignComponentVariableMetaData>&, Meta);
DECLARE_DYNAMIC_DELEGATE_TwoParams(FOnRegisteredConstantVariable, FString, Data, const TArray<FSignComponentVariableMetaData>&, MetaData);

USTRUCT()
struct FVariableValueBindingData
{
	GENERATED_BODY()

public:

	UPROPERTY()
	int InputIndex;

	UPROPERTY()
	FOnComponentVariableUpdate Event;

	UPROPERTY()
	TArray<FSignComponentVariableMetaData> MetaData;

	void Call(AFGWiremodBuildable* Sign) const { if(Sign) Event.ExecuteIfBound(Sign->GetConnection(InputIndex), MetaData); }

	FVariableValueBindingData(){}
	
	FVariableValueBindingData(int InputIndex, FOnComponentVariableUpdate Event, TArray<FSignComponentVariableMetaData> MetaData)
	{
		this->InputIndex = InputIndex;
		this->Event = Event;
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
		for(auto EventData : VariableToBuildableInputMap)
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
				for(auto Meta : Var.MetaData)
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
	void TryBindFunctionToUpdate(FOnComponentVariableUpdate Event, FOnRegisteredConstantVariable ConstantValueFallback, TSubclassOf<USignComponentVariableName> VariableName)
	{
		auto const BindingString = USignComponentUtilityFunctions::GetBindingPrefixString();
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


				//Otherwise add it to the bindings array to be called later during updates
				auto VariableBindingValue = Var.Data.Replace(*BindingString, *FString());

				int Input = FCString::Atoi(*VariableBindingValue);

				VariableToBuildableInputMap.Add(VariableName, FVariableValueBindingData(Input, Event, Var.MetaData));
				return;
			}
		}
	}

	UPROPERTY()
	TMap<TSubclassOf<USignComponentVariableName>, FVariableValueBindingData> VariableToBuildableInputMap;
};

