#pragma once

#include <jampio/shared/shared.h>
#include "../../filefmt.h"
#include "skyParms.h"
#include "fogParms.h"
#include "cullType.h"
#include "fogPass.h"
#include "deformStage.h"
#include "shaderStage.h"

constexpr auto MAX_SHADER_DEFORMS = 3;

typedef struct shader_s {
	char		name[MAX_QPATH];		// game path, including extension
	int			lightmapIndex[MAXLIGHTMAPS];	// for a shader to match, both name and lightmapIndex must match
	byte		styles[MAXLIGHTMAPS];

	int			index;					// this shader == tr.shaders[index]
	int			sortedIndex;			// this shader == tr.sortedShaders[sortedIndex]

	float		sort;					// lower numbered shaders draw before higher numbered

	int			surfaceFlags;			// if explicitlyDefined, this will have SURF_* flags
	int			contentFlags;

	bool		defaultShader;			// we want to return index 0 if the shader failed to
										// load for some reason, but R_FindShader should
										// still keep a name allocated for it, so if
										// something calls RE_RegisterShader again with
										// the same name, we don't try looking for it again
	bool		explicitlyDefined;		// found in a .shader file
	bool		entityMergable;			// merge across entites optimizable (smoke, blood)

	bool		isBumpMap;

#ifdef _XBOX
	bool		needsNormal;
	bool		needsTangent;
#endif

	skyParms_t	*sky;
	fogParms_t	*fogParms;

	float		portalRange;			// distance to fog out at

	int			multitextureEnv;		// 0, GL_MODULATE, GL_ADD (FIXME: put in stage)

	cullType_t	cullType;				// CT_FRONT_SIDED, CT_BACK_SIDED, or CT_TWO_SIDED
	bool		polygonOffset;			// set for decals and other items that must be offset 
	bool		noMipMaps;				// for console fonts, 2D elements, etc.
	bool		noPicMip;				// for images that must always be full resolution
	bool		noTC;					// for images that don't want to be texture compressed (eg skies)

	fogPass_t	fogPass;				// draw a blended pass, possibly with depth test equals

	vec3_t		bumpVector;				// The given light vector for bump-mapping

	deformStage_t	*deforms[MAX_SHADER_DEFORMS];
	short		numDeforms;

	short		numUnfoggedPasses;
	shaderStage_t	*stages;		

  float clampTime;                                  // time this shader is clamped to
  float timeOffset;                                 // current time offset for this shader

#ifndef _XBOX	// GLOWXXX
	// True if this shader has a stage with glow in it (just an optimization).
	bool hasGlow;
#endif

/*
  int numStates;                                    // if non-zero this is a state shader
  struct shader_s *currentShader;                   // current state if this is a state shader
  struct shader_s *parentShader;                    // current state if this is a state shader
  int currentState;                                 // current state index for cycle purposes
  long expireTime;                                  // time in milliseconds this expires

  int shaderStates[MAX_STATES_PER_SHADER];          // index to valid shader states
*/

	struct shader_s *remappedShader;                  // current shader this one is remapped too
	struct	shader_s	*next;
} shader_t;