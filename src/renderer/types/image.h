#pragma once

#include <jampio/shared/shared.h>
#include <cstdint>

typedef struct image_s {
	char		imgName[MAX_QPATH];		// game path, including extension
	unsigned short		width, height;	// after power of two and picmip but not including clamp to MAX_TEXTURE_SIZE
	uint32_t		texnum;					// gl texture binding

	int			frameUsed;			// for texture usage in frame statistics

	int			internalFormat;
	int			wrapClampMode;		// GL_CLAMP or GL_REPEAT

	bool		mipmap;
	bool		allowPicmip;

	short		iLastLevelUsedOn;

} image_t;