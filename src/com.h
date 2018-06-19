// qcommon.h -- definitions common between client and server, but not game.or ref modules
#pragma once

#include <jampio/shared/shared.h>

char		*CopyString( const char *in );
void		Info_Print( const char *s );
void		Com_BeginRedirect (char *buffer, int buffersize, void (*flush)(char *));
void		Com_EndRedirect( void );
void 		QDECL Com_Printf( const char *fmt, ... );
void 		QDECL Com_DPrintf( const char *fmt, ... );
void		QDECL Com_OPrintf( const char *fmt, ...); // Outputs to the VC / Windows Debug window (only in debug compile)
void 		QDECL Com_Error( int code, const char *fmt, ... );
void 		Com_Quit_f( void );
int			Com_EventLoop( void );
int			Com_Milliseconds( void );	// will be journaled properly
unsigned	Com_BlockChecksum( const void *buffer, int length );
unsigned	Com_BlockChecksumKey (void *buffer, int length, int key);
int			Com_HashKey(char *string, int maxlen);
int			Com_Filter(const char *filter, char *name, int casesensitive);
int			Com_FilterPath(const char *filter, char *name, int casesensitive);
int			Com_RealTime(qtime_t *qtime);
void Com_Init(int argc, const char **argv);
void Com_Frame( void );
void Com_Shutdown( void );
//rwwRMG: Inserted:
bool Com_ParseTextFile(const char *file, class CGenericParser2 &parser, bool cleanFirst = true);
CGenericParser2 *Com_ParseTextFile(const char *file, bool cleanFirst, bool writeable);
void Com_ParseTextFileDestroy(class CGenericParser2 &parser);
