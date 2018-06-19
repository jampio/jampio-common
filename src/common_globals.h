#pragma once

#include <jampio/shared/shared.h>

extern	cvar_t	*com_developer;
extern	cvar_t	*com_vmdebug;
extern	cvar_t	*com_dedicated;
extern	cvar_t	*com_speeds;
extern	cvar_t	*com_timescale;
extern	cvar_t	*com_sv_running;
extern	cvar_t	*com_cl_running;
extern	cvar_t	*com_viewlog;			// 0 = hidden, 1 = visible, 2 = minimized
extern	cvar_t	*com_version;
extern	cvar_t	*com_blood;
extern	cvar_t	*com_buildScript;		// for building release pak files
extern	cvar_t	*com_journal;
extern	cvar_t	*com_cameraMode;

extern	cvar_t	*com_optvehtrace;

#ifdef G2_PERFORMANCE_ANALYSIS
extern	cvar_t	*com_G2Report;
#endif

extern	cvar_t	*com_RMG;

// both client and server must agree to pause
extern	cvar_t	*cl_paused;
extern	cvar_t	*sv_paused;

// com_speeds times
extern	int		time_game;
extern	int		time_frontend;
extern	int		time_backend;		// renderer backend time

extern	int		com_frameTime;
extern	int		com_frameMsec;

extern	qboolean	com_errorEntered;

extern	fileHandle_t	logfile;
extern	fileHandle_t	com_journalFile;
extern	fileHandle_t	com_journalDataFile;
