#pragma once

#include <jampio/shared/shared.h>
#include "textureBundle.h"
#include "waveForm.h"
#include "colorGen.h"
#include "alphaGen.h"
#include "acff.h"
#include "surfaceSprite.h"
#include "EGLFogOverride.h"

constexpr auto NUM_TEXTURE_BUNDLES = 2;

typedef struct {
	bool			active;
	bool			isDetail;
#ifdef _XBOX
	byte			isEnvironment;
#endif
#ifdef VV_LIGHTING
	byte			isSpecular;
	byte			isBumpMap;
#endif 
	byte			index;						// index of stage
	byte			lightmapStyle;
	
	textureBundle_t	bundle[NUM_TEXTURE_BUNDLES];

	waveForm_t		rgbWave;
	colorGen_t		rgbGen;

	waveForm_t		alphaWave;
	alphaGen_t		alphaGen;

	byte			constantColor[4];			// for CGEN_CONST and AGEN_CONST

	unsigned int	stateBits;					// GLS_xxxx mask

	acff_t			adjustColorsForFog;

	EGLFogOverride	mGLFogColorOverride;

	surfaceSprite_t	*ss;

	// Whether this object emits a glow or not.
	bool			glow;
} shaderStage_t;