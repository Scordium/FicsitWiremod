// 


#include "CommonLib/InterpolationFuncs.h"

float UInterpolationFuncs::BounceIn(double A, double B, double Alpha)
{
	const double ModifiedAlpha = TransformAlpha_BounceIn(Alpha);
	return FMath::Lerp(A, B, ModifiedAlpha);
}

float UInterpolationFuncs::BounceInOut(double A, double B, double Alpha)
{
	const double ModifiedAlpha = TransformAlpha_BounceInOut(Alpha);
	return FMath::Lerp(A, B, ModifiedAlpha);
}

float UInterpolationFuncs::BounceOut(double A, double B, double Alpha)
{
	const double ModifiedAlpha = TransformAlpha_BounceOut(Alpha);
	return FMath::Lerp(A, B, ModifiedAlpha);
}

float UInterpolationFuncs::TransformAlpha_BounceIn(double Alpha)
{
	return 1 - TransformAlpha_BounceOut(1 - Alpha);
}

float UInterpolationFuncs::TransformAlpha_BounceOut(double Alpha)
{
	if(Alpha < 4/11.0) return (121 * Alpha * Alpha)/16.0;
	else if(Alpha < 8/11.0) return (363/40.0 * Alpha * Alpha) - (99/10.0 * Alpha) + 17/5.0;
	else if(Alpha < 9/10.0) return (4356/361.0 * Alpha * Alpha) - (35442/1805.0 * Alpha) + 16061/1805.0;
	else return (54/5.0 * Alpha * Alpha) - (513/25.0 * Alpha) + 268/25.0;
}

float UInterpolationFuncs::TransformAlpha_BounceInOut(double Alpha)
{
	if(Alpha < 0.5) return 0.5 * TransformAlpha_BounceIn(Alpha*2);
	else return 0.5 * TransformAlpha_BounceOut(Alpha * 2 - 1) + 0.5;
}
