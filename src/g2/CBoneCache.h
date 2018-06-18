#pragma once

#include <cassert>
#include <vector>
#include "mdx.h"
#include "SBoneCalc.h"
#include "CTransformBone.h"
#include "../renderer/types/model.h"
#include "transform.h"

class CBoneCache
{
	void SetRenderMatrix(CTransformBone *bone)
	{
#ifdef _XBOX
		float *src = bone->boneMatrix.matrix[0];
		float *dst = bone->renderMatrix;

		dst[0] = src[0];
		dst[1] = src[4];
		dst[2] = src[8];
		dst[3] = 0;

		dst[4] = src[1];
		dst[5] = src[5];
		dst[6] = src[9];
		dst[7] = 0;

		dst[8] = src[2];
		dst[9] = src[6];
		dst[10] = src[10];
		dst[11] = 0;

		dst[12] = src[3];
		dst[13] = src[7];
		dst[14] = src[11];
		dst[15] = 1;
#endif
	}

	void EvalLow(int index)
	{
		assert(index>=0&&index<mBones.size());
		if (mFinalBones[index].touch!=mCurrentTouch)
		{
			// need to evaluate the bone
#ifdef _XBOX
			assert((mFinalBones[index].parent>=0&&mFinalBones[index].parent<mNumBones)||(index==0&&mFinalBones[index].parent==-1));
#else
			assert((mFinalBones[index].parent>=0&&mFinalBones[index].parent<mFinalBones.size())||(index==0&&mFinalBones[index].parent==-1));
#endif
			if (mFinalBones[index].parent>=0)
			{
				EvalLow(mFinalBones[index].parent); // make sure parent is evaluated
				SBoneCalc &par=mBones[mFinalBones[index].parent];
				mBones[index].newFrame=par.newFrame;
				mBones[index].currentFrame=par.currentFrame;
				mBones[index].backlerp=par.backlerp;
				mBones[index].blendFrame=par.blendFrame;
				mBones[index].blendOldFrame=par.blendOldFrame;
				mBones[index].blendMode=par.blendMode;
				mBones[index].blendLerp=par.blendLerp;
			}
			G2_TransformBone(index,*this);
#ifdef _XBOX
			SetRenderMatrix(mFinalBones + index);
#endif
			mFinalBones[index].touch=mCurrentTouch;
		}
	}
//rww - RAGDOLL_BEGIN
	void SmoothLow(int index)
	{
		if (mSmoothBones[index].touch==mLastTouch)
		{
			int i;
			float *oldM=&mSmoothBones[index].boneMatrix.matrix[0][0];
			float *newM=&mFinalBones[index].boneMatrix.matrix[0][0];
#if 0 //this is just too slow. I need a better way.
			static float smoothFactor;

			smoothFactor = mSmoothFactor;

			//Special rag smoothing -rww
			if (smoothFactor < 0)
			{ //I need a faster way to do this but I do not want to store more in the bonecache
				static int blistIndex;
				assert(mod);
				assert(rootBoneList);
				blistIndex = G2_Find_Bone_ByNum(mod, *rootBoneList, index);

				assert(blistIndex != -1);

				boneInfo_t &bone = (*rootBoneList)[blistIndex];

				if (bone.flags & BONE_ANGLES_RAGDOLL)
				{
					if ((bone.RagFlags & (0x00008)) || //pelvis
                        (bone.RagFlags & (0x00004))) //model_root
					{ //pelvis and root do not smooth much
						smoothFactor = 0.2f;
					}
					else if (bone.solidCount > 4)
					{ //if stuck in solid a lot then snap out quickly
						smoothFactor = 0.1f;
					}
					else
					{ //otherwise smooth a bunch
						smoothFactor = 0.8f;
					}
				}
				else
				{ //not a rag bone
					smoothFactor = 0.3f;
				}
			}
#endif

			for (i=0;i<12;i++,oldM++,newM++)
			{
				*oldM=mSmoothFactor*(*oldM-*newM)+*newM;
			}
		}
		else
		{
			memcpy(&mSmoothBones[index].boneMatrix,&mFinalBones[index].boneMatrix,sizeof(mdxaBone_t));
		}
		mdxaSkelOffsets_t *offsets = (mdxaSkelOffsets_t *)((byte *)header + sizeof(mdxaHeader_t));
		mdxaSkel_t *skel = (mdxaSkel_t *)((byte *)header + sizeof(mdxaHeader_t) + offsets->offsets[index]);
		mdxaBone_t tempMatrix;
		Multiply_3x4Matrix(&tempMatrix,&mSmoothBones[index].boneMatrix, &skel->BasePoseMat);
		float maxl;
		maxl=VectorLength(&skel->BasePoseMat.matrix[0][0]);
		VectorNormalize(&tempMatrix.matrix[0][0]);
		VectorNormalize(&tempMatrix.matrix[1][0]);
		VectorNormalize(&tempMatrix.matrix[2][0]);

		VectorScale(&tempMatrix.matrix[0][0],maxl,&tempMatrix.matrix[0][0]);
		VectorScale(&tempMatrix.matrix[1][0],maxl,&tempMatrix.matrix[1][0]);
		VectorScale(&tempMatrix.matrix[2][0],maxl,&tempMatrix.matrix[2][0]);
		Multiply_3x4Matrix(&mSmoothBones[index].boneMatrix,&tempMatrix,&skel->BasePoseMatInv);
#ifdef _XBOX
		// Added by BTO (VV) - I hope this is right.
		SetRenderMatrix(mSmoothBones + index);
#endif
		mSmoothBones[index].touch=mCurrentTouch;
#ifdef _DEBUG
		for ( int i = 0; i < 3; i++ )
		{
			for ( int j = 0; j < 4; j++ )
			{
				assert( !_isnan(mSmoothBones[index].boneMatrix.matrix[i][j]));
			}
		}
#endif// _DEBUG
	}
//rww - RAGDOLL_END
public:
	int					frameSize; 
	const mdxaHeader_t	*header; 
	const model_t		*mod; 

	// these are split for better cpu cache behavior
	std::vector<SBoneCalc> mBones;
#ifdef _XBOX
	CTransformBone*		   mFinalBones;
	CTransformBone*		   mSmoothBones;
#else
	std::vector<CTransformBone> mFinalBones;

	std::vector<CTransformBone> mSmoothBones; // for render smoothing
#endif
	//vector<mdxaSkel_t *>   mSkels;

#ifdef _XBOX
	int				mNumBones;
#endif
	typedef std::vector <boneInfo_t> boneInfo_v;
	boneInfo_v		*rootBoneList;
	mdxaBone_t		rootMatrix;
	int				incomingTime;

	int				mCurrentTouch;
	//rww - RAGDOLL_BEGIN
	int				mCurrentTouchRender;
	int				mLastTouch; 
	int				mLastLastTouch;
	//rww - RAGDOLL_END

	// for render smoothing
	bool			mSmoothingActive;
	bool			mUnsquash;
	float			mSmoothFactor;

	CBoneCache(const model_t *amod,const mdxaHeader_t *aheader) :
		mod(amod),
		header(aheader)
	{
		assert(amod);
		assert(aheader);
		mSmoothingActive=false;
		mUnsquash=false;
		mSmoothFactor=0.0f;

#ifdef _XBOX
		mNumBones = header->numBones;
#endif
		int numBones=header->numBones;
		mBones.resize(numBones);
#ifdef _XBOX
		mFinalBones = (CTransformBone*)Z_Malloc(sizeof(CTransformBone) * mNumBones, TAG_GHOUL2, qtrue, 16);
		mSmoothBones = (CTransformBone*)Z_Malloc(sizeof(CTransformBone) * mNumBones, TAG_GHOUL2, qtrue, 16);
#else
		mFinalBones.resize(numBones);
		mSmoothBones.resize(numBones);
#endif
//		mSkels.resize(numBones);
		//rww - removed mSkels
		mdxaSkelOffsets_t *offsets;
		mdxaSkel_t		*skel;
		offsets = (mdxaSkelOffsets_t *)((byte *)header + sizeof(mdxaHeader_t));

		int i;
		for (i=0;i<numBones;i++)
		{
			skel = (mdxaSkel_t *)((byte *)header + sizeof(mdxaHeader_t) + offsets->offsets[i]);
			//mSkels[i]=skel;
			//ditto
			mFinalBones[i].parent=skel->parent;
		}
		mCurrentTouch=3;
//rww - RAGDOLL_BEGIN
		mLastTouch=2;
		mLastLastTouch=1;
//rww - RAGDOLL_END
	}
#ifdef _XBOX
	~CBoneCache ()
	{
		// Alignment
		Z_Free(mFinalBones);
		Z_Free(mSmoothBones);
	}
#endif

	SBoneCalc &Root()
	{
		assert(mBones.size());
		return mBones[0];
	}
	const mdxaBone_t &EvalUnsmooth(int index)
	{
		EvalLow(index);
		if (mSmoothingActive&&mSmoothBones[index].touch)
		{
			return mSmoothBones[index].boneMatrix;
		}
		return mFinalBones[index].boneMatrix;
	}
	const mdxaBone_t &Eval(int index)
	{
		/*
		bool wasEval=EvalLow(index);
		if (mSmoothingActive)
		{
			if (mSmoothBones[index].touch!=incomingTime||wasEval)
			{
				float dif=float(incomingTime)-float(mSmoothBones[index].touch);
				if (mSmoothBones[index].touch&&dif<300.0f)
				{

					if (dif<16.0f)  // 60 fps
					{
						dif=16.0f;
					}
					if (dif>100.0f) // 10 fps
					{
						dif=100.0f;
					}
					float f=1.0f-pow(1.0f-mSmoothFactor,16.0f/dif);

					int i;
					float *oldM=&mSmoothBones[index].boneMatrix.matrix[0][0];
					float *newM=&mFinalBones[index].boneMatrix.matrix[0][0];
					for (i=0;i<12;i++,oldM++,newM++)
					{
						*oldM=f*(*oldM-*newM)+*newM;
					}
					if (mUnsquash)
					{
						mdxaBone_t tempMatrix;
						Multiply_3x4Matrix(&tempMatrix,&mSmoothBones[index].boneMatrix, &mSkels[index]->BasePoseMat);
						float maxl;
						maxl=VectorLength(&mSkels[index]->BasePoseMat.matrix[0][0]);
						VectorNormalize(&tempMatrix.matrix[0][0]);
						VectorNormalize(&tempMatrix.matrix[1][0]);
						VectorNormalize(&tempMatrix.matrix[2][0]);

						VectorScale(&tempMatrix.matrix[0][0],maxl,&tempMatrix.matrix[0][0]);
						VectorScale(&tempMatrix.matrix[1][0],maxl,&tempMatrix.matrix[1][0]);
						VectorScale(&tempMatrix.matrix[2][0],maxl,&tempMatrix.matrix[2][0]);
						Multiply_3x4Matrix(&mSmoothBones[index].boneMatrix,&tempMatrix,&mSkels[index]->BasePoseMatInv);
					}
				}
				else
				{
					memcpy(&mSmoothBones[index].boneMatrix,&mFinalBones[index].boneMatrix,sizeof(mdxaBone_t));
				}
				mSmoothBones[index].touch=incomingTime;
			}
			return mSmoothBones[index].boneMatrix;
		}
		return mFinalBones[index].boneMatrix;
		*/

		//Hey, this is what sof2 does. Let's try it out.
		assert(index>=0&&index<mBones.size());
		if (mFinalBones[index].touch!=mCurrentTouch)
		{
			EvalLow(index);
		}
		return mFinalBones[index].boneMatrix;
	}
	//rww - RAGDOLL_BEGIN
	const inline mdxaBone_t &EvalRender(int index)
	{
		assert(index>=0&&index<mBones.size());
		if (mFinalBones[index].touch!=mCurrentTouch)
		{
			mFinalBones[index].touchRender=mCurrentTouchRender;
			EvalLow(index);
		}
		if (mSmoothingActive)
		{
			if (mSmoothBones[index].touch!=mCurrentTouch)
			{
				SmoothLow(index);
			}
			return mSmoothBones[index].boneMatrix;
		}
		return mFinalBones[index].boneMatrix;
	}
	//rww - RAGDOLL_END
	//rww - RAGDOLL_BEGIN
	bool WasRendered(int index)
	{
		assert(index>=0&&index<mBones.size());
		return mFinalBones[index].touchRender==mCurrentTouchRender;
	}
	int GetParent(int index)
	{
		if (index==0)
		{
			return -1;
		}
		assert(index>=0&&index<mBones.size());
		return mFinalBones[index].parent;
	}
	//rww - RAGDOLL_END

#ifdef _XBOX
	CTransformBone *EvalFull(int index)
	{
		EvalRender(index);
		if (mSmoothingActive)
		{
			return mSmoothBones + index;
		}
		return mFinalBones + index;
	}
#endif
};