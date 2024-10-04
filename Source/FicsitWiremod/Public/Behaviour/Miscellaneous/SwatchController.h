// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Behaviour/FGWiremodBuildable.h"
#include "SwatchController.generated.h"

UCLASS()
class FICSITWIREMOD_API ASwatchController : public AFGWiremodBuildable
{
	GENERATED_BODY()

public:

	virtual void ServerProcess_Implementation(double DeltaTime) override
	{
		if(!IsConnected(0) || !IsConnected(1) || !IsConnected(2)) return;
		
		const uint8 Slot = GetConnection(0).GetFloat();
		const auto PrimaryColor = GetConnection(1).GetColor();
		const auto SecondaryColor = GetConnection(2).GetColor();

		if(Cache.PrimaryColor == PrimaryColor && Cache.SecondaryColor == SecondaryColor && Slot == SlotCache) return;
		
		if(State == nullptr) State = Cast<AFGGameState>(UGameplayStatics::GetGameState(this));
		
		if(State)
		{
			Cache.PrimaryColor = PrimaryColor;
			Cache.SecondaryColor = SecondaryColor;
			SlotCache = Slot;
			State->Server_SetBuildingColorDataForSlot(Slot, Cache);
		}
	}

	UPROPERTY()
	AFGGameState* State;

	UPROPERTY()
	FFactoryCustomizationColorSlot Cache;

	UPROPERTY()
	int SlotCache;
};

UCLASS()
class ACustomSwatchController : public AFGWiremodBuildable
{
	GENERATED_BODY()

public:

	virtual void ServerProcess_Implementation(double DeltaTime) override
	{
		if(!IsConnected(0) || !IsConnected(1) || !IsConnected(2)) return;
		
		const auto Player = Cast<AFGCharacterPlayer>(GetConnection(0).GetEntity());

		if(!Player) return;

		const auto PlayerState = Cast<AFGPlayerState>(Player->GetPlayerState());
		if(!PlayerState) return;

		const auto PrimaryColor = GetConnection(1).GetColor();
		const auto SecondaryColor = GetConnection(2).GetColor();
		const auto Metallic = GetConnection(3).GetFloat();
		const auto Roughness = GetConnection(4).GetFloat();

		auto SlotData = FFactoryCustomizationColorSlot(PrimaryColor, SecondaryColor);

		if(SlotData == Cache) return;

		PlayerState->Server_SetPlayerCustomizationSlotData(SlotData);
		Cache = SlotData;
	}

	UPROPERTY()
	FFactoryCustomizationColorSlot Cache;
	
};
