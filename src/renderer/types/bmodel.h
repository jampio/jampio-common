#pragma once

#include <jampio/shared/vec.h>
#include "msurface.h"

typedef struct {
	vec3_t		bounds[2];		// for culling
	msurface_t	*firstSurface;
	int			numSurfaces;
} bmodel_t;