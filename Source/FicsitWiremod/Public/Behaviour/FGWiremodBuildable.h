// 

#pragma once

#include "CoreMinimal.h"
#include "CircuitryConnectionsProvider.h"
#include "FGCharacterPlayer.h"
#include "FGPlayerState.h"
#include "WiremodBuildableHologram.h"
#include "CircuitryInterface.h"
#include "WiremodReflection.h"
#include "Buildables/FGBuildable.h"
#include "CommonLib/OwnerDataFunctions.h"
#include "Kismet/KismetMathLibrary.h"
#include "Net/UnrealNetwork.h"
#include "Utility/WiremodDecalMesh.h"
#include "Utility/WiremodGameWorldModule.h"
#include "FGWiremodBuildable.generated.h"


UENUM(BlueprintType)
enum EConnectionOccupationState
{
	Unspecified,
	Free,
	Connected,
	Disabled,
	GlobalConnection
};

UCLASS(Abstract)
class FICSITWIREMOD_API AFGWiremodBuildable : public AFGBuildable, public ICircuitryProcessableInterface, public ICircuitryConnectionsProvider
{
	GENERATED_BODY()

	virtual bool ShouldSave_Implementation() const override{return true;};
	
public:

	// Begin IWiremodInterface
	virtual void OnInputConnected_Implementation(const FConnectionData& Data, int Index, UObject* Setter) override;
	virtual void OnInputDisconnected_Implementation(int Index, UObject* Setter) override;
	virtual TSubclassOf<UUserWidget> GetCompactWidget_Implementation() override;
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

	virtual void OnInputConnected_Internal(const FConnectionData& Data, int Index);
	virtual void OnInputDisconnected_Internal(int Index);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void SetupGate();
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void PostGateSetup();
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void DrawWires();
	
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintNativeEvent, meta=(DeprecatedFunction, DeprecationMessage="Use server and client process functions instead!"))
	void Process(double DeltaTime);
	virtual void Process_Implementation(double DeltaTime) {}

	UFUNCTION(BlueprintNativeEvent)
	void ServerProcess(double DeltaTime);
	virtual void ServerProcess_Implementation(double DeltaTime){}

	UFUNCTION(BlueprintNativeEvent)
	void ClientProcess(double DeltaTime);
	virtual void ClientProcess_Implementation(double DeltaTime){}

	UFUNCTION(BlueprintCallable)
	void SetOutputType(int Index, EConnectionType Type);

	UFUNCTION(BlueprintCallable)
	void SetInputType(int Index, EConnectionType Type);

	UFUNCTION(BlueprintPure)
	EConnectionType GetOutputType(int Index);

	UFUNCTION(BlueprintPure)
	EConnectionType GetInputType(int Index);
	
	UFUNCTION(BlueprintCallable)
	FORCEINLINE double WM_GetFloat(int InputIndex, double DefaultValue = 0.f) { return GetConnection(InputIndex).GetFloat(DefaultValue); }
	
	UFUNCTION(BlueprintCallable)
	FORCEINLINE FString WM_GetString(int InputIndex, FString DefaultValue = "") { return GetConnection(InputIndex).GetString(DefaultValue); }

public:
	virtual void GetLifetimeReplicatedProps( TArray<FLifetimeProperty>& OutLifetimeProps ) const override;
	

	UFUNCTION(BlueprintPure)
	UPARAM(DisplayName="Out") FConnectionData GetConnection(int Index);

	UFUNCTION(BlueprintPure)
	void GetAllConnected(TArray<FConnectionData>& Out);

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
	double netFunc_getWireNumber(FString FunctionName, double DefaultValue = 0);

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
	
	
	virtual TArray<FBuildingConnection> GetConnectionsInfo_Implementation(EConnectionDirection direction, int& Count, FBuildableNote& Note) override;
	virtual TArray<FBuildingConnection> GetConnections_Implementation(EConnectionDirection direction) override
	{
		int a;
		FBuildableNote b;
		return GetConnectionsInfo_Implementation(direction, a, b);
	}
	
	UFUNCTION(BlueprintCallable)
	void GetInputOccupationStatus(EConnectionType AllowedType, TArray<TEnumAsByte<EConnectionOccupationState>>& Out);

	virtual bool ReplicateSubobjects(UActorChannel* Channel, FOutBunch* Bunch, FReplicationFlags* RepFlags) override
	{
		bool WroteSomething = Super::ReplicateSubobjects(Channel, Bunch, RepFlags);

		auto Objects = GatherReplicatedObjects();

		for(auto Object : Objects)
			WroteSomething |= Channel->ReplicateSubobject(Object, *Bunch, *RepFlags);

		return WroteSomething;
	}
	
	UFUNCTION(BlueprintImplementableEvent)
	TArray<UObject*> GatherReplicatedObjects();

	UPROPERTY(VisibleInstanceOnly, meta=(HideInDetailPanel=true))
	bool AllowStatelessEdit = true;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame, Replicated, meta=(EditCondition="AllowStatelessEdit"))
	FBuildingConnections ConnectionsInfo;
	
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, SaveGame, Replicated)
	TArray<FConnectionData> InputConnections;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, SaveGame, Replicated)
	FWiremodOwnerData OwnerData;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame, meta=(DeprecatedProperty, DeprecationMessage="Use server and client process functions instead!"))
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
