// Copyright Coffee Stain Studios. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "FGPowerCircuit.h"
#include "Buildables/FGBuildableFactory.h"
#include "Buildables/FGBuildableWidgetSign.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Resources/FGBuildingDescriptor.h"
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
	Building,
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
	ConnectionData
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
};

USTRUCT(BlueprintType)
struct FBuildingConnections : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame)
	TArray<FBuildingConnection> Inputs;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame)
	TArray<FBuildingConnection> Outputs;

	/*
	 * Whether this buildable acts as a proxy between game systems and wiremod. For example GameTime gate proxies TimeSubsystem.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame)
	bool OverrideOutputObject;
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

	
	FNewConnectionData operator =(const FNewConnectionData& data)
	{
		Object = data.Object;
		DisplayName = data.DisplayName;
		FunctionName  = data.FunctionName;
		ConnectionType = data.ConnectionType;

		return *this;
	}

	
	bool operator ==(const FNewConnectionData& other) const
	{
		return Object == other.Object
		&& FunctionName == other.FunctionName
		&& ConnectionType == other.ConnectionType
		&& DisplayName == other.DisplayName;
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
	static  TArray<FString> GetFunctionStringArray(const FNewConnectionData& data);
	
	UFUNCTION(BlueprintCallable)
	static UFGInventoryComponent* GetFunctionInventory(const FNewConnectionData& data);

	UFUNCTION(BlueprintCallable)
	static FInventoryStack GetFunctionStackResult(const FNewConnectionData& data);

	UFUNCTION(BlueprintCallable)
	static TArray<FInventoryStack> GetFunctionInventoryStackArrays(const FNewConnectionData& data);

	UFUNCTION(BlueprintCallable)
	static UFGPowerCircuit* GetFunctionPowerCircuitResult(const FNewConnectionData& data);

	UFUNCTION(BlueprintCallable)
	static AActor* GetFunctionEntityResult(const FNewConnectionData& data);

	UFUNCTION(BlueprintCallable)
	static TArray<AActor*> GetFunctionEntityArray(const FNewConnectionData& data);
	
	//Set
	UFUNCTION(BlueprintCallable)
	static void SetFunctionBoolValue(const FNewConnectionData& data, bool value_);

	UFUNCTION(BlueprintCallable)
	static void SetFunctionStringValue(const FNewConnectionData& data, FString value_);
	
	UFUNCTION(BlueprintCallable)
	static void SetFunctionNumberValue(const FNewConnectionData& data, float value_);

	UFUNCTION(BlueprintCallable)
	static void SetFunctionColorValue(const FNewConnectionData& data, FLinearColor value_);


	//Utility
	UFUNCTION(BlueprintCallable)
	static void HandleDynamicConnections(TArray<FDynamicConnectionData> connections);
	static void HandleDynamicConnection(const FNewConnectionData& transmitter, const FNewConnectionData& receiver);
	static FNewConnectionData GetRecursiveData(const FNewConnectionData& data)
	{
		if(!data.Object || !IsValid(data.Object)) return FNewConnectionData();

		UFunction* function = data.Object->FindFunction(data.FunctionName);

		struct { FNewConnectionData RetVal; } params;

		data.Object->ProcessEvent(function, &params);
	
		return params.RetVal;
	}
	
	UFUNCTION(CallInEditor, BlueprintCallable)
	static void MarkDirty(UObject* object) {object->Modify();};
};
