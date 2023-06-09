// 

#pragma once

#include "CoreMinimal.h"
#include "CCDynamicValueBase.h"
#include "WiremodReflection.h"
#include "CCArrayValueBase.generated.h"

/**
 * 
 */
UCLASS(Abstract, Blueprintable, BlueprintType)
class FICSITWIREMOD_API UCCArrayValueBase : public UCCDynamicValueBase
{
	GENERATED_BODY()

public:

	UCCArrayValueBase(){}
	explicit UCCArrayValueBase(EConnectionType Type) : Super(Type){}
	
	UFUNCTION(BlueprintCallable) virtual void AddElement(const FConnectionData& Element) { checkf(0, TEXT("Function ADD_ELEMENT is not implemented for this class!")) }
	UFUNCTION(BlueprintCallable) virtual void Clear() { checkf(0, TEXT("Function CLEAR_ARRAY is not implemented for this class!")) }
	UFUNCTION(BlueprintCallable) virtual UCCDynamicValueBase* GetElement(int Index) { checkf(0, TEXT("Function GET_ELEMENT is not implemented for this class!")) return nullptr;}
	UFUNCTION(BlueprintCallable) virtual void InsertElement(const FConnectionData& Element, int Index) { checkf(0, TEXT("Function INSERT_ELEMENT is not implemented for this class!")) }
	UFUNCTION(BlueprintCallable) virtual int Length() { checkf(0, TEXT("Function ARRAY_LENGTH is not implemented for this class!")) return 0;}
	UFUNCTION(BlueprintCallable) virtual void RemoveElement(int Index) { checkf(0, TEXT("Function REMOVE_ELEMENT is not implemented for this class!")) }
	UFUNCTION(BlueprintCallable) virtual void Resize(int NewSize) { checkf(0, TEXT("Function RESIZE_ARRAY is not implemented for this class!")) }
	UFUNCTION(BlueprintCallable) virtual void SetElement(const FConnectionData& Element, int Index) { checkf(0, TEXT("Function SET_ELEMENT is not implemented for this class!")) }
};
