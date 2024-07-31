#pragma once

#include "CoreMinimal.h"
#include "FGFactoryConnectionComponent.h"
#include "Behaviour/FGWiremodBuildable.h"
#include "Buildables/FGBuildableConveyorBelt.h"
#include "ItemBuffer.generated.h"

UCLASS()
class FICSITWIREMOD_API AItemBuffer : public AFGWiremodBuildable
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintImplementableEvent)
	UFGFactoryConnectionComponent* GetInputConnector();

	UFUNCTION(BlueprintImplementableEvent)
	UFGFactoryConnectionComponent* GetOutputConnector();

	UFUNCTION(BlueprintImplementableEvent)
	UFGFactoryConnectionComponent* GetExcessOutputConnector();

	AItemBuffer(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get()) : Super(ObjectInitializer)
	{
		TempStorage = CreateDefaultSubobject<UFGInventoryComponent>("TempStorage");
		mFactoryTickFunction.bCanEverTick = true;
	}

	virtual void BeginPlay() override
	{
		Super::BeginPlay();

		if(FlipMesh) SetActorScale3D(FVector(1, -1, 1));
	}
	
	virtual void ServerProcess_Implementation(double DeltaTime) override
	{
		DesiredBufferCount = GetConnection(0).GetFloat(1);
		DesiredBufferDescriptor = GetConnection(1).GetItemDescriptor();
		if(GetConnection(2).GetBool()) CurrentlyFlushingBuffer = true;
	}

	virtual void Factory_Tick(float dt) override
	{
		if(!GetInputConnector() || !TempStorage) return;

		//If the buffer is currently being flushed, do not add new items to it.
		if(CurrentlyFlushingBuffer) return;

		FInventoryItem Item;
		float Offset;
		if(GetInputConnector()->Factory_GrabOutput(Item, Offset))
		{
			if(!TempStorage->HasEnoughSpaceForItem(Item)) return;
			
			TempStorage->AddItem(Item);
			if(DesiredBufferDescriptor == nullptr || Item.GetItemClass() == DesiredBufferDescriptor)
			{
				if(TempStorage->GetNumItems(DesiredBufferDescriptor) >= DesiredBufferCount) CurrentlyFlushingBuffer = true;
			}
		}
	}


	virtual bool Factory_GrabOutput_Implementation(UFGFactoryConnectionComponent* Connection, FInventoryItem& OutItem, float& OutOffsetBeyond, TSubclassOf<UFGItemDescriptor> ItemType) override
	{
		if(!GetOutputConnector() || !GetExcessOutputConnector() || !TempStorage) return false;
		if(TempStorage->IsEmpty())
		{
			CurrentlyFlushingBuffer = false;
			return false;
		}

		//If the connection that requested an item is the "Buffer" output, then check whether we're currently flushing the buffer before proceeding.
		if(Connection == GetOutputConnector())
		{
			if(!CurrentlyFlushingBuffer) return false;

			auto IndexWithDesiredItem = FindIndexOfBufferItem();

			// If there's no index that contains our desired item, stop the flushing.
			if(IndexWithDesiredItem == INDEX_NONE)
			{
				CurrentlyFlushingBuffer = false;
				return false;
			}

			FInventoryStack Stack;
			TempStorage->GetStackFromIndex(IndexWithDesiredItem, Stack);

			OutItem = Stack.Item;
			TempStorage->RemoveFromIndex(IndexWithDesiredItem, 1);
			return true;
		}
		//If the desired buffer item is not set, then nothing should go to "excess" output
		//If currently flushing, don't allow items to go to "excess" output
		else if(DesiredBufferDescriptor != nullptr && !CurrentlyFlushingBuffer)
		{
			TArray<FInventoryStack> Stacks;
			TempStorage->GetInventoryStacks(Stacks);

			for (FInventoryStack& Stack : Stacks)
			{
				if(Stack.Item.GetItemClass() == DesiredBufferDescriptor) continue;

				OutItem = Stack.Item;
				TempStorage->Remove(Stack.Item.GetItemClass(), 1);

				return true;
			}

			return false;
		}

		return false;
	}

	int FindIndexOfBufferItem() const
	{
		if(!TempStorage || TempStorage->IsEmpty()) return INDEX_NONE;

		TArray<FInventoryStack> Stacks;
		TempStorage->GetInventoryStacks(Stacks, true);

		for(int i = 0; i < Stacks.Num(); i++)
		{
			auto& Stack = Stacks[i];

			if(!Stack.HasItems() || !Stack.Item.IsValid()) continue;

			if(DesiredBufferDescriptor == nullptr || Stack.Item.GetItemClass() == DesiredBufferDescriptor) return i;
		}
		
		return INDEX_NONE;
	}

	virtual void Dismantle_Implementation() override
	{
		if(auto Excess = GetExcessOutputConnector()) Excess->ClearConnection();
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

	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override
	{
		Super::GetLifetimeReplicatedProps(OutLifetimeProps);

		DOREPLIFETIME(ThisClass, CurrentlyFlushingBuffer)
		DOREPLIFETIME(ThisClass, FlipMesh)
	}

	UPROPERTY(SaveGame)
	int DesiredBufferCount = 1;

	UPROPERTY(SaveGame)
	TSubclassOf<UFGItemDescriptor> DesiredBufferDescriptor;

	UPROPERTY(Replicated, SaveGame)
	bool CurrentlyFlushingBuffer = false;

	UPROPERTY(Replicated, SaveGame)
	bool FlipMesh = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame)
	class UFGInventoryComponent* TempStorage;
};
