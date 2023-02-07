// 

#pragma once

#include "CoreMinimal.h"
#include "FGCharacterPlayer.h"
#include "FGPlayerState.h"
#include "WiremodInterface.h"
#include "WiremodReflection.h"
#include "Buildables/FGBuildable.h"
#include "FGWiremodBuildable.generated.h"

UENUM(BlueprintType)
enum EWiremodInteractionRule
{
	NoOne,
	OwnerOnly,
	Anyone
};

USTRUCT(BlueprintType)
struct FWiremodOwnerData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame)
	FString OwnerId;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame)
	FString OwnerName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame)
	FText CustomName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame)
	TEnumAsByte<EWiremodInteractionRule> Connecting = EWiremodInteractionRule::Anyone;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame)
	TEnumAsByte<EWiremodInteractionRule> Disconnecting = EWiremodInteractionRule::Anyone;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame)
	TEnumAsByte<EWiremodInteractionRule> Configuring = EWiremodInteractionRule::Anyone;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame)
	bool AllowDismantle = true;

	bool IsOwner(UObject* entity){ return OwnerId == GetUserIDOrDefault(entity); }
	bool HasOwner(){return OwnerId.Len() > 0;}
	
	static AFGPlayerState* GetState(UObject* entity)
	{
		if(auto player = Cast<AFGCharacterPlayer>(entity))
			return Cast<AFGPlayerState>(player->GetPlayerState());

		return nullptr;
	}
	
	static FString GetUserIDOrDefault(UObject* entity)
	{
		if(auto state = GetState(entity)) return state->GetUserID();
		return "";
	}

	static FString GetUsernameOrDefault(UObject* entity)
	{
		if(auto state = GetState(entity)) return state->GetPlayerName();
		return "";
	}
};

UCLASS(Abstract)
class FICSITWIREMOD_API AFGWiremodBuildable : public AFGBuildable, public IWiremodInterface
{
	GENERATED_BODY()

	virtual void GetLifetimeReplicatedProps( TArray<FLifetimeProperty>& OutLifetimeProps ) const override
	{
		Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	}
	
	virtual void PreReplication( IRepChangedPropertyTracker& ChangedPropertyTracker ) override
	{
		Super::PreReplication(ChangedPropertyTracker);
	}
	
	
	// Begin IWiremodInterface
	void OnInputConnected_Implementation(FNewConnectionData Data, int Index, UObject* Setter) override;
	void OnInputDisconnected_Implementation(int Index, UObject* Setter) override;
	TArray<FBuildingConnection> GetAvailableConnections_Implementation(EConnectionDirection direction) override;
	TSubclassOf<UUserWidget> GetCompactWidget_Implementation() override;
	// End IWiremodInterface
	

public:
	AFGWiremodBuildable()
	{
		PrimaryActorTick.bCanEverTick = true;
		SetReplicates(true);
	};

protected:
	virtual void BeginPlay() override
	{
		Super::BeginPlay();
		SetupGate();
		PostGateSetup();
	};

	UFUNCTION(BlueprintCallable)
	virtual void SetupGate(){};

	UFUNCTION(BlueprintCallable)
	virtual void PostGateSetup(){};
	
	UFUNCTION(BlueprintCallable)
	virtual void DrawWires(){};

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override
	{
		if(ProcessLocally || this->HasAuthority()) Process(DeltaTime);
	};

	UFUNCTION(BlueprintNativeEvent)
	void Process(float DeltaTime);
	virtual void Process_Implementation(float DeltaTime) {}

	UFUNCTION(BlueprintPure)
	FNewConnectionData GetConnection(int Index)
	{
		if(!InputConnections.IsValidIndex(Index)) return FNewConnectionData();
		return InputConnections[Index];
	}

	UFUNCTION(BlueprintPure)
	void GetAllConnected(TArray<FNewConnectionData>& Out)
	{
		for (auto InputConnection : InputConnections)
			if(IsValid(InputConnection.Object))
				Out.Add(InputConnection);
	}

	UFUNCTION(BlueprintPure)
	bool IsConnected(int Index)
	{
		if(!InputConnections.IsValidIndex(Index)) return false;
		return IsValid(InputConnections[Index].Object);
	}

	UFUNCTION(BlueprintPure)
	int GetNumConnected()
	{
		int i = 0;
		for (auto Connection : InputConnections)
			if(IsValid(Connection.Object)) i++;
		
		return i;
	}

	UFUNCTION(BlueprintCallable)
	void SetOutputType(int Index, EConnectionType Type)
	{
		if(!ConnectionsInfo.Outputs.IsValidIndex(Index)) return;
		ConnectionsInfo.Outputs[Index].ConnectionType = Type;
	}

	UFUNCTION(BlueprintCallable)
	void SetInputType(int Index, EConnectionType Type)
	{
		if(!ConnectionsInfo.Inputs.IsValidIndex(Index)) return;
		ConnectionsInfo.Inputs[Index].ConnectionType = Type;
	}

	UFUNCTION(BlueprintPure)
	EConnectionType GetOutputType(int Index)
	{
		if(!ConnectionsInfo.Outputs.IsValidIndex(Index)) return Unknown;
		return ConnectionsInfo.Outputs[Index].ConnectionType;
	}

	UFUNCTION(BlueprintPure)
	EConnectionType GetInputType(int Index)
	{
		if(!ConnectionsInfo.Inputs.IsValidIndex(Index)) return Unknown;
		return ConnectionsInfo.Inputs[Index].ConnectionType;
	}

	UFUNCTION(BlueprintPure)
	bool IsOwner(UObject* entity){ return OwnerData.IsOwner(entity); }

	UFUNCTION(BlueprintPure)
	bool HasOwner() { return OwnerData.HasOwner(); }

	UFUNCTION(BlueprintPure)
	bool IsActionAuthorized(UObject* actor){ return !HasOwner() || IsOwner(actor); }

	UFUNCTION(BlueprintCallable)
	void SetBuildableOwner(UObject* NewOwner, UObject* Setter)
	{
		if(!IsActionAuthorized(Setter)) return;
		
		OwnerData.OwnerId = OwnerData.GetUserIDOrDefault(NewOwner);
		OwnerData.OwnerName = OwnerData.GetUsernameOrDefault(NewOwner);
		OwnerData.Connecting = IsValid(NewOwner) ? OwnerOnly : Anyone;
		OwnerData.Disconnecting = IsValid(NewOwner) ? OwnerOnly : Anyone;
	}

	void SetBuildableName(FText Name, UObject* Setter)
	{
		if(OwnerData.Configuring == NoOne) return;
		if(OwnerData.Configuring == OwnerOnly && !IsOwner(Setter)) return;

		OwnerData.CustomName = Name;
	}

	void SetCanDismantle(bool AllowDismantle, UObject* Setter)
	{
		if(OwnerData.Configuring == NoOne) return;
		if(OwnerData.Configuring == OwnerOnly && !IsOwner(Setter)) return;

		OwnerData.AllowDismantle = AllowDismantle;
	}

	void SetConnectingRule(EWiremodInteractionRule Rule, UObject* Setter)
	{
		if(OwnerData.Configuring == NoOne) return;
		if(OwnerData.Configuring == OwnerOnly && !IsOwner(Setter)) return;

		OwnerData.Connecting = Rule;
	}

	bool GetCanConnect(UObject* Actor)
	{
		if(OwnerData.Connecting == NoOne) return false;
		if(OwnerData.Connecting == OwnerOnly && !IsOwner(Actor)) return false;

		//Leaving this in case i'll want some custom logic for this later
		return true;
	}
	

	void SetDisconnectingRule(EWiremodInteractionRule Rule, UObject* Setter)
	{
		if(OwnerData.Configuring == NoOne) return;
		if(OwnerData.Configuring == OwnerOnly && !IsOwner(Setter)) return;

		OwnerData.Disconnecting = Rule;
	}

	bool GetCanDisconnect(UObject* Actor)
	{
		if(OwnerData.Disconnecting == NoOne) return false;
		if(OwnerData.Disconnecting == OwnerOnly && !IsOwner(Actor)) return false;

		//Leaving this in case i'll want some custom logic for this later
		return true;
	}
	
	virtual bool CanDismantle_Implementation() const override { return OwnerData.AllowDismantle; };
	
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame, Replicated)
	FBuildingConnections ConnectionsInfo;
	
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, SaveGame)
	TArray<FNewConnectionData> InputConnections;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, SaveGame)
	FWiremodOwnerData OwnerData;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame)
	bool ProcessLocally;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<UUserWidget> CompactWidget;
};
