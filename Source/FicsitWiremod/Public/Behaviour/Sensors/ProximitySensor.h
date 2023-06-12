// 

#pragma once

#include "CoreMinimal.h"
#include "FGVehicle.h"
#include "Behaviour/FGWiremodBuildable.h"
#include "ProximitySensor.generated.h"

#define SCALE GetConnection(0).GetVector(FVector(10))
#define AREA_VISIBLE GetConnection(1).GetBool()
#define DETECT_PLAYER GetConnection(2).GetBool(true)
#define DETECT_ENEMY GetConnection(3).GetBool(true)
#define DETECT_PASSIVE GetConnection(4).GetBool(true)
#define DETECT_VEHICLE GetConnection(5).GetBool(true)
#define OFFSET GetConnection(6).GetVector()

UCLASS()
class FICSITWIREMOD_API AProximitySensor : public AFGWiremodBuildable
{
	GENERATED_BODY()

public:
	virtual void Process_Implementation(float DeltaTime) override
	{
		SetProximityData(SCALE, OFFSET, AREA_VISIBLE);
	}

	UFUNCTION()
	int GetCount() { return Overlapping.Num(); }

protected:
	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override
	{
		Super::NotifyActorBeginOverlap(OtherActor);

		if(IsPlayer(OtherActor) && DETECT_PLAYER) Overlapping.AddUnique(OtherActor);
		else if (IsHostile(OtherActor) && DETECT_ENEMY) Overlapping.AddUnique(OtherActor);
		else if (IsFriendly(OtherActor) && DETECT_PASSIVE) Overlapping.AddUnique(OtherActor);
		else if (IsVehicle(OtherActor) && DETECT_VEHICLE) Overlapping.AddUnique(OtherActor);
	}
	
	virtual void NotifyActorEndOverlap(AActor* OtherActor) override
	{
		Super::NotifyActorEndOverlap(OtherActor);
		Overlapping.Remove(OtherActor);
	}
	
	UFUNCTION(BlueprintImplementableEvent)
	void SetProximityData(FVector Scale, FVector Offset, bool Visible);

private:
	static bool IsPlayer(AActor* Entity) { return IsValid(Cast<AFGCharacterPlayer>(Entity)); }
	
	static bool IsHostile(AActor* Entity)
	{
		if(auto Creature = Cast<AFGCreature>(Entity)) return !Creature->IsPassiveCreature();
		return false;
	}
	static bool IsFriendly(AActor* Entity)
	{
		if(auto Creature = Cast<AFGCreature>(Entity)) return Creature->IsPassiveCreature();
		return false;
	}
	
	static bool IsVehicle(AActor* Entity) { return IsValid(Cast<AFGVehicle>(Entity)); }

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override
	{
		Super::GetLifetimeReplicatedProps(OutLifetimeProps);

		DOREPLIFETIME(AProximitySensor, Overlapping)
	}

public:
	UPROPERTY(Replicated)
	TArray<AActor*> Overlapping;
};
