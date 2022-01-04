#ifndef _SYSINIT_H
#define _SYSINIT_H

extern void canInit(void);

#ifdef _SYSINIT_C
	void System_Init(void);
#else
	extern void System_Init(void);
#endif

#endif

