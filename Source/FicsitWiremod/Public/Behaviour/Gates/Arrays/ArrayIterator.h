// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Behaviour/FGWiremodBuildable.h"
#include "CommonLib/DynamicValues/CCArrayValueBase.h"
#include "CommonLib/DynamicValues/CCDynamicValueUtils.h"
#include "ArrayIterator.generated.h"

UCLASS()
class FICSITWIREMOD_API AArrayIterator : public AFGWiremodBuildable, public IDynamicValuePasser
{
	GENERATED_BODY()

public:
	virtual void ServerProcess_Implementation(double DeltaTime) override
	{
		ArrayCache = Cast<UCCArrayValueBase>(UCCDynamicValueUtils::FromValue(GetConnection(0), ArrayCache));

		if(ArrayCache && ArrayCache->Length() > 0)
		{
			if(Out == nullptr)
			{
				Out = ArrayCache->GetElement(CurrentElementIndex);
			}
			else
			{
				const bool IncrementIndex = GetConnection(1).GetBool();
				const bool DecrementIndex = GetConnection(2).GetBool();

				if(!IncrementIndex && !DecrementIndex) HasTicked = false;
				else
				{
					if(!HasTicked)
					{
						HasTicked = true;
					
						if(IncrementIndex)
						{
							CurrentElementIndex++;
							if(CurrentElementIndex >= ArrayCache->Length()) CurrentElementIndex = 0;
						}
						else if(DecrementIndex)
						{
							CurrentElementIndex--;
							if(CurrentElementIndex < 0) CurrentElementIndex = ArrayCache->Length() - 1;
						}


						Out = ArrayCache->GetElement(CurrentElementIndex);
					}
				}
			}
		}
		else
		{
			Out = nullptr;
			CurrentElementIndex = 0;
		}

		IsFirstElement = ArrayCache && CurrentElementIndex == 0;
		IsLastElement = ArrayCache ? CurrentElementIndex == (ArrayCache->Length() - 1) : false;
		SetOutputType(0, Out ? Out->ConnectionType.GetValue() : Unknown);
	}

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override
	{
		Super::GetLifetimeReplicatedProps(OutLifetimeProps);

		DOREPLIFETIME(AArrayIterator, Out);
		DOREPLIFETIME(AArrayIterator, CurrentElementIndex);
	}

	virtual UObject* GetValue_Implementation(const FString& ValueName) override{ return Out; }

	virtual void GatherReplicatedObjects_Implementation(TArray<UObject*>& OutObjects) override
	{
		Super::GatherReplicatedObjects_Implementation(OutObjects);

		OutObjects.Add(Out);
	}
	
	UPROPERTY(Replicated, SaveGame)
	UCCDynamicValueBase* Out;

	UPROPERTY(Replicated, SaveGame)
	int CurrentElementIndex;

	UPROPERTY() UCCArrayValueBase* ArrayCache;
	UPROPERTY() bool HasTicked;
	UPROPERTY() bool IsFirstElement;
	UPROPERTY() bool IsLastElement;
	
};
