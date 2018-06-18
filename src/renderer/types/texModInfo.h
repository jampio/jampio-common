#pragma once

#include "texMod.h"
#include "waveForm.h"

typedef struct {
	texMod_t		type;

	// used for TMOD_TURBULENT and TMOD_STRETCH
	waveForm_t		wave;

	// used for TMOD_TRANSFORM
	float			matrix[2][2];		// s' = s * m[0][0] + t * m[1][0] + trans[0]
	float			translate[2];		// t' = s * m[0][1] + t * m[0][1] + trans[1]

	// used for TMOD_SCALE
	//(moved to translate)
//	float			scale[2];			// s *= scale[0]
	                                    // t *= scale[1]

	// used for TMOD_SCROLL
	//(moved to translate)
//	float			scroll[2];			// s' = s + scroll[0] * time
										// t' = t + scroll[1] * time

	// + = clockwise
	// - = counterclockwise
	////(moved to translate[0])
//	float			rotateSpeed;

} texModInfo_t;
