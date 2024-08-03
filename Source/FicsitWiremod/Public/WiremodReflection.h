// Copyright Coffee Stain Studios. All Rights Reserved.

#pragma once
#include "FGInventoryComponent.h"
#include "FGPowerCircuit.h"
#include "FGRailroadTimeTable.h"
#include "FGRecipe.h"
#include "Behaviour/Displays/CCImageData.h"
#include "CommonLib/ConnectionType.h"
#include "CommonLib/CustomStruct.h"
#include "CommonLib/ReflectionUtilities.h"
#include "GameFramework/PlayerState.h"
#include "Utility/CircuitryLogger.h"

#include "WiremodReflection.generated.h"



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
		&& DisplayName.EqualTo(other.DisplayName);
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

	bool IsValidForWire() const
	{
		return !WireHidden && ::IsValid(Object) && !Object->GetClass()->IsChildOf(UCCDynamicValueBase::StaticClass());
	}

	bool IsA(EConnectionType Type) { return UConnectionTypeFunctions::IsValidConnectionPair(ConnectionType, Type); }
	bool IsA(EConnectionType Type) const { return UConnectionTypeFunctions::IsValidConnectionPair(ConnectionType, Type); }
	
	bool GetBool(bool DefaultValue = false) const { return UReflectionUtilities::GetBool(Object, FunctionName, FromProperty, DefaultValue); }
	double GetFloat(double DefaultValue = 0) const { return UReflectionUtilities::GetFloat(Object, FunctionName, FromProperty, DefaultValue); }
	FString GetString(FString DefaultValue = "") const { return UReflectionUtilities::GetString(Object, FunctionName, FromProperty, DefaultValue); }
	FVector GetVector(FVector DefaultValue = FVector::ZeroVector) const { return UReflectionUtilities::GetVector(Object, FunctionName, FromProperty, DefaultValue); }
	UFGInventoryComponent* GetInventory() const { return UReflectionUtilities::GetInventory(Object, FunctionName, FromProperty); }
	UFGPowerCircuit* GetCircuit() const { return UReflectionUtilities::GetCircuit(Object, FunctionName, FromProperty); }
	AActor* GetEntity() const { return UReflectionUtilities::GetEntity(Object, FunctionName, FromProperty); }
	TSubclassOf<UFGRecipe> GetRecipe() const { return UReflectionUtilities::GetRecipe(Object, FunctionName, FromProperty); }
	FLinearColor GetColor(FLinearColor DefaultValue = FLinearColor::Black) const { return UReflectionUtilities::GetColor(Object, FunctionName, FromProperty, DefaultValue); }
	FInventoryStack GetStack() const { return UReflectionUtilities::GetStack(Object, FunctionName, FromProperty); }
	FItemAmount GetItemAmount() const { return UReflectionUtilities::GetItemAmount(Object, FunctionName, FromProperty); }
	FCustomStruct GetCustomStruct() const { return UReflectionUtilities::GetUnmanaged<FCustomStruct>(Object, FunctionName, FromProperty); }
	FPixelScreenData GetPixelImage() const { return UReflectionUtilities::GetUnmanaged<FPixelScreenData>(Object, FunctionName, FromProperty); }
	UTexture* GetTexture() const { return UReflectionUtilities::GetTexture(Object, FunctionName, FromProperty); }
	FSplitterSortRule GetSplitterRule() const { return UReflectionUtilities::GetSplitterRule(Object, FunctionName, FromProperty); }
	TSubclassOf<UFGItemDescriptor> GetItemDescriptor(TSubclassOf<UFGItemDescriptor> DefaultValue = TSubclassOf<UFGItemDescriptor>()) const { return UReflectionUtilities::GetItemDescriptor(Object, FunctionName, FromProperty, DefaultValue); }
	AFGRailroadTimeTable* GetTimeTable() const { return UReflectionUtilities::GetUnmanaged<AFGRailroadTimeTable*>(Object, FunctionName, FromProperty, nullptr); }
	FTimeTableStopData GetTimeTableStop() const { return UReflectionUtilities::GetTrainStop(Object, FunctionName, FromProperty); }
	
	TArray<bool> GetBoolArray() const { return UReflectionUtilities::GetBoolArray(Object, FunctionName, FromProperty); }
	TArray<double> GetFloatArray() const { return UReflectionUtilities::GetFloatArray(Object, FunctionName, FromProperty); }
	TArray<FString> GetStringArray() const { return UReflectionUtilities::GetStringArray(Object, FunctionName, FromProperty); }
	TArray<FVector> GetVectorArray() const { return UReflectionUtilities::GetVectorArray(Object, FunctionName, FromProperty); }
	TArray<UFGInventoryComponent*> GetInventoryArray() const { return UReflectionUtilities::GetInventoryArray(Object, FunctionName, FromProperty); }
	TArray<UFGPowerCircuit*> GetCircuitArray() const { return UReflectionUtilities::GetCircuitArray(Object, FunctionName, FromProperty); }
	TArray<AActor*> GetEntityArray() const { return UReflectionUtilities::GetEntityArray(Object, FunctionName, FromProperty); }
	TArray<TSubclassOf<UFGRecipe>> GetRecipeArray() const { return UReflectionUtilities::GetRecipeArray(Object, FunctionName, FromProperty); }
	TArray<FLinearColor> GetColorArray() const { return UReflectionUtilities::GetColorArray(Object, FunctionName, FromProperty); }
	TArray<FInventoryStack> GetStackArray() const { return UReflectionUtilities::GetStackArray(Object, FunctionName, FromProperty); }
	TArray<FItemAmount> GetItemAmountArray() const { return UReflectionUtilities::GetItemAmountArray(Object, FunctionName, FromProperty); }
	TArray<FCustomStruct> GetCustomStructArray() const { return UReflectionUtilities::GetUnmanaged<TArray<FCustomStruct>>(Object, FunctionName, FromProperty); }
	TArray<FPixelScreenData> GetPixelImageArray() const { return UReflectionUtilities::GetUnmanaged<TArray<FPixelScreenData>>(Object, FunctionName, FromProperty); }
	TArray<UTexture*> GetTextureArray() const { return UReflectionUtilities::GetTextureArray(Object, FunctionName, FromProperty); }
	TArray<FSplitterSortRule> GetSplitterRuleArray() const { return UReflectionUtilities::GetSplitterRuleArray(Object, FunctionName, FromProperty); }
	TArray<TSubclassOf<UFGItemDescriptor>> GetItemDescriptorArray() const { return UReflectionUtilities::GetItemDescriptorArray(Object, FunctionName, FromProperty); }
	TArray<FTimeTableStopData> GetTimeTableStops() const { return UReflectionUtilities::GetTrainStopArray(Object, FunctionName, FromProperty); }
	
	void SetBool(bool Value) const { UReflectionUtilities::SetBool(Object, FunctionName, FromProperty, Value); }
	void SetFloat(double Value) const { UReflectionUtilities::SetFloat(Object, FunctionName, FromProperty, Value); }
	void SetString(FString Value) const { UReflectionUtilities::SetString(Object, FunctionName, FromProperty, Value); }
	void SetColor(FLinearColor Value) const { UReflectionUtilities::SetColor(Object, FunctionName, FromProperty, Value); }
	void SetRecipe(TSubclassOf<UFGRecipe> Value) const { UReflectionUtilities::SetRecipe(Object, FunctionName, FromProperty, Value); }

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
		switch (ConnectionType)
		{
		case Unknown: return "?";
		case Boolean: return GetBool() ? "true" : "false";
		case Number: return FString::SanitizeFloat(GetFloat());
		case String: return GetString();
		case Vector: return GetVector().ToCompactString();
		case Inventory:
			{
				auto inv = GetInventory();
				if(!inv) return "Invalid Inv.";

				TArray<FInventoryStack> Stacks;
				inv->GetInventoryStacks(Stacks);
				
				return CC_INT(Stacks.Num()) + "/" + CC_INT(inv->GetSizeLinear()) + " slots occupied";
			};
		case PowerGrid: return "?";
		case Entity:
			{
				auto Entity = GetEntity();
				auto ObjectName = UKismetSystemLibrary::GetObjectName(Entity);
				
				if(auto Player = Cast<AFGCharacterPlayer>(Entity))
				{
					//Check if the player state is valid. If the player is offline it will be null and crash if not handled properly
					if(auto State = Player->GetPlayerState()) return ObjectName + "(Player " + State->GetPlayerName() + ")";
					return ObjectName + "(Offline player)";
				}
				return ObjectName;
			}
		case Recipe:
			{
				auto Recipe = GetRecipe();
				return ::IsValid(Recipe) ? UFGRecipe::GetRecipeName(Recipe).ToString() : FString();
			}
		case Color: return GetColor().ToString();
		case ArrayOfBoolean: return "[" + CC_INT(GetBoolArray().Num()) + " elements]";
		case ArrayOfNumber: return "[" + CC_INT(GetFloatArray().Num()) + " elements]";
		case ArrayOfString: return "[" + CC_INT(GetStringArray().Num()) + " elements]";
		case ArrayOfVector: return "[" + CC_INT(GetVectorArray().Num()) + " elements]";
		case ArrayOfEntity: return "[" + CC_INT(GetEntityArray().Num()) + " elements]";
		case ArrayOfColor: return "[" + CC_INT(GetColorArray().Num()) + " elements]";
		case ArrayOfInventory: return "[" + CC_INT(GetInventoryArray().Num()) + " elements]";
		case ArrayOfPowerGrid: return "[" + CC_INT(GetCircuitArray().Num()) + " elements]";
		case ArrayOfStack: return "[" + CC_INT(GetStackArray().Num()) + " elements]";
		case ArrayOfRecipe: return "[" + CC_INT(GetRecipeArray().Num()) + " elements]";
		case Stack:
			{
				auto Stack = GetStack();
				return CC_INT(Stack.NumItems) + " " + UFGItemDescriptor::GetItemName(Stack.Item.GetItemClass()).ToString();
			}
		case Integer: return CC_INT(GetFloat());
		case Any: return "?";
		case AnyArray: return "?";
		case AnyNonArray: return "?";
		case ItemAmount:
			{
				auto Item = GetItemAmount();
				return CC_INT(Item.Amount) + " of " + UFGItemDescriptor::GetItemName(Item.ItemClass).ToString();
			}
		case ArrayOfItemAmount: return "[" + CC_INT(GetItemAmountArray().Num()) + " elements]";
		case CustomStruct:
			{
				auto Val = GetCustomStruct();
				return  Val.Name + " [" + CC_INT(Val.Values.Num()) + " values]";
			}

		case PixelImage:
			{
				auto Value = GetPixelImage();
				return CC_INT(Value.Width) + "x" + CC_INT(Value.Height);
			}

		case ArrayOfPixelImage: return "[" + CC_INT(GetPixelImageArray().Num()) + " elements]";
		case Texture: return "?";
		case ArrayOfTexture: return "[" + CC_INT(GetTextureArray().Num()) + " elements]";
		case SplitterRule:
			{
				auto Val = GetSplitterRule();
				auto Name = FStringFormatArg(Val.ItemClass == nullptr ? "N/A" : UFGItemDescriptor::GetItemName(Val.ItemClass).ToString());
				auto Index = FStringFormatArg(Val.OutputIndex);
				auto Args = FStringFormatOrderedArguments(TArray{Name, Index});
				return FString::Format(*FString("{0} [{1}]"), Args);
			}
		case ArrayOfSplitterRule: return "[" + CC_INT(GetSplitterRuleArray().Num()) + " elements]";
		case ItemDescriptor: return UFGItemDescriptor::GetItemName(GetItemDescriptor()).ToString();
		case ArrayOfItemDescriptor: return "[" + CC_INT(GetItemDescriptorArray().Num()) + " elements]";
		case TrainStop:
			{
				auto Station = GetTimeTableStop().Station;
				return Station ? Station->GetStationName().ToString() : "N/A";
			}
		case ArrayOfTrainStop: return "[" + CC_INT(GetTimeTableStops().Num()) + " elements]";
		default:
			auto TypeString = UEnum::GetValueAsString<EConnectionType>(ConnectionType);
			ACircuitryLogger::DispatchErrorEvent("Failed to find switch case for EConnectionType::" + TypeString + " in function GET_STRINGIFIED_VALUE. Returning default value instead...");
			return "?";
		}
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
	bool IsValidForWire() const { return Transmitter.IsValidForWire() && Receiver.IsValidForWire() && !Transmitter.WireHidden; }
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
	static UFGPowerInfoComponent* GetPowerInfo(const FConnectionData& Data) { return UReflectionUtilities::GetPowerInfo(Data.Object); }

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
