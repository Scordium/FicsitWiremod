// 

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "BaseHelpModule.generated.h"

/**
 * 
 */
UCLASS(BlueprintType, EditInlineNew, DefaultToInstanced)
class FICSITWIREMOD_API UBaseHelpModule : public UObject
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void Execute();
	virtual void Execute_Implementation(){}

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void ExecuteOnObject(UObject* Object);
	virtual void ExecuteOnObject_Implementation(UObject* Object) { Execute(); }

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void ExecuteOnClass(TSubclassOf<UObject> Class);
	virtual void ExecuteOnClass_Implementation(TSubclassOf<UObject> Class) { Execute(); }
};
