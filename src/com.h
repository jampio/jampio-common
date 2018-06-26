// qcommon.h -- definitions common between client and server, but not game.or ref modules
#pragma once

#include <jampio/shared/shared.h>
#include "CvarSystem.h"
#include "CommandLine.h"
#include "CommandBuffer.h"
#include "CommandSystem.h"

char *CopyString(const char *in);
void Info_Print(const char *s);
void Com_BeginRedirect(char *buffer, int buffersize, void (*flush)(char *));
void Com_EndRedirect(void);
void QDECL Com_Printf( const char *fmt, ... );
void QDECL Com_DPrintf( const char *fmt, ... );
void QDECL Com_OPrintf( const char *fmt, ...); // Outputs to the VC / Windows Debug window (only in debug compile)
void QDECL Com_Error( int code, const char *fmt, ... );
int Com_EventLoop(CvarSystem& cvars, CommandBuffer& cbuf); 
int Com_Milliseconds( void );	// will be journaled properly
unsigned Com_BlockChecksum( const void *buffer, int length );
unsigned Com_BlockChecksumKey (void *buffer, int length, int key);
int Com_HashKey(char *string, int maxlen);
int Com_Filter(const char *filter, const char *name, int casesensitive);
int Com_FilterPath(const char *filter, char *name, int casesensitive);
int Com_RealTime(qtime_t *qtime);
void Com_Init(CvarSystem& cvars, CommandLine& cli, CommandBuffer& cbuf, CommandSystem& cmd);
void Com_Frame(CvarSystem& cvars, CommandBuffer& cbuf);
void Com_Shutdown(CvarSystem& cvars);
bool Com_ParseTextFile(const char *file, class CGenericParser2 &parser, bool cleanFirst = true);
//CGenericParser2 *Com_ParseTextFile(const char *file, bool cleanFirst, bool writeable);
void Com_ParseTextFileDestroy(class CGenericParser2 &parser);
