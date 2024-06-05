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
#include "Holo/ManagedSignVanillaHologram.h"
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
	double TicksPerSecond = 100;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame)
	TArray<FSignComponentData> Components;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame)
	TArray<FManagedSignConnectionSettings> Connections;

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
		&& DotsPerInch == Other.DotsPerInch
		&& EmissionLevel == Other.EmissionLevel
		&& TicksPerSecond == Other.TicksPerSecond
		&& MatteLevel == Other.MatteLevel;
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

	AManagedSign(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get()) : Super(ObjectInitializer)
	{
		mHologramClass = AManagedSignVanillaHologram::StaticClass();
	}

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

#if WITH_EDITOR
	UFUNCTION(CallInEditor, BlueprintCallable)
	void SetLayoutFromJson()
	{
		FString Json;
		FPlatformMisc::ClipboardPaste(Json);

		FManagedSignData Layout;
		TSharedPtr<FJsonObject> Object;
		if(!FJsonSerializer::Deserialize(TJsonReaderFactory<>::Create(Json), Object)) return;

		if(!FJsonObjectConverter::JsonObjectToUStruct(Object.ToSharedRef(), &Layout)) return;

		Data = Layout;
	}
#endif

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

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	bool ShouldSetMeshSize = false;

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

UCLASS(Blueprintable, BlueprintType)
class USignComponentsCategory : public UObject
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadOnly)
	FString CategoryName;
	
	UPROPERTY(BlueprintReadOnly)
	TMap<FString, USignComponentsCategory*> Subcategories;

	UPROPERTY(BlueprintReadOnly)
	TArray<USignEditorComponentBase*> Components;

	void AddItem(const FString& ItemCategory, USignEditorComponentBase* Component, int NestingLevel = 0);

	//Generates category string all the way to the top category. Used for category collapsing.
	UFUNCTION(BlueprintCallable)
	FString CreateCategoryString(FString CurrentString)
	{
		auto Outer = GetTypedOuter<USignComponentsCategory>();
		
		if(CurrentString.Len() == 0) return Outer ? Outer->CreateCategoryString(CategoryName) : CategoryName;
		else return Outer ? Outer->CreateCategoryString(CategoryName + " | " + CurrentString) : CategoryName + " | " + CurrentString;  
	}
};

UCLASS()
class UManagedSignUtilityFunctions : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable)
	static TArray<USignComponentsCategory*> SplitComponentsByCategories(const TArray<USignEditorComponentBase*>& Components, UObject* WorldContext)
	{
		auto TopCategory = NewObject<USignComponentsCategory>(WorldContext);
		TopCategory->CategoryName = "Default";

		for (USignEditorComponentBase* Component : Components)
			TopCategory->AddItem(Component->ComponentData.Metadata.Category, Component);
		
		TArray<USignComponentsCategory*> Categories;
		TopCategory->Subcategories.GenerateValueArray(Categories);
		TopCategory->Subcategories.Empty();

		if(TopCategory->Components.Num() != 0) Categories.Insert(TopCategory, 0);

		return Categories;
	}

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
	
	UFUNCTION(BlueprintCallable)
	static void GetCommonVariables(const TArray<USignEditorComponentBase*>& Components, TArray<FSignComponentVariableData>& Out)
	{
		TMap<TSubclassOf<USignComponentVariableName>, TArray<FSignComponentVariableData>> DataMap;

		//First store all variables in all components
		for (USignEditorComponentBase* Component : Components)
		{
			for (const FSignComponentVariableData& Variable : Component->ComponentData.Variables)
			{
				DataMap.FindOrAdd(Variable.Name).Add(Variable);
			}
		}

		auto FindCommonData = [&](const TArray<FSignComponentVariableData>& Variables, FString& OutValue, FString& OutDefaultValue, TArray<FSignComponentVariableMetaData>& OutMetadata) -> void
		{
			if(Variables.Num() == 0) return;
			
			OutValue = Variables[0].Data;
			OutDefaultValue = Variables[0].DefaultValue;
			
			TMap<FName, TArray<FString>> MetadataMap;

			for (const FSignComponentVariableData& Variable : Variables)
			{
				if(!Variable.Data.Equals(OutValue)) OutValue = "";
				if(!Variable.DefaultValue.Equals(OutDefaultValue)) OutDefaultValue = "";
					
				for (const FSignComponentVariableMetaData& MetaData : Variable.MetaData)
				{
					MetadataMap.FindOrAdd(MetaData.Name).Add(MetaData.Value);
				}
			}

			for (const TTuple<FName, TArray<FString>>& Data : MetadataMap)
			{
				if(Data.Value.Num() != Variables.Num()) continue;

				FString CommonMetaValue = Data.Value[0];

				for (FString Value : Data.Value)
				{
					if(!Value.Equals(CommonMetaValue)) CommonMetaValue = "";
				}

				OutMetadata.Add(FSignComponentVariableMetaData(Data.Key, CommonMetaValue));
			}
		};
		
		for (const auto& Variable : DataMap)
		{
			//If the number of variables in this tuple is not equal to the number of components, then this variable isn't present in all components.
			if(Variable.Value.Num() != Components.Num()) continue;
			
			FSignComponentVariableData Data;
			Data.Name = Variable.Key;
			FindCommonData(Variable.Value, Data.Data, Data.DefaultValue, Data.MetaData);

			Out.Add(Data);
		}
	}
};
