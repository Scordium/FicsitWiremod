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
		Sign = Cast<AFGBuildable>(WM_GetEntity(0));
		ElementIndex = WM_GetInt(1);

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

		auto data = FNewConnectionData(Sign.Get(), "netFunc_Element_SetText");
		UWiremodReflection::ProcessFunction(data, &params);
	}

	void SetElementTextSize()
	{
		struct{ int arg1; int32 elementIndex; } params{ WM_GetInt(3), ElementIndex};
		
		auto data = FNewConnectionData(Sign.Get(), "netFunc_Element_SetTextSize");
		UWiremodReflection::ProcessFunction(data, &params);
	}
	
	void SetElementPosition()
	{
		struct{ FVector2D arg1; int32 elementIndex; } params{ FVector2D(WM_GetVector(4)), ElementIndex};
		
		auto data = FNewConnectionData(Sign.Get(), "netFunc_Element_SetPosition");
		UWiremodReflection::ProcessFunction(data, &params);
	}
	
	void SetElementRotation()
	{
		struct{ float arg1; int32 elementIndex; } params{ WM_GetFloat(5), ElementIndex};
		
		auto data = FNewConnectionData(Sign.Get(), "netFunc_Element_SetRotation");
		UWiremodReflection::ProcessFunction(data, &params);
	}
	
	void SetElementZIndex()
	{
		struct{ int arg1; int32 elementIndex; } params{ WM_GetInt(6), ElementIndex};
		
		auto data = FNewConnectionData(Sign.Get(), "netFunc_Element_SetZIndex");
		UWiremodReflection::ProcessFunction(data, &params);
	}
	
	void SetElementColor()
	{
		struct{ FLinearColor arg1; int32 elementIndex; } params{ WM_GetColor(7), ElementIndex};
		
		auto data = FNewConnectionData(Sign.Get(), "netFunc_Element_SetColor");
		UWiremodReflection::ProcessFunction(data, &params);
	}
	
	void SetElementOpacity()
	{
		struct{ float arg1; int32 elementIndex; } params{WM_GetFloat(8), ElementIndex};
		
		auto data = FNewConnectionData(Sign.Get(), "netFunc_Element_SetOpacity");
		UWiremodReflection::ProcessFunction(data, &params);
	}
	
	void SetSignBackgroundColor()
	{
		struct{ FLinearColor arg1; } params{WM_GetColor(9)};
		
		auto data = FNewConnectionData(Sign.Get(), "netFunc_SetSignBackgroundColor");
		UWiremodReflection::ProcessFunction(data, &params);
	}
	
	void SetSignEmission()
	{
		struct{ float arg1; } params{WM_GetFloat(10)};
		
		auto data = FNewConnectionData(Sign.Get(), "netFunc_SetSignEmmisive");
		UWiremodReflection::ProcessFunction(data, &params);
	}
	
	void SetTextElementIsBold()
	{
		struct{ bool arg1; int32 elementIndex; } params{WM_GetBool(11), ElementIndex};
		
		auto data = FNewConnectionData(Sign.Get(), "netFunc_Element_SetIsBold");
		UWiremodReflection::ProcessFunction(data, &params);
	}

	void SetTextElementIsUpperCase()
	{
		struct{ bool arg1; int32 elementIndex; } params{ WM_GetBool(12), ElementIndex};
		
		auto data = FNewConnectionData(Sign.Get(), "netFunc_Element_SetUppercase");
		UWiremodReflection::ProcessFunction(data, &params);
	}
	
	void SetElementScaleAndSpeed()
	{
		struct{ FLinearColor arg1; } params{ WM_GetColor(13)};
		
		auto data = FNewConnectionData(Sign.Get(), "netFunc_Material_SetAnimation");
		UWiremodReflection::ProcessFunction(data, &params);
	}
	
	void SetElementJustify()
	{
		struct{ int arg1; int32 elementIndex; } params{ WM_GetInt(14), ElementIndex};
		
		auto data = FNewConnectionData(Sign.Get(), "netFunc_Element_SetJustify");
		UWiremodReflection::ProcessFunction(data, &params);
	}
	
	void SetImageElementSize()
	{
		struct{ FVector2D arg1; int32 elementIndex; } params{ FVector2D(WM_GetVector(15)), ElementIndex};
		
		auto data = FNewConnectionData(Sign.Get(), "netFunc_Element_SetImageSize");
		UWiremodReflection::ProcessFunction(data, &params);
	}
	
	void SetImageElementOverwriteSize()
	{
		struct{ FVector2D arg1; int32 elementIndex;} params{ FVector2D(WM_GetVector(16)), ElementIndex};
		
		auto data = FNewConnectionData(Sign.Get(), "netFunc_Element_SetOverwriteImageSize");
		UWiremodReflection::ProcessFunction(data, &params);
	}
	
	void SetElementPannerIsStep()
	{
		struct{ bool arg1; int32 elementIndex; } params{ WM_GetBool(17), ElementIndex};
		
		auto data = FNewConnectionData(Sign.Get(), "netFunc_Element_SetPannerType");
		UWiremodReflection::ProcessFunction(data, &params);
	}
	
	void SetPannerStepFrequency()
	{
		struct{ float arg1; int32 elementIndex; } params{ WM_GetFloat(18), ElementIndex};
		
		auto data = FNewConnectionData(Sign.Get(), "netFunc_Element_SetStepFrequenz");
		UWiremodReflection::ProcessFunction(data, &params);
	}
	
	void SetImageElementTexture()
	{
		struct{ UTexture* arg1; int32 elementIndex; } params{ UTextureUtilities::GetTextureFromIconId(WM_GetInt(19)), ElementIndex};
		
		auto data = FNewConnectionData(Sign.Get(), "netFunc_Element_SetTexture");
		UWiremodReflection::ProcessFunction(data, &params);
	}
	
	
	
	
	
	TWeakObjectPtr<AFGBuildable> Sign;
	int ElementIndex;
};
