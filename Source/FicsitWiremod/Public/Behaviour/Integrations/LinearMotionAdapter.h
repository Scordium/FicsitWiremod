// 

#pragma once

#include "CoreMinimal.h"
#include "Behaviour/FGWiremodBuildable.h"
#include "LinearMotionAdapter.generated.h"

UCLASS()
class FICSITWIREMOD_API ALinearMotionAdapter : public AFGWiremodBuildable
{
	GENERATED_BODY()

public:
	virtual void Process_Implementation(float DeltaTime) override
	{
		Lift = GetConnection(0).GetEntity();
		FloorIndex = GetConnection(1).GetFloat();

		if(IsConnected(2)) SetFloorName();
		if(IsConnected(3)) SetFloorHeight();
	}

	UFUNCTION()
	void SetFloorName()
	{
		auto Name = GetConnection(2).GetString();
		if(Name == GetFloorName()) return;

		
		auto data = FConnectionData(Lift, "netFunc_SetFloorName");
		struct { int floorIndex; FString name; } params {FloorIndex, Name};
		data.ProcessFunction(&params);
	}

	UFUNCTION()
	void SetFloorHeight()
	{
		auto Height = GetConnection(3).GetFloat();
		if(Height == GetFloorHeight()) return;
		
		auto data = FConnectionData(Lift, "netFunc_SetFloorHeight");
		struct { int floorIndex; float height; } params {FloorIndex, Height};
		data.ProcessFunction(&params);
	}
	

	UFUNCTION()
	FString GetFloorName()
	{
		auto data = FConnectionData(Lift, "netFunc_GetFloorInfo");
		struct {int floorIndex; float out_Height; FString out_Name; } params{FloorIndex};
		data.ProcessFunction(&params);
		return params.out_Name;
	}

	UFUNCTION()
	float GetFloorHeight()
	{
		auto data = FConnectionData(Lift, "netFunc_GetFloorInfo");
		struct {int floorIndex; float out_Height; FString out_Name; } params{FloorIndex};
		data.ProcessFunction(&params);
		return params.out_Height;
	}


	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override
	{
		Super::GetLifetimeReplicatedProps(OutLifetimeProps);

		DOREPLIFETIME(ALinearMotionAdapter, Lift)
		DOREPLIFETIME(ALinearMotionAdapter, FloorIndex)
	}

	UPROPERTY(Replicated)
	AActor* Lift;

	UPROPERTY(Replicated, SaveGame)
	int FloorIndex;
};
