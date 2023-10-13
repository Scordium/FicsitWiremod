// 

#pragma once

#include "CoreMinimal.h"
#include "Behaviour/FGWiremodBuildable.h"
#include "RemoteControlReceiver.generated.h"

USTRUCT(BlueprintType)
struct FRemoteControlData
{
	GENERATED_BODY()

	UPROPERTY(EditInstanceOnly, BlueprintReadOnly)
	TArray<bool> Keys;
	
	UPROPERTY(EditInstanceOnly, BlueprintReadOnly)
	FString TextInput;

	UPROPERTY(EditInstanceOnly, BlueprintReadOnly)
	AActor* HitActor = nullptr;

	UPROPERTY(EditInstanceOnly, BlueprintReadOnly)
	int CurrentSet = 0;

	FRemoteControlData()
	{
		Keys.SetNum(10);
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

	UFUNCTION() bool GetKey1(){ return RemoteControlData.Keys[0]; }
	UFUNCTION() bool GetKey2(){ return RemoteControlData.Keys[1]; }
	UFUNCTION() bool GetKey3(){ return RemoteControlData.Keys[2]; }
	UFUNCTION() bool GetKey4(){ return RemoteControlData.Keys[3]; }
	UFUNCTION() bool GetKey5(){ return RemoteControlData.Keys[4]; }
	UFUNCTION() bool GetKey6(){ return RemoteControlData.Keys[5]; }
	UFUNCTION() bool GetKey7(){ return RemoteControlData.Keys[6]; }
	UFUNCTION() bool GetKey8(){ return RemoteControlData.Keys[7]; }
	UFUNCTION() bool GetKey9(){ return RemoteControlData.Keys[8]; }
	UFUNCTION() bool GetKey10(){ return RemoteControlData.Keys[9]; }

	UFUNCTION()
	FString GetText() { return RemoteControlData.TextInput; }

	UFUNCTION()
	TArray<bool> GetAllKeys() { return RemoteControlData.Keys; }

	UFUNCTION()
	AActor* GetEntity() { return RemoteControlData.HitActor; }

	UFUNCTION()
	int GetCurrentSet() { return RemoteControlData.CurrentSet; }

	UPROPERTY(Replicated, SaveGame, EditInstanceOnly, BlueprintReadWrite)
	FRemoteControlData RemoteControlData;
};
