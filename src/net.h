#pragma once
/*
==============================================================

NET

==============================================================
*/
#include <jampio/shared/shared.h>
#include "netadr.h"
#include "msgtype.h"

constexpr auto PACKET_BACKUP = 32;  // number of old messages that must be kept on client and
                                   // server for delta comrpession and ping estimation
constexpr auto PACKET_MASK = (PACKET_BACKUP-1);
constexpr auto MAX_PACKET_USERCMDS = 32; // max number of usercmd_t in a packet
constexpr auto PORT_ANY = -1;
constexpr auto MAX_RELIABLE_COMMANDS = 128; // max string commands buffered for restransmit
constexpr auto MAX_MSGLEN = 49152; // max length of a message, which may
                                   // be fragmented into multiple packets
//rww - 6/28/02 - Changed from 16384 to match sof2's. This does seem rather huge, but I guess it doesn't really hurt anything.
constexpr auto MAX_DOWNLOAD_WINDOW = 8; // max of eight download frames
constexpr auto MAX_DOWNLOAD_BLKSIZE = 2048; // 2048 byte block chunks

typedef enum {
	NS_CLIENT,
	NS_SERVER
} netsrc_t;

void		NET_Init( void );
void		NET_Shutdown( void );
void		NET_Restart( void );
void		NET_Config( qboolean enableNetworking );

void		NET_SendPacket (netsrc_t sock, int length, const void *data, netadr_t to);
void		QDECL NET_OutOfBandPrint( netsrc_t net_socket, netadr_t adr, const char *format, ...);
void		QDECL NET_OutOfBandData( netsrc_t sock, netadr_t adr, byte *format, int len );

qboolean	NET_CompareAdr (netadr_t a, netadr_t b);
qboolean	NET_CompareBaseAdr (netadr_t a, netadr_t b);
qboolean	NET_IsLocalAddress (netadr_t adr);
const char	*NET_AdrToString (netadr_t a);
qboolean	NET_StringToAdr ( const char *s, netadr_t *a);
qboolean	NET_GetLoopPacket (netsrc_t sock, netadr_t *net_from, msg_t *net_message);
void		NET_Sleep(int msec);

/*
Netchan handles packet fragmentation and out of order / duplicate suppression
*/

typedef struct {
	netsrc_t	sock;

	int			dropped;			// between last packet and previous

	netadr_t	remoteAddress;
	int			qport;				// qport value to write when transmitting

	// sequencing variables
	int			incomingSequence;
	int			outgoingSequence;

	// incoming fragment assembly buffer
	int			fragmentSequence;
	int			fragmentLength;	
	byte		fragmentBuffer[MAX_MSGLEN];

	// outgoing fragment buffer
	// we need to space out the sending of large fragmented messages
	qboolean	unsentFragments;
	int			unsentFragmentStart;
	int			unsentLength;
	byte		unsentBuffer[MAX_MSGLEN];
} netchan_t;

void Netchan_Init( int qport );
void Netchan_Setup( netsrc_t sock, netchan_t *chan, netadr_t adr, int qport );

void Netchan_Transmit( netchan_t *chan, int length, const byte *data );
void Netchan_TransmitNextFragment( netchan_t *chan );

qboolean Netchan_Process( netchan_t *chan, msg_t *msg );

