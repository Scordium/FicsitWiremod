// 

#pragma once

#include "CoreMinimal.h"
#include "Behaviour/FGWiremodBuildable.h"
#include "CommonLib/TextureUtilities.h"
#include "ReallySimpleSignsAdapter.generated.h"

UCLASS()
class FICSITWIREMOD_API AReallySimpleSignsAdapter : public AFGWiremodBuildable
{
	GENERATED_BODY()

public:
	virtual void Process_Implementation(float DeltaTime) override
	{
		Sign = Cast<AFGBuildable>(GetConnection(0).GetEntity());
		ElementIndex = GetConnection(1).GetFloat();

		if(IsConnected(2)) SetElementText();
		if(IsConnected(3)) SetElementTextSize();
		if(IsConnected(4)) SetElementPosition();
		if(IsConnected(5)) SetElementRotation();
		if(IsConnected(6)) SetElementZIndex();
		if(IsConnected(7)) SetElementColor();
		if(IsConnected(8)) SetElementOpacity();
		if(IsConnected(9)) SetSignBackgroundColor();
		if(IsConnected(10)) SetSignEmission();
		if(IsConnected(11)) SetTextElementIsBold();
		if(IsConnected(12)) SetTextElementIsUpperCase();
		if(IsConnected(13)) SetElementScaleAndSpeed();
		if(IsConnected(14)) SetElementJustify();
		if(IsConnected(15)) SetImageElementSize();
		if(IsConnected(16)) SetImageElementOverwriteSize();
		if(IsConnected(17)) SetElementPannerIsStep();
		if(IsConnected(18)) SetPannerStepFrequency();
		if(IsConnected(19)) SetImageElementTexture();
	}


	void SetElementText()
	{
		struct{ FString arg1; int32 elementIndex; } params{ WM_GetString(2), ElementIndex};

		auto data = FConnectionData(Sign, "netFunc_Element_SetText");
		data.ProcessFunction(&params);
	}

	void SetElementTextSize()
	{
		struct{ int arg1; int32 elementIndex; } params{ (int)GetConnection(3).GetFloat(), ElementIndex};
		
		auto data = FConnectionData(Sign, "netFunc_Element_SetTextSize");
		data.ProcessFunction(&params);
	}
	
	void SetElementPosition()
	{
		struct{ FVector2D arg1; int32 elementIndex; } params{ FVector2D(GetConnection(4).GetVector()), ElementIndex};
		
		auto data = FConnectionData(Sign, "netFunc_Element_SetPosition");
		data.ProcessFunction(&params);
	}
	
	void SetElementRotation()
	{
		struct{ float arg1; int32 elementIndex; } params{ GetConnection(5).GetFloat(), ElementIndex};
		
		auto data = FConnectionData(Sign, "netFunc_Element_SetRotation");
		data.ProcessFunction(&params);
	}
	
	void SetElementZIndex()
	{
		struct{ int arg1; int32 elementIndex; } params{ (int)GetConnection(6).GetFloat(), ElementIndex};
		
		auto data = FConnectionData(Sign, "netFunc_Element_SetZIndex");
		data.ProcessFunction(&params);
	}
	
	void SetElementColor()
	{
		struct{ FLinearColor arg1; int32 elementIndex; } params{ GetConnection(7).GetColor(), ElementIndex};
		
		auto data = FConnectionData(Sign, "netFunc_Element_SetColor");
		data.ProcessFunction(&params);
	}
	
	void SetElementOpacity()
	{
		struct{ float arg1; int32 elementIndex; } params{GetConnection(8).GetFloat(), ElementIndex};
		
		auto data = FConnectionData(Sign, "netFunc_Element_SetOpacity");
		data.ProcessFunction(&params);
	}
	
	void SetSignBackgroundColor()
	{
		struct{ FLinearColor arg1; } params{GetConnection(9).GetColor()};
		
		auto data = FConnectionData(Sign, "netFunc_SetSignBackgroundColor");
		data.ProcessFunction(&params);
	}
	
	void SetSignEmission()
	{
		struct{ float arg1; } params{GetConnection(10).GetFloat()};
		
		auto data = FConnectionData(Sign, "netFunc_SetSignEmmisive");
		data.ProcessFunction(&params);
	}
	
	void SetTextElementIsBold()
	{
		struct{ bool arg1; int32 elementIndex; } params{GetConnection(11).GetBool(), ElementIndex};
		
		auto data = FConnectionData(Sign, "netFunc_Element_SetIsBold");
		data.ProcessFunction(&params);
	}

	void SetTextElementIsUpperCase()
	{
		struct{ bool arg1; int32 elementIndex; } params{ GetConnection(12).GetBool(), ElementIndex};
		
		auto data = FConnectionData(Sign, "netFunc_Element_SetUppercase");
		data.ProcessFunction(&params);
	}
	
	void SetElementScaleAndSpeed()
	{
		struct{ FLinearColor arg1; } params{ GetConnection(13).GetColor()};
		
		auto data = FConnectionData(Sign, "netFunc_Material_SetAnimation");
		data.ProcessFunction(&params);
	}
	
	void SetElementJustify()
	{
		struct{ int arg1; int32 elementIndex; } params{ (int)GetConnection(14).GetFloat(), ElementIndex};
		
		auto data = FConnectionData(Sign, "netFunc_Element_SetJustify");
		data.ProcessFunction(&params);
	}
	
	void SetImageElementSize()
	{
		struct{ FVector2D arg1; int32 elementIndex; } params{ FVector2D(GetConnection(15).GetVector()), ElementIndex};
		
		auto data = FConnectionData(Sign, "netFunc_Element_SetImageSize");
		data.ProcessFunction(&params);
	}
	
	void SetImageElementOverwriteSize()
	{
		struct{ FVector2D arg1; int32 elementIndex;} params{ FVector2D(GetConnection(16).GetVector()), ElementIndex};
		
		auto data = FConnectionData(Sign, "netFunc_Element_SetOverwriteImageSize");
		data.ProcessFunction(&params);
	}
	
	void SetElementPannerIsStep()
	{
		struct{ bool arg1; int32 elementIndex; } params{ GetConnection(17).GetBool(), ElementIndex};
		
		auto data = FConnectionData(Sign, "netFunc_Element_SetPannerType");
		data.ProcessFunction(&params);
	}
	
	void SetPannerStepFrequency()
	{
		struct{ float arg1; int32 elementIndex; } params{ GetConnection(18).GetFloat(), ElementIndex};
		
		auto data = FConnectionData(Sign, "netFunc_Element_SetStepFrequenz");
		data.ProcessFunction(&params);
	}
	
	void SetImageElementTexture()
	{
		struct{ UTexture* arg1; int32 elementIndex; } params{ UTextureUtilities::GetTextureFromIconId(GetConnection(19).GetFloat()), ElementIndex};
		
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
	
	
	UPROPERTY()
	AFGBuildable* Sign;
	int ElementIndex;
};
