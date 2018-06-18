#pragma once

#include <jampio/shared/vec.h>

typedef struct surfaceSprite_s {
	int				surfaceSpriteType;
	float			width, height, density, wind, windIdle, fadeDist, fadeMax, fadeScale;
	float			fxAlphaStart, fxAlphaEnd, fxDuration, vertSkew;
	vec2_t			variance, fxGrow;
	int				facing;		// Hangdown on vertical sprites, faceup on others.
} surfaceSprite_t;