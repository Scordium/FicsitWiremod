// 

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SignComponentVariableName.generated.h"

UCLASS(Abstract)
class FICSITWIREMOD_API USignComponentVariableEditorBase : public UUserWidget
{
	GENERATED_BODY()

public:
	
};

UCLASS(BlueprintType, Blueprintable)
class USignComponentVariableCategory : public UObject
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	FText Category;
};

UCLASS(BlueprintType, Blueprintable)
class USignComponentVariableName : public UObject
{
	GENERATED_BODY()

public:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FText Name;

	UFUNCTION(BlueprintPure)
	FText GetName() { return Name; }

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta=(MultiLine="true"))
	FText TooltipText;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Instanced)
	USignComponentVariableEditorBase* VariableEditor = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<USignComponentVariableCategory> Category;
};
