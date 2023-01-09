// Copyright Coffee Stain Studios. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "FGPowerCircuit.h"
#include "Buildables/FGBuildableFactory.h"
#include "Buildables/FGBuildableWidgetSign.h"
#include "Kismet/BlueprintFunctionLibrary.h"
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
	ConnectionData,
	AnyArray,
	AnyNonArray,
	ArrayOfPowerGrid,
	ArrayOfInventory,
	ArrayOfRecipe
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

	UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame)
	UFGInventoryComponent* Inventory;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame)
	TArray<UFGInventoryComponent*> InventoryArr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame)
	UFGPowerCircuit* PowerGrid;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame)
	TArray<UFGPowerCircuit*> PowerGridArr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame)
	AActor* Entity;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame)
	TArray<AActor*> EntityArr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame)
	TSubclassOf<UFGRecipe> Recipe;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame)
	TArray< TSubclassOf<UFGRecipe> > RecipeArr;

	

	/*FDynamicValue operator =(const FDynamicValue& other)
	{
		ConnectionType = other.ConnectionType;
		StoredBool = other.StoredBool;
		BoolArr = other.BoolArr;
		StoredFloat = other.StoredFloat;
		NumberArr = other.NumberArr;
		StoredString = other.StoredString;
		StringArr = other.StringArr;
		StoredVector = other.StoredVector;
		VectorArr = other.VectorArr;
		StoredColor = other.StoredColor;
		Stack = other.Stack;
		StackArr = other.StackArr;
		Inventory = other.Inventory;
		InventoryArr = other.InventoryArr;
		PowerGrid = other.PowerGrid;
		PowerGridArr = other.PowerGridArr;
		Entity = other.Entity;
		EntityArr = other.EntityArr;

		return *this;
	}*/
};


USTRUCT(BlueprintType)
struct FBuildingConnection : public FTableRowBase
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame)
	FString DisplayName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame)
	FName FunctionName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame)
	TEnumAsByte<EConnectionType> ConnectionType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame)
	bool Dynamic;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame, meta=(MultiLine="true"))
	FText Description;
};

USTRUCT(BlueprintType)
struct FBuildableNote
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta=(MultiLine="true"))
	FText Text;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FLinearColor Color1 = FLinearColor(0.783538, 0.291771, 0.059511, 1);

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FLinearColor Color2;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	bool Animated;
};

USTRUCT(BlueprintType)
struct FBuildingConnections : public FTableRowBase
{
	GENERATED_BODY()

	/**
	 * The inputs this building has.
	 * @note If you don't handle connecting the data yourself, or want wiremod to call the function for you, mark the input as Dynamic.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame)
	TArray<FBuildingConnection> Inputs;


	/**
	 * The outputs this building has.
	 * "Dynamic" doesn't do anything in this context.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame)
	TArray<FBuildingConnection> Outputs;

	/**
	 * Fill with the name of the buildable, if this buildable uses a list of connections that already exists.
	 * Reduces the amount of work required to update each list when a new output/input is added to the vanilla buildings.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame)
	FName RedirectTo;

	/**
	 * Note to show to the user when they aim at the buildable
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame)
	FBuildableNote Note;

	
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
	TEnumAsByte<EConnectionType> ConnectionType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame)
	FLinearColor WireColor = FLinearColor::White;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame)
	bool WireHidden;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame)
	TArray<FVector> WirePositions;
	
	FNewConnectionData operator =(const FNewConnectionData& data)
	{
		Object = data.Object;
		DisplayName = data.DisplayName;
		FunctionName  = data.FunctionName;
		ConnectionType = data.ConnectionType;
		WireColor = data.WireColor;
		WireHidden = data.WireHidden;
		WirePositions = data.WirePositions;

		return *this;
	}
	
	FORCEINLINE bool operator ==(const FNewConnectionData& other) const
	{
		return Object == other.Object
		&& FunctionName == other.FunctionName
		&& DisplayName == other.DisplayName;
	}


	FNewConnectionData(){};

	FNewConnectionData(UObject* object, FName functionName)
	{
		this->Object = object;
		this->FunctionName = functionName;
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
};



/**
 * 
 */
UCLASS()
class FICSITWIREMOD_API UWiremodReflection : public UBlueprintFunctionLibrary
{
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

	//Utility
	UFUNCTION(BlueprintCallable)
	static void HandleDynamicConnections(TArray<FDynamicConnectionData> connections, bool& NoneWereValid)
	{
		NoneWereValid = true;
		for (auto ConnectionData : connections)
		{
			bool HasNullPtr = !ConnectionData.Transmitter.Object || !ConnectionData.Receiver.Object;
			if(HasNullPtr) continue;

			NoneWereValid = false;
			HandleDynamicConnection(ConnectionData.Transmitter, ConnectionData.Receiver);
		}
	}
	
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

	UFUNCTION(BlueprintCallable)
	static void PrintAllFunctions(UObject* object)
	{
		for ( TFieldIterator<UFunction> FIT ( object->GetClass(), EFieldIteratorFlags::IncludeSuper ); FIT; ++FIT) {

			UFunction* Function = *FIT;
			TMap<FString, FString> properties;
			Function->GetNativePropertyValues(properties);
			UE_LOG(LogTemp, Warning, TEXT("[WIREMOD REFLECTION] FUNCTION: %s"), *Function->GetName())
			for (TTuple<FString, FString> Property : properties) {UE_LOG(LogTemp, Warning, TEXT("%s -> %s"), *Property.Key, *Property.Value);}
		}
	}



	//Utility function to add backwards compatibility. Before this was done manually, so the function could have invalid info.
	//Now it's done automatically and supported by all wiremod gates.

	static bool IsInteger(const FNewConnectionData& data)
	{
		auto checkData = FNewConnectionData(data.Object, "netFunc_getFunctionReturnType");
		auto function = GetFunction(checkData);
		if(!function) return data.ConnectionType == Integer;

		struct
		{
			FString FuncName;
			FString TypeString;
			int TypeInt;
		} params{data.FunctionName.ToString()};

		checkData.Object->ProcessEvent(function, &params);

		return params.TypeInt == EConnectionType::Integer;
	}

	template<typename T>
	static T ProcessFunction(const FNewConnectionData& data, T params)
	{
		if(IsValid(data.Object))
		{
			auto function = GetFunction(data);
			if(function)
				data.Object->ProcessEvent(function, &params);
		}
		return params;
	}
};
