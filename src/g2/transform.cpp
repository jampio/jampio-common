#include "CBoneCache.h"

// nasty little matrix multiply going on here..
void Multiply_3x4Matrix(mdxaBone_t *out, mdxaBone_t *in2, mdxaBone_t *in) 
{
	// first row of out                                                                                      
	out->matrix[0][0] = (in2->matrix[0][0] * in->matrix[0][0]) + (in2->matrix[0][1] * in->matrix[1][0]) + (in2->matrix[0][2] * in->matrix[2][0]);
	out->matrix[0][1] = (in2->matrix[0][0] * in->matrix[0][1]) + (in2->matrix[0][1] * in->matrix[1][1]) + (in2->matrix[0][2] * in->matrix[2][1]);
	out->matrix[0][2] = (in2->matrix[0][0] * in->matrix[0][2]) + (in2->matrix[0][1] * in->matrix[1][2]) + (in2->matrix[0][2] * in->matrix[2][2]);
	out->matrix[0][3] = (in2->matrix[0][0] * in->matrix[0][3]) + (in2->matrix[0][1] * in->matrix[1][3]) + (in2->matrix[0][2] * in->matrix[2][3]) + in2->matrix[0][3];
	// second row of outf out                                                                                     
	out->matrix[1][0] = (in2->matrix[1][0] * in->matrix[0][0]) + (in2->matrix[1][1] * in->matrix[1][0]) + (in2->matrix[1][2] * in->matrix[2][0]);
	out->matrix[1][1] = (in2->matrix[1][0] * in->matrix[0][1]) + (in2->matrix[1][1] * in->matrix[1][1]) + (in2->matrix[1][2] * in->matrix[2][1]);
	out->matrix[1][2] = (in2->matrix[1][0] * in->matrix[0][2]) + (in2->matrix[1][1] * in->matrix[1][2]) + (in2->matrix[1][2] * in->matrix[2][2]);
	out->matrix[1][3] = (in2->matrix[1][0] * in->matrix[0][3]) + (in2->matrix[1][1] * in->matrix[1][3]) + (in2->matrix[1][2] * in->matrix[2][3]) + in2->matrix[1][3];
	// third row of out  out                                                                                      
	out->matrix[2][0] = (in2->matrix[2][0] * in->matrix[0][0]) + (in2->matrix[2][1] * in->matrix[1][0]) + (in2->matrix[2][2] * in->matrix[2][0]);
	out->matrix[2][1] = (in2->matrix[2][0] * in->matrix[0][1]) + (in2->matrix[2][1] * in->matrix[1][1]) + (in2->matrix[2][2] * in->matrix[2][1]);
	out->matrix[2][2] = (in2->matrix[2][0] * in->matrix[0][2]) + (in2->matrix[2][1] * in->matrix[1][2]) + (in2->matrix[2][2] * in->matrix[2][2]);
	out->matrix[2][3] = (in2->matrix[2][0] * in->matrix[0][3]) + (in2->matrix[2][1] * in->matrix[1][3]) + (in2->matrix[2][2] * in->matrix[2][3]) + in2->matrix[2][3]; 
}

void G2_TransformBone(int child, CBoneCache &BC)
{
	SBoneCalc &TB=BC.mBones[child];
	static mdxaBone_t		tbone[6];
// 	mdxaFrame_t		*aFrame=0;
//	mdxaFrame_t		*bFrame=0;
//	mdxaFrame_t		*aoldFrame=0;
//	mdxaFrame_t		*boldFrame=0;
	static mdxaSkel_t		*skel;
	static mdxaSkelOffsets_t *offsets;
	boneInfo_v		&boneList = *BC.rootBoneList;
	static int				j, boneListIndex;
	int				angleOverride = 0;

#if DEBUG_G2_TIMING
	bool printTiming=false;
#endif
	// should this bone be overridden by a bone in the bone list?
	boneListIndex = G2_Find_Bone_In_List(boneList, child);
	if (boneListIndex != -1)
	{
		// we found a bone in the list - we need to override something here.
		
		// do we override the rotational angles?
		if ((boneList[boneListIndex].flags) & (BONE_ANGLES_TOTAL))
		{
			angleOverride = (boneList[boneListIndex].flags) & (BONE_ANGLES_TOTAL);
		}

		// set blending stuff if we need to
		if (boneList[boneListIndex].flags & BONE_ANIM_BLEND)
		{
			float blendTime = BC.incomingTime - boneList[boneListIndex].blendStart;
			// only set up the blend anim if we actually have some blend time left on this bone anim - otherwise we might corrupt some blend higher up the hiearchy
			if (blendTime>=0.0f&&blendTime < boneList[boneListIndex].blendTime)
			{
				TB.blendFrame	 = boneList[boneListIndex].blendFrame;
				TB.blendOldFrame = boneList[boneListIndex].blendLerpFrame;
				TB.blendLerp = (blendTime / boneList[boneListIndex].blendTime);
				TB.blendMode = true;
			}
			else
			{
				TB.blendMode = false;
			}
		}
		else if (/*r_Ghoul2NoBlend->integer||*/((boneList[boneListIndex].flags) & (BONE_ANIM_OVERRIDE_LOOP | BONE_ANIM_OVERRIDE)))
		// turn off blending if we are just doing a straing animation override
		{
			TB.blendMode = false;
		}

		// should this animation be overridden by an animation in the bone list?
		if ((boneList[boneListIndex].flags) & (BONE_ANIM_OVERRIDE_LOOP | BONE_ANIM_OVERRIDE))
		{
			G2_TimingModel(boneList[boneListIndex],BC.incomingTime,BC.header->numFrames,TB.currentFrame,TB.newFrame,TB.backlerp);
		}
#if DEBUG_G2_TIMING
		printTiming=true;
#endif
		/*
		if ((r_Ghoul2NoLerp->integer)||((boneList[boneListIndex].flags) & (BONE_ANIM_NO_LERP)))
		{
			TB.backlerp = 0.0f;
		}
		*/
		//rwwFIXMEFIXME: Use?
	}
	// figure out where the location of the bone animation data is
	assert(TB.newFrame>=0&&TB.newFrame<BC.header->numFrames);
	if (!(TB.newFrame>=0&&TB.newFrame<BC.header->numFrames))
	{
		TB.newFrame=0;
	}
//	aFrame = (mdxaFrame_t *)((byte *)BC.header + BC.header->ofsFrames + TB.newFrame * BC.frameSize );
	assert(TB.currentFrame>=0&&TB.currentFrame<BC.header->numFrames);
	if (!(TB.currentFrame>=0&&TB.currentFrame<BC.header->numFrames))
	{
		TB.currentFrame=0;
	}
//	aoldFrame = (mdxaFrame_t *)((byte *)BC.header + BC.header->ofsFrames + TB.currentFrame * BC.frameSize );

	// figure out where the location of the blended animation data is
	assert(!(TB.blendFrame < 0.0 || TB.blendFrame >= (BC.header->numFrames+1)));
	if (TB.blendFrame < 0.0 || TB.blendFrame >= (BC.header->numFrames+1) )
	{
		TB.blendFrame=0.0;
	}
//	bFrame = (mdxaFrame_t *)((byte *)BC.header + BC.header->ofsFrames + (int)TB.blendFrame * BC.frameSize );
	assert(TB.blendOldFrame>=0&&TB.blendOldFrame<BC.header->numFrames);
	if (!(TB.blendOldFrame>=0&&TB.blendOldFrame<BC.header->numFrames))
	{
		TB.blendOldFrame=0;
	}
#if DEBUG_G2_TIMING

#if DEBUG_G2_TIMING_RENDER_ONLY
	if (!HackadelicOnClient)
	{
		printTiming=false;
	}
#endif
	if (printTiming)
	{
		char mess[1000];
		if (TB.blendMode)
		{
			sprintf(mess,"b %2d %5d   %4d %4d %4d %4d  %f %f\n",boneListIndex,BC.incomingTime,(int)TB.newFrame,(int)TB.currentFrame,(int)TB.blendFrame,(int)TB.blendOldFrame,TB.backlerp,TB.blendLerp);
		}
		else
		{
			sprintf(mess,"a %2d %5d   %4d %4d            %f\n",boneListIndex,BC.incomingTime,TB.newFrame,TB.currentFrame,TB.backlerp);
		}
		OutputDebugString(mess);
		const boneInfo_t &bone=boneList[boneListIndex];
		if (bone.flags&BONE_ANIM_BLEND)
		{
			sprintf(mess,"                                                                    bfb[%2d] %5d  %5d  (%5d-%5d) %4.2f %4x   bt(%5d-%5d) %7.2f %5d\n",
				boneListIndex,
				BC.incomingTime,
				bone.startTime,
				bone.startFrame,
				bone.endFrame,
				bone.animSpeed,
				bone.flags,
				bone.blendStart,
				bone.blendStart+bone.blendTime,
				bone.blendFrame,
				bone.blendLerpFrame
				);
		}
		else
		{
			sprintf(mess,"                                                                    bfa[%2d] %5d  %5d  (%5d-%5d) %4.2f %4x\n",
				boneListIndex,
				BC.incomingTime,
				bone.startTime,
				bone.startFrame,
				bone.endFrame,
				bone.animSpeed,
				bone.flags
				);
		}
//		OutputDebugString(mess);
	}
#endif
//	boldFrame = (mdxaFrame_t *)((byte *)BC.header + BC.header->ofsFrames + TB.blendOldFrame * BC.frameSize );

//	mdxaCompBone_t	*compBonePointer = (mdxaCompBone_t *)((byte *)BC.header + BC.header->ofsCompBonePool);

	assert(child>=0&&child<BC.header->numBones);
//	assert(bFrame->boneIndexes[child]>=0);
//	assert(boldFrame->boneIndexes[child]>=0);
//	assert(aFrame->boneIndexes[child]>=0);
//	assert(aoldFrame->boneIndexes[child]>=0);

	// decide where the transformed bone is going

	// are we blending with another frame of anim?
	if (TB.blendMode)
	{
		float backlerp = TB.blendFrame - (int)TB.blendFrame;
		float frontlerp = 1.0 - backlerp;
		
// 		MC_UnCompress(tbone[3].matrix,compBonePointer[bFrame->boneIndexes[child]].Comp);
// 		MC_UnCompress(tbone[4].matrix,compBonePointer[boldFrame->boneIndexes[child]].Comp);
		UnCompressBone(tbone[3].matrix, child, BC.header, TB.blendFrame);
		UnCompressBone(tbone[4].matrix, child, BC.header, TB.blendOldFrame);

		for ( j = 0 ; j < 12 ; j++ ) 
		{
  			((float *)&tbone[5])[j] = (backlerp * ((float *)&tbone[3])[j])
				+ (frontlerp * ((float *)&tbone[4])[j]);
		}
	}
	
  	//
  	// lerp this bone - use the temp space on the ref entity to put the bone transforms into
  	//
  	if (!TB.backlerp)
  	{
// 		MC_UnCompress(tbone[2].matrix,compBonePointer[aoldFrame->boneIndexes[child]].Comp);
		UnCompressBone(tbone[2].matrix, child, BC.header, TB.currentFrame);

		// blend in the other frame if we need to
		if (TB.blendMode)
		{
			float blendFrontlerp = 1.0 - TB.blendLerp;
	  		for ( j = 0 ; j < 12 ; j++ ) 
			{
  				((float *)&tbone[2])[j] = (TB.blendLerp * ((float *)&tbone[2])[j])
					+ (blendFrontlerp * ((float *)&tbone[5])[j]);
			}
		}

  		if (!child)
		{
			// now multiply by the root matrix, so we can offset this model should we need to
			Multiply_3x4Matrix(&BC.mFinalBones[child].boneMatrix, &BC.rootMatrix, &tbone[2]);
 		}
  	}
	else
  	{
		float frontlerp = 1.0 - TB.backlerp;
// 		MC_UnCompress(tbone[0].matrix,compBonePointer[aFrame->boneIndexes[child]].Comp);
//		MC_UnCompress(tbone[1].matrix,compBonePointer[aoldFrame->boneIndexes[child]].Comp);
		UnCompressBone(tbone[0].matrix, child, BC.header, TB.newFrame);
		UnCompressBone(tbone[1].matrix, child, BC.header, TB.currentFrame);		

		for ( j = 0 ; j < 12 ; j++ ) 
		{
  			((float *)&tbone[2])[j] = (TB.backlerp * ((float *)&tbone[0])[j])
				+ (frontlerp * ((float *)&tbone[1])[j]);
		}

		// blend in the other frame if we need to
		if (TB.blendMode)
		{
			float blendFrontlerp = 1.0 - TB.blendLerp;
	  		for ( j = 0 ; j < 12 ; j++ ) 
			{
  				((float *)&tbone[2])[j] = (TB.blendLerp * ((float *)&tbone[2])[j])
					+ (blendFrontlerp * ((float *)&tbone[5])[j]);
			}
		}

  		if (!child)
  		{
			// now multiply by the root matrix, so we can offset this model should we need to
			Multiply_3x4Matrix(&BC.mFinalBones[child].boneMatrix, &BC.rootMatrix, &tbone[2]);
  		}
	}
	// figure out where the bone hirearchy info is
	offsets = (mdxaSkelOffsets_t *)((byte *)BC.header + sizeof(mdxaHeader_t));
	skel = (mdxaSkel_t *)((byte *)BC.header + sizeof(mdxaHeader_t) + offsets->offsets[child]);
//	skel = BC.mSkels[child];
	//rww - removed mSkels

	int parent=BC.mFinalBones[child].parent;
	assert((parent==-1&&child==0)||(parent>=0&&parent<BC.mBones.size()));
	if (angleOverride & BONE_ANGLES_REPLACE)
	{
		bool isRag=!!(angleOverride & BONE_ANGLES_RAGDOLL);
		if (!isRag)
		{ //do the same for ik.. I suppose.
			isRag = !!(angleOverride & BONE_ANGLES_IK);
		}

		mdxaBone_t &bone = BC.mFinalBones[child].boneMatrix;
		boneInfo_t &boneOverride = boneList[boneListIndex];

		if (isRag)
		{
			mdxaBone_t temp, firstPass;
			// give us the matrix the animation thinks we should have, so we can get the correct X&Y coors
			Multiply_3x4Matrix(&firstPass, &BC.mFinalBones[parent].boneMatrix, &tbone[2]);
			// this is crazy, we are gonna drive the animation to ID while we are doing post mults to compensate.
			Multiply_3x4Matrix(&temp,&firstPass, &skel->BasePoseMat);
			float	matrixScale = VectorLength((float*)&temp);
			static mdxaBone_t		toMatrix = 
			{ 
				1.0f, 0.0f, 0.0f, 0.0f,
				0.0f, 1.0f, 0.0f, 0.0f,
				0.0f, 0.0f, 1.0f, 0.0f
			};
			toMatrix.matrix[0][0]=matrixScale;
			toMatrix.matrix[1][1]=matrixScale;
			toMatrix.matrix[2][2]=matrixScale;
			toMatrix.matrix[0][3]=temp.matrix[0][3];
			toMatrix.matrix[1][3]=temp.matrix[1][3];
			toMatrix.matrix[2][3]=temp.matrix[2][3];

 			Multiply_3x4Matrix(&temp, &toMatrix,&skel->BasePoseMatInv); //dest first arg

			float blendTime = BC.incomingTime - boneList[boneListIndex].boneBlendStart;
			float blendLerp = (blendTime / boneList[boneListIndex].boneBlendTime);
			if (blendLerp>0.0f)
			{
				// has started
				if (blendLerp>1.0f)
				{
					// done
//					Multiply_3x4Matrix(&bone, &BC.mFinalBones[parent].boneMatrix,&temp);
					memcpy (&bone,&temp, sizeof(mdxaBone_t));
				}
				else
				{
//					mdxaBone_t lerp;
					// now do the blend into the destination
					float blendFrontlerp = 1.0 - blendLerp;
	  				for ( j = 0 ; j < 12 ; j++ ) 
					{
  						((float *)&bone)[j] = (blendLerp * ((float *)&temp)[j])
							+ (blendFrontlerp * ((float *)&tbone[2])[j]);
					}
//					Multiply_3x4Matrix(&bone, &BC.mFinalBones[parent].boneMatrix,&lerp);
				}
			}
		}
		else
		{
			mdxaBone_t temp, firstPass;

			// give us the matrix the animation thinks we should have, so we can get the correct X&Y coors
			Multiply_3x4Matrix(&firstPass, &BC.mFinalBones[parent].boneMatrix, &tbone[2]);

			// are we attempting to blend with the base animation? and still within blend time?
			if (boneOverride.boneBlendTime && (((boneOverride.boneBlendTime + boneOverride.boneBlendStart) < BC.incomingTime)))
			{
				// ok, we are supposed to be blending. Work out lerp
				float blendTime = BC.incomingTime - boneList[boneListIndex].boneBlendStart;
				float blendLerp = (blendTime / boneList[boneListIndex].boneBlendTime);

				if (blendLerp <= 1)
				{
					if (blendLerp < 0)
					{
						assert(0);
					}

					// now work out the matrix we want to get *to* - firstPass is where we are coming *from*
					Multiply_3x4Matrix(&temp, &firstPass, &skel->BasePoseMat);

					float	matrixScale = VectorLength((float*)&temp);

					mdxaBone_t	newMatrixTemp;
					
					if (HackadelicOnClient)
					{
						for (int i=0; i<3;i++)
						{
							for(int x=0;x<3; x++)
							{
								newMatrixTemp.matrix[i][x] = boneOverride.newMatrix.matrix[i][x]*matrixScale;
							}
						}

						newMatrixTemp.matrix[0][3] = temp.matrix[0][3];
						newMatrixTemp.matrix[1][3] = temp.matrix[1][3]; 
						newMatrixTemp.matrix[2][3] = temp.matrix[2][3];
					}
					else
					{
						for (int i=0; i<3;i++)
						{
							for(int x=0;x<3; x++)
							{
								newMatrixTemp.matrix[i][x] = boneOverride.matrix.matrix[i][x]*matrixScale;
							}
						}

						newMatrixTemp.matrix[0][3] = temp.matrix[0][3];
						newMatrixTemp.matrix[1][3] = temp.matrix[1][3]; 
						newMatrixTemp.matrix[2][3] = temp.matrix[2][3];
					}
				
 					Multiply_3x4Matrix(&temp, &newMatrixTemp,&skel->BasePoseMatInv);

					// now do the blend into the destination
					float blendFrontlerp = 1.0 - blendLerp;
	  				for ( j = 0 ; j < 12 ; j++ ) 
					{
  						((float *)&bone)[j] = (blendLerp * ((float *)&temp)[j])
							+ (blendFrontlerp * ((float *)&firstPass)[j]);
					}
				}
				else
				{
					bone = firstPass;
				}
			}
			// no, so just override it directly
			else
			{

				Multiply_3x4Matrix(&temp,&firstPass, &skel->BasePoseMat);
				float	matrixScale = VectorLength((float*)&temp);

				mdxaBone_t	newMatrixTemp;
				
				if (HackadelicOnClient)
				{
					for (int i=0; i<3;i++)
					{
						for(int x=0;x<3; x++)
						{
							newMatrixTemp.matrix[i][x] = boneOverride.newMatrix.matrix[i][x]*matrixScale;
						}
					}
					
					newMatrixTemp.matrix[0][3] = temp.matrix[0][3];
					newMatrixTemp.matrix[1][3] = temp.matrix[1][3]; 
					newMatrixTemp.matrix[2][3] = temp.matrix[2][3];
				}
				else
				{
					for (int i=0; i<3;i++)
					{
						for(int x=0;x<3; x++)
						{
							newMatrixTemp.matrix[i][x] = boneOverride.matrix.matrix[i][x]*matrixScale;
						}
					}
					
					newMatrixTemp.matrix[0][3] = temp.matrix[0][3];
					newMatrixTemp.matrix[1][3] = temp.matrix[1][3]; 
					newMatrixTemp.matrix[2][3] = temp.matrix[2][3];
				}
			
 				Multiply_3x4Matrix(&bone, &newMatrixTemp,&skel->BasePoseMatInv);
			}
		}
	}
	else if (angleOverride & BONE_ANGLES_PREMULT)
	{
		if ((angleOverride&BONE_ANGLES_RAGDOLL) || (angleOverride&BONE_ANGLES_IK))
		{
			mdxaBone_t	tmp;
			if (!child)
			{
				if (HackadelicOnClient)
				{
					Multiply_3x4Matrix(&tmp, &BC.rootMatrix, &boneList[boneListIndex].newMatrix);
				}
				else
				{
					Multiply_3x4Matrix(&tmp, &BC.rootMatrix, &boneList[boneListIndex].matrix);
				}
			}
			else
			{
				if (HackadelicOnClient)
				{
					Multiply_3x4Matrix(&tmp, &BC.mFinalBones[parent].boneMatrix, &boneList[boneListIndex].newMatrix);
				}
				else
				{
					Multiply_3x4Matrix(&tmp, &BC.mFinalBones[parent].boneMatrix, &boneList[boneListIndex].matrix);
				}
			}
			Multiply_3x4Matrix(&BC.mFinalBones[child].boneMatrix,&tmp, &tbone[2]);
		}
		else
		{
			if (!child)
			{
				// use the in coming root matrix as our basis
				if (HackadelicOnClient)
				{
					Multiply_3x4Matrix(&BC.mFinalBones[child].boneMatrix, &BC.rootMatrix, &boneList[boneListIndex].newMatrix);
				}
				else
				{
					Multiply_3x4Matrix(&BC.mFinalBones[child].boneMatrix, &BC.rootMatrix, &boneList[boneListIndex].matrix);
				}
 			}
			else
			{
				// convert from 3x4 matrix to a 4x4 matrix
				if (HackadelicOnClient)
				{
					Multiply_3x4Matrix(&BC.mFinalBones[child].boneMatrix, &BC.mFinalBones[parent].boneMatrix, &boneList[boneListIndex].newMatrix);
				}
				else
				{
					Multiply_3x4Matrix(&BC.mFinalBones[child].boneMatrix, &BC.mFinalBones[parent].boneMatrix, &boneList[boneListIndex].matrix);
				}
			}
		}
	}
	else
	// now transform the matrix by it's parent, asumming we have a parent, and we aren't overriding the angles absolutely
	if (child)
	{
		Multiply_3x4Matrix(&BC.mFinalBones[child].boneMatrix, &BC.mFinalBones[parent].boneMatrix, &tbone[2]);
	}

	// now multiply our resulting bone by an override matrix should we need to
	if (angleOverride & BONE_ANGLES_POSTMULT)
	{
		mdxaBone_t	tempMatrix;
		memcpy (&tempMatrix,&BC.mFinalBones[child].boneMatrix, sizeof(mdxaBone_t));
		if (HackadelicOnClient)
		{
		  	Multiply_3x4Matrix(&BC.mFinalBones[child].boneMatrix, &tempMatrix, &boneList[boneListIndex].newMatrix);
		}
		else
		{
		  	Multiply_3x4Matrix(&BC.mFinalBones[child].boneMatrix, &tempMatrix, &boneList[boneListIndex].matrix);
		}
	}
	/*
	if (r_Ghoul2UnSqash->integer)
	{
		mdxaBone_t tempMatrix;
		Multiply_3x4Matrix(&tempMatrix,&BC.mFinalBones[child].boneMatrix, &skel->BasePoseMat);
		float maxl;
		maxl=VectorLength(&skel->BasePoseMat.matrix[0][0]);
		VectorNormalize(&tempMatrix.matrix[0][0]);
		VectorNormalize(&tempMatrix.matrix[1][0]);
		VectorNormalize(&tempMatrix.matrix[2][0]);

		VectorScale(&tempMatrix.matrix[0][0],maxl,&tempMatrix.matrix[0][0]);
		VectorScale(&tempMatrix.matrix[1][0],maxl,&tempMatrix.matrix[1][0]);
		VectorScale(&tempMatrix.matrix[2][0],maxl,&tempMatrix.matrix[2][0]);
		Multiply_3x4Matrix(&BC.mFinalBones[child].boneMatrix,&tempMatrix,&skel->BasePoseMatInv);
	}
	*/
	//rwwFIXMEFIXME: Care?

}