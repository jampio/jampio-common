#pragma once

#include "genFunc.h"

typedef struct {
	genFunc_t	func;

	float base;
	float amplitude;
	float phase;
	float frequency;
} waveForm_t;