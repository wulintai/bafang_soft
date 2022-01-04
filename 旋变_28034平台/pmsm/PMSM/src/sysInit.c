/*
 * sysInit.c
 *
 *  Created on: 2014-3-17
 *      Author: ThinkPad
 */
#ifndef _SYSINIT_C
#define _SYSINIT_C

#include "include_c.h"

extern void InitSpiaGpio();
extern void sub_spi_init();

void System_Init(void)
{
	InitSpiaGpio();
	sub_spi_init();
	eeprom_Init();
	canInit();
	CANA_init();
	taskInit();
	sysFSMInit();
	mtrFSMInit();
	diagFSMInit();
	GlobalVar_Init();
	motorParameterInit();
	trqLoop_Init();
	exactTuneinit();
	Resolver_read();
}

#endif

