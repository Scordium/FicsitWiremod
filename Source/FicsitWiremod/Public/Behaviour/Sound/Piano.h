// 

#pragma once

#include "CoreMinimal.h"
#include "Behaviour/FGWiremodBuildable.h"
#include "Piano.generated.h"

UCLASS()
class FICSITWIREMOD_API APiano : public AFGWiremodBuildable
{
	GENERATED_BODY()

public:
	virtual void ClientProcess_Implementation(double DeltaTime) override
	{
		auto OctaveValue = (int) GetConnection(0).GetFloat();
		if(Octave != OctaveValue) OnOctaveChanged(OctaveValue);
		Octave = OctaveValue;
		
		Range = GetConnection(1).GetFloat(1500);
		Volume = GetConnection(2).GetFloat(1);
		Damper = GetConnection(3).GetFloat(0.2);

		auto AMinor = GetConnection(4).GetBool();
		auto A = GetConnection(5).GetBool();
		auto BMinor = GetConnection(6).GetBool();
		auto B = GetConnection(7).GetBool();
		auto C = GetConnection(8).GetBool();
		auto DMinor = GetConnection(9).GetBool();
		auto D = GetConnection(10).GetBool();
		auto EMinor = GetConnection(11).GetBool();
		auto E = GetConnection(12).GetBool();
		auto F = GetConnection(13).GetBool();
		auto GMinor = GetConnection(14).GetBool();
		auto G = GetConnection(15).GetBool();

		if(AMinor)
		{
			if(!ActiveAMinor) OnAMinorChanged(true);
		}
		else OnAMinorChanged(false);

		if(A)
		{
			if(!ActiveA) OnAChanged(true);
		}
		else OnAChanged(false);
		
		if(BMinor)
		{
			if(!ActiveBMinor) OnBMinorChanged(true);
		}
		else OnBMinorChanged(false);

		if(B)
		{
			if(!ActiveB) OnBChanged(true);
		}
		else OnBChanged(false);
		
		if(C)
		{
			if(!ActiveC) OnCChanged(true);
		}
		else OnCChanged(false);

		if(DMinor)
		{
			if(!ActiveDMinor) OnDMinorChanged(true);
		}
		else OnDMinorChanged(true);
		
		if(D)
		{
			if(!ActiveD) OnDChanged(true);
		}
		else OnDChanged(false);

		if(EMinor)
		{
			if(!ActiveEMinor) OnEMinorChanged(true);
		}
		else OnEMinorChanged(false);

		if(E)
		{
			if(!ActiveE) OnEChanged(true);
		}
		else OnEChanged(false);

		if(F)
		{
			if(!ActiveF) OnFChanged(true);
		}
		else OnFChanged(false);

		if(GMinor)
		{
			if(!ActiveGMinor) OnGMinorChanged(true);
		}
		else OnGMinorChanged(false);

		if(G)
		{
			if(!ActiveG) OnGChanged(true);
		}
		else OnGChanged(false);


		ActiveAMinor = AMinor;
		ActiveA = A;
		ActiveBMinor = BMinor;
		ActiveB = B;
		ActiveC = C;
		ActiveDMinor = DMinor;
		ActiveD = D;
		ActiveEMinor = EMinor;
		ActiveE = E;
		ActiveF = F;
		ActiveGMinor = GMinor;
		ActiveG = G;
	}

	UFUNCTION(BlueprintImplementableEvent)
	void OnAMinorChanged(bool Active);

	UFUNCTION(BlueprintImplementableEvent)
	void OnAChanged(bool Active);

	UFUNCTION(BlueprintImplementableEvent)
	void OnBMinorChanged(bool Active);

	UFUNCTION(BlueprintImplementableEvent)
	void OnBChanged(bool Active);

	UFUNCTION(BlueprintImplementableEvent)
	void OnCChanged(bool Active);

	UFUNCTION(BlueprintImplementableEvent)
	void OnDMinorChanged(bool Active);

	UFUNCTION(BlueprintImplementableEvent)
	void OnDChanged(bool Active);

	UFUNCTION(BlueprintImplementableEvent)
	void OnEMinorChanged(bool Active);

	UFUNCTION(BlueprintImplementableEvent)
	void OnEChanged(bool Active);

	UFUNCTION(BlueprintImplementableEvent)
	void OnFChanged(bool Active);

	UFUNCTION(BlueprintImplementableEvent)
	void OnGMinorChanged(bool Active);
	
	UFUNCTION(BlueprintImplementableEvent)
	void OnGChanged(bool Active);

	UFUNCTION(BlueprintImplementableEvent)
	void OnOctaveChanged(int NewOctave);
	
	bool ActiveAMinor;
	bool ActiveA;
	bool ActiveBMinor;
	bool ActiveB;
	bool ActiveC;
	bool ActiveDMinor;
	bool ActiveD;
	bool ActiveEMinor;
	bool ActiveE;
	bool ActiveF;
	bool ActiveGMinor;
	bool ActiveG;

	UPROPERTY(BlueprintReadOnly)
	int Octave;

	UPROPERTY(BlueprintReadOnly)
	double Range;

	UPROPERTY(BlueprintReadOnly)
	double Volume;

	UPROPERTY(BlueprintReadOnly)
	double Damper;
};
