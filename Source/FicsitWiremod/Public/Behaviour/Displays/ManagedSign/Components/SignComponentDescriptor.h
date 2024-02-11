// 

#pragma once

#include "CoreMinimal.h"
#include "SignBindingsProvider.h"
#include "SignComponentVariableName.h"
#include "UObject/Object.h"
#include "SignComponentDescriptor.generated.h"


/**
 * 
 */
UCLASS(BlueprintType, Blueprintable)
class FICSITWIREMOD_API USignComponentDescriptor : public UObject
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText DisplayName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText Description;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UTexture2D* Icon;
	
	UFUNCTION(BlueprintImplementableEvent)
	UUserWidget* GetEditorComponentTooltipWidget();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<UUserWidget> EditorWidget;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<UUserWidget> SignWidget;
};

USTRUCT(BlueprintType)
struct FSignComponentVariableMetaData
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame)
	FName Name;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame)
	FString Value;

	FSignComponentVariableMetaData(){}
	FSignComponentVariableMetaData(FName InName, FString InValue) : Name(InName), Value(InValue){}

	bool operator==(const FSignComponentVariableMetaData& Other) const
	{
		return Name == Other.Name && Value == Other.Value;
	}
	
};

USTRUCT(Blueprintable, BlueprintType)
struct FSignComponentVariableEditorMetadata
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame)
	FString Category;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame)
	bool GroupWithSameEntries;

	bool operator==(const FSignComponentVariableEditorMetadata& Other) const
	{
		return Category == Other.Category
		&& GroupWithSameEntries == Other.GroupWithSameEntries;
	}
};

USTRUCT(BlueprintType)
struct FSignComponentVariableData
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame)
	TSubclassOf<USignComponentVariableName> Name;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame)
	FString Data;

	/*
	 * Value that will be used in editor preview when this variable is bound to some input.
	 */
	UPROPERTY(VisibleInstanceOnly, BlueprintReadWrite, SaveGame)
	FString DefaultValue = "[BIND PLACEHOLDER]";

	UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame)
	TArray<FSignComponentVariableMetaData> MetaData;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame)
	FSignComponentVariableEditorMetadata EditorMetadata;

	bool operator==(const FSignComponentVariableData& Other) const
	{
		return
		Name == Other.Name
		&& Data == Other.Data
		&& DefaultValue == Other.DefaultValue
		&& MetaData == Other.MetaData
		&& EditorMetadata == Other.EditorMetadata;
	}

	void SetValue(const FString& NewValue)
	{
		if(!USignBindingsFunctions::IsBinding(Data)) Data = NewValue;
		else DefaultValue = NewValue;
	}
};

USTRUCT(Blueprintable, BlueprintType)
struct FSignComponentEditorMetadata
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame)
	FText ComponentName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame)
	FString Category;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame)
	bool CanInteract = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame)
	bool Visible = true;

	bool operator==(const FSignComponentEditorMetadata& Other) const
	{
		return ComponentName.EqualTo(Other.ComponentName)
		&& Category == Other.Category
		&& CanInteract == Other.CanInteract
		&& Visible == Other.Visible;
	}
};

USTRUCT(BlueprintType)
struct FSignComponentData
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame)
	TSubclassOf<USignComponentDescriptor> ComponentDescriptor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame)
	TArray<FSignComponentVariableData> Variables;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame)
	FSignComponentEditorMetadata Metadata;

	FSignComponentData(){}
	FSignComponentData(TSubclassOf<USignComponentDescriptor> Descriptor, const TArray<FSignComponentVariableData>& InVariables) : ComponentDescriptor(Descriptor), Variables(InVariables) {}

	bool operator==(const FSignComponentData& Other) const
	{
		return
		ComponentDescriptor == Other.ComponentDescriptor
		&& Variables == Other.Variables
		&& Metadata == Other.Metadata;
	}
};


UCLASS()
class USignComponentUtilityFunctions : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintPure)
	static UUserWidget* GetTooltipWidget(const FSignComponentData& Data)
	{
		return Data.ComponentDescriptor.GetDefaultObject()->GetEditorComponentTooltipWidget();
	}

	UFUNCTION(BlueprintPure)
	static TSubclassOf<UUserWidget> GetEditorWidget(const FSignComponentData& Data)
	{
		if(auto CDO = Data.ComponentDescriptor.GetDefaultObject()) return CDO->EditorWidget;
		return nullptr;
	}

	UFUNCTION(BlueprintPure)
	static TSubclassOf<UUserWidget> GetRuntimeWidget(const FSignComponentData& Data)
	{
		if(auto CDO = Data.ComponentDescriptor.GetDefaultObject()) return CDO->SignWidget;
		return nullptr;
	}

	UFUNCTION(BlueprintPure)
	static FString GetConstValue(const FSignComponentVariableData& Variable)
	{
		if(!USignBindingsFunctions::IsBinding(Variable.Data)) return Variable.Data;
		else return Variable.DefaultValue;
	}
	
	UFUNCTION(BlueprintPure, meta=(ScriptOperator="=="), DisplayName="Compare sign component variable")
	static bool VariableEquals(const FSignComponentVariableData& Data1, const FSignComponentVariableData& Data2){ return Data1 == Data2; }

	UFUNCTION(BlueprintPure, meta=(ScriptOperator="=="), DisplayName="Compare sign component")
	static bool ComponentEquals(const FSignComponentData& Data1, const FSignComponentData& Data2) { return Data1 == Data2; }

	UFUNCTION(BlueprintPure)
	static FString FindMetadataValue(const TArray<FSignComponentVariableMetaData>& MetadataArray, FName MetadataName, bool& Success)
	{
		for(auto& MetaEntry : MetadataArray)
		{
			Success = MetaEntry.Name == MetadataName;
			if(Success) return MetaEntry.Value;
		}

		return "";
	}
};
