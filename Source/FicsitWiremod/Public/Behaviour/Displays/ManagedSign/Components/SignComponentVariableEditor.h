// 

#pragma once

#include "CoreMinimal.h"
#include "SignBindingsProvider.h"
#include "SignComponentDescriptor.h"
#include "Blueprint/DragDropOperation.h"
#include "Kismet/GameplayStatics.h"
#include "SignComponentVariableEditor.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnComponentVariableValueChanged, UUserWidget*, VariableEditorWidget, const FSignComponentVariableData&, Data);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FOnComponentVariableMetadataChanged, UUserWidget*, VariableEditorWidget, TSubclassOf<USignComponentVariableName>, Name, FName, MetadataName, FString, NewValue);

UENUM(Blueprintable, BlueprintType)
enum EVariableEditorType
{
	ConstantOnly,
	BindOnly,
	BindOrConstant
};


DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnBindingSelected, const FString&, BindingValue);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnBindingReset);

UCLASS(Blueprintable, BlueprintType)
class USignVariableBindingHandler : public UUserWidget
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable)
	bool TryResolveBinding(const FString& Value)
	{
		bool Success;
		FManagedSignConnectionSettings NewData = USignBindingsFunctions::ResolveBinding(BindingsProvider, Value, Success);

		if(!Success || !IsCompatibleBinding(NewData))
		{
			ConfigureIdleStyle();
			return false;
		}
		else
		{
			Data = NewData;

			const auto BindingString = USignBindingsFunctions::GetBindingPrefixString() + Data.InternalName.ToString();
			OnSelected.Broadcast(BindingString);
			
			ConfigureBindingStyle();
			return true;
		}
	}

	UFUNCTION(BlueprintCallable)
	void TryUpdateBinding(const FManagedSignConnectionSettings& NewData)
	{
		if(Data.InternalName != NewData.InternalName || !IsCompatibleBinding(NewData))
		{
			ConfigureIdleStyle();
		}
		else
		{
			Data = NewData;

			const auto BindingString = USignBindingsFunctions::GetBindingPrefixString() + Data.InternalName.ToString();
			OnSelected.Broadcast(BindingString);
			
			ConfigureBindingStyle();
		}
	}

	UFUNCTION(BlueprintPure)
	bool IsCompatibleBinding(const FManagedSignConnectionSettings& TestData)
	{
		return UConnectionTypeFunctions::IsValidConnectionPair(AllowedType, TestData.Type);
	}

	UPROPERTY(BlueprintReadWrite, EditInstanceOnly)
	TEnumAsByte<EConnectionType> AllowedType;

	UPROPERTY(BlueprintReadWrite)
	TScriptInterface<ISignComponentVariableBindingsProvider> BindingsProvider;

	UPROPERTY(BlueprintReadWrite)
	FManagedSignConnectionSettings Data;

	UPROPERTY(BlueprintAssignable)
	FOnBindingSelected OnSelected;

	UPROPERTY(BlueprintAssignable)
	FOnBindingReset OnReset;
protected:

	UFUNCTION(BlueprintImplementableEvent)
	void ConfigureBindingStyle();

	UFUNCTION(BlueprintImplementableEvent)
	void ConfigureIdleStyle();

	UFUNCTION(BlueprintCallable)
	void ResetBinding()
	{
		Data = FManagedSignConnectionSettings();
		ConfigureIdleStyle();
		OnReset.Broadcast();
	}
};

UENUM(Blueprintable, BlueprintType)
enum ESignBindingDragEvent
{
	Started,
	Ended,
	EnterValid,
	EnterInvalid,
	Leave,
	Drop
};

UCLASS(Blueprintable, BlueprintType)
class FICSITWIREMOD_API USignComponentVariableEditor : public USignComponentVariableEditorBase
{
	GENERATED_BODY()

public:

	void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override
	{
		auto DeltaChange = InDeltaTime * (1/.12);
		if(BindingDragState == 2) DeltaChange *= -1;

		OpacityAlpha = FMath::Clamp(OpacityAlpha + DeltaChange, 0.0, 1.0);

		auto NewOpacity = FMath::InterpEaseOut(.2, 1.0, OpacityAlpha, 1.0);
		SetRenderOpacity(NewOpacity);
	}

	virtual void NativeOnDragEnter(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override
	{
		if(EditorType == ConstantOnly) return;
		if(auto BindingPayload = Cast<USignBindingPayload>(InOperation->Payload))
		{
			if(auto Handler = GetBindingHandlerWidget(); Handler && Handler->IsCompatibleBinding(BindingPayload->Data))
				OnBindingDragEvent(EnterValid, BindingPayload);
		}
	}

	virtual void NativeOnDragLeave(const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override
	{
		OnBindingDragEvent(Leave, InOperation->Payload);
	}

	virtual bool NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override
	{
		if(EditorType == ConstantOnly) return false;
		if(auto BindingPayload = Cast<USignBindingPayload>(InOperation->Payload))
		{
			if(auto Handler = GetBindingHandlerWidget())
			{
				const auto BindingString = USignBindingsFunctions::GetBindingPrefixString() + BindingPayload->Data.InternalName.ToString();
				if(Handler->TryResolveBinding(BindingString))
				{
					OnBindingDragEvent(Drop, BindingPayload);
					return true;
				}
			}
		}

		return false;
	}

	int BindingDragState = 0;
	double OpacityAlpha = 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(ExposeOnSpawn="true"))
	TScriptInterface<ISignComponentVariableBindingsProvider> BindingsProvider;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Editor Settings")
	TEnumAsByte<EVariableEditorType> EditorType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(ExposeOnSpawn="true"))
	FSignComponentVariableData VariableData;
	
	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FOnComponentVariableValueChanged OnComponentVariableValueChanged;

	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FOnComponentVariableMetadataChanged OnComponentVariableMetadataChanged;

	/*
	 * Function that initializes editor (such as unchanging values and configurations)
	 */
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void InitializeEditor(const FSignComponentVariableData& Data);

	/*
	 * Function that sets the value in editor's interface.
	 * This should be implemented in a way that allows to call it multiple times, as container window might call it again on component's request.
	 */
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void InitializeValue(const FString& Value);
	
	/*
	 * Returns widget that is responsible for setting constant (not binding) value
	 */
	UFUNCTION(BlueprintPure, BlueprintImplementableEvent)
	UUserWidget* GetValueWidget();

	/*
	 * Returns widget that is responsible for setting binding for this variable
	 */
	UFUNCTION(BlueprintPure, BlueprintImplementableEvent)
	USignVariableBindingHandler* GetBindingHandlerWidget();


	UFUNCTION(BlueprintCallable)
	void OnBindingDragStarted(UObject* Payload)
	{
		if(auto BindingPayload = Cast<USignBindingPayload>(Payload))
		{
			if(auto Handler = GetBindingHandlerWidget())
			{
				OnBindingDragEvent(Started, BindingPayload);
				if(Handler->IsCompatibleBinding(BindingPayload->Data) && EditorType != ConstantOnly) BindingDragState = 1;
				else BindingDragState = 2;
			}
		}
	}

	UFUNCTION(BlueprintCallable)
	void OnBindingDragEnded()
	{
		OnBindingDragEvent(Ended, nullptr);
		BindingDragState = 0;
	}

	UFUNCTION(BlueprintImplementableEvent)
	void OnBindingDragEvent(ESignBindingDragEvent Event, UObject* Payload);
protected:

	/*
	 * Used to bind variables to inputs.
	 * When a variable is bound, its const value is moved to the "Default value" which is used in editor previews and in cases when the binding is not connected yet.
	 * If the variable is already bound when handler broadcasts a change, does not replace the default value and just updates current binding.
	 */
	UFUNCTION(BlueprintCallable)
	void ProcessBindingHandlerUpdate(const FString& NewValue)
	{
		if(!USignBindingsFunctions::IsBinding(VariableData.Data)) VariableData.DefaultValue = VariableData.Data;
		VariableData.Data = NewValue;
		OnComponentVariableValueChanged.Broadcast(this, VariableData);
	}

	/*
	 * Used to unbind variables from inputs.
	 * When handler broadcasts binding clear event, this will move const value from "Default value" variable to the Data variable, essentially making variable const again.
	 */
	UFUNCTION(BlueprintCallable)
	void ProcessBindingClearCall()
	{
		if(USignBindingsFunctions::IsBinding(VariableData.Data))
		{
			VariableData.Data = VariableData.DefaultValue;
			VariableData.DefaultValue = "";	
			OnComponentVariableValueChanged.Broadcast(this, VariableData);
		}
	}
	
	/* 
	 * Updates data, then broadcasts a data update to the component when called, can be used to update a displayed value or to simply refresh the current one
	 */
	UFUNCTION(BlueprintCallable)
	void UpdateData(const FString& Value)
	{
		VariableData.SetValue(Value);
		OnComponentVariableValueChanged.Broadcast(this, VariableData);
	}
	
	/* 
	 * Updates metadata, then broadcasts a metadata update to the component when called, can be used to update some extra parameter 
	 */
	UFUNCTION(BlueprintCallable)
	void UpdateMetadata(FName MetadataName, FString Value)
	{
		SetMetadata(MetadataName, Value);
		OnComponentVariableMetadataChanged.Broadcast(this, VariableData.Name, MetadataName, Value);
	}

	UFUNCTION(BlueprintCallable)
	void SetMetadata(FName MetadataName, FString Value)
	{
		for(auto& MetadataEntry : VariableData.MetaData)
		{
			if(MetadataEntry.Name == MetadataName)
			{
				MetadataEntry.Value = Value;
				return;
			}
		}
		ACircuitryLogger::DispatchWarningEvent("Failed to find metadata with name \"" + MetadataName.ToString() + "\" for variable " + VariableData.Name->GetName());
	}

	UFUNCTION(BlueprintPure)
	FString GetMetadata(FName MetadataName, bool& Success)
	{
		for(auto& MetadataEntry : VariableData.MetaData)
		{
			Success = MetadataEntry.Name == MetadataName;
			if(Success) return MetadataEntry.Value;
		}

		return "";
	}

	UFUNCTION(BlueprintPure)
	FText GetName()
	{
		const auto CDO = VariableData.Name.GetDefaultObject();
		return CDO ? CDO->GetName() : FText();
	}
};
