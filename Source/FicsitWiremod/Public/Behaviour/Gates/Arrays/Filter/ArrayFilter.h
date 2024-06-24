// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Behaviour/FGWiremodBuildable.h"
#include "CommonLib/DynamicValues/CCDynamicValueUtils.h"
#include "Filters/CircuitryArrayFilterBase.h"
#include "ArrayFilter.generated.h"

UCLASS()
class FICSITWIREMOD_API AArrayFilter : public AFGWiremodBuildable, public IDynamicValuePasser
{
	GENERATED_BODY()

public:
	virtual void ServerProcess_Implementation(double DeltaTime) override
	{
		const auto& Connection = GetConnection(0);
		if(!Out || Connection.ConnectionType != Out->ConnectionType)
		{
			Out = nullptr;
			SetOutputType(0, Unknown);
			if(auto NewValue = Cast<UCCArrayValueBase>(UCCDynamicValueUtils::FromType(Connection.ConnectionType, this)))
			{
				IsFilterActive = NewValue->SetFilter(FilterData);
				Out = NewValue;
			}
		}

		if(Out)
		{
			Out->FromConnectionValue(Connection.Object, Connection.FunctionName, Connection.FromProperty);
			if(IsFilterActive) Out->ApplyFilter();
			SetOutputType(0, Out->ConnectionType);
		}
	}

	virtual UObject* GetValue_Implementation(const FString& ValueName) override { return Out; }

	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override
	{
		Super::GetLifetimeReplicatedProps(OutLifetimeProps);

		DOREPLIFETIME(ThisClass, Out);
	}

	virtual void GatherReplicatedObjects_Implementation(TArray<UObject*>& Output) override
	{
		Super::GatherReplicatedObjects_Implementation(Output);
		Output.Add(Out);
	}

	UFUNCTION(BlueprintCallable)
	void SetFilter(const FCircuitryArrayFilterData& NewFilterData, UObject* Setter)
	{
		if(!GetCanConfigure(Setter)) return;
		
		SetFilter_Internal(NewFilterData);
	}

	void SetFilter_Internal(const FCircuitryArrayFilterData& NewFilterData)
	{
		if(FilterData != NewFilterData)
		{
			IsFilterActive = Out->SetFilter(NewFilterData);
			FilterData = NewFilterData;
		}
	}
	
	UPROPERTY(Replicated, BlueprintReadWrite, SaveGame)
	UCCArrayValueBase* Out;

	UPROPERTY(BlueprintReadWrite, SaveGame)
	bool IsFilterActive;
	
	UPROPERTY(BlueprintReadWrite)
	FCircuitryArrayFilterData FilterData;
};
