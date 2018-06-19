#pragma once
// NOTE NOTE NOTE!!!!!!!!!!!!!
//
// Any CPUID_XXXX defined as higher than CPUID_INTEL_MMX *must* have MMX support (eg like CPUID_AMD_3DNOW (0x30) has),
//	this allows convenient MMX capability checking. If you for some reason want to support some new processor that does
//	*NOT* have MMX (yeah, right), then define it as a lower number. -slc
//
// ( These values are returned by Sys_GetProcessorId )
//
#define CPUID_GENERIC			0			// any unrecognized processor

#define CPUID_AXP				0x10

#define CPUID_INTEL_UNSUPPORTED	0x20			// Intel 386/486
#define CPUID_INTEL_PENTIUM		0x21			// Intel Pentium or PPro
#define CPUID_INTEL_MMX			0x22			// Intel Pentium/MMX or P2/MMX
#define CPUID_INTEL_KATMAI		0x23			// Intel Katmai
#define CPUID_INTEL_WILLIAMETTE	0x24			// Intel Williamette

#define CPUID_AMD_3DNOW			0x30			// AMD K6 3DNOW!