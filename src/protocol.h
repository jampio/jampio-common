#pragma once
/*
==============================================================

PROTOCOL

==============================================================
*/

#define	PROTOCOL_VERSION	26

#ifndef _XBOX	// No gethostbyname(), and can't really use this stuff
#define	UPDATE_SERVER_NAME		"updatejk3.ravensoft.com"
#define MASTER_SERVER_NAME		"masterjk3.ravensoft.com"

#ifdef USE_CD_KEY
#define	AUTHORIZE_SERVER_NAME	"authorizejk3.ravensoft.com"
#endif
#endif	// _XBOX

#ifdef _XBOX	// Use port number 1000 for less bandwidth!
#define	PORT_SERVER			1000
#define NUM_SERVER_PORTS	1
#else
#define	PORT_MASTER			29060
#define	PORT_UPDATE			29061
//#define	PORT_AUTHORIZE		29062
#define	PORT_SERVER			29070	//...+9 more for multiple servers
#define	NUM_SERVER_PORTS	4		// broadcast scan this many ports after
									// PORT_SERVER so a single machine can
									// run multiple servers
#endif

// the svc_strings[] array in cl_parse.c should mirror this
//
// server to client
//
enum svc_ops_e {
	svc_bad,
	svc_nop,
	svc_gamestate,
	svc_configstring,			// [short] [string] only in gamestate messages
	svc_baseline,				// only in gamestate messages
	svc_serverCommand,			// [string] to be executed by client game module
	svc_download,				// [short] size [size bytes]
	svc_snapshot,
	svc_setgame,
	svc_mapchange,
#ifdef _XBOX
	svc_newpeer,				//jsw//inform current clients about new player
	svc_removepeer,				//jsw//inform current clients about dying player
	svc_xbInfo,					//jsw//update client with current server xbOnlineInfo
#endif
	svc_EOF
};

//
// client to server
//
enum clc_ops_e {
	clc_bad,
	clc_nop, 		
	clc_move,				// [[usercmd_t]
	clc_moveNoDelta,		// [[usercmd_t]
	clc_clientCommand,		// [string] message
	clc_EOF
};
