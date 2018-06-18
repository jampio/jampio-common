#pragma once

#include "mdx.h"

struct boltInfo_t{
	int			boneNumber;		// bone number bolt attaches to	
	int			surfaceNumber;	// surface number bolt attaches to 
	int			surfaceType;	// if we attach to a surface, this tells us if it is an original surface or a generated one - doesn't go across the network
	int			boltUsed;		// nor does this
	mdxaBone_t	position;		// this does not go across the network
	boltInfo_t():
	boneNumber(-1),
	surfaceNumber(-1),
	surfaceType(0),
	boltUsed(0)
	{}
};