#pragma once

#include <jampio/shared/shared.h>
#include "Cvar.h"

extern	Cvar	*com_developer;
extern	Cvar	*com_vmdebug;
extern	Cvar	*com_dedicated;
extern	Cvar	*com_speeds;
extern	Cvar	*com_timescale;
extern	Cvar	*com_sv_running;
extern	Cvar	*com_cl_running;
extern	Cvar	*com_viewlog;			// 0 = hidden, 1 = visible, 2 = minimized
extern	Cvar	*com_version;
extern	Cvar	*com_blood;
extern	Cvar	*com_buildScript;		// for building release pak files
extern	Cvar	*com_journal;
extern	Cvar	*com_cameraMode;
extern	Cvar	*com_optvehtrace;
#ifdef G2_PERFORMANCE_ANALYSIS
extern	Cvar	*com_G2Report;
#endif
extern	Cvar	*com_RMG;
// both client and server must agree to pause
extern	Cvar	*cl_paused;
extern	Cvar	*sv_paused;
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
