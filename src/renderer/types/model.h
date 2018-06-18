#pragma once

#include "../../g2/mdx.h"
#include "modtype.h"
#include "bmodel.h"

typedef struct model_s {
	char		name[MAX_QPATH];
	modtype_t	type;
	int			index;				// model = tr.models[model->index]

	int			dataSize;			// just for listing purposes
	bmodel_t	*bmodel;			// only if type == MOD_BRUSH
	md3Header_t	*md3[MD3_MAX_LODS];	// only if type == MOD_MESH
/*
Ghoul2 Insert Start
*/
	mdxmHeader_t *mdxm;				// only if type == MOD_GL2M which is a GHOUL II Mesh file NOT a GHOUL II animation file
	mdxaHeader_t *mdxa;				// only if type == MOD_GL2A which is a GHOUL II Animation file
/*
Ghoul2 Insert End
*/
	int			 numLods;
	qboolean	bspInstance;
} model_t;