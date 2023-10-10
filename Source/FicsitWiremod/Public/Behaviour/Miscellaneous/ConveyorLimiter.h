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
	virtual void ServerProcess_Implementation(double DeltaTime) override
	{
		if(GetConnection(0).GetBool()) TotalPassed = 0;
	}

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override
	{
		Super::GetLifetimeReplicatedProps(OutLifetimeProps);
		
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
		auto Avg = Sum / (double)PassedHistory.Num();

		// (60 * (1s / LoopTime)) * Avg
		Throughput = MaxElements * FMath::CeilToInt(Avg);
	}
	
	virtual bool Factory_GrabOutput_Implementation(UFGFactoryConnectionComponent* connection, FInventoryItem& out_item, float& out_OffsetBeyond, TSubclassOf<UFGItemDescriptor> type) override
	{
		if(!IsValid(Input)) return false;
		
		if(Input->Factory_GrabOutput(out_item, out_OffsetBeyond, type))
		{
			CyclePassed++;
			TotalPassed++;
			return true;
		}

		return false;
	}
	
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
	TArray<double> PassedHistory;

	UPROPERTY(EditDefaultsOnly)
	double LoopTime = 1;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UFGFactoryConnectionComponent* Input;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UFGFactoryConnectionComponent* Output;
};
