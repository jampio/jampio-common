#pragma once

#include <vector>
#include "mdx.h"
#include "surfaceInfo.h"
#include "boneInfo.h"
#include "boltInfo.h"
#include "../renderer/types/model.h"
#include "CBoneCache.h"

class CGhoul2Info
{
public:
	typedef std::vector <surfaceInfo_t> surfaceInfo_v;
	typedef std::vector <boneInfo_t> boneInfo_v;
	typedef std::vector <boltInfo_t> boltInfo_v;

	surfaceInfo_v 	mSlist;
	boltInfo_v		mBltlist;
	boneInfo_v		mBlist;
// save from here
	int				mModelindex;
	qhandle_t		mCustomShader;
	qhandle_t		mCustomSkin;
	int				mModelBoltLink;
	int				mSurfaceRoot;
	int				mLodBias;
	int				mNewOrigin;	// this contains the bolt index of the new origin for this model
#ifdef _G2_GORE
	int				mGoreSetTag;
#endif
	qhandle_t		mModel;		// this and the next entries do NOT go across the network. They are for gameside access ONLY
	char			mFileName[MAX_QPATH];
	int				mAnimFrameDefault;
	int				mSkelFrameNum;
	int				mMeshFrameNum;
	int				mFlags;	// used for determining whether to do full collision detection against this object
// to here
	int				*mTransformedVertsArray;	// used to create an array of pointers to transformed verts per surface for collision detection
	CBoneCache		*mBoneCache;
	int				mSkin;

	// these occasionally are not valid (like after a vid_restart)
	// call the questionably efficient G2_SetupModelPointers(this) to insure validity
	bool				mValid; // all the below are proper and valid
	const model_s		*currentModel;
	int					currentModelSize;
	const model_s		*animModel;
	int					currentAnimModelSize;
	const mdxaHeader_t	*aHeader;

#ifdef _G2_LISTEN_SERVER_OPT
	int					entityNum;
#endif

	CGhoul2Info():
	mModelindex(-1),
	mCustomShader(0),
	mCustomSkin(0),
	mModelBoltLink(0),
	mModel(0),
	mSurfaceRoot(0),
	mAnimFrameDefault(0),
	mSkelFrameNum(-1),
	mMeshFrameNum(-1),
	mFlags(0),
	mTransformedVertsArray(0),
	mLodBias(0),
	mSkin(0),
	mNewOrigin(-1),
#ifdef _G2_GORE
	mGoreSetTag(0),
#endif
	mBoneCache(0),
	currentModel(0),
	currentModelSize(0),
	animModel(0),
	currentAnimModelSize(0),
	aHeader(0),
#ifdef _G2_LISTEN_SERVER_OPT
	entityNum(ENTITYNUM_NONE),
#endif
	mValid(false)
	{
		mFileName[0] = 0;
	}
}; 