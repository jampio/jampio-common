#pragma once

#include "gore.h"
#include "../MiniHeap.h"
#include "CGhoul2Info.h"
#include "CGhoul2Info_v.h"

// globals
extern qboolean gG2_GBMNoReconstruct;
extern qboolean gG2_GBMUseSPMethod;

void		G2API_SetTime(int currentTime, int clock);
int			G2API_GetTime(int argTime);

qhandle_t	G2API_PrecacheGhoul2Model(const char *fileName);

int			G2API_InitGhoul2Model(CGhoul2Info_v **ghoul2Ptr, const char *fileName, int modelIndex, qhandle_t customSkin = NULL,
								  qhandle_t customShader = NULL, int modelFlags = 0, int lodBias = 0);
qboolean	G2API_SetLodBias(CGhoul2Info *ghlInfo, int lodBias);
qboolean	G2API_SetSkin(CGhoul2Info *ghlInfo, qhandle_t customSkin, qhandle_t renderSkin);
qboolean	G2API_SetShader(CGhoul2Info *ghlInfo, qhandle_t customShader);
qboolean	G2API_HasGhoul2ModelOnIndex(CGhoul2Info_v **ghlRemove, const int modelIndex);
qboolean	G2API_RemoveGhoul2Model(CGhoul2Info_v **ghlRemove, const int modelIndex);
qboolean	G2API_RemoveGhoul2Models(CGhoul2Info_v **ghlRemove);
qboolean	G2API_SetSurfaceOnOff(CGhoul2Info_v &ghoul2, const char *surfaceName, const int flags);
int			G2API_GetSurfaceOnOff(CGhoul2Info *ghlInfo, const char *surfaceName);
qboolean	G2API_SetRootSurface(CGhoul2Info_v &ghoul2, const int modelIndex, const char *surfaceName);
qboolean	G2API_RemoveSurface(CGhoul2Info *ghlInfo, const int index);
int			G2API_AddSurface(CGhoul2Info *ghlInfo, int surfaceNumber, int polyNumber, float BarycentricI, float BarycentricJ, int lod );
qboolean	G2API_SetBoneAnim(CGhoul2Info_v &ghoul2, const int modelIndex, const char *boneName, const int startFrame, const int endFrame,
							  const int flags, const float animSpeed, const int currentTime, const float setFrame = -1, const int blendTime = -1);
qboolean	G2API_GetBoneAnim(CGhoul2Info *ghlInfo, const char *boneName, const int currentTime, float *currentFrame,
							  int *startFrame, int *endFrame, int *flags, float *animSpeed, qhandle_t *modelList);
qboolean	G2API_GetAnimRange(CGhoul2Info *ghlInfo, const char *boneName,	int *startFrame, int *endFrame);
qboolean	G2API_PauseBoneAnim(CGhoul2Info *ghlInfo, const char *boneName, const int currentTime);
qboolean	G2API_IsPaused(CGhoul2Info *ghlInfo, const char *boneName);
qboolean	G2API_StopBoneAnim(CGhoul2Info *ghlInfo, const char *boneName);


qboolean G2API_SetBoneAngles(CGhoul2Info_v &ghoul2, const int modelIndex, const char *boneName, const vec3_t angles, const int flags,
							 const Eorientations up, const Eorientations left, const Eorientations forward,
							 qhandle_t *modelList, int blendTime, int currentTime );

qboolean	G2API_StopBoneAngles(CGhoul2Info *ghlInfo, const char *boneName);
qboolean	G2API_RemoveBone(CGhoul2Info *ghlInfo, const char *boneName);
void		G2API_AnimateG2Models(CGhoul2Info_v &ghoul2, float speedVar);
qboolean	G2API_RemoveBolt(CGhoul2Info *ghlInfo, const int index);
int			G2API_AddBolt(CGhoul2Info_v &ghoul2, const int modelIndex, const char *boneName);
int			G2API_AddBoltSurfNum(CGhoul2Info *ghlInfo, const int surfIndex);
void		G2API_SetBoltInfo(CGhoul2Info_v &ghoul2, int modelIndex, int boltInfo);
qboolean	G2API_AttachG2Model(CGhoul2Info_v &ghoul2From, int modelFrom, CGhoul2Info_v &ghoul2To, int toBoltIndex, int toModel);
qboolean	G2API_DetachG2Model(CGhoul2Info *ghlInfo);
qboolean	G2API_AttachEnt(int *boltInfo, CGhoul2Info *ghlInfoTo, int toBoltIndex, int entNum, int toModelNum);
void		G2API_DetachEnt(int *boltInfo);

qboolean	G2API_GetBoltMatrix(CGhoul2Info_v &ghoul2, const int modelIndex, const int boltIndex, mdxaBone_t *matrix,
								const vec3_t angles, const vec3_t position, const int frameNum, qhandle_t *modelList, vec3_t scale);

void		G2API_ListSurfaces(CGhoul2Info *ghlInfo);
void		G2API_ListBones(CGhoul2Info *ghlInfo, int frame);
qboolean	G2API_HaveWeGhoul2Models(CGhoul2Info_v &ghoul2);
void		G2API_SetGhoul2ModelIndexes(CGhoul2Info_v &ghoul2, qhandle_t *modelList, qhandle_t *skinList);
qboolean	G2API_SetGhoul2ModelFlags(CGhoul2Info *ghlInfo, const int flags);
int			G2API_GetGhoul2ModelFlags(CGhoul2Info *ghlInfo);

qboolean	G2API_GetAnimFileName(CGhoul2Info *ghlInfo, char **filename);
void		G2API_CollisionDetect(CollisionRecord_t *collRecMap, CGhoul2Info_v &ghoul2, const vec3_t angles, const vec3_t position,
										  int frameNumber, int entNum, vec3_t rayStart, vec3_t rayEnd, vec3_t scale, CMiniHeap *G2VertSpace, int traceFlags, int useLod, float fRadius);
void		G2API_CollisionDetectCache(CollisionRecord_t *collRecMap, CGhoul2Info_v &ghoul2, const vec3_t angles, const vec3_t position,
										  int frameNumber, int entNum, vec3_t rayStart, vec3_t rayEnd, vec3_t scale, CMiniHeap *G2VertSpace, int traceFlags, int useLod, float fRadius);

void		G2API_GiveMeVectorFromMatrix(mdxaBone_t *boltMatrix, Eorientations flags, vec3_t vec);
int			G2API_CopyGhoul2Instance(CGhoul2Info_v &g2From, CGhoul2Info_v &g2To, int modelIndex);
void		G2API_CleanGhoul2Models(CGhoul2Info_v **ghoul2Ptr);
int			G2API_GetParentSurface(CGhoul2Info *ghlInfo, const int index);
int			G2API_GetSurfaceIndex(CGhoul2Info *ghlInfo, const char *surfaceName);
char		*G2API_GetSurfaceName(CGhoul2Info *ghlInfo, int surfNumber);
char		*G2API_GetGLAName(CGhoul2Info_v &ghoul2, int modelIndex);
qboolean	G2API_SetBoneAnglesMatrix(CGhoul2Info *ghlInfo, const char *boneName, const mdxaBone_t &matrix, const int flags,
									  qhandle_t *modelList, int blendTime = 0, int currentTime = 0);
qboolean	G2API_SetNewOrigin(CGhoul2Info_v &ghoul2, const int boltIndex);
int			G2API_GetBoneIndex(CGhoul2Info *ghlInfo, const char *boneName);
qboolean	G2API_StopBoneAnglesIndex(CGhoul2Info *ghlInfo, const int index);
qboolean	G2API_StopBoneAnimIndex(CGhoul2Info *ghlInfo, const int index);
qboolean	G2API_SetBoneAnglesIndex(CGhoul2Info *ghlInfo, const int index, const vec3_t angles, const int flags,
							 const int yaw, const int pitch, const int roll,
							 qhandle_t *modelList, int blendTime, int currentTime);
qboolean	G2API_SetBoneAnglesMatrixIndex(CGhoul2Info *ghlInfo, const int index, const mdxaBone_t &matrix,
								   const int flags, qhandle_t *modelList, int blendTime, int currentTime);
qboolean	G2API_DoesBoneExist(CGhoul2Info *ghlInfo, const char *boneName);
qboolean	G2API_SetBoneAnimIndex(CGhoul2Info *ghlInfo, const int index, const int startFrame, const int endFrame, const int flags, const float animSpeed, const int currentTime, const float setFrame, const int blendTime);
qboolean	G2API_SaveGhoul2Models(CGhoul2Info_v &ghoul2, char **buffer, int *size);
void		G2API_LoadGhoul2Models(CGhoul2Info_v &ghoul2, char *buffer);
void		G2API_LoadSaveCodeDestructGhoul2Info(CGhoul2Info_v &ghoul2);
void		G2API_FreeSaveBuffer(char *buffer);
char		*G2API_GetAnimFileNameIndex(qhandle_t modelIndex);
int			G2API_GetSurfaceRenderStatus(CGhoul2Info *ghlInfo, const char *surfaceName);
void		G2API_CopySpecificG2Model(CGhoul2Info_v &ghoul2From, int modelFrom, CGhoul2Info_v &ghoul2To, int modelTo);
void		G2API_DuplicateGhoul2Instance(CGhoul2Info_v &g2From, CGhoul2Info_v **g2To);
void		G2API_SetBoltInfo(CGhoul2Info_v &ghoul2, int modelIndex, int boltInfo);

void		G2API_AbsurdSmoothing(CGhoul2Info_v &ghoul2, qboolean status);

void		G2API_SetRagDoll(CGhoul2Info_v &ghoul2,CRagDollParams *parms);
void		G2API_ResetRagDoll(CGhoul2Info_v &ghoul2);
void		G2API_AnimateG2Models(CGhoul2Info_v &ghoul2, int AcurrentTime,CRagDollUpdateParams *params);

qboolean	G2API_RagPCJConstraint(CGhoul2Info_v &ghoul2, const char *boneName, vec3_t min, vec3_t max);
qboolean	G2API_RagPCJGradientSpeed(CGhoul2Info_v &ghoul2, const char *boneName, const float speed);
qboolean	G2API_RagEffectorGoal(CGhoul2Info_v &ghoul2, const char *boneName, vec3_t pos);
qboolean	G2API_GetRagBonePos(CGhoul2Info_v &ghoul2, const char *boneName, vec3_t pos, vec3_t entAngles, vec3_t entPos, vec3_t entScale);
qboolean	G2API_RagEffectorKick(CGhoul2Info_v &ghoul2, const char *boneName, vec3_t velocity);
qboolean	G2API_RagForceSolve(CGhoul2Info_v &ghoul2, qboolean force);

qboolean	G2API_SetBoneIKState(CGhoul2Info_v &ghoul2, int time, const char *boneName, int ikState, sharedSetBoneIKStateParams_t *params);
qboolean	G2API_IKMove(CGhoul2Info_v &ghoul2, int time, sharedIKMoveParams_t *params);

void		G2API_AttachInstanceToEntNum(CGhoul2Info_v &ghoul2, int entityNum, qboolean server);
void		G2API_ClearAttachedInstance(int entityNum);
void		G2API_CleanEntAttachments(void);
qboolean	G2API_OverrideServerWithClientData(CGhoul2Info *serverInstance);

qboolean	G2API_SkinlessModel(CGhoul2Info *g2);

#ifdef _G2_GORE
int			G2API_GetNumGoreMarks(CGhoul2Info *g2);
void		G2API_AddSkinGore(CGhoul2Info_v &ghoul2,SSkinGoreData &gore);
void		G2API_ClearSkinGore ( CGhoul2Info_v &ghoul2 );
#endif // _SOF2

int			G2API_Ghoul2Size ( CGhoul2Info_v &ghoul2 );