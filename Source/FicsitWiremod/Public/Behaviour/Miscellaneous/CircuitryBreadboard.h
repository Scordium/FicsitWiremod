// 

#pragma once

#include "CoreMinimal.h"
#include "CircuitryLogger.h"
#include "FGBuildable.h"
#include "UnrealNetwork.h"
#include "CircuitryBreadboard.generated.h"


USTRUCT(Blueprintable, BlueprintType)
struct FBreadboardSize
{
	GENERATED_BODY()

public:

	FBreadboardSize()
	{
		Width = 0;
		Height = 0;
	}

	FBreadboardSize(int32 InWidth, int32 InHeight)
	{
		Width = InWidth;
		Height = InHeight;
	}

	FBreadboardSize(FVector2D InSize)
	{
		Width = InSize.X;
		Height = InSize.Y;
	}
	
	UPROPERTY(SaveGame, BlueprintReadWrite)
	int Width;

	UPROPERTY(SaveGame, BlueprintReadWrite)
	int Height;

	bool IsValidSize() const { return Width > 0 && Height > 0; }
};

USTRUCT(BlueprintType, Blueprintable)
struct FBreadboardCellData
{
	GENERATED_BODY()

public:
	UPROPERTY(SaveGame, BlueprintReadWrite)
	FVector2D Position;

	UPROPERTY(SaveGame, BlueprintReadWrite)
	AFGBuildable* ConnectedObject = nullptr;
};

USTRUCT(BlueprintType, Blueprintable)
struct FBreadboardZoneData
{
	GENERATED_BODY()

public:
	UPROPERTY(SaveGame, BlueprintReadWrite)
	FString Name = "New Section";
	
	UPROPERTY(SaveGame, BlueprintReadWrite)
	FVector2D Start;

	UPROPERTY(SaveGame, BlueprintReadWrite)
	FVector2D End;

	UPROPERTY(SaveGame, BlueprintReadWrite)
	FLinearColor Color;
};

UCLASS()
class FICSITWIREMOD_API ACircuitryBreadboard : public AFGBuildable
{
	GENERATED_BODY()

public:
	virtual void BeginPlay() override;

	UFUNCTION()
	FVector GetClosestGridPoint(const FVector& AimPosition);

	UFUNCTION()
	FVector2D GetClosestBoardPointCoordinates(const FVector& AimPosition);

	UFUNCTION(BlueprintCallable)
	void SetBuildableAtCell(const FVector2D& CellPosition, AFGBuildable* Buildable)
	{
		auto& CellData = GetCellData(CellPosition);
		if (IsValid(CellData.ConnectedObject))
		{
			ACircuitryLogger::DispatchErrorEvent("Cell at " + CellPosition.ToString() + " is already occupied.");
			return;
		}

		CellData.ConnectedObject = Buildable;
	}

	UFUNCTION(BlueprintCallable)
	bool IsCellOccupied(const FVector2D& CellPosition)
	{
		for (const FBreadboardCellData& Data : CellsData)
		{
			if (Data.Position == CellPosition) return IsValid(Data.ConnectedObject);
		}
		
		return false;
	}

	UFUNCTION(BlueprintCallable)
	bool TryGetCell(const FVector2D& CellPosition, FBreadboardCellData& Out)
	{
		for (const FBreadboardCellData& Data : CellsData)
		{
			if (Data.Position == CellPosition)
			{
				Out = Data;
				return true;
			}
		}

		return false;
	}

	UPROPERTY(BlueprintReadWrite, SaveGame, Replicated)
	FBreadboardSize BoardSize;

	UPROPERTY(BlueprintReadOnly, SaveGame, Replicated)
	TArray<FBreadboardCellData> CellsData;

	UPROPERTY(BlueprintReadOnly, SaveGame, Replicated)
	TArray<FBreadboardZoneData> BoardZones;

	UPROPERTY(BlueprintReadOnly)
	float GridSize;
	
private:
	
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override
	{
		Super::GetLifetimeReplicatedProps(OutLifetimeProps);
		DOREPLIFETIME(ThisClass, BoardSize)
		DOREPLIFETIME(ThisClass, CellsData)
		DOREPLIFETIME(ThisClass, BoardZones)
	}

	FBreadboardCellData& GetCellData(const FVector2D& Position)
	{
		for (FBreadboardCellData& Data : CellsData)
		{
			if (Data.Position == Position) return Data;
		}

		auto Data = FBreadboardCellData();
		Data.Position = Position;
		
		return CellsData[CellsData.Add(Data)];
	}
};
