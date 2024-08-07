// 

#pragma once

#include "CoreMinimal.h"
#include "Behaviour/FGWiremodBuildable.h"
#include "CommonLib/DynamicValues/CCDynamicValueUtils.h"
#include "ArrayInsert.generated.h"

UCLASS()
class FICSITWIREMOD_API AArrayInsert : public AFGWiremodBuildable, public IDynamicValuePasser
{
	GENERATED_BODY()
    
public:
	virtual void ServerProcess_Implementation(double DeltaTime) override
	{
		const int Index = GetConnection(2).GetFloat();

		Out = UCCDynamicValueUtils::FromValue(GetConnection(0), Out);
		if(auto Array = Cast<UCCArrayValueBase>(Out))
			Array->InsertElement(GetConnection(1), Index);
		
		SetOutputType(0, Out ? Out->ConnectionType.GetValue() : Unknown);
	}
    
	virtual void GetLifetimeReplicatedProps( TArray<FLifetimeProperty>& OutLifetimeProps ) const override
	{
		Super::GetLifetimeReplicatedProps(OutLifetimeProps);
    
		DOREPLIFETIME(AArrayInsert, Out);
	}

	virtual void GatherReplicatedObjects_Implementation(TArray<UObject*>& OutObjects) override
	{
		Super::GatherReplicatedObjects_Implementation(OutObjects);
		OutObjects.Add(Out);
	}

	virtual UObject* GetValue_Implementation(const FString& ValueName) override{ return Out; }

	virtual void OnInputConnected_Implementation(const FConnectionData& Data, int Index, UObject* Setter) override
	{
		//If a new array was connected
		if(Index == 0)
		{
			//Disconnect the previous element input if the array type is not compatible with it.
			if(!UConnectionTypeFunctions::IsValidConnectionPair(GetConnection(0).ConnectionType, Data.ConnectionType))
				OnInputDisconnected_Internal(1);

			SetInputType(1, UConnectionTypeFunctions::ArrayToBase(Data.ConnectionType));
		}
		Super::OnInputConnected_Implementation(Data, Index, Setter);
	}
	
	UPROPERTY(Replicated, SaveGame, VisibleInstanceOnly)
	UCCDynamicValueBase* Out;
};
