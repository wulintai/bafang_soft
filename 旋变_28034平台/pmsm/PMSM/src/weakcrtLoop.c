#ifndef _WEAKCRTLOOP_C_
#define _WEAKCRTLOOP_C_

#include "include_c.h"

#pragma	CODE_SECTION(weakcrtLoopin,		"ramfuncs");
#pragma	CODE_SECTION(weakcrtLoopctrl,	"ramfuncs");
//#pragma	CODE_SECTION(weakcrtLoopout,	"ramfuncs");

void weakcrtLoopslowin(void)
{
	weakcrtLoopvars.weak_mode   			=  EV_MCU_Para.field.Weak_enable;//:0:不自动弱磁 1：查表自动弱磁 2：固定电压利用率弱磁  3：固定电压电动弱磁
	if(scsw2.field.MTPA_mode)               weakcrtLoopvars.weak_mode = 0;
	weakcrtLoopvars.sub_weak_mode           = EV_MCU_Para.field.Sub_Weak_enable;//0:id=-电流弱磁 1:超前角弱磁
	weakcrtLoopvars.weakrunLoop_state       = scsw2.field.runLoop_state;
	weakcrtLoopvars.CCP_Motor_UDCMAP        = _IQ15toIQ(EV_MCU_Para.field.Motor_UDCMAP);
	weakcrtLoopvars.Weak_Vol_Utilization  	= EV_MCU_Para.field.Weak_Vol_Utilization;
	weakcrtLoopvars.weak_ismax            = _IQ15toIQ(EV_MCU_Para.field.Weak_is_Limit);
	weakcrtLoopvars.weak_idmax            = _IQ15toIQ(EV_MCU_Para.field.Weak_id_Limt);//自动弱磁1时为调节限制量，>1时为限量
	weakcrtLoopvars.WeakIdPid_Limit       = _IQ15toIQ(EV_MCU_Para.field.Weak_idcoff_Limt);
	weakcrtLoopvars.weakIdPid_Kp	      =	_IQ12toIQ(EV_MCU_Para.field.Weak_Kp);
	weakcrtLoopvars.weakIdPid_Ki	      =	_IQ12toIQ(EV_MCU_Para.field.Weak_Ki);
	//weakcrtLoopvars.weakIdPid_Kd          = 0;
	weakcrtLoopvars.WeakIqPid_Limit       = _IQ15toIQ(EV_MCU_Para.field.Weak_iqcoff_Limt);
	weakcrtLoopvars.weakIqPid_Kp	      =	_IQ12toIQ(EV_MCU_Para.field.Weak_Kp_iq);
	weakcrtLoopvars.weakIqPid_Ki	      =	_IQ12toIQ(EV_MCU_Para.field.Weak_Ki_iq);
	//weakcrtLoopvars.weakIqPid_Kd          = 0;
	weakcrtLoopvars.weak_thetamax         = _IQ12toIQ(EV_MCU_Para.field.Weak_theta_Limt);
	//weakcrtLoopvars.iphase                = SysBase.iphase;
}

void weakcrtLoopin(void)
{
	weakcrtLoopvars.Vdc						= sysCfgPara.Vdc;
	weakcrtLoopvars.Vdq_Filter            	= voltLoopPara.Vdq_Filter;
	weakcrtLoopvars.IdCmd_in                = crtCmd_vars.IdCmd;
	weakcrtLoopvars.IqCmd_in                = crtCmd_vars.IqCmd;
	//weakcrtLoopvars.omegaEle                = sysCfgPara.Omega_ele;
}

void weakcrtLoopctrl(tag_weakcrtLoopvars *v)
{
	}

void weakcrtLoopout(void)
{
	//alarm.field.iderr_alarm = weakcrtLoopvars.iderr_alarm;
}

void weakcrtLoopctrlclc(void)
{
	weakcrtLoopvars.weak_flag            = 0;

	crtCmd_vars.IdCmd                    = 0;
	crtCmd_vars.IqCmd                    = 0;

	weakcrtLoopvars.weakIdPid_Outfilter  = 0;
	weakcrtLoopvars.weakIdPid_Err_old    = 0;
	weakcrtLoopvars.weakIdPid_Ui         = 0;
	weakcrtLoopvars.weakIdPid_Out        = 0;
	weakcrtLoopvars.weakIdPid_LimitState = PI_NO_LIMIT;
	weakcrtLoopvars.weakIdPid_Up         = 0;
	weakcrtLoopvars.weakIdPid_Fdk        = 0;

	weakcrtLoopvars.IdCmd                =0;
	weakcrtLoopvars.IdCmd_in             =0;

	weakcrtLoopvars.weakIqPid_Outfilter  = 0;
	weakcrtLoopvars.weakIqPid_Err_old    = 0;
	weakcrtLoopvars.weakIqPid_Ui         = 0;
	weakcrtLoopvars.weakIqPid_Out        = 0;
	weakcrtLoopvars.weakIqPid_LimitState =PI_NO_LIMIT;
	weakcrtLoopvars.weakIqPid_Up         = 0;
	weakcrtLoopvars.weakIqPid_Fdk        = 0;

	weakcrtLoopvars.IqCmd                =0;
	weakcrtLoopvars.IqCmd_in             =0;

	weakcrtLoopvars.weak_swap_flag       = 0;
}



#endif
