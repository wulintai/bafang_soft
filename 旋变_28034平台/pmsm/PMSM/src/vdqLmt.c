#ifndef _VDQLMT_C
#define _VDQLMT_C

#include "include_c.h"

//#pragma	CODE_SECTION(vdqlmtin,		"ramfuncs");
#pragma	CODE_SECTION(vdqlmtctrl,	    "ramfuncs");

void vdqlmtin(void)
{
	/*vdqLmtvars.Vdc              = sysCfgPara.Vdc;
	vdqLmtvars.Id_PI_Vd         = crtLoopPara.Id_PI_Vd;
	vdqLmtvars.Iq_PI_Vq         = crtLoopPara.Iq_PI_Vq;
	vdqLmtvars.IdPid_Out_fwd	= mtrfwdvars.IdPid_Out_fwd;
	vdqLmtvars.IqPid_Out_fwd	= mtrfwdvars.IqPid_Out_fwd;*/
}

void vdqlmtctrl(void)
{
	_iq vd_tmp = 0;
	_iq vq_tmp = 0;

	vd_tmp = crtLoopPara.Id_PI_Vd+mtrfwdvars.IdPid_Out_fwd;
	vq_tmp = crtLoopPara.Iq_PI_Vq+mtrfwdvars.IqPid_Out_fwd;

	vdqLmtvars.VdqLmt_Vd = _IQsat(vd_tmp,sysCfgPara.Vdc,-sysCfgPara.Vdc);
	vdqLmtvars.VdqLmt_Vq = _IQsat(vq_tmp,sysCfgPara.Vdc,-sysCfgPara.Vdc);
}

void vdqlmtout(void)
{

}

void vdqlmtclc(void)
{
	vdqLmtvars.VdqLmt_Vd = 0;
	vdqLmtvars.VdqLmt_Vq = 0;
}

#endif

