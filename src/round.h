#pragma once

#ifdef __cplusplus
#include <cmath>
#else
#include <math.h>
#endif

#define RoundUp(N, M) ((N) + ((unsigned int)(M)) - (((unsigned int)(N)) % ((unsigned int)(M))))
#define RoundDown(N, M) ((N) - (((unsigned int)(N)) % ((unsigned int)(M))))

inline int Round(float value) {
	return((int)floorf(value + 0.5f));
}