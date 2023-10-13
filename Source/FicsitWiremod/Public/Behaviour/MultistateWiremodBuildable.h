// 

#pragma once

#include "CoreMinimal.h"
#include "FGWiremodBuildable.h"
#include "MultistateWiremodBuildable.generated.h"

USTRUCT(BlueprintType)
struct FWiremodBuildableState
{
	GENERATED_BODY()

	/*
	 * Name for this state
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(MultiLine = true))
	FText Name;

	/*
	 * Description of this state
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(MultiLine = true))
	FText Description;


	/*
	 * Connections that this buildable has in this state
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FBuildingConnections Connections;

	/*
	 * Icon for this state.
	 * Icon is displayed in the UI and printed on the gate when it's placed down
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UTexture2D* Icon;

	/*
	 * Whether the buildable should disconnect all of its inputs when switching to this state
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool ForceDisconnectInputs = true;

	/**
	 * Whether this state should be hidden from the user and only used as an internal building state.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool IsHidden = false;
};



UCLASS()
class FICSITWIREMOD_API AMultistateWiremodBuildable : public AFGWiremodBuildable
{
	GENERATED_BODY()

public:
	AMultistateWiremodBuildable(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get()) : Super(ObjectInitializer)
	{
		AllowStatelessEdit = false;
	}

	virtual void BeginPlay() override
	{
		Super::BeginPlay();

		ConnectionsInfo = States[CurrentStateIndex].Connections;
	}

	virtual void GetLifetimeReplicatedProps( TArray<FLifetimeProperty>& OutLifetimeProps ) const override
	{
		Super::GetLifetimeReplicatedProps(OutLifetimeProps);

		DOREPLIFETIME(AMultistateWiremodBuildable, CurrentStateIndex);
	}

	UFUNCTION(BlueprintCallable)
	virtual void OnStateSelected(int Index, UObject* Setter)
	{
		if(!GetCanConfigure(Setter)) return;
		if(!States.IsValidIndex(Index)) return;
		
		OnStateSelected_Internal(Index);
	}

	UFUNCTION(BlueprintCallable)
	virtual TArray<FWiremodBuildableState> GetAvailableModes(bool IncludeHidden = false)
	{
		TArray<FWiremodBuildableState> Out;

		for(auto State : States)
			if(!State.IsHidden || IncludeHidden) Out.Add(State);

		return Out;
	}

	virtual void OnStateSelected_Internal(int Index)
	{
		auto NewState = States[Index];
		if(NewState.ForceDisconnectInputs) OnInputDisconnected_Internal(-1);
		
		CurrentStateIndex = Index;
		ConnectionsInfo = NewState.Connections;
	}

	UFUNCTION(BlueprintPure)
	FWiremodBuildableState GetCurrentState()
	{
		if(!States.IsValidIndex(CurrentStateIndex)) return FWiremodBuildableState();
		
		return States[CurrentStateIndex];
	}

	virtual UTexture2D* GetTexture() override
	{
		if(!States.IsValidIndex(CurrentStateIndex)) return Super::GetTexture();
		
		return States[CurrentStateIndex].Icon;
	}

	UPROPERTY(VisibleInstanceOnly, BlueprintReadWrite, SaveGame, Replicated)
	int CurrentStateIndex;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FWiremodBuildableState> States;
};
