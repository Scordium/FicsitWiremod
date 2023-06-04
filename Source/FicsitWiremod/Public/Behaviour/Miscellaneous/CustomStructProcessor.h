// 

#pragma once

#include "CoreMinimal.h"
#include "Behaviour/FGWiremodBuildable.h"
#include "CustomStructProcessor.generated.h"

UCLASS()
class FICSITWIREMOD_API ACustomStructProcessor : public AFGWiremodBuildable
{
	GENERATED_BODY()

public:
	virtual void Process_Implementation(float DeltaTime) override
	{
		for(int i = 0; i < Out.Values.Num(); i++)
		{
			//Since all inputs will be disconnected when we change the structure, we don't have to check if the connection types match
			//The only case where that could happen is if someone edited their save data in which case it's their own fault if something crashes
			WM::FillDynamicStructFromData(GetConnection(i), Out.Values[i].Value);
		}
	}



	UFUNCTION(BlueprintCallable)
	void SetCustomStruct(const FCustomStruct& NewStruct, UObject* Setter)
	{
		if(!GetCanConfigure(Setter)) return;

		//Disconnect all inputs
		OnInputDisconnected_Internal(-1);
		Out = NewStruct;
		Recompile();
	}

	UFUNCTION(BlueprintPure)
	bool RequiresRecompile(const FCustomStruct& CompareTo)
	{
		if(CompareTo.Values.Num() != Out.Values.Num()) return true;
		
		for(int i = 0; i < Out.Values.Num() && i < CompareTo.Values.Num(); i++)
		{
			auto A = Out.Values[i];
			auto B = CompareTo.Values[i];

			if(A != B) return true;
		}

		return false;
	}

	void Recompile()
	{
		ConnectionsInfo.Inputs.Empty();

		for(auto Field : Out.Values)
		{
			auto Input = FBuildingConnection(Field.Name, "", Field.Value.ConnectionType);
			ConnectionsInfo.Inputs.Add(Input);
		}
	}


	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override
	{
		Super::GetLifetimeReplicatedProps(OutLifetimeProps);

		DOREPLIFETIME(ACustomStructProcessor, Out)
	}

	UPROPERTY(SaveGame, BlueprintReadWrite, Replicated)
	FCustomStruct Out;
};
