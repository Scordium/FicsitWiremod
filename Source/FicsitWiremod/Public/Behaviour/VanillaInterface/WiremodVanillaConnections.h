// 

#pragma once

#include "CoreMinimal.h"
#include "Behaviour/FGWiremodBuildable.h"
#include "Subsystem/ModSubsystem.h"
#include "WiremodVanillaConnections.generated.h"

USTRUCT(BlueprintType)
struct FVanillaInterfaceData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, SaveGame, BlueprintReadWrite)
	TArray<FDynamicConnectionData> Connections;
	
	UPROPERTY(EditAnywhere, SaveGame, BlueprintReadWrite)
	FWiremodOwnerData OwnerData;

	FVanillaInterfaceData(){};

	FVanillaInterfaceData(TArray<FDynamicConnectionData> Connections, FWiremodOwnerData OwnerData)
	{
		this->Connections = Connections;
		this->OwnerData = OwnerData;
	}
	
};

USTRUCT(BlueprintType)
struct FVanillaBuildingDataKeyValuePair
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, SaveGame, BlueprintReadWrite)
	UObject* Buildable;

	UPROPERTY(EditAnywhere, SaveGame, BlueprintReadWrite)
	FVanillaInterfaceData Data;

	FVanillaBuildingDataKeyValuePair(){};
	
	FVanillaBuildingDataKeyValuePair(UObject* Object)
	{
		this->Buildable = Object;
	}
	
	FVanillaBuildingDataKeyValuePair(UObject* Buildable, FVanillaInterfaceData Data)
	{
		this->Buildable = Buildable;
		this->Data = Data;
	}

	bool operator==(const FVanillaBuildingDataKeyValuePair& Other) const
	{
		return this->Buildable == Other.Buildable;
	}
	
};


UCLASS()
class FICSITWIREMOD_API AWiremodVanillaConnections : public AModSubsystem
{
	GENERATED_BODY()

public:

	//A shitty bandaid because USubsystemActorManager can't find the subsystem instance despite it being registered and accessible through this variable.
	inline static AWiremodVanillaConnections* Self;
	
	// Sets default values for this actor's properties
	AWiremodVanillaConnections() : Super()
	{
		PrimaryActorTick.bCanEverTick = true;
		NetDormancy = DORM_Never;
		ReplicationPolicy = ESubsystemReplicationPolicy::SpawnOnServer_Replicate;
	}

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override
	{
		Super::BeginPlay();
		Self = this;
		if(!HasAuthority()) return;

		LoadConnections();
		DrawWires(true);
	}

	FVanillaBuildingDataKeyValuePair GetOrDefault(UObject* Object)
	{
		for(auto& Data : SavedVanillaBuildableData)
		{
			if(Data.Buildable == Object) return Data;
		}

		return FVanillaBuildingDataKeyValuePair(Object);
	}
	
	void LoadConnections()
	{
		Game_VanillaBuildableData.Empty();
		
		for(auto Data : SavedVanillaBuildableData)
		{
			if(!Data.Buildable) continue;
			
			Game_VanillaBuildableData.Add(Data.Buildable, Data.Data);
		}

		UE_LOG(LogActor, Warning, TEXT("Loaded %d vanilla connections"), Game_VanillaBuildableData.Num())
	}

	//Draw or redraw wires for all active buildables
	//Do not call this without the need as it is very expensive to do this.
	void DrawWires(bool SkipDestruct = false)
	{
		for(auto KeyValuePair : SavedVanillaBuildableData)
			DrawWiresForBuildable(KeyValuePair, SkipDestruct);
	}


	UFUNCTION()
	void DrawWiresForBuildable(FVanillaBuildingDataKeyValuePair KeyValuePair, bool SkipDestruct = false);

public:
	virtual void Tick(float DeltaTime) override;

	static bool HandleDynamicConnections(TArray<FDynamicConnectionData> connections);

	void FilterNullPointers(TArray<int> ClearIndexes);

	UFUNCTION(BlueprintCallable)
	void AddConnection(FDynamicConnectionData Connection, int Index, UObject* Setter)
	{
		auto Data = GetOrDefault(Connection.Receiver.Object);

		if(!Data.Data.OwnerData.GetCanConnect(Setter)) return;
		
		if(!Data.Data.Connections.IsValidIndex(Index))
			Data.Data.Connections.SetNum(Index+1);

		Data.Data.Connections[Index] = Connection;

		UpdateBuildable(Connection.Receiver.Object, Data.Data.Connections, Data.Data.OwnerData);
		DrawWiresForBuildable(Data);
	}

	UFUNCTION(BlueprintCallable)
	void ResetConnection(UObject* Buildable, int Index, UObject* Setter)
	{
		auto Data = GetOrDefault(Buildable);

		if(!Data.Data.OwnerData.GetCanDisconnect(Setter)) return;

		if(Index == -1)
			Data.Data.Connections.Empty();

		if(Data.Data.Connections.IsValidIndex(Index))
			Data.Data.Connections[Index] = FDynamicConnectionData();

		UpdateBuildable(Buildable, Data.Data.Connections, Data.Data.OwnerData);
		DrawWiresForBuildable(Data);
	}

	UFUNCTION(BlueprintCallable)
	void SetBuildableOwner(UObject* Buildable, UObject* NewOwner, UObject* Setter)
	{
		auto Data = GetOrDefault(Buildable);
		Data.Data.OwnerData.SetOwner(NewOwner, Setter);
		UpdateBuildable(Buildable, Data.Data.Connections, Data.Data.OwnerData);
	}

	UFUNCTION(BlueprintCallable)
	void SetConnectingRule(UObject* Buildable, EWiremodInteractionRule Rule, UObject* Setter)
	{
		auto Data = GetOrDefault(Buildable);
		Data.Data.OwnerData.SetConnectingRule(Rule, Setter);
		UpdateBuildable(Buildable, Data.Data.Connections, Data.Data.OwnerData);
	}

	UFUNCTION(BlueprintCallable)
	void SetDisconnectingRule(UObject* Buildable, EWiremodInteractionRule Rule, UObject* Setter)
	{
		auto Data = GetOrDefault(Buildable);
		Data.Data.OwnerData.SetDisconnectingRule(Rule, Setter);
		UpdateBuildable(Buildable, Data.Data.Connections, Data.Data.OwnerData);
	}


	UFUNCTION(BlueprintPure)
	FDynamicConnectionData GetConnection(UObject* Buildable, int Index)
	{
		auto Data = Game_VanillaBuildableData.Find(Buildable);

		if(!Data || !Data->Connections.IsValidIndex(Index)) return FDynamicConnectionData();

		return Data->Connections[Index];
	}

	UFUNCTION(BlueprintPure)
	TArray<FDynamicConnectionData> GetAllConnections(UObject* Buildable)
	{
		auto Data = Game_VanillaBuildableData.Find(Buildable);
		if(!Data) return TArray<FDynamicConnectionData>();

		return Data->Connections;
	}

	UFUNCTION(BlueprintPure)
	FWiremodOwnerData GetOwnerData(UObject* Buildable)
	{
		auto Data = Game_VanillaBuildableData.Find(Buildable);
		if(!Data) return FWiremodOwnerData();

		return Data->OwnerData;
	}

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	void GetOccupationStatus(UObject* Object, EConnectionType AllowedType, TArray<TEnumAsByte<EConnectionOccupationState>>& Statuses);


	UFUNCTION(BlueprintCallable)
	void UpdateBuildable(UObject* Buildable, TArray<FDynamicConnectionData> Connections, FWiremodOwnerData OwnerData)
	{
		auto element = SavedVanillaBuildableData.FindByPredicate([Buildable](const FVanillaBuildingDataKeyValuePair& X){ return X.Buildable == Buildable; });
		
		if(!element) SavedVanillaBuildableData.Add(FVanillaBuildingDataKeyValuePair(Buildable, FVanillaInterfaceData(Connections, OwnerData)));
		else element->Data = FVanillaInterfaceData(Connections, OwnerData);
		OnRep_SavedVanillaData();
	}

	UFUNCTION(BlueprintCallable)
	void RemoveBuildable(UObject* Buildable)
	{
		auto element = SavedVanillaBuildableData.FindByPredicate([Buildable](const FVanillaBuildingDataKeyValuePair& X){ return X.Buildable == Buildable; });
		if(!element) return; // This should never happen, but this is checked just in case it somehow does
		SavedVanillaBuildableData.Remove(*element);
		OnRep_SavedVanillaData();
	}

	UFUNCTION(BlueprintCallable)
	void SetWireHidden(UObject* Buildable, int Index, bool Hidden, UObject* Setter)
	{
		//Get data
		auto Data = GetOrDefault(Buildable).Data;

		//Check if the user can configure this object
		if(!Data.OwnerData.GetCanConfigure(Setter)) return;

		//Check if the index for the wire is valid (this should never happen under normal circumstances, but we're checking it anyway.)
		if(!Data.Connections.IsValidIndex(Index)) return;

		//Set value
		Data.Connections[Index].Transmitter.WireHidden = Hidden;

		UpdateBuildable(Buildable, Data.Connections, Data.OwnerData);
		DrawWiresForBuildable(FVanillaBuildingDataKeyValuePair(Buildable, Data));
	}

	UFUNCTION(BlueprintCallable)
	void SetWireColor(UObject* Buildable, int Index, FLinearColor Color, UObject* Setter)
	{
		//Get data
		auto Data = GetOrDefault(Buildable).Data;

		//Check if the user can configure this object
		if(!Data.OwnerData.GetCanConfigure(Setter)) return;

		//Check if the index for the wire is valid (this should never happen under normal circumstances, but we're checking it anyway.)
		if(!Data.Connections.IsValidIndex(Index)) return;

		//Set value
		Data.Connections[Index].Transmitter.WireColor = Color;

		UpdateBuildable(Buildable, Data.Connections, Data.OwnerData);
		DrawWiresForBuildable(FVanillaBuildingDataKeyValuePair(Buildable, Data));
	}
	
	UFUNCTION()
	void OnRep_SavedVanillaData() { LoadConnections(); }

	virtual void GetLifetimeReplicatedProps( TArray<FLifetimeProperty>& OutLifetimeProps ) const override
	{
		Super::GetLifetimeReplicatedProps(OutLifetimeProps);

		DOREPLIFETIME(AWiremodVanillaConnections, SavedVanillaBuildableData);
	}

	UPROPERTY(VisibleInstanceOnly, SaveGame, ReplicatedUsing=OnRep_SavedVanillaData, BlueprintReadWrite)
	TArray<FVanillaBuildingDataKeyValuePair> SavedVanillaBuildableData;

	UPROPERTY(VisibleInstanceOnly, SaveGame, BlueprintReadWrite)
	TMap<UObject*, FVanillaInterfaceData> Game_VanillaBuildableData;
};
