#pragma once
// abstract service interface
#include <jampio/shared/shared.h>
#include "../netadr.h"
#include "../msgtype.h"
#include "../CvarSystem.h"

void SV_Init(void);
void SV_Shutdown(CvarSystem& cvars, const char *finalmsg);
void SV_Frame(int msec);
void SV_PacketEvent(netadr_t from, msg_t *msg);
qboolean SV_GameCommand(void);