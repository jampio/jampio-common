#pragma once

#include <jampio/shared/vec.h>
#include "image.h"
#include "texCoordGen.h"
#include "texModInfo.h"

typedef struct {
	image_t			*image;

	texCoordGen_t	tcGen;
	vec3_t			*tcGenVectors;

	texModInfo_t	*texMods;
	short			numTexMods;
	short			numImageAnimations;
	float			imageAnimationSpeed;

	bool			isLightmap;
	bool			oneShotAnimMap;
	bool			vertexLightmap;
	bool			isVideoMap;

	int				videoMapHandle;
} textureBundle_t;