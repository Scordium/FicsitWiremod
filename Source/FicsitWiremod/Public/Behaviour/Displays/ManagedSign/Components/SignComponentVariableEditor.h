// 

#pragma once

#include "CoreMinimal.h"
#include "SignComponentVariableName.h"
#include "Blueprint/UserWidget.h"
#include "CommonLib/ConnectionType.h"
#include "SignComponentVariableEditor.generated.h"

USTRUCT(BlueprintType)
struct FSignComponentVariableMetaData
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame)
	FName Name;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame)
	FString Value;

	bool operator==(const FSignComponentVariableMetaData& Other) const
	{
		return Name == Other.Name && Value == Other.Value;
	}
	
};


DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnComponentVariableValueChanged, UUserWidget*, VariableEditorWidget, TSubclassOf<USignComponentVariableName>, Name, FString, NewValue);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FOnComponentVariableMetadataChanged, UUserWidget*, VariableEditorWidget, TSubclassOf<USignComponentVariableName>, Name, FName, MetadataName, FString, NewValue);

USTRUCT(BlueprintType)
struct FManagedSignConnectionSettings
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame)
	FString Name;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame)
	TEnumAsByte<EConnectionType> Type;


	bool operator==(const FManagedSignConnectionSettings& Other) const
	{
		return Name == Other.Name && Type == Other.Type;
	}
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

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(ExposeOnSpawn="true"))
	TArray<FSignComponentVariableMetaData> Metadata;
	
	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FOnComponentVariableValueChanged OnComponentVariableValueChanged;

	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FOnComponentVariableMetadataChanged OnComponentVariableMetadataChanged;

	/* 
	 *	Call this after constructing the widget if some of your components (such as spin boxes) don't work properly
	 */
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void InitializeEditor(const FString& Value);

protected:
	
	/*
	 * Broadcasts an update to the component when called, can be used to update a displayed value or to simply refresh the current one
	 */
	UFUNCTION(BlueprintCallable)
	void CallChanged() { OnComponentVariableValueChanged.Broadcast(this, Name, Data); }

	/* 
	 * Updates metadata, then broadcasts a metadata update to the component when called, can be used to update some extra parameter 
	 */
	UFUNCTION(BlueprintCallable)
	void UpdateMetadata(FName MetadataName, FString Value)
	{
		SetMetadata(MetadataName, Value);
		OnComponentVariableMetadataChanged.Broadcast(this, Name, MetadataName, Value);
	}

	UFUNCTION(BlueprintCallable)
	void SetMetadata(FName MetadataName, FString Value)
	{
		for(auto& MetadataEntry : Metadata)
		{
			if(MetadataEntry.Name == MetadataName)
			{
				MetadataEntry.Value = Value;
				return;
			}
		}
		ACircuitryLogger::DispatchWarningEvent("Failed to find metadata with name \"" + MetadataName.ToString() + "\" for variable " + Name->GetName());
	}

	UFUNCTION(BlueprintPure)
	FString GetMetadata(FName MetadataName, bool& Success)
	{
		for(auto& MetadataEntry : Metadata)
		{
			Success = MetadataEntry.Name == MetadataName;
			if(Success) return MetadataEntry.Value;
		}

		return "";
	}

	UFUNCTION(BlueprintPure)
	FText GetName() { return Name.GetDefaultObject()->GetName(); }
};
