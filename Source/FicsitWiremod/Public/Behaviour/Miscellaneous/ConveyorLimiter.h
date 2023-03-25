// 

#pragma once

#include "CoreMinimal.h"
#include "FGFactoryConnectionComponent.h"
#include "Behaviour/FGWiremodBuildable.h"
#include "ConveyorLimiter.generated.h"

UCLASS()
class FICSITWIREMOD_API AConveyorLimiter : public AFGWiremodBuildable
{
	GENERATED_BODY()

public:

	AConveyorLimiter(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get()) : Super(ObjectInitializer)
	{
		mFactoryTickFunction.bCanEverTick = true;
		TempStorage = CreateDefaultSubobject<UFGInventoryComponent>("TempStorage");
	}
	
	virtual void Process_Implementation(float DeltaTime) override
	{
		Enabled = WM_GetBool(0, EnabledByDefault);

		if(WM_GetBool(1)) TotalPassed = 0;
	}

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override
	{
		Super::GetLifetimeReplicatedProps(OutLifetimeProps);

		DOREPLIFETIME(AConveyorLimiter, Enabled);
		DOREPLIFETIME(AConveyorLimiter, TotalPassed);
		DOREPLIFETIME(AConveyorLimiter, Throughput);
		DOREPLIFETIME(AConveyorLimiter, PassedHistory);
	}

	virtual void BeginPlay() override
	{
		Super::BeginPlay();
		
		if(!HasAuthority()) return;
		UKismetSystemLibrary::K2_SetTimer(this, "UpdateAverage", LoopTime, true);
	}


	UFUNCTION(BlueprintCallable)
	void UpdateAverage()
	{
		const int MaxElements = (60/LoopTime);

		//Store items passed this cycle
		PassedHistory.Add(CyclePassed);
		if(PassedHistory.Num() > MaxElements) PassedHistory.RemoveAt(0);
		CyclePassed = 0;


		//Calculate average with history
		int Sum = 0;
		for(int Element : PassedHistory) Sum += Element;
		float Avg = Sum / (PassedHistory.Num() + 0.f);

		// (60 * (1s / LoopTime)) * Avg
		Throughput = MaxElements * FMath::CeilToInt(Avg);
	}

	virtual void Factory_Tick(float dt) override
	{
		if(!HasAuthority()) return;

		if(!Enabled) return;
		if(!IsValid(Input) || !IsValid(TempStorage)) return;
		if(!Input->IsConnected()) return;
		
		TArray<FInventoryItem> Items;
		if(!Input->Factory_PeekOutput(Items)) return;
		if(!TempStorage->HasEnoughSpaceForItem(Items[0])) return;

		FInventoryItem Item = FInventoryItem();
		float Offset = 0;
		Input->Factory_GrabOutput(Item, Offset);
		TempStorage->AddItem(Item);
	}


	virtual bool Factory_GrabOutput_Implementation(UFGFactoryConnectionComponent* connection, FInventoryItem& out_item, float& out_OffsetBeyond, TSubclassOf<UFGItemDescriptor> type) override
	{
		if(!IsValid(TempStorage) || !IsValid(Output)) return false;
		if(!Enabled || TempStorage->IsEmpty()) return false;
		
		
		FInventoryStack Stack = FInventoryStack();
		if(TempStorage->GetStackFromIndex(0, Stack))
		{
			out_item = Stack.Item;
			CyclePassed++;
			TotalPassed++;
			TempStorage->RemoveFromIndex(0, 1);
			return true;
		}

		return false;
	}


	UPROPERTY(VisibleInstanceOnly, Replicated)
	bool Enabled;

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
	int Throughput;

	UPROPERTY(VisibleInstanceOnly, Replicated, SaveGame)
	TArray<float> PassedHistory;

	UPROPERTY(EditDefaultsOnly)
	float LoopTime = 1;


	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UFGFactoryConnectionComponent* Input;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UFGFactoryConnectionComponent* Output;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame)
	class UFGInventoryComponent* TempStorage;

	UPROPERTY(BlueprintReadWrite)
	bool EnabledByDefault;
};
