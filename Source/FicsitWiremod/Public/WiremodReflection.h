// Copyright Coffee Stain Studios. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "FGPowerCircuit.h"
#include "GeneratedCodeHelpers.h"
#include "Buildables/FGBuildableFactory.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Utility/HelpModule/BaseHelpModule.h"
#include "WiremodReflection.generated.h"

UENUM(BlueprintType)
enum EConnectionType
{
	Unknown,
	Boolean,
	Number,
	String,
	Vector,
	Inventory,
	PowerGrid,
	Entity,
	Recipe,
	Color,
	ArrayOfBoolean,
	ArrayOfNumber,
	ArrayOfString,
	ArrayOfVector,
	ArrayOfEntity,
	ArrayOfColor,
	Stack,
	ArrayOfStack,
	Any,
	Integer,
	CustomStruct,
	AnyArray,
	AnyNonArray,
	ArrayOfPowerGrid,
	ArrayOfInventory,
	ArrayOfRecipe,
	NonReferenceable,
	ItemAmount,
	ArrayOfItemAmount
};

USTRUCT(BlueprintType)
struct FDynamicValue
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame)
	TEnumAsByte<EConnectionType> ConnectionType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame)
	bool StoredBool;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame)
	TArray<bool> BoolArr;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame)
	float StoredFloat;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame)
	TArray<float> NumberArr;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame)
	FString StoredString;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame)
	TArray<FString> StringArr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame)
	FVector StoredVector;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame)
	TArray<FVector> VectorArr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame)
	FLinearColor StoredColor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame)
	TArray<FLinearColor> ColorArr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame, NotReplicated)
	FInventoryStack Stack;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame, NotReplicated)
	TArray<FInventoryStack> StackArr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UFGInventoryComponent* Inventory = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<UFGInventoryComponent*> InventoryArr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UFGPowerCircuit* PowerGrid = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<UFGPowerCircuit*> PowerGridArr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame)
	AActor* Entity = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame)
	TArray<AActor*> EntityArr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame)
	TSubclassOf<UFGRecipe> Recipe;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame)
	TArray< TSubclassOf<UFGRecipe> > RecipeArr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame)
	FItemAmount ItemAmount;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame)
	TArray<FItemAmount> ItemAmountArr;

	FDynamicValue(){}
	
	FDynamicValue(bool Value)
	{
		ConnectionType = Boolean;
		StoredBool = Value;
	}

	FDynamicValue(FString Value)
	{
		ConnectionType = String;
		StoredString = Value;
	}

	FDynamicValue(float Value)
	{
		ConnectionType = Number;
		StoredFloat = Value;
	}
	
	FDynamicValue(int Value)
	{
		ConnectionType = Number;
		StoredFloat = Value;
	}

	FDynamicValue(FVector Value)
	{
		ConnectionType = Vector;
		StoredVector = Value;
	}

	FDynamicValue(FLinearColor Value)
	{
		ConnectionType = Color;
		StoredColor = Value;
	}
};

USTRUCT(BlueprintType)
struct FNamedValue
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame)
	FString Name;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame)
	FDynamicValue Value = FDynamicValue();

	FNamedValue(FString Name, FDynamicValue Value)
	{
		this->Name = Name;
		this->Value = Value;
	}

	FNamedValue(){}

	operator FDynamicValue() {return this->Value; }

	bool operator ==(const FNamedValue& Other)
	{
		return Name != Other.Name || Value.ConnectionType != Other.Value.ConnectionType;
	}

	bool operator !=(const FNamedValue& Other) { return !(*this == Other); }
};


USTRUCT(BlueprintType)
struct FCustomStruct
{
	GENERATED_BODY()

	UPROPERTY(SaveGame, BlueprintReadWrite)
	FString Name;

	UPROPERTY(SaveGame, BlueprintReadWrite)
	TArray<FNamedValue> Values;
};


USTRUCT(BlueprintType)
struct FBuildingConnection : public FTableRowBase
{
	GENERATED_BODY()

	/**
	 * Display name for this connection that wiremod will display to the user
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame)
	FString DisplayName;

	/**
	 * Function/property name that wiremod will call/read to get/set the value
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame)
	FName FunctionName;
	
    /**
     * The type of value this function/property returns/stores
     * Setting this to an incorrect type might cause crashes
     */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame)
	TEnumAsByte<EConnectionType> ConnectionType = Unknown;

	/**
	 * Whether wiremod should skip trying to find the function with "FunctionName" and just directly try to get the value from property.
	 * Wiremod will try to find a property with such name anyway in case a function was not found.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame)
	bool FromProperty = false;
	
	/**
	 * Description that the user will see in the UI, pretty useless as localization is not possible unless set up by the dev.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame, meta=(MultiLine="true"))
	FText Description;


	/* 
	 * Module that should be executed when trying to receive help/tips for this connection
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Instanced)
	UBaseHelpModule* HelpInfo = nullptr;
	
	FBuildingConnection(){};

	FBuildingConnection(FString DisplayName, FString FunctionName, EConnectionType ConnectionType, bool IsProperty = false)
	{
		this->DisplayName = DisplayName;
		this->FunctionName = FName(FunctionName);
		this->ConnectionType = ConnectionType;
		this->FromProperty = IsProperty;
	}
};

USTRUCT(BlueprintType)
struct FBuildableNote
{
	GENERATED_BODY()

	/**
	 * Text for the note, pretty useless as localization is not possible unless set up by the dev.
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta=(MultiLine="true"))
	FText Text;

	/**
	 * Color for the note
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FLinearColor Color1 = FLinearColor(0.783538, 0.291771, 0.059511, 1);

	/**
	 * Whether note should "blink". This will be ignored if the user enabled photoepilepsy mode.
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	bool Animated = false;
};

USTRUCT(BlueprintType)
struct FBuildingConnections : public FTableRowBase
{
	GENERATED_BODY()

	/**
	 * The inputs this building has.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame)
	TArray<FBuildingConnection> Inputs;


	/**
	 * The outputs this building has.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame)
	TArray<FBuildingConnection> Outputs;

	/**
	 * Fill with the mod reference and name of the buildable, if this buildable uses a list of connections that already exists.
	 * Schema: ModRef__BuildableName.
	 * Note: You need to use double underscore (`_`) to separate mod reference from buildable name
	 * Note: Buildable name should not contain the following prefixes: "Build_", "BP_". For example if your buildable is called "Build_SuperCoolSmelter", you should only leave the "SuperCoolSmelter" part.
	 * Example: Satisfactory's mod reference is "Game"
	 * Reduces the amount of work required to update each list when a new output/input is added to the vanilla buildings.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame)
	FName RedirectTo;

	/**
	 * Note to show to the user when they aim at the buildable
	 * Useless to other mod devs unless shared with wiremod developer.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame)
	FBuildableNote Note;


	/* 
	 * Module that should be executed when trying to receive help/tips for this buildable
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Instanced)
	UBaseHelpModule* HelpInfo = nullptr;

	
};


USTRUCT(BlueprintType)
struct FNewConnectionData
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame)
	UObject* Object;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame)
	FString DisplayName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame)
	FName FunctionName;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame)
	TEnumAsByte<EConnectionType> ConnectionType = Unknown;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame)
	FLinearColor WireColor = FLinearColor::White;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame)
	bool WireHidden = false;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame)
	TArray<FVector> WirePositions;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame)
	bool FromProperty = false;
	
	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, SaveGame)
	bool UseLocalWirePosition = false;
	
	FNewConnectionData operator =(const FNewConnectionData& data)
	{
		Object = data.Object;
		DisplayName = data.DisplayName;
		FunctionName  = data.FunctionName;
		ConnectionType = data.ConnectionType;
		WireColor = data.WireColor;
		WireHidden = data.WireHidden;
		WirePositions = data.WirePositions;
		FromProperty = data.FromProperty;
		UseLocalWirePosition = data.UseLocalWirePosition;

		return *this;
	}
	
	FORCEINLINE bool operator ==(const FNewConnectionData& other) const
	{
		return Object == other.Object
		&& FunctionName == other.FunctionName
		&& DisplayName == other.DisplayName;
	}


	FNewConnectionData()
	{
		this->Object = NULL;
	};

	FNewConnectionData(UObject* object, FName functionName, EConnectionType Type = Unknown)
	{
		this->Object = object;
		this->FunctionName = functionName;
		this->ConnectionType = Type;
	}

	//Constructor for updating to local positions
	FNewConnectionData(const FNewConnectionData& Original, TArray<FVector> LocalPositions)
	{
		Object = Original.Object;
		DisplayName = Original.DisplayName;
		FunctionName  = Original.FunctionName;
		ConnectionType = Original.ConnectionType;
		WireColor = Original.WireColor;
		WireHidden = Original.WireHidden;
		WirePositions = LocalPositions;
		FromProperty = Original.FromProperty;
		UseLocalWirePosition = true;
	}

	bool IsValid()
	{
		return ::IsValid(Object);
	}

	//Const version
	bool IsValid() const
	{
		return ::IsValid(Object);
	}

	
};


USTRUCT(BlueprintType)
struct FDynamicConnectionData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame)
	FNewConnectionData Transmitter;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame)
	FNewConnectionData Receiver;


	bool operator ==(const FDynamicConnectionData& other) const
	{
		return
		Transmitter == other.Transmitter
		&& Receiver == other.Receiver;
	}

	FDynamicConnectionData(){}

	FDynamicConnectionData(FNewConnectionData Transmitter, FNewConnectionData Receiver)
	{
		this->Transmitter = Transmitter;
		this->Receiver = Receiver;
	}
	
	bool IsValid() { return Transmitter.IsValid() && Receiver.IsValid(); }
	bool IsValidForWire() { return Transmitter.IsValid() && Receiver.IsValid() && !Transmitter.WireHidden; }
};



/**
 * 
 */
UCLASS()
class FICSITWIREMOD_API UWiremodReflection : public UBlueprintFunctionLibrary
{
	friend FNewConnectionData;
	GENERATED_BODY()
public:

	//Get
	UFUNCTION(BlueprintCallable)
	static bool GetFunctionBoolResult(const FNewConnectionData& data, bool defaultValue = false);
	
	UFUNCTION(BlueprintCallable)
	static FString GetFunctionStringResult(const FNewConnectionData& data, FString defaultValue = "");
	
	UFUNCTION(BlueprintCallable)
	static float GetFunctionNumberResult(const FNewConnectionData& data, float defaultValue = 0);
	
	UFUNCTION(BlueprintCallable)
	static FVector GetFunctionVectorResult(const FNewConnectionData& data, FVector defaultValue = FVector::ZeroVector);
	
	UFUNCTION(BlueprintCallable)
	static FLinearColor GetFunctionColorResult(const FNewConnectionData& data, FLinearColor defaultValue = FLinearColor::Black);
	
	UFUNCTION(BlueprintCallable)
	static UFGInventoryComponent* GetFunctionInventory(const FNewConnectionData& data);
	
	UFUNCTION(BlueprintCallable)
	static FInventoryStack GetFunctionStackResult(const FNewConnectionData& data);
	
	UFUNCTION(BlueprintCallable)
	static UFGPowerCircuit* GetFunctionPowerCircuitResult(const FNewConnectionData& data);
	
	UFUNCTION(BlueprintCallable)
	static AActor* GetFunctionEntityResult(const FNewConnectionData& data);

	UFUNCTION(BlueprintCallable)
	static TSubclassOf<UFGRecipe> GetFunctionRecipeResult(const FNewConnectionData& data);

	UFUNCTION(BlueprintCallable)
	static FItemAmount GetItemAmount(const FNewConnectionData& data);

	UFUNCTION(BlueprintCallable)
	static FCustomStruct GetCustomStruct(const FNewConnectionData& Data);

	//Array Get
	UFUNCTION(BlueprintCallable)
	static TArray<bool> GetBoolArray(const FNewConnectionData& data);
	
	UFUNCTION(BlueprintCallable)
	static TArray<FString> GetStringArray(const FNewConnectionData& data);
	
	UFUNCTION(BlueprintCallable)
	static TArray<float> GetNumberArray(const FNewConnectionData& data);
	
	UFUNCTION(BlueprintCallable)
	static TArray<FVector> GetVectorArray(const FNewConnectionData& data);
	
	UFUNCTION(BlueprintCallable)
	static TArray<FLinearColor> GetColorArray(const FNewConnectionData& data);
	
	UFUNCTION(BlueprintCallable)
	static TArray<UFGInventoryComponent*> GetInventoryArray(const FNewConnectionData& data);
	
	UFUNCTION(BlueprintCallable)
	static TArray<FInventoryStack> GetItemStackArray(const FNewConnectionData& data);
	
	UFUNCTION(BlueprintCallable)
	static TArray<AActor*> GetEntityArray(const FNewConnectionData& data);
	
	UFUNCTION(BlueprintCallable)
	static TArray<UFGPowerCircuit*> GetPowerGridArray(const FNewConnectionData& data);
	
	UFUNCTION(BlueprintCallable)
	static TArray< TSubclassOf<UFGRecipe> > GetRecipeArray(const FNewConnectionData& data);

	UFUNCTION(BlueprintCallable)
	static TArray<FItemAmount> GetItemAmountArray(const FNewConnectionData& data);
	
	//Set
	UFUNCTION(BlueprintCallable)
	static void SetFunctionBoolValue(const FNewConnectionData& data, bool value_);

	UFUNCTION(BlueprintCallable)
	static void SetFunctionStringValue(const FNewConnectionData& data, FString value_);
	
	UFUNCTION(BlueprintCallable)
	static void SetFunctionNumberValue(const FNewConnectionData& data, float value_);

	UFUNCTION(BlueprintCallable)
	static void SetFunctionColorValue(const FNewConnectionData& data, FLinearColor value_);

	UFUNCTION(BlueprintCallable)
	static void SetFunctionRecipeValue(const FNewConnectionData& data, TSubclassOf<UFGRecipe> recipe);

	
	
	static void HandleDynamicConnection(const FNewConnectionData& transmitter, const FNewConnectionData& receiver);

	UFUNCTION(BlueprintCallable)
	static void FillDynamicStructFromData(const FNewConnectionData& data, FDynamicValue& Out);
	
	UFUNCTION(CallInEditor, BlueprintCallable)
	static void MarkDirty(UObject* object) {object->Modify();};

	static UFunction* GetFunction(const FNewConnectionData& data)
	{
		if(!data.Object) return nullptr;
		if(!IsValid(data.Object)) return nullptr;
		
		UFunction* function = data.Object->FindFunction(data.FunctionName);
		return function;
	}

	
	static bool IsInteger(const FNewConnectionData& data)
	{
		return data.ConnectionType == Integer;
	}


	/**
	 * Calls `ProcessEvent` on the object stored in data with params
	 * @returns Whether the function was called successfully
	 */
	static bool ProcessFunction(const FNewConnectionData& data, void* params)
	{
		if(IsValid(data.Object))
		{
			auto function = GetFunction(data);
			if(function)
			{
				data.Object->ProcessEvent(function, params);
				return true;
			}
		}
		return false;
	}

	template<typename O>
	static O FromProperty(const FNewConnectionData& data, O defaultValue)
	{
		if(!data.Object) return defaultValue;
		
		auto val = FindProperty(data);
		if(!val) return defaultValue;
		return *val->ContainerPtrToValuePtr<O>(data.Object);
	}

	static FProperty* FindProperty(const FNewConnectionData& data)
	{
		if(!data.Object) return nullptr;
		return data.Object->GetClass()->FindPropertyByName(data.FunctionName);
	}
};
