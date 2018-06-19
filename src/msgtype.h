#pragma once

#include <jampio/shared/shared.h>

typedef struct {
	qboolean	allowoverflow;	// if false, do a Com_Error
	qboolean	overflowed;		// set to true if the buffer size failed (with allowoverflow set)
	qboolean	oob;			// set to true if the buffer size failed (with allowoverflow set)
	byte	*data;
	int		maxsize;
	int		cursize;
	int		readcount;
	int		bit;				// for bitwise reads and writes
} msg_t;
