// 

#pragma once

#include "CoreMinimal.h"
#include "FGCharacterPlayer.h"
#include "FGPlayerState.h"
#include "WiremodBuildableHologram.h"
#include "WiremodInterface.h"
#include "WiremodReflection.h"
#include "WiremodUtils.h"
#include "Buildables/FGBuildable.h"
#include "Module/GameInstanceModuleManager.h"
#include "Net/UnrealNetwork.h"
#include "Utility/WiremodDecalMesh.h"
#include "Utility/WiremodGameWorldModule.h"
#include "FGWiremodBuildable.generated.h"

#define WM UWiremodReflection


UENUM(BlueprintType)
enum EConnectionOccupationState
{
	Unspecified,
	Free,
	Connected,
	Disabled,
	GlobalConnection
};

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
	bool HasOwner(){return !OwnerId.IsEmpty();}
	
	bool GetCanConfigure(UObject* Actor)
	{
		if(Configuring == NoOne) return !HasOwner() || IsOwner(Actor);
		if(Configuring == OwnerOnly && !IsOwner(Actor)) return false;

		//Leaving this in case i'll want some custom logic for this later
		return true;
	}

	bool GetCanConnect(UObject* Actor)
	{
		if(Connecting == NoOne) return false;
		if(Connecting == OwnerOnly && !IsOwner(Actor)) return false;
		
		return true;
	}

	bool GetCanDisconnect(UObject* Actor)
	{
		if(Disconnecting == NoOne) return false;
		if(Disconnecting == OwnerOnly && !IsOwner(Actor)) return false;
		
		return true;
	}

	void SetOwner(UObject* NewOwner, UObject* Setter)
	{
		if(!GetCanConfigure(Setter)) return;

		OwnerId = GetUserIDOrDefault(NewOwner);
		OwnerName = GetUsernameOrDefault(NewOwner);
		Connecting = IsValid(NewOwner) ? OwnerOnly : Anyone;
		Disconnecting = IsValid(NewOwner) ? OwnerOnly : Anyone;
	}

	void SetConnectingRule(EWiremodInteractionRule Rule, UObject* Setter)
	{
		if(!GetCanConfigure(Setter)) return;
		Connecting = Rule;
	}

	void SetDisconnectingRule(EWiremodInteractionRule Rule, UObject* Setter)
	{
		if(!GetCanConfigure(Setter)) return;
		Disconnecting = Rule;
	}

	void SetConfiguringRule(EWiremodInteractionRule Rule, UObject* Setter)
	{
		if(!GetCanConfigure(Setter)) return;
		Configuring = Rule;
	}

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

	bool operator ==(const FWiremodOwnerData& Other) const
	{
		return
		OwnerId == Other.OwnerId
		&& CustomName.EqualTo(Other.CustomName)
		&& AllowDismantle == Other.AllowDismantle
		&& Configuring == Other.Configuring
		&& Disconnecting == Other.Disconnecting
		&& Connecting == Other.Connecting;
	}

};

UCLASS(Abstract)
class FICSITWIREMOD_API AFGWiremodBuildable : public AFGBuildable, public IWiremodInterface
{
	GENERATED_BODY()

	virtual bool ShouldSave_Implementation() const override{return true;};
	
public:

	// Begin IWiremodInterface
	void OnInputConnected_Implementation(const FNewConnectionData& Data, int Index, UObject* Setter) override;
	void OnInputDisconnected_Implementation(int Index, UObject* Setter) override;
	TSubclassOf<UUserWidget> GetCompactWidget_Implementation() override;
	virtual UTexture2D* GetTexture() override { return UFGItemDescriptor::GetBigIcon(GetBuiltWithDescriptor()); }
	// End IWiremodInterface

	
	AFGWiremodBuildable(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get()) : Super(ObjectInitializer)
	{
		//Tick prerequisites
		PrimaryActorTick.bCanEverTick = true;
		PrimaryActorTick.bStartWithTickEnabled = true;
		mFactoryTickFunction.bCanEverTick = false;
		
		//Multiplayer configuring
		NetDormancy = DORM_Never;
		
		//Visuals
		Mesh->AttachToComponent(RootComponent, FAttachmentTransformRules(EAttachmentRule::KeepRelative, false));
		Mesh->SetStaticMesh(UWiremodGameWorldModule::Self->WiremodGateMesh);
		Mesh->SetRelativeScale3D(FVector(20, 20, 20));

		mHologramClass = AWiremodBuildableHologram::StaticClass();
		
		BuildableDecal->AttachToComponent(RootComponent, FAttachmentTransformRules(EAttachmentRule::KeepRelative, false));
		FINConnector->AttachToComponent(RootComponent, FAttachmentTransformRules(EAttachmentRule::KeepRelative, false));
	}

	
protected:
	virtual void BeginPlay() override
	{
		Super::BeginPlay();
		SetupGate();
		PostGateSetup();
	}

	void OnInputConnected_Internal(const FNewConnectionData& Data, int Index);
	void OnInputDisconnected_Internal(int Index);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void SetupGate();
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void PostGateSetup();
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void DrawWires();

	bool IsActiveEntity(){ return ProcessLocally || this->HasAuthority(); }
	
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintNativeEvent)
	void Process(float DeltaTime);
	virtual void Process_Implementation(float DeltaTime) {}

	UFUNCTION(BlueprintCallable)
	void SetOutputType(int Index, EConnectionType Type);

	UFUNCTION(BlueprintCallable)
	void SetInputType(int Index, EConnectionType Type);

	UFUNCTION(BlueprintPure)
	EConnectionType GetOutputType(int Index);

	UFUNCTION(BlueprintPure)
	EConnectionType GetInputType(int Index);


	UFUNCTION(BlueprintCallable)
	FORCEINLINE bool WM_GetBool(int InputIndex, bool DefaultValue = false) { return WM::GetFunctionBoolResult(GetConnection(InputIndex), DefaultValue); }
	
	UFUNCTION(BlueprintCallable)
	FORCEINLINE float WM_GetFloat(int InputIndex, float DefaultValue = 0.f) { return WM::GetFunctionNumberResult(GetConnection(InputIndex), DefaultValue); }
	
	UFUNCTION(BlueprintCallable)
	FORCEINLINE int WM_GetInt(int InputIndex, int DefaultValue = 0) { return WM::GetFunctionNumberResult(GetConnection(InputIndex), DefaultValue); }
	
	UFUNCTION(BlueprintCallable)
	FORCEINLINE FLinearColor WM_GetColor(int InputIndex, FLinearColor DefaultValue = FLinearColor::Black) { return WM::GetFunctionColorResult(GetConnection(InputIndex), DefaultValue); }

	UFUNCTION(BlueprintCallable)
	FORCEINLINE FString WM_GetString(int InputIndex, FString DefaultValue = "") { return WM::GetFunctionStringResult(GetConnection(InputIndex), DefaultValue); }

	UFUNCTION(BlueprintCallable)
	FORCEINLINE FVector WM_GetVector(int InputIndex, FVector DefaultValue = FVector::ZeroVector) { return WM::GetFunctionVectorResult(GetConnection(InputIndex), DefaultValue); }

	UFUNCTION(BlueprintCallable)
	FORCEINLINE FInventoryStack WM_GetStack(int InputIndex) { return WM::GetFunctionStackResult(GetConnection(InputIndex)); }

	UFUNCTION(BlueprintCallable)
	FORCEINLINE AActor* WM_GetEntity(int InputIndex) { return WM::GetFunctionEntityResult(GetConnection(InputIndex)); }

	UFUNCTION(BlueprintCallable)
	FORCEINLINE UFGInventoryComponent* WM_GetInventory(int InputIndex) { return WM::GetFunctionInventory(GetConnection(InputIndex)); }

	UFUNCTION(BlueprintCallable)
	FORCEINLINE UFGPowerCircuit* WM_GetPowerCircuit(int InputIndex) { return WM::GetFunctionPowerCircuitResult(GetConnection(InputIndex)); }

	UFUNCTION(BlueprintCallable)
	FORCEINLINE TSubclassOf<UFGRecipe> WM_GetRecipe(int InputIndex) { return WM::GetFunctionRecipeResult(GetConnection(InputIndex)); }

	UFUNCTION(BlueprintCallable)
	FORCEINLINE FItemAmount WM_GetItemAmount(int InputIndex) { return WM::GetItemAmount(GetConnection(InputIndex)); }

public:
	virtual void GetLifetimeReplicatedProps( TArray<FLifetimeProperty>& OutLifetimeProps ) const override;
	

	UFUNCTION(BlueprintPure)
	UPARAM(DisplayName="Out") FNewConnectionData GetConnection(int Index);

	UFUNCTION(BlueprintPure)
	void GetAllConnected(TArray<FNewConnectionData>& Out);

	UFUNCTION(BlueprintPure)
	bool IsConnected(int Index);

	UFUNCTION(BlueprintPure)
	int GetNumConnected();
	
	UFUNCTION(BlueprintCallable)
	void SetWireHidden(int Index, bool IsHidden, UObject* Setter)
	{
		if(!GetCanConfigure(Setter)) return;
		if(!InputConnections.IsValidIndex(Index)) return;

		InputConnections[Index].WireHidden = IsHidden;

		DrawWires();
	}

	UFUNCTION(BlueprintCallable)
	void SetWireColor(int Index, FLinearColor Color, UObject* Setter)
	{
		if(!GetCanConfigure(Setter)) return;
		if(!InputConnections.IsValidIndex(Index)) return;

		InputConnections[Index].WireColor = Color;

		DrawWires();
	}
	

	UFUNCTION(BlueprintPure)
	FText GetCustomNameOrDefault()
	{
		if(OwnerData.CustomName.IsEmpty()) return mDisplayName;
		return OwnerData.CustomName;
	}

	UFUNCTION(BlueprintPure)
	bool IsOwner(UObject* Actor){ return OwnerData.IsOwner(Actor); }

	UFUNCTION(BlueprintPure)
	bool HasOwner() { return OwnerData.HasOwner(); }

	UFUNCTION(BlueprintPure)
	bool IsActionAuthorized(UObject* Actor){ return !HasOwner() || IsOwner(Actor); }

	UFUNCTION(BlueprintCallable)
	void SetBuildableOwner(UObject* NewOwner, UObject* Setter) { OwnerData.SetOwner(NewOwner, Setter); }

	UFUNCTION(BlueprintCallable)
	void SetBuildableName(FText Name, UObject* Setter)
	{
		if(!OwnerData.GetCanConfigure(Setter)) return;
		OwnerData.CustomName = Name;
	}

	UFUNCTION(BlueprintCallable)
	void SetCanDismantle(bool AllowDismantle, UObject* Setter)
	{
		if(!OwnerData.GetCanConfigure(Setter)) return;
		OwnerData.AllowDismantle = AllowDismantle;
	}

	UFUNCTION(BlueprintCallable)
	void SetConnectingRule(EWiremodInteractionRule Rule, UObject* Setter) { OwnerData.SetConnectingRule(Rule, Setter); }

	UFUNCTION(BlueprintPure)
	bool GetCanConnect(UObject* Actor) { return OwnerData.GetCanConnect(Actor); }
	
	UFUNCTION(BlueprintCallable)
	void SetDisconnectingRule(EWiremodInteractionRule Rule, UObject* Setter) { OwnerData.SetDisconnectingRule(Rule, Setter); }

	UFUNCTION(BlueprintPure)
	bool GetCanDisconnect(UObject* Actor) { return OwnerData.GetCanDisconnect(Actor); }

	UFUNCTION(BlueprintCallable)
	void SetConfiguringRule(EWiremodInteractionRule Rule, UObject* Setter) { OwnerData.SetConfiguringRule(Rule, Setter); }
	
	UFUNCTION(BlueprintPure)
	bool GetCanConfigure(UObject* Actor) { return OwnerData.GetCanConfigure(Actor); }
	
	virtual bool CanDismantle_Implementation() const override { return OwnerData.AllowDismantle; };

#pragma region FicsitNetworks
	UFUNCTION(BlueprintCallable)
	int netFunc_getFunctionReturnType(FString FunctionName);

	UFUNCTION(BlueprintCallable)
	bool netFunc_getWireBool(FString FunctionName, bool DefaultValue = false);

	UFUNCTION(BlueprintCallable)
	float netFunc_getWireNumber(FString FunctionName, float DefaultValue = 0);

	UFUNCTION(BlueprintCallable)
	FString netFunc_getWireString(FString FunctionName, FString DefaultValue = "");

	UFUNCTION(BlueprintCallable)
	FVector netFunc_getWireVector(FString FunctionName, FVector DefaultValue = FVector::ZeroVector);

	UFUNCTION(BlueprintCallable)
	FLinearColor netFunc_getWireColor(FString FunctionName, FLinearColor DefaultValue = FLinearColor::Black);

	UFUNCTION(BlueprintCallable)
	void netFunc_getAllWiremodFuncs(TArray<FString>& Out);

	UFUNCTION(BlueprintCallable)
	FString netFunc_getDebuggerOutputString(FString FunctionName);

	UFUNCTION(BlueprintPure, DisplayName="Is Blueprinted")
	bool netFunc_isBlueprinted();
#pragma endregion 
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	TArray<FBuildingConnection> GetAvailableConnections(EConnectionDirection direction, int& Count, FBuildableNote& Note);

	UFUNCTION(BlueprintCallable)
	TArray<FBuildingConnection> GetAvailableConnections_Slim(EConnectionDirection direction)
	{
		int a;
		FBuildableNote b;
		return GetAvailableConnections(direction, a, b);
	}
	
	UFUNCTION(BlueprintCallable)
	void GetInputOccupationStatus(EConnectionType AllowedType, TArray<TEnumAsByte<EConnectionOccupationState>>& Out);

	UPROPERTY(VisibleInstanceOnly, meta=(HideInDetailPanel=true))
	bool AllowStatelessEdit = true;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame, Replicated, meta=(EditCondition="AllowStatelessEdit"))
	FBuildingConnections ConnectionsInfo;
	
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, SaveGame, Replicated)
	TArray<FNewConnectionData> InputConnections;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, SaveGame, Replicated)
	FWiremodOwnerData OwnerData;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame)
	bool ProcessLocally;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<UUserWidget> CompactWidget;


	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UStaticMeshComponent* Mesh = CreateDefaultSubobject<UStaticMeshComponent>("Mesh");
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UWiremodDecalMesh* BuildableDecal = CreateDefaultSubobject<UWiremodDecalMesh>("Buildable Decal");
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UStaticMeshComponent* FINConnector = CreateDefaultSubobject<UStaticMeshComponent>("FINConnector");


	//Legacy wire positions converter
	TArray<FVector> ConvertConnections(TArray<FVector> Old)
	{
		TArray<FVector> Out;

		for(auto Pos : Old)
		{
			auto NewPos = UKismetMathLibrary::InverseTransformLocation(GetActorTransform(), Pos);
			Out.Add(NewPos);
		}

		return Out;
	}
};
