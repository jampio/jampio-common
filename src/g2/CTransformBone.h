#pragma once

#include "mdx.h"

class CTransformBone
{
public:
#ifdef _XBOX
	float			renderMatrix[16];
#endif
	int				touch; // for minimal recalculation
	//rww - RAGDOLL_BEGIN
	int				touchRender;
	//rww - RAGDOLL_END
	mdxaBone_t		boneMatrix; //final matrix
	int				parent; // only set once
#ifdef _XBOX
	// This shouldn't be done like this. use declspec(aligned)?!
	int				pad[1]; // must be 16-byte aligned!
#endif
	CTransformBone()
	{
		touch=0;
	//rww - RAGDOLL_BEGIN
		touchRender = 0;
	//rww - RAGDOLL_END
	}

};