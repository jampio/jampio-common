#pragma once

#include <jampio/shared/vec.h>
#include "deform.h"
#include "waveForm.h"

typedef struct {
	deform_t	deformation;			// vertex coordinate modification type

	vec3_t		moveVector;
	waveForm_t	deformationWave;
	float		deformationSpread;

	float		bulgeWidth;
	float		bulgeHeight;
	float		bulgeSpeed;
} deformStage_t;