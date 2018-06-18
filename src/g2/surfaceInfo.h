#pragma once

struct surfaceInfo_t 
{
	int			offFlags;		// what the flags are for this model
	int			surface;		// index into array held inside the model definition of pointers to the actual surface data loaded in - used by both client and game
	float		genBarycentricJ;	// point 0 barycentric coors
	float		genBarycentricI;	// point 1 barycentric coors - point 2 is 1 - point0 - point1
	int			genPolySurfaceIndex; // used to point back to the original surface and poly if this is a generated surface
	int			genLod;			// used to determine original lod of original surface and poly hit location
	
surfaceInfo_t():
	offFlags(0),
	surface(0),
	genBarycentricJ(0),
	genBarycentricI(0),
	genPolySurfaceIndex(0),
	genLod(0)
	{}

};