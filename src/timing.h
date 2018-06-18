#pragma once

#include "common.h"

// jampio - current implementation depends on x86 specific rdtsc instruction
// the end results are not actually used by any current modules
// going to use Sys_Millseconds() for now

class timing_c
{
private:
#if 0
	__int64	start;
	__int64	end;
#endif
	int start, end;

	int		reset;
public:
	timing_c(void)
	{
	}
	void Start()
	{
#if 0
		const __int64 *s = &start;
		__asm
		{
			push eax
			push ebx
			push edx

			rdtsc
			mov ebx, s
			mov	[ebx], eax
			mov [ebx + 4], edx

			pop edx
			pop ebx
			pop eax
		}
#endif
		start = Sys_Milliseconds();
	}
	int End()
	{
#if 0
		const __int64 *e = &end;
		__int64	time;
#ifndef __linux__
		__asm
		{
			push eax
			push ebx
			push edx

			rdtsc
			mov ebx, e
			mov	[ebx], eax
			mov [ebx + 4], edx

			pop edx
			pop ebx
			pop eax
		}
#endif
#endif
		end = Sys_Milliseconds();
		int time = end - start;
		if (time < 0)
		{
			time = 0;
		}
		return((int)time);
	}
};
// end