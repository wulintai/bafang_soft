/*
 * osc_view.c
 *
 *  Created on: 2014-4-11
 *      Author: ThinkPad
 */

#ifndef	_OSCVIEW_C
#define _OSCVIEW_C

#include "include_c.h"

#if 0

void oscinit(void){

	for(osc.point = 0;osc.point < OSC_NUM;osc.point++)
	{
		osc.view0[osc.point] = 0;
		osc.view1[osc.point] = 0;
		osc.view2[osc.point] = 0;
		osc.view3[osc.point] = 0;
	}
	osc.point = 0;
}

//#pragma CODE_SECTION(oscview, 	"ramfuncs");
void oscview(void)
{
	if(1 == osc.OSC_enable)
	{
		if(0 == osc.transmitFlag)
		{
			osc.view0[osc.point] = sysFbkPara.Theta_fbk_Ele_HMI;
			osc.view1[osc.point] = _IQtoIQ15(sysFbkPara.Id_fbk);
			osc.view2[osc.point] = _IQtoIQ15(crtLoopPara.IqPid_Ref);
			osc.view3[osc.point] = _IQtoIQ15(sysFbkPara.Iq_fbk);
			osc.point++;
			if(osc.point>=OSC_NUM)
			{
				osc.transmitFlag=1;
				osc.point=0;
			}
		}
	}
}

#endif

#endif


