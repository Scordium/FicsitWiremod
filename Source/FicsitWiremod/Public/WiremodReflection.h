// Copyright Coffee Stain Studios. All Rights Reserved.

#pragma once
#include "CCDynamicValueBase.h"
#include "FGInventoryComponent.h"
#include "FGPowerCircuit.h"
#include "FGRailroadTimeTable.h"
#include "FGRecipe.h"
#include "Behaviour/Displays/CCImageData.h"
#include "CommonLib/ConnectionType.h"
#include "CommonLib/CustomStruct.h"
#include "CommonLib/ReflectionUtilities.h"
#include "Utility/ValueStringResolver/ValueStringResolverBase.h"

#include "WiremodReflection.generated.h"

//
//Experimental
#define GENERATE_GET(type, name_suffix) type Get##name_suffix() const { return UReflectionUtilities::Get##name_suffix(Object, FunctionName, FromProperty); }
#define GENERATE_SET(type, name_suffix) void Set##name_suffix(type Value) const { UReflectionUtilities::Set##name_suffix(Object, FunctionName, FromProperty, Value); }

#define GENERATE_GET_DEFAULT(type, name_suffix, default_get) type Get##name_suffix(type DefaultValue = default_get) const { return UReflectionUtilities::Get##name_suffix(Object, FunctionName, FromProperty, DefaultValue); }

#define GENERATE_GET_SET(type, name_suffix) \
	GENERATE_GET(type, name_suffix) \
	GENERATE_SET(type, name_suffix)

#define GENERATE_GET_SET_DEFAULT(type, name_suffix, default_get) \
	GENERATE_GET_DEFAULT(type, name_suffix, default_get) \
	GENERATE_SET(type, name_suffix)

#define CIRCUITRY_READWRITE(type, name_suffix) \
	GENERATE_GET_SET(type, name_suffix) \
	GENERATE_GET_SET(TArray<type>, name_suffix##Array) \
	

#define CIRCUITRY_READWRITE_DEFAULT(type, name_suffix, default_get_value) \
	GENERATE_GET_SET_DEFAULT(type, name_suffix, default_get_value) \
	GENERATE_GET_SET(TArray<type>, name_suffix##Array)

#define CIRCUITRY_READONLY(type, name_suffix) \
	GENERATE_GET(type, name_suffix) \
	GENERATE_GET(TArray<type>, name_suffix##Array)

#define CIRCUITRY_READONLY_DEFAULT(type, name_suffix, default_get_value) \
	GENERATE_GET_DEFAULT(type, name_suffix, default_get_value) \
	GENERATE_GET(TArray<type>, name_suffix##Array)

#define GENERATE_GET_UNMANAGED(type, get_name) \
	type get_name() const { return UReflectionUtilities::GetUnmanaged<type>(Object, FunctionName, FromProperty); }

//
//

USTRUCT(BlueprintType)
struct FConnectionData
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame)
	UObject* Object;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame)
	FText DisplayName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame)
	FName FunctionName;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame)
	TEnumAsByte<EConnectionType> ConnectionType = Unknown;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame)
	FLinearColor WireColor = FLinearColor::White;
	
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
		WirePositions = data.WirePositions;
		FromProperty = data.FromProperty;
		UseLocalWirePosition = data.UseLocalWirePosition;

		return *this;
	}
	
	FORCEINLINE bool operator ==(const FConnectionData& other) const
	{
		return Object == other.Object
		&& FunctionName == other.FunctionName
		&& DisplayName.EqualTo(other.DisplayName);
	}

	bool EqualsFully(const FConnectionData& Other) const
	{
		return Object == Other.Object
		&& FunctionName == Other.FunctionName
		&& DisplayName.EqualTo(Other.DisplayName)
		&& ConnectionType == Other.ConnectionType
		&& WireColor == Other.WireColor
		&& WirePositions == Other.WirePositions
		&& FromProperty == Other.FromProperty;
	}


	FConnectionData()
	{
		this->Object = nullptr;
	};

	FConnectionData(UObject* Object, FName FunctionName, EConnectionType Type = Unknown, bool IsProperty = false)
	{
		this->Object = Object;
		this->FunctionName = FunctionName;
		this->ConnectionType = Type;
		this->FromProperty = IsProperty;
	}

	//Constructor for updating to local positions
	FConnectionData(const FConnectionData& Original, const TArray<FVector>& LocalPositions)
	{
		Object = Original.Object;
		DisplayName = Original.DisplayName;
		FunctionName  = Original.FunctionName;
		ConnectionType = Original.ConnectionType;
		WireColor = Original.WireColor;
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

	bool IsValidForWire() const
	{
		return IsValid() && !Object->GetClass()->IsChildOf(UCCDynamicValueBase::StaticClass());
	}
	
	bool IsA(EConnectionType Type) { return UConnectionTypeFunctions::IsValidConnectionPair(ConnectionType, Type); }
	bool IsA(EConnectionType Type) const { return UConnectionTypeFunctions::IsValidConnectionPair(ConnectionType, Type); }

	CIRCUITRY_READWRITE_DEFAULT(bool, Bool, false)
	CIRCUITRY_READWRITE_DEFAULT(double, Float, 0)
	CIRCUITRY_READWRITE_DEFAULT(FString, String, "")
	CIRCUITRY_READWRITE_DEFAULT(FVector, Vector, FVector::ZeroVector)
	CIRCUITRY_READWRITE(UFGInventoryComponent*, Inventory)
	CIRCUITRY_READWRITE(UFGPowerCircuit*, Circuit)
	CIRCUITRY_READWRITE(AActor*, Entity)
	CIRCUITRY_READWRITE(TSubclassOf<UFGRecipe>, Recipe)
	CIRCUITRY_READWRITE_DEFAULT(FLinearColor, Color, FLinearColor::Black)
	CIRCUITRY_READWRITE(FInventoryStack, Stack)
	CIRCUITRY_READWRITE(FItemAmount, ItemAmount)
	CIRCUITRY_READWRITE(FCustomStruct, CustomStruct)
	CIRCUITRY_READWRITE(FPixelScreenData, PixelImage)
	CIRCUITRY_READWRITE(UTexture*, Texture)
	CIRCUITRY_READWRITE(FSplitterSortRule, SplitterRule)
	CIRCUITRY_READWRITE_DEFAULT(TSubclassOf<UFGItemDescriptor>, ItemDescriptor, TSubclassOf<UFGItemDescriptor>())
	GENERATE_GET_UNMANAGED(AFGRailroadTimeTable*, GetTimeTable)
	CIRCUITRY_READWRITE(FTimeTableStopData, TimeTableStop)

	template<typename T>
	void Set(T Value) const { UReflectionUtilities::SetUnmanaged(Object, FunctionName, FromProperty, Value); }
	
	bool ProcessFunction(void* Params) const
	{
		if(IsValid())
		{
			if(auto Function = Object->FindFunction(FunctionName))
			{
				Object->ProcessEvent(Function, Params);
				return true;
			}
		}
		return false;
	}
	
	FString GetStringifiedValue() const
	{
		return UValueStringResolverBase::StaticClass()->GetDefaultObject<UValueStringResolverBase>()->ResolveString(Object, FunctionName, ConnectionType, FromProperty);
	}
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
	
	bool IsValid() const { return Transmitter.IsValid() && Receiver.IsValid(); }
	bool IsValidForWire() const { return Transmitter.IsValidForWire() && Receiver.IsValidForWire(); }
};

UCLASS()
class UBlueprintReflectionFunctions : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintPure)
	static FString GetStringifiedValue(const FConnectionData& Data)
	{
		return Data.GetStringifiedValue();
	}

	UFUNCTION(BlueprintCallable)
	static bool GetBool(const FConnectionData& Data, bool DefaultValue = false) { return Data.GetBool(DefaultValue); }

	UFUNCTION(BlueprintCallable)
	static double GetFloat(const FConnectionData& Data, double DefaultValue = 0) { return Data.GetFloat(DefaultValue); }
	
	UFUNCTION(BlueprintCallable)
	static FString GetString(const FConnectionData& Data, FString DefaultValue = "") { return Data.GetString(DefaultValue); }
	
	UFUNCTION(BlueprintCallable)
	static FVector GetVector(const FConnectionData& Data, FVector DefaultValue = FVector::ZeroVector) { return Data.GetVector(DefaultValue); }
	
	UFUNCTION(BlueprintCallable)
	static UFGInventoryComponent* GetInventory(const FConnectionData& Data) { return Data.GetInventory(); }
	
	UFUNCTION(BlueprintCallable)
	static UFGPowerCircuit* GetCircuit(const FConnectionData& Data) { return Data.GetCircuit(); }
	
	UFUNCTION(BlueprintCallable)
	static AActor* GetEntity(const FConnectionData& Data) { return Data.GetEntity(); }
	
	UFUNCTION(BlueprintCallable)
	static TSubclassOf<UFGRecipe> GetRecipe(const FConnectionData& Data) { return Data.GetRecipe(); }
	
	UFUNCTION(BlueprintCallable)
	static FLinearColor GetColor(const FConnectionData& Data, FLinearColor DefaultValue = FLinearColor::Black) { return Data.GetColor(DefaultValue); }
	
	UFUNCTION(BlueprintCallable)
	static FInventoryStack GetStack(const FConnectionData& Data) { return Data.GetStack(); }
	
	UFUNCTION(BlueprintCallable)
	static FItemAmount GetItemAmount(const FConnectionData& Data) { return Data.GetItemAmount(); }
	
	UFUNCTION(BlueprintCallable)
	static FCustomStruct GetCustomStruct(const FConnectionData& Data) { return Data.GetCustomStruct(); }
	
	UFUNCTION(BlueprintCallable)
	static FPixelScreenData GetPixelImage(const FConnectionData& Data) { return Data.GetPixelImage(); }
	
	UFUNCTION(BlueprintCallable)
	static UTexture* GetTexture(const FConnectionData& Data) { return Data.GetTexture(); }
	
	UFUNCTION(BlueprintCallable)
	static FSplitterSortRule GetSplitterRule(const FConnectionData& Data) { return Data.GetSplitterRule(); }

	UFUNCTION(BlueprintCallable)
	static TArray<bool> GetBoolArray(const FConnectionData& Data) { return Data.GetBoolArray(); }
	
	UFUNCTION(BlueprintCallable)
	static TArray<double> GetFloatArray(const FConnectionData& Data) { return Data.GetFloatArray(); }
	
	UFUNCTION(BlueprintCallable)
	static TArray<FString> GetStringArray(const FConnectionData& Data) { return Data.GetStringArray(); }
	
	UFUNCTION(BlueprintCallable)
	static TArray<FVector> GetVectorArray(const FConnectionData& Data) { return Data.GetVectorArray(); }
	
	UFUNCTION(BlueprintCallable)
	static TArray<UFGInventoryComponent*> GetInventoryArray(const FConnectionData& Data) { return Data.GetInventoryArray(); }
	
	UFUNCTION(BlueprintCallable)
	static TArray<UFGPowerCircuit*> GetCircuitArray(const FConnectionData& Data) { return Data.GetCircuitArray(); }
	
	UFUNCTION(BlueprintCallable)
	static TArray<AActor*> GetEntityArray(const FConnectionData& Data) { return Data.GetEntityArray(); }
	
	UFUNCTION(BlueprintCallable)
	static TArray<TSubclassOf<UFGRecipe>> GetRecipeArray(const FConnectionData& Data) { return Data.GetRecipeArray(); }
	
	UFUNCTION(BlueprintCallable)
	static TArray<FLinearColor> GetColorArray(const FConnectionData& Data) { return Data.GetColorArray(); }
	
	UFUNCTION(BlueprintCallable)
	static TArray<FInventoryStack> GetStackArray(const FConnectionData& Data) { return Data.GetStackArray(); }
	
	UFUNCTION(BlueprintCallable)
	static TArray<FItemAmount> GetItemAmountArray(const FConnectionData& Data) { return Data.GetItemAmountArray(); }
	
	UFUNCTION(BlueprintCallable)
	static TArray<FCustomStruct> GetCustomStructArray(const FConnectionData& Data) { return Data.GetCustomStructArray(); }
	
	UFUNCTION(BlueprintCallable)
	static TArray<FPixelScreenData> GetPixelImageArray(const FConnectionData& Data) { return Data.GetPixelImageArray(); }
	
	UFUNCTION(BlueprintCallable)
	static TArray<UTexture*> GetTextureArray(const FConnectionData& Data) { return Data.GetTextureArray(); }
	
	UFUNCTION(BlueprintCallable)
	static TArray<FSplitterSortRule> GetSplitterRuleArray(const FConnectionData& Data) { return Data.GetSplitterRuleArray(); }
	
	UFUNCTION(BlueprintCallable)
	static void SetBool(const FConnectionData& Data, bool Value) { Data.SetBool(Value); }
	
	UFUNCTION(BlueprintCallable)
	static void SetFloat(const FConnectionData& Data, double Value) { Data.SetFloat(Value); }
	
	UFUNCTION(BlueprintCallable)
	static void SetString(const FConnectionData& Data, FString Value) { Data.SetString(Value); }
	
	UFUNCTION(BlueprintCallable)
	static void SetColor(const FConnectionData& Data, FLinearColor Value) { Data.SetColor(Value); }
	
	UFUNCTION(BlueprintCallable)
	static void SetRecipe(const FConnectionData& Data, TSubclassOf<UFGRecipe> Value) { Data.SetRecipe(Value); }
};
