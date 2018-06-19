#pragma once

#include <jampio/shared/shared.h>
#include "netadr.h"

typedef enum {
	AXIS_SIDE,
	AXIS_FORWARD,
	AXIS_UP,
	AXIS_ROLL,
	AXIS_YAW,
	AXIS_PITCH,
	MAX_JOYSTICK_AXIS
} joystickAxis_t;

typedef enum {
  // bk001129 - make sure SE_NONE is zero
	SE_NONE = 0,	// evTime is still valid
	SE_KEY,		// evValue is a key code, evValue2 is the down flag
	SE_CHAR,	// evValue is an ascii char
	SE_MOUSE,	// evValue and evValue2 are reletive signed x / y moves
	SE_JOYSTICK_AXIS,	// evValue is an axis number and evValue2 is the current state (-127 to 127)
	SE_CONSOLE,	// evPtr is a char*
	SE_PACKET	// evPtr is a netadr_t followed by data bytes to evPtrLength
} sysEventType_t;

typedef struct {
	int				evTime;
	sysEventType_t	evType;
	int				evValue, evValue2;
	int				evPtrLength;	// bytes of data pointed to by evPtr, for journaling
	void			*evPtr;			// this must be manually freed if not NULL
} sysEvent_t;

sysEvent_t	Sys_GetEvent( void );

void	Sys_Init (void);

// general development dll loading for virtual machine testing
void	* QDECL Sys_LoadDll( const char *name, int (QDECL **entryPoint)(int, ...),
				  int (QDECL *systemcalls)(int, ...) );
void	Sys_UnloadDll( void *dllHandle );

void	Sys_UnloadGame( void );
void	*Sys_GetGameAPI( void *parms );

void	Sys_UnloadCGame( void );
void	*Sys_GetCGameAPI( void );

void	Sys_UnloadUI( void );
void	*Sys_GetUIAPI( void );

//bot libraries
void	Sys_UnloadBotLib( void );
void	*Sys_GetBotLibAPI( void *parms );

char	*Sys_GetCurrentUser( void );

void	QDECL Sys_Error( const char *error, ...);
void	Sys_Quit (void);
char	*Sys_GetClipboardData( void );	// note that this isn't journaled...

void	Sys_Print( const char *msg );

// Sys_Milliseconds should only be used for profiling purposes,
// any game related timing information should come from event timestamps
int		Sys_Milliseconds (bool baseTime = false);

#if __linux__
extern "C" void	Sys_SnapVector( float *v );

#else
void	Sys_SnapVector( float *v );
#endif

// the system console is shown when a dedicated server is running
void	Sys_DisplaySystemConsole( qboolean show );

int		Sys_GetProcessorId( void );
int		Sys_GetCPUSpeed( void );
int		Sys_GetPhysicalMemory(void);

void	Sys_BeginStreamedFile( fileHandle_t f, int readahead );
void	Sys_EndStreamedFile( fileHandle_t f );
int		Sys_StreamedRead( void *buffer, int size, int count, fileHandle_t f );
void	Sys_StreamSeek( fileHandle_t f, int offset, int origin );

void	Sys_ShowConsole( int level, qboolean quitOnClose );
void	Sys_SetErrorText( const char *text );

void	Sys_SendPacket( int length, const void *data, netadr_t to );

qboolean	Sys_StringToAdr( const char *s, netadr_t *a );
//Does NOT parse port numbers, only base addresses.

qboolean	Sys_IsLANAddress (netadr_t adr);
void		Sys_ShowIP(void);

qboolean	Sys_CheckCD( void );

void	Sys_Mkdir( const char *path );
char	*Sys_Cwd( void );
void	Sys_SetDefaultCDPath(const char *path);
char	*Sys_DefaultCDPath(void);
void	Sys_SetDefaultInstallPath(const char *path);
char	*Sys_DefaultInstallPath(void);
void     Sys_SetDefaultHomePath(const char *path);
char	*Sys_DefaultHomePath(void);
char	*Sys_DefaultBasePath(void);

char **Sys_ListFiles( const char *directory, const char *extension, const char *filter, int *numfiles, qboolean wantsubs );
void	Sys_FreeFileList( char **fileList );
//rwwRMG - changed to fileList to not conflict with list type

void	Sys_BeginProfiling( void );
void	Sys_EndProfiling( void );

int Sys_FunctionCmp(void *f1, void *f2);
int Sys_FunctionCheckSum(void *f1);

qboolean Sys_LowPhysicalMemory();
unsigned int Sys_ProcessorCount();

int Sys_MonkeyShouldBeSpanked( void );