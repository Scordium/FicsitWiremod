// 

#pragma once

#include "CoreMinimal.h"
#include "FGFactoryConnectionComponent.h"
#include "Behaviour/FGWiremodBuildable.h"
#include "FactoryGame/Public/Buildables/FGBuildableConveyorBelt.h"
#include "ConveyorLimiter.generated.h"

UCLASS()
class FICSITWIREMOD_API AConveyorLimiter : public AFGWiremodBuildable
{
	GENERATED_BODY()

public:
	
	virtual void ServerProcess_Implementation(double DeltaTime) override
	{
		StopConveyor = GetConnection(0).GetBool();
		
		auto SecondPerItemData = GetConnection(1);

		if(SecondPerItemData.IsValid())
		{
			auto SecondsPerItem = 60.0 / SecondPerItemData.GetFloat();
			if(LimitSecondsPerItem != SecondsPerItem)
			{
				LimitSecondsPerItem = SecondsPerItem;
				GetWorld()->GetTimerManager().ClearTimer(LimiterTickHandle);
				if(SecondsPerItem <= 0) return;
				GetWorld()->GetTimerManager().SetTimer(LimiterTickHandle, this, &AConveyorLimiter::TryGrabInputItem, SecondsPerItem, true);
			}
		}
		else TryGrabInputItem();
		
		if(GetConnection(2).GetBool()) TotalPassed = 0;
	}

	AConveyorLimiter(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get()) : Super(ObjectInitializer)
	{
		TempStorage = CreateDefaultSubobject<UFGInventoryComponent>("TempStorage");
	}

	virtual void BeginPlay() override
	{
		Super::BeginPlay();
		
		if(!HasAuthority()) return;
		GetWorld()->GetTimerManager().SetTimer(AverageUpdateHandle, this, &AConveyorLimiter::UpdateAverage, LoopTime, true);
	}

	void TryGrabInputItem()
	{
		if(!HasAuthority()) return;
		
		if(StopConveyor || !Input || !TempStorage || !Input->IsConnected()) return;
		
		TArray<FInventoryItem> Items;
		if(!Input->Factory_PeekOutput(Items)) return;

		FInventoryStack StoredStack;
		TempStorage->GetStackFromIndex(0, StoredStack);
		if(!TempStorage->HasEnoughSpaceForItem(Items[0]) || StoredStack.NumItems >= 10) return;
		FInventoryItem Item = FInventoryItem();
		float Offset = 0;
		Input->Factory_GrabOutput(Item, Offset);
		TempStorage->AddItem(Item);
	}


	UFUNCTION(BlueprintCallable)
	void UpdateAverage()
	{
		const auto Multiplier = 60.0 / LoopTime;
		PassedHistory.Add(CyclePassed);
		if(PassedHistory.Num() > 50) PassedHistory.RemoveAt(0);
		
		Throughput = (double)CyclePassed * Multiplier;
		CyclePassed = 0;
	}
	
	virtual bool Factory_GrabOutput_Implementation(UFGFactoryConnectionComponent* connection, FInventoryItem& out_item, float& out_OffsetBeyond, TSubclassOf<UFGItemDescriptor> type) override
	{
		if(!IsValid(TempStorage) || TempStorage->IsEmpty()) return false;
		
		FInventoryStack Stack = FInventoryStack();
		if(TempStorage->GetStackFromIndex(0, Stack))
		{
			out_item = Stack.Item;
			LastPassedItem = Stack.Item.GetItemClass();
			CyclePassed++;
			TotalPassed++;
			TempStorage->RemoveFromIndex(0, 1);
			return true;
		}
		return false;
	}

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override
	{
		Super::GetLifetimeReplicatedProps(OutLifetimeProps);
		
		DOREPLIFETIME(AConveyorLimiter, TotalPassed);
		DOREPLIFETIME(AConveyorLimiter, Throughput);
		DOREPLIFETIME(AConveyorLimiter, PassedHistory);
		DOREPLIFETIME(AConveyorLimiter, LastPassedItem);
	}

	virtual void Dismantle_Implementation() override
	{
		if(GetInputConnector()->GetConnection() && GetOutputConnector()->GetConnection())
		{
			auto Connection1 = GetInputConnector()->GetConnection();
			auto Connection2 = GetOutputConnector()->GetConnection();

			if(Connection1 && Connection2)
			{
				auto Conveyor1 = Cast<AFGBuildableConveyorBelt>(Connection1->GetOwner());
				auto Conveyor2 = Cast<AFGBuildableConveyorBelt>(Connection2->GetOwner());

				if(Conveyor1 && Conveyor2)
				{
					Connection1->ClearConnection();
					Connection2->ClearConnection();

					Connection1->SetConnection(Connection2);

					TArray<AFGBuildableConveyorBelt*> Belts;
					Belts.Add(Conveyor1);
					Belts.Add(Conveyor2);
				
					AFGBuildableConveyorBelt::Merge(Belts);
				}
			}
		}

		Super::Dismantle_Implementation();
	}

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	UFGFactoryConnectionComponent* GetOutputConnector();

	UFUNCTION(BlueprintCallable)
	UFGFactoryConnectionComponent* GetInputConnector() { return Input; }
	
	/*
	 * How many items passed through the limiter this cycle
	 */
	UPROPERTY(VisibleInstanceOnly)
	int CyclePassed;
	
	/*
	 * How many items passed through the limiter in total
	 */
	UPROPERTY(VisibleInstanceOnly, Replicated, SaveGame)
	int TotalPassed;
	
	UPROPERTY(VisibleInstanceOnly, Replicated, SaveGame)
	double Throughput;

	UPROPERTY(VisibleInstanceOnly, Replicated, SaveGame)
	TArray<double> PassedHistory;

	UPROPERTY(EditDefaultsOnly)
	double LoopTime = 1;

	UPROPERTY(VisibleInstanceOnly, Replicated, SaveGame)
	TSubclassOf<UFGItemDescriptor> LastPassedItem;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UFGFactoryConnectionComponent* Input;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame)
	class UFGInventoryComponent* TempStorage;

	bool StopConveyor;

	FTimerHandle LimiterTickHandle;
	FTimerHandle AverageUpdateHandle;
	double LimitSecondsPerItem = -1;
};
