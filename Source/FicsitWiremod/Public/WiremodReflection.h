// Copyright Coffee Stain Studios. All Rights Reserved.

#pragma once
#include "FGInventoryComponent.h"
#include "FGPowerCircuit.h"
#include "FGRecipe.h"
#include "Behaviour/Displays/CCImageData.h"
#include "CommonLib/ConnectionType.h"
#include "CommonLib/CustomStruct.h"
#include "CommonLib/ReflectionUtilities.h"
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

	
	FConnectionData()
	{
		this->Object = nullptr;
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

	bool IsValidForWire() const
	{
		return ::IsValid(Object) && !Object->GetClass()->IsChildOf(UCCDynamicValueBase::StaticClass());
	}

	bool IsA(EConnectionType Type) { return UConnectionTypeFunctions::IsValidConnectionPair(ConnectionType, Type); }
	bool IsA(EConnectionType Type) const { return UConnectionTypeFunctions::IsValidConnectionPair(ConnectionType, Type); }
	
	bool GetBool(bool DefaultValue = false) const { return UReflectionUtilities::GetBool(Object, FunctionName, FromProperty, DefaultValue); }
	float GetFloat(float DefaultValue = 0) const { return UReflectionUtilities::GetFloat(Object, FunctionName, FromProperty, ConnectionType == Integer, DefaultValue); }
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

	TArray<bool> GetBoolArray() const { return UReflectionUtilities::GetBoolArray(Object, FunctionName, FromProperty); }
	TArray<float> GetFloatArray() const { return UReflectionUtilities::GetFloatArray(Object, FunctionName, FromProperty); }
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
	
	void SetBool(bool Value) const { UReflectionUtilities::SetBool(Object, FunctionName, FromProperty, Value); }
	void SetFloat(float Value) const { UReflectionUtilities::SetFloat(Object, FunctionName, FromProperty, ConnectionType == Integer, Value); }
	void SetString(FString Value) const { UReflectionUtilities::SetString(Object, FunctionName, FromProperty, Value); }
	void SetColor(FLinearColor Value) const { UReflectionUtilities::SetColor(Object, FunctionName, FromProperty, Value); }
	void SetRecipe(TSubclassOf<UFGRecipe> Value) const { UReflectionUtilities::SetRecipe(Object, FunctionName, FromProperty, Value); }

	
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
					return UKismetSystemLibrary::GetObjectName(Player) + "(Offline player)";
				}
				return UKismetSystemLibrary::GetObjectName(Entity);
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
		default:
			ACircuitryLogger::DispatchErrorEvent("Failed to find switch case for EConnectionType::" + CC_INT(ConnectionType) + " in function GET_STRINGIFIED_VALUE. Returning default value instead...");
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
	
	bool IsValid() { return Transmitter.IsValid() && Receiver.IsValid(); }
	bool IsValidForWire() { return Transmitter.IsValidForWire() && Receiver.IsValidForWire() && !Transmitter.WireHidden; }
};
