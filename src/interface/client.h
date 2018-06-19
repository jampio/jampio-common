#pragma once
// Abstract client interface
#include <jampio/shared/shared.h>
#include "../netadr.h"
#include "../msgtype.h"

void CL_InitKeyCommands( void );
// the keyboard binding interface must be setup before execing
// config files, but the rest of client startup will happen later

void CL_Init( void );
void CL_Disconnect( qboolean showMainMenu );
void CL_Shutdown( void );
void CL_Frame( int msec );
qboolean CL_GameCommand( void );
void CL_KeyEvent (int key, qboolean down, unsigned time);

void CL_CharEvent( int key );
// char events are for field typing, not game control

void CL_MouseEvent( int dx, int dy, int time );

void CL_JoystickEvent( int axis, int value, int time );

void CL_PacketEvent( netadr_t from, msg_t *msg );

void CL_ConsolePrint( const char *text, qboolean silent );

void CL_MapLoading( void );
// do a screen update before starting to load a map
// when the server is going to load a new map, the entire hunk
// will be cleared, so the client must shutdown cgame, ui, and
// the renderer

void	CL_ForwardCommandToServer( const char *string );
// adds the current command line as a clc_clientCommand to the client message.
// things like godmode, noclip, etc, are commands directed to the server,
// so when they are typed in at the console, they will need to be forwarded.

void CL_ShutdownAll( void );
// shutdown all the client stuff

void CL_FlushMemory( void );
// dump all memory on an error

void CL_StartHunkUsers( void );
// start all the client stuff using the hunk

void Key_WriteBindings( fileHandle_t f );
// for writing the config files

void S_ClearSoundBuffer( void );
// call before filesystem access

void SCR_DebugGraph (float value, int color);	// FIXME: move logging to common?