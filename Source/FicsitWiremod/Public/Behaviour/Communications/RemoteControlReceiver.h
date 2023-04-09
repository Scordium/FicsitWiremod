// 

#pragma once

#include "CoreMinimal.h"
#include "Behaviour/FGWiremodBuildable.h"
#include "RemoteControlReceiver.generated.h"

USTRUCT(BlueprintType)
struct FRemoteControlData
{
	GENERATED_BODY()

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite)
	TArray<bool> Keys;
	
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite)
	FString TextInput;

	FRemoteControlData()
	{
		Keys.SetNum(5);
	}
};


UCLASS()
class FICSITWIREMOD_API ARemoteControlReceiver : public AFGWiremodBuildable
{
	GENERATED_BODY()

public:

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override
	{
		Super::GetLifetimeReplicatedProps(OutLifetimeProps);

		DOREPLIFETIME(ARemoteControlReceiver, RemoteControlData);
	}

	UFUNCTION(BlueprintCallable)
	void SetData(const FRemoteControlData& NewData, UObject* Setter)
	{
		if(!GetCanConfigure(Setter)) return;

		RemoteControlData = NewData;
	}

	UFUNCTION()
	bool GetKey1(){ return RemoteControlData.Keys[0]; }

	UFUNCTION()
	bool GetKey2(){ return RemoteControlData.Keys[1]; }

	UFUNCTION()
	bool GetKey3(){ return RemoteControlData.Keys[2]; }

	UFUNCTION()
	bool GetKey4(){ return RemoteControlData.Keys[3]; }

	UFUNCTION()
	bool GetKey5(){ return RemoteControlData.Keys[4]; }

	UFUNCTION()
	FString GetText() { return RemoteControlData.TextInput; }

	UFUNCTION()
	TArray<bool> GetAllKeys() { return RemoteControlData.Keys; }

	UPROPERTY(Replicated, EditInstanceOnly, BlueprintReadWrite)
	FRemoteControlData RemoteControlData;
};
