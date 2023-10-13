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
	virtual void ServerProcess_Implementation(double DeltaTime) override
	{
		Lift = GetConnection(0).GetEntity();
		FloorIndex = GetConnection(1).GetFloat();

		if(IsConnected(2)) SetFloorName();
		if(IsConnected(3)) SetFloorHeight();
		if(IsConnected(4)) SetFloorIndex();
	}
	
	void SetFloorName()
	{
		const auto Name = GetConnection(2).GetString();
		if(Name == GetFloorName()) return;
		
		struct { int FloorIndex; FString Name; } Params {FloorIndex, Name};
		FConnectionData(Lift, "netFunc_SetFloorName").ProcessFunction(&Params);
	}
	
	void SetFloorHeight()
	{
		const auto Height = GetConnection(3).GetFloat();
		if(Height == GetFloorHeight()) return;
		
		struct { int FloorIndex; double Height; } Params {FloorIndex, Height};
		FConnectionData(Lift, "netFunc_SetFloorHeight").ProcessFunction(&Params);
	}
	
	void SetFloorIndex()
	{
		const int NewFloor = GetConnection(4).GetFloat();
		if(NewFloor == GetCurrentFloor()) return;

		FConnectionData(Lift, "serverMoveToFloor").SetFloat(NewFloor);
	}

	UFUNCTION()
	FString GetFloorName() const
	{
		struct {int FloorIndex; double Out_Height; FString Out_Name; } Params{FloorIndex};
		FConnectionData(Lift, "netFunc_GetFloorInfo").ProcessFunction(&Params);
		return Params.Out_Name;
	}

	UFUNCTION()
	double GetFloorHeight() const
	{
		struct {int FloorIndex; double Out_Height; FString Out_Name; } Params{FloorIndex};
		FConnectionData(Lift, "netFunc_GetFloorInfo").ProcessFunction(&Params);
		return Params.Out_Height;
	}

	UFUNCTION()
	int GetCurrentFloor() const { return (int) FConnectionData(Lift, "CurrentFloor", Integer, true).GetFloat(); }


	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override
	{
		Super::GetLifetimeReplicatedProps(OutLifetimeProps);

		DOREPLIFETIME(ALinearMotionAdapter, Lift)
		DOREPLIFETIME(ALinearMotionAdapter, FloorIndex)
	}

	UPROPERTY(Replicated, SaveGame)
	AActor* Lift;

	UPROPERTY(Replicated, SaveGame)
	int FloorIndex;
};
