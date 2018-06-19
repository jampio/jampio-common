#pragma once

#include <jampio/shared/shared.h>

typedef enum {
	NA_BOT,
	NA_BAD,					// an address lookup failed
	NA_LOOPBACK,
	NA_BROADCAST,
	NA_IP,
	NA_IPX,
	NA_BROADCAST_IPX
} netadrtype_t;

typedef struct {
	netadrtype_t	type;

	byte	ip[4];
	byte	ipx[10];

	unsigned short	port;
} netadr_t;