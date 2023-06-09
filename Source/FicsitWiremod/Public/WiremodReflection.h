// Copyright Coffee Stain Studios. All Rights Reserved.

#pragma once
#include "FGInventoryComponent.h"
#include "FGPowerCircuit.h"
#include "FGRecipe.h"
#include "CommonLib/ConnectionType.h"
#include "CommonLib/CustomStruct.h"
#include "CommonLib/ReflectionHelpers.h"
#include "Engine/DataTable.h"
#include "Utility/HelpModule/BaseHelpModule.h"

#include "WiremodReflection.generated.h"

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
struct FConnectionData
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
	
	FConnectionData operator =(const FConnectionData& data)
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
	
	FORCEINLINE bool operator ==(const FConnectionData& other) const
	{
		return Object == other.Object
		&& FunctionName == other.FunctionName
		&& DisplayName == other.DisplayName;
	}

	operator FValueReflectionSource() const
	{
		return FValueReflectionSource(Object, FunctionName, FromProperty);
	}


	FConnectionData()
	{
		this->Object = NULL;
	};

	FConnectionData(UObject* Object, FName FunctionName, EConnectionType Type = Unknown)
	{
		this->Object = Object;
		this->FunctionName = FunctionName;
		this->ConnectionType = Type;
	}

	//Constructor for updating to local positions
	FConnectionData(const FConnectionData& Original, const TArray<FVector>& LocalPositions)
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

	bool IsA(EConnectionType Type) { return UConnectionTypeFunctions::IsValidConnectionPair(ConnectionType, Type); }
	bool IsA(EConnectionType Type) const { return UConnectionTypeFunctions::IsValidConnectionPair(ConnectionType, Type); }
	
	bool GetBool(bool DefaultValue = false) const;
	float GetFloat(float DefaultValue = 0) const;
	FString GetString(FString DefaultValue = "") const;
	FVector GetVector(FVector DefaultValue = FVector::ZeroVector) const;
	UFGInventoryComponent* GetInventory() const;
	UFGPowerCircuit* GetCircuit() const;
	AActor* GetEntity() const;
	TSubclassOf<UFGRecipe> GetRecipe() const;
	FLinearColor GetColor(FLinearColor DefaultValue = FLinearColor::Black) const;
	FInventoryStack GetStack() const;
	FItemAmount GetItemAmount() const;
	FCustomStruct GetCustomStruct() const;

	TArray<bool> GetBoolArray() const;
	TArray<float> GetFloatArray() const;
	TArray<FString> GetStringArray() const;
	TArray<FVector> GetVectorArray() const;
	TArray<UFGInventoryComponent*> GetInventoryArray() const;
	TArray<UFGPowerCircuit*> GetCircuitArray() const;
	TArray<AActor*> GetEntityArray() const;
	TArray<TSubclassOf<UFGRecipe>> GetRecipeArray() const;
	TArray<FLinearColor> GetColorArray() const;
	TArray<FInventoryStack> GetStackArray() const;
	TArray<FItemAmount> GetItemAmountArray() const;
	TArray<FCustomStruct> GetCustomStructArray() const;

	void SetBool(bool Value) const;
	void SetFloat(float Value) const;
	void SetString(FString Value) const;
	void SetColor(FLinearColor Value) const;
	void SetRecipe(TSubclassOf<UFGRecipe> Value) const;

	
	bool ProcessFunction(void* Params) const;
	
	FString GetStringifiedValue() const;
};


USTRUCT(BlueprintType)
struct FDynamicConnectionData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame)
	FConnectionData Transmitter;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame)
	FConnectionData Receiver;


	bool operator ==(const FDynamicConnectionData& Other) const
	{
		return
		Transmitter == Other.Transmitter
		&& Receiver == Other.Receiver;
	}

	FDynamicConnectionData(){}

	FDynamicConnectionData(FConnectionData Transmitter, FConnectionData Receiver)
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
	GENERATED_BODY()
public:

	static FLinearColor GetColor(const FConnectionData& Data) { return Data.GetColor(); }
};
