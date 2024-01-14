// 

#pragma once

#include "CoreMinimal.h"
#include "Behaviour/FGWiremodBuildable.h"
#include "CommonLib/DynamicValues/CCDynamicValueUtils.h"
#include "ArrayMake.generated.h"

UCLASS()
class FICSITWIREMOD_API AArrayMake : public AFGWiremodBuildable, public IDynamicValuePasser
{
	GENERATED_BODY()

public:

	virtual void BeginPlay() override
	{
		Super::BeginPlay();
		MakeInputList();
	}
	
	virtual void ServerProcess_Implementation(double DeltaTime) override
	{
		TArray<FConnectionData> Elements;
		GetAllConnected(Elements);

		auto Array = Cast<UCCArrayValueBase>(Out);
		if(Elements.Num() == 0 || !Array)
		{
			SetOutputType(0, Unknown);
			return;
		}

		Array->Clear();
		for(auto& Element : Elements) Array->AddElement(Element);
	}


	virtual void GetLifetimeReplicatedProps( TArray<FLifetimeProperty>& OutLifetimeProps ) const override
	{
		Super::GetLifetimeReplicatedProps(OutLifetimeProps);
    
		DOREPLIFETIME(AArrayMake, Out);
	}

	virtual void GatherReplicatedObjects_Implementation(TArray<UObject*>& OutObjects) override
	{
		Super::GatherReplicatedObjects_Implementation(OutObjects);
		OutObjects.Add(Out);
	}

	virtual UObject* GetValue_Implementation(const FString& ValueName) override{ return Out; }
	
	UPROPERTY(Replicated, SaveGame, VisibleInstanceOnly)
	UCCDynamicValueBase* Out;

	UPROPERTY(EditDefaultsOnly)
	FText ItemDisplayNameFormat = FText::FromString("Item");

	virtual void OnInputConnected_Implementation(const FConnectionData& Data, int Index, UObject* Setter) override
	{
		Super::OnInputConnected_Implementation(Data, Index, Setter);
		MakeInputList();
	}

	virtual void OnInputDisconnected_Implementation(int Index, UObject* Setter) override
	{
		Super::OnInputDisconnected_Implementation(Index, Setter);
		MakeInputList();
	}
	
	void MakeInputList()
	{
		for(int i = InputConnections.Num() - 1; i >= 0; i--)
		{
			if(InputConnections[i].IsValid())
			{
				GenerateInputList(InputConnections[i].ConnectionType, i + 1);
				return;
			}
		}

		GenerateInputList(AnyNonArray);
	}


	void GenerateInputList(EConnectionType OfType, int InputCount = 8)
	{
		ConnectionsInfo.Inputs.Empty();
		if(InputCount < 8) InputCount = 8;

		for(int i = 0; i <= InputCount; i++)
		{
			auto item = FBuildingConnection(
				ItemDisplayNameFormat.ToString() + " " + FString::FromInt(i),
				"",
				OfType);
			ConnectionsInfo.Inputs.Add(item);
		}

		if(OfType == AnyNonArray) SetOutputType(0, Unknown);
		else
		{
			Out = UCCDynamicValueUtils::FromType(UConnectionTypeFunctions::BaseToArray(OfType), Out ? Out->GetWorld() : this->GetWorld());
			SetOutputType(0, UConnectionTypeFunctions::BaseToArray(OfType));
		}
	}
};
