#pragma once

struct SBoneCalc
{
	int				newFrame;
	int				currentFrame;
	float			backlerp;
	float			blendFrame;
	int				blendOldFrame;
	bool			blendMode;
	float			blendLerp;
};