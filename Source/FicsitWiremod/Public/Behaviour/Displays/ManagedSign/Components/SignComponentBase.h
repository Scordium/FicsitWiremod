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

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
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

	UFUNCTION(BlueprintCallable)
	void LoadVariableValues(TArray<FSignComponentVariableData> SavedVariables)
	{
		for (auto SavedVariable : SavedVariables)
		{
			UpdateVariableValue(SavedVariable.Name, SavedVariable.Data);
		}
	}
	
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
