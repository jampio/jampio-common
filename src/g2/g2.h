#pragma once

//rww - RAGDOLL_BEGIN
#define G2T_SV_TIME (0)
#define G2T_CG_TIME (1)
#define NUM_G2T_TIME (2)

#if 0
#ifdef _SOF2
typedef enum
{
	PGORE_NONE,
	PGORE_ARMOR,
	PGORE_BULLETSMALL,
	PGORE_BULLETMED,
	PGORE_BULLETBIG,
	PGORE_HEGRENADE,
	PGORE_COUNT
} goreEnum_t;

struct goreEnumShader_t
{
	goreEnum_t		shaderEnum;
	char			shaderName[MAX_QPATH];
};

struct SSkinGoreData
{
	vec3_t			angles;
	vec3_t			position;
	int				currentTime;
	int				entNum;
	vec3_t			rayDirection;	// in world space
	vec3_t			hitLocation;	// in world space
	vec3_t			scale;
	float			SSize;			// size of splotch in the S texture direction in world units
	float			TSize;			// size of splotch in the T texture direction in world units
	float			theta;			// angle to rotate the splotch

//	qhandle_t		shader;			// handle to shader for gore, this better be rendered after the shader of the underlying surface					
									// this shader should also have "clamp" mode, not tiled.
	goreEnum_t		shaderEnum;		// enum that'll get switched over to the shader's actual handle
};
#endif // _SOF2
#endif

#define MAX_GHOUL_COUNT_BITS 8 // bits required to send across the MAX_G2_MODELS inside of the networking - this is the only restriction on ghoul models possible per entity

// defines for stuff to go into the mflags
#define		GHOUL2_NOCOLLIDE 0x001
#define		GHOUL2_NORENDER	 0x002
#define		GHOUL2_NOMODEL	 0x004
#define		GHOUL2_NEWORIGIN 0x008

//for transform optimization -rww
#define		GHOUL2_ZONETRANSALLOC	0x2000

// collision detection stuff
#define G2_FRONTFACE 1
#define	G2_BACKFACE	 0


// calling defines for the trace function
enum EG2_Collision
{
	G2_NOCOLLIDE,
	G2_COLLIDE,
	G2_RETURNONHIT
};


//====================================================================

