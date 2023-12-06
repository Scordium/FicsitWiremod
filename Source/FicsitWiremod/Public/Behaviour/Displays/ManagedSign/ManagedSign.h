// 

#pragma once

#include "CoreMinimal.h"
#include "Behaviour/FGWiremodBuildable.h"
#include "CommonLib/PlayerOwnedClipboardData.h"
#include "Components/SignComponentBase.h"
#include "Components/SignComponentDescriptor.h"
#include "ManagedSign.generated.h"

UENUM(Blueprintable, BlueprintType)
enum ESignDataVersion : uint8
{
	Release = 0,
	RendererChange = 1
};

USTRUCT(BlueprintType)
struct FManagedSignData
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame)
	FVector2D Size;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame, DisplayName="Screen DPI")
	int DotsPerInch = 200;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame)
	double EmissionLevel = 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame)
	double TicksPerSecond = 100;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame)
	TArray<FSignComponentData> Components;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame)
	TArray<FManagedSignConnectionSettings> Connections;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame)
	TArray<FSignComponentVariableData> Variables;

	float GetTickInterval() const
	{
		if(TicksPerSecond == 0) return 0;
		else return 1.0 / TicksPerSecond;
	}

	bool operator==(const FManagedSignData& Other) const
	{
		return Size == Other.Size
		&& Components == Other.Components
		&& Connections == Other.Connections
		&& Variables == Other.Variables
		&& DotsPerInch == Other.DotsPerInch
		&& EmissionLevel == Other.EmissionLevel
		&& TicksPerSecond == Other.TicksPerSecond;
	}
};

UCLASS()
class UManagedSignClipboardData : public UPlayerOwnedClipboardData
{
	GENERATED_BODY()

public:

	UPROPERTY(BlueprintReadWrite)
	FManagedSignData Data;
	
};

UCLASS()
class FICSITWIREMOD_API AManagedSign : public AFGWiremodBuildable
{
	GENERATED_BODY()

public:
	virtual void BeginPlay() override
	{
		Super::BeginPlay();
		PrimaryActorTick.UpdateTickIntervalAndCoolDown(Data.GetTickInterval());
		OnGenerateSign(Data);
	}
	
	void ApplySignLayout(const FManagedSignData& NewData, UObject* Setter)
	{
		if(!GetCanConfigure(Setter)) return;
		ApplySignLayout_Internal(NewData);
	}

	void ApplySignLayout_Internal(const FManagedSignData& NewData)
	{
		Data = NewData;
		PrimaryActorTick.UpdateTickIntervalAndCoolDown(Data.GetTickInterval());
		
		ConnectionsInfo.Inputs.Empty();
		for (auto& [Name, Type] : Data.Connections)
		{
			ConnectionsInfo.Inputs.Add(FBuildingConnection(Name, "", Type.GetValue()));
		}
		
		OnSignDataChanged(NewData);
	}

	virtual void ClientProcess_Implementation(double DeltaTime) override
	{
		for (auto Component : ActiveComponents)
		{
			if(Component) Component->OnUpdate(this);
		}
	}

	UFUNCTION(NetMulticast, Reliable)
	void OnSignDataChanged(const FManagedSignData& NewData);
	void OnSignDataChanged_Implementation(const FManagedSignData& NewData){ OnGenerateSign(NewData); }
	
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void OnGenerateSign(const FManagedSignData& NewData);
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame, Replicated)
	FManagedSignData Data;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<USignComponentBase*> ActiveComponents;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<USignComponentBase*> StaticComponents;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override
	{
		Super::GetLifetimeReplicatedProps(OutLifetimeProps);

		DOREPLIFETIME(AManagedSign, Data)
	}

	virtual bool CanUseFactoryClipboard_Implementation() override { return true; }
	virtual TSubclassOf<UObject> GetClipboardMappingClass_Implementation() override { return StaticClass();}
	virtual TSubclassOf<UObject> GetClipboardMappingClass_Implementation() override { return GetClass();}
	virtual TSubclassOf<UFGFactoryClipboardSettings> GetClipboardSettingsClass_Implementation() override { return UManagedSignClipboardData::StaticClass(); }

	virtual UFGFactoryClipboardSettings* CopySettings_Implementation() override
	{
		auto Clipboard = NewObject<UManagedSignClipboardData>(this);
		Clipboard->Data = Data;
		return Clipboard;
	}

	virtual bool PasteSettings_Implementation(UFGFactoryClipboardSettings* factoryClipboard) override
	{
		if(auto Clipboard = Cast<UManagedSignClipboardData>(factoryClipboard))
		{
			ApplySignLayout_Internal(Clipboard->Data);
			return true;
		}

		return false;
	}
};


UCLASS()
class UManagedSignUtilityFunctions : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintPure, meta=(ScriptOperator="=="), DisplayName="Sign layout equals")
	static bool SignLayoutEquals(const FManagedSignData& Data1, const FManagedSignData& Data2) { return Data1 == Data2; }	
};
