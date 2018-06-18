#pragma once

typedef enum {
	FP_NONE,		// surface is translucent and will just be adjusted properly
	FP_EQUAL,		// surface is opaque but possibly alpha tested
	FP_LE,			// surface is trnaslucent, but still needs a fog pass (fog surface)
	FP_GLFOG
} fogPass_t;