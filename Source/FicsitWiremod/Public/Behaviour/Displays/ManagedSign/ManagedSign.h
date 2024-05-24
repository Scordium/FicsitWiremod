// 

#pragma once

#include "CoreMinimal.h"
#include "FGGameState.h"
#include "JsonObjectConverter.h"
#include "Behaviour/FGWiremodBuildable.h"
#include "CommonLib/FileUtilities.h"
#include "CommonLib/PlayerOwnedClipboardData.h"
#include "Components/SignBindingsProvider.h"
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
struct FManagedSignData : public FTableRowBase
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
	double MatteLevel = 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame)
	bool UsesLumen = false;

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
		&& TicksPerSecond == Other.TicksPerSecond
		&& MatteLevel == Other.MatteLevel
		&& UsesLumen == Other.UsesLumen;
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
	virtual void ClientProcess_Implementation(double DeltaTime) override
	{
		for (const auto& Component : ActiveComponents)
		{
			if(Component) Component->OnUpdate(this);
		}
	}
	
	virtual void BeginPlay() override
	{
		Super::BeginPlay();
		PrimaryActorTick.UpdateTickIntervalAndCoolDown(Data.GetTickInterval());
		GenerateSignWidget(Data);
	}
	
	void ApplySignLayout(const FManagedSignData& NewData, UObject* Setter)
	{
		PERMISSION_CHECK(Setter);
		ApplySignLayout_Internal(NewData);
	}

	void ApplySignLayout_Internal(const FManagedSignData& NewData)
	{
		Data = NewData;
		PrimaryActorTick.UpdateTickIntervalAndCoolDown(Data.GetTickInterval());
		OnSignDataChanged(NewData);
	}

	UFUNCTION(NetMulticast, Reliable)
	void OnSignDataChanged(const FManagedSignData& NewData);
	void OnSignDataChanged_Implementation(const FManagedSignData& NewData){ GenerateSignWidget(NewData); }
	
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void GenerateSignWidget(const FManagedSignData& NewData);
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame, Replicated)
	FManagedSignData Data;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite)
	TArray<USignComponentBase*> ActiveComponents;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite)
	TArray<USignComponentBase*> StaticComponents;

	UPROPERTY(EditDefaultsOnly)
	double HologramOffset = 0;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AFGBuildableWidgetSign> VanillaVersion;

	virtual TArray<FBuildingConnection> GetConnectionsInfo_Implementation(EConnectionDirection direction, FBuildableNote& Note) override
	{
		if(direction == Output) return Super::GetConnectionsInfo_Implementation(direction, Note);
		
		TArray<FBuildingConnection> Out;
		for(auto& Connection : Data.Connections)
		{
			Out.Add(FBuildingConnection(Connection.Name, Connection.InternalName.ToString(), Connection.Type));
		}

		return Out;
	}
	
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override
	{
		Super::GetLifetimeReplicatedProps(OutLifetimeProps);

		DOREPLIFETIME(AManagedSign, Data)
	}

	virtual bool CanUseFactoryClipboard_Implementation() override { return true; }
	virtual TSubclassOf<UObject> GetClipboardMappingClass_Implementation() override { return StaticClass();}
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
			if(Data.Size != Clipboard->Data.Size) return false;
			
			ApplySignLayout_Internal(Clipboard->Data);
			return true;
		}

		return false;
	}
};

USTRUCT(Blueprintable, BlueprintType)
struct FSignComponentEditorVariablesCategory
{
	GENERATED_BODY()

public:

	UPROPERTY(BlueprintReadOnly)
	TSubclassOf<USignComponentVariableCategory> Category = nullptr;

	UPROPERTY(BlueprintReadOnly)
	TArray<FSignComponentVariableData> Variables = TArray<FSignComponentVariableData>();
};

UCLASS()
class UManagedSignUtilityFunctions : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintPure, meta=(ScriptOperator="=="), DisplayName="Sign layout equals")
	static bool SignLayoutEquals(const FManagedSignData& Data1, const FManagedSignData& Data2) { return Data1 == Data2; }

	UFUNCTION(BlueprintPure)
	static FString GetSignLayoutsFolderPath(UObject* WorldContext)
	{
		auto const GameState = Cast<AFGGameState>(UGameplayStatics::GetGameState(WorldContext));
		if(!GameState) return "ERROR: SIGN BUILDING IS NULL";
		auto const SessionName = GameState->GetSessionName();
		
		return FPaths::ProjectSavedDir() + "Circuitry/" + SessionName + "/SignLayouts/";
	}

	UFUNCTION(BlueprintCallable)
	static void GetSignLayoutFileList(UObject* WorldContext, TArray<FString>& Out) { UFileUtilities::FindAllFilesInDirectory(GetSignLayoutsFolderPath(WorldContext), TArray<FString>{".csl"}, Out); }
	
	UFUNCTION(BlueprintCallable)
	static bool ParseSignDataToJson(const FManagedSignData& Data, FString& Out)
	{
		TSharedPtr<FJsonObject> Object = FJsonObjectConverter::UStructToJsonObject(Data);
		if(!Object) return false;
		
		return FJsonSerializer::Serialize(Object.ToSharedRef(), TJsonWriterFactory<>::Create(&Out, 0));
	}

	UFUNCTION(BlueprintCallable)
	static bool LoadSignFromJson(const FString& Json, FManagedSignData& Out)
	{
		TSharedPtr<FJsonObject> Object;
		if(!FJsonSerializer::Deserialize(TJsonReaderFactory<>::Create(Json), Object)) return false;

		if(!FJsonObjectConverter::JsonObjectToUStruct(Object.ToSharedRef(), &Out)) return false;

		return true;
	}

	UFUNCTION(BlueprintCallable)
	static TArray<FSignComponentEditorVariablesCategory> SortVariablesByCategory(const TArray<FSignComponentVariableData>& Variables, TSubclassOf<USignComponentVariableCategory> DefaultCategory)
	{
		TMap<TSubclassOf<USignComponentVariableCategory>, TArray<FSignComponentVariableData>> Categories;

		//The reason this is done this way is to ensure that "Default" category will be at the correct position in the list (i.e. last or first in the list)
		//If instead we just check category and assign a default one if it's null, "Default" might appear anywhere in the list.
		FSignComponentEditorVariablesCategory DefaultCategoryData;
		DefaultCategoryData.Category = DefaultCategory;

		for (const auto& Variable : Variables)
		{
			auto Category = Variable.Name.GetDefaultObject()->Category;
			if(!Category)
			{
				DefaultCategoryData.Variables.Add(Variable);
				continue;
			}
			
			Categories.FindOrAdd(Category).Add(Variable);
		}
		
		TArray<FSignComponentEditorVariablesCategory> Out;
		for (auto& Category : Categories)
		{
			FSignComponentEditorVariablesCategory Cat;
			Cat.Category = Category.Key;
			Cat.Variables = Category.Value;
			Out.Add(Cat);
		}
		if(DefaultCategoryData.Variables.Num() > 0) Out.Add(DefaultCategoryData);
		return Out;
	}
};
