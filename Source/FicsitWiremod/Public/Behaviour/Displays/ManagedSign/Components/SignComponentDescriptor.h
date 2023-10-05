// 

#pragma once

#include "CoreMinimal.h"
#include "SignComponentVariableEditor.h"
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
struct FSignComponentVariableData
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame)
	TSubclassOf<USignComponentVariableName> Name;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame)
	FString Data;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame)
	TArray<FSignComponentVariableMetaData> MetaData;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame)
	TSubclassOf<USignComponentVariableEditor> ValueEditorClass;

	bool operator==(const FSignComponentVariableData& Other) const
	{
		return
		Name == Other.Name
		&& Data == Other.Data
		&& ValueEditorClass == Other.ValueEditorClass;
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

	bool operator==(const FSignComponentData& Other) const
	{
		return
		ComponentDescriptor == Other.ComponentDescriptor
		&& Variables == Other.Variables;
	}
};


UCLASS()
class USignComponentUtilityFunctions : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintPure)
	static FString GetBindingPrefixString() { return "\u25C8"; }

	UFUNCTION(BlueprintPure)
	static UUserWidget* GetTooltipWidget(TSubclassOf<USignComponentDescriptor> Descriptor)
	{
		return Descriptor.GetDefaultObject()->GetEditorComponentTooltipWidget();
	}

	UFUNCTION(BlueprintPure)
	static TSubclassOf<UUserWidget> GetEditorWidget(TSubclassOf<USignComponentDescriptor> Descriptor)
	{
		if(auto CDO = Descriptor.GetDefaultObject()) return CDO->EditorWidget;
		return nullptr;
	}

	UFUNCTION(BlueprintPure)
	static TSubclassOf<UUserWidget> GetRuntimeWidget(TSubclassOf<USignComponentDescriptor> Descriptor)
	{
		if(auto CDO = Descriptor.GetDefaultObject()) return CDO->SignWidget;
		return nullptr;
	}
	
	UFUNCTION(BlueprintPure, meta=(ScriptOperator="=="), DisplayName="Compare sign component variable")
	static bool VariableEquals(const FSignComponentVariableData& Data1, const FSignComponentVariableData& Data2){ return Data1 == Data2; }

	UFUNCTION(BlueprintPure, meta=(ScriptOperator="=="), DisplayName="Compare sign component")
	static bool ComponentEquals(const FSignComponentData& Data1, const FSignComponentData& Data2) { return Data1 == Data2; }

	UFUNCTION(BlueprintPure)
	static FString FindMetadataValue(const TArray<FSignComponentVariableMetaData>& MetadataArray, FName MetadataName, bool& Success)
	{
		for(auto MetaEntry : MetadataArray)
		{
			Success = MetaEntry.Name == MetadataName;
			if(Success) return MetaEntry.Value;
		}

		return "";
	}
};
