#pragma once

#include <jampio/shared/shared.h>

typedef struct 
{
	int			mType;
	int			mSide;
	vec3_t		mOrigin;
} rmAutomapSymbol_t;

#define	MAX_AUTOMAP_SYMBOLS	512