// 

#pragma once

#include "CoreMinimal.h"
#include "Behaviour/FGWiremodBuildable.h"
#include "Behaviour/MultistateWiremodBuildable.h"
#include "CommonLib/TextureUtilities.h"
#include "ReallySimpleSignsAdapter.generated.h"

UCLASS()
class FICSITWIREMOD_API AReallySimpleSignsAdapter : public AMultistateWiremodBuildable
{
	GENERATED_BODY()

public:
	virtual void Process_Implementation(float DeltaTime) override
	{
		ElementIndex = GetConnection(1).GetFloat();

		//Default mode - process 1 sign
		if(CurrentStateIndex == 0)
		{
			auto Sign = Cast<AFGBuildable>(GetConnection(0).GetEntity());
			ProcessSign(Sign);
		}
		//Array mode - process all signs in array
		else if(CurrentStateIndex == 1)
		{
			auto SignArray = GetConnection(0).GetEntityArray();
			for(auto Entity : SignArray)
			{
				if(auto Sign = Cast<AFGBuildable>(Entity)) ProcessSign(Sign);
			}
		}
	}

	

	void ProcessSign(AFGBuildable* Sign)
	{
		if(IsConnected(2)) SetElementText(Sign);
		if(IsConnected(3)) SetElementTextSize(Sign);
		if(IsConnected(4)) SetElementPosition(Sign);
		if(IsConnected(5)) SetElementRotation(Sign);
		if(IsConnected(6)) SetElementZIndex(Sign);
		if(IsConnected(7)) SetElementColor(Sign);
		if(IsConnected(8)) SetElementOpacity(Sign);
		if(IsConnected(9)) SetSignBackgroundColor(Sign);
		if(IsConnected(10)) SetSignEmission(Sign);
		if(IsConnected(11)) SetTextElementIsBold(Sign);
		if(IsConnected(12)) SetTextElementIsUpperCase(Sign);
		if(IsConnected(13)) SetElementScaleAndSpeed(Sign);
		if(IsConnected(14)) SetElementJustify(Sign);
		if(IsConnected(15)) SetImageElementSize(Sign);
		if(IsConnected(16)) SetImageElementOverwriteSize(Sign);
		if(IsConnected(17)) SetElementPannerIsStep(Sign);
		if(IsConnected(18)) SetPannerStepFrequency(Sign);
		if(IsConnected(19)) SetImageElementTexture(Sign);
		if(IsConnected(20)) SetImageElementTexture(Sign);
	}

	virtual void OnStateSelected_Internal(int Index) override
	{
		//Disconnect sign input
		OnInputDisconnected_Internal(0);
		Super::OnStateSelected_Internal(Index);
	}


	void SetElementText(AFGBuildable* Sign)
	{
		struct{ FString arg1; int32 elementIndex; } params{ WM_GetString(2), ElementIndex};

		auto data = FConnectionData(Sign, "netFunc_Element_SetText");
		data.ProcessFunction(&params);
	}

	void SetElementTextSize(AFGBuildable* Sign)
	{
		struct{ int arg1; int32 elementIndex; } params{ (int)GetConnection(3).GetFloat(), ElementIndex};
		
		auto data = FConnectionData(Sign, "netFunc_Element_SetTextSize");
		data.ProcessFunction(&params);
	}
	
	void SetElementPosition(AFGBuildable* Sign)
	{
		struct{ FVector2D arg1; int32 elementIndex; } params{ FVector2D(GetConnection(4).GetVector()), ElementIndex};
		
		auto data = FConnectionData(Sign, "netFunc_Element_SetPosition");
		data.ProcessFunction(&params);
	}
	
	void SetElementRotation(AFGBuildable* Sign)
	{
		struct{ float arg1; int32 elementIndex; } params{ GetConnection(5).GetFloat(), ElementIndex};
		
		auto data = FConnectionData(Sign, "netFunc_Element_SetRotation");
		data.ProcessFunction(&params);
	}
	
	void SetElementZIndex(AFGBuildable* Sign)
	{
		struct{ int arg1; int32 elementIndex; } params{ (int)GetConnection(6).GetFloat(), ElementIndex};
		
		auto data = FConnectionData(Sign, "netFunc_Element_SetZIndex");
		data.ProcessFunction(&params);
	}
	
	void SetElementColor(AFGBuildable* Sign)
	{
		struct{ FLinearColor arg1; int32 elementIndex; } params{ GetConnection(7).GetColor(), ElementIndex};
		
		auto data = FConnectionData(Sign, "netFunc_Element_SetColor");
		data.ProcessFunction(&params);
	}
	
	void SetElementOpacity(AFGBuildable* Sign)
	{
		struct{ float arg1; int32 elementIndex; } params{GetConnection(8).GetFloat(), ElementIndex};
		
		auto data = FConnectionData(Sign, "netFunc_Element_SetOpacity");
		data.ProcessFunction(&params);
	}
	
	void SetSignBackgroundColor(AFGBuildable* Sign)
	{
		struct{ FLinearColor arg1; } params{GetConnection(9).GetColor()};
		
		auto data = FConnectionData(Sign, "netFunc_SetSignBackgroundColor");
		data.ProcessFunction(&params);
	}
	
	void SetSignEmission(AFGBuildable* Sign)
	{
		struct{ float arg1; } params{GetConnection(10).GetFloat()};
		
		auto data = FConnectionData(Sign, "netFunc_SetSignEmmisive");
		data.ProcessFunction(&params);
	}
	
	void SetTextElementIsBold(AFGBuildable* Sign)
	{
		struct{ bool arg1; int32 elementIndex; } params{GetConnection(11).GetBool(), ElementIndex};
		
		auto data = FConnectionData(Sign, "netFunc_Element_SetIsBold");
		data.ProcessFunction(&params);
	}

	void SetTextElementIsUpperCase(AFGBuildable* Sign)
	{
		struct{ bool arg1; int32 elementIndex; } params{ GetConnection(12).GetBool(), ElementIndex};
		
		auto data = FConnectionData(Sign, "netFunc_Element_SetUppercase");
		data.ProcessFunction(&params);
	}
	
	void SetElementScaleAndSpeed(AFGBuildable* Sign)
	{
		struct{ FLinearColor arg1; } params{ GetConnection(13).GetColor()};
		
		auto data = FConnectionData(Sign, "netFunc_Material_SetAnimation");
		data.ProcessFunction(&params);
	}
	
	void SetElementJustify(AFGBuildable* Sign)
	{
		struct{ int arg1; int32 elementIndex; } params{ (int)GetConnection(14).GetFloat(), ElementIndex};
		
		auto data = FConnectionData(Sign, "netFunc_Element_SetJustify");
		data.ProcessFunction(&params);
	}
	
	void SetImageElementSize(AFGBuildable* Sign)
	{
		struct{ FVector2D arg1; int32 elementIndex; } params{ FVector2D(GetConnection(15).GetVector()), ElementIndex};
		
		auto data = FConnectionData(Sign, "netFunc_Element_SetImageSize");
		data.ProcessFunction(&params);
	}
	
	void SetImageElementOverwriteSize(AFGBuildable* Sign)
	{
		struct{ FVector2D arg1; int32 elementIndex;} params{ FVector2D(GetConnection(16).GetVector()), ElementIndex};
		
		auto data = FConnectionData(Sign, "netFunc_Element_SetOverwriteImageSize");
		data.ProcessFunction(&params);
	}
	
	void SetElementPannerIsStep(AFGBuildable* Sign)
	{
		struct{ bool arg1; int32 elementIndex; } params{ GetConnection(17).GetBool(), ElementIndex};
		
		auto data = FConnectionData(Sign, "netFunc_Element_SetPannerType");
		data.ProcessFunction(&params);
	}
	
	void SetPannerStepFrequency(AFGBuildable* Sign)
	{
		struct{ float arg1; int32 elementIndex; } params{ GetConnection(18).GetFloat(), ElementIndex};
		
		auto data = FConnectionData(Sign, "netFunc_Element_SetStepFrequenz");
		data.ProcessFunction(&params);
	}
	
	void SetImageElementTextureFromId(AFGBuildable* Sign)
	{
		SetTexture(Sign, UTextureUtilities::GetTextureFromIconId(GetConnection(19).GetFloat()));
	}

	void SetImageElementTexture(AFGBuildable* Sign)
	{
		SetTexture(Sign, GetConnection(20).GetTexture());
	}

	void SetTexture(AFGBuildable* Sign, UTexture* Texture)
	{
		struct{ UTexture* arg1; int32 elementIndex; } params{Texture, ElementIndex};
		
		auto data = FConnectionData(Sign, "netFunc_Element_SetTexture");
		data.ProcessFunction(&params);
	}
	
	UFUNCTION(BlueprintCallable)
	static int GetElementCountOfType(AFGBuildable* sign, int type)
	{
		struct{ int type; int ReturnValue; } params{type};
		auto data = FConnectionData(sign, "netFunc_GetNumOfElementsOfType");
		data.ProcessFunction(&params);
		return params.ReturnValue;
	}
	
	int ElementIndex;
};
