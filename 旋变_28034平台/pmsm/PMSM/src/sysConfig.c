/*
 * sysConfig.h
 *
 *  Created on: 2014-3-19
 *      Author: ThinkPad
 */
#ifndef _SYSCONFIG_C
#define _SYSCONFIG_C

#include "include_c.h"

#pragma CODE_SECTION(thetaConfig, 	"ramfuncs");
#pragma CODE_SECTION(vdcConfig, 	"ramfuncs");

void thetaConfig(void)
{
	_iq fTemp1 =0;
	_iq fTemp2 =0;
	_iq Temp = 0;
	
	switch (sccw2.field.theta_mode)
	{
		case THETA_MODE_FEEDBACK:
			scsw2.field.theta_state	= THETA_MODE_FEEDBACK;
			sysCfgPara.Omega_ele	= sysFbkPara.Omega_fbk_Ele;
			sysCfgPara.Theta_ele    = sysFbkPara.Theta_fbk_Ele;
			break;
		case THETA_MODE_MANUAL:
			scsw2.field.theta_state	= THETA_MODE_MANUAL;
			sysCfgPara.Theta_ele	= sysCfgPara.Theta_man.value<<12;
			break;
		case THETA_MODE_OMEGA:
			scsw2.field.theta_state	= THETA_MODE_OMEGA;
			fTemp1 = sysCfgPara.Omega_man.value/10;
			sysCfgPara.Omega_ele 	= _IQmpyI32(fTemp1,SysBase.invomega);
			Temp = sysCfgPara.Omega_man.value;
			fTemp2	= ((sysCfgPara.Omega_period.value/1000)*Temp)/10;//theta = f*t
			sysCfgPara.Theta_ele 	+= fTemp2;
			break;
		default:
			break;
	}
	while((sysCfgPara.Theta_ele < 0)
		||(sysCfgPara.Theta_ele >= _IQ(1)))
	{
		if (sysCfgPara.Theta_ele < 0)
		{
			sysCfgPara.Theta_ele += _IQ(1);
		}
		else if (sysCfgPara.Theta_ele >= _IQ(1))
		{
			sysCfgPara.Theta_ele -= _IQ(1);
		}
		else
		{
		}
	}
	sysCfgPara.Theta_ele_HMI = _IQtoIQ12(sysCfgPara.Theta_ele);//0-4096
}

void vdcConfig(void)
{
	_iq temp = 0;
	
	switch (sccw2.field.vdc_mode)
	{
		case VDC_MODE_FEEDBACK:
			scsw2.field.vdc_mode	= VDC_MODE_FEEDBACK;
			sysCfgPara.Vdc			= sysFbkPara.Vdc_fbk;
			break;
		case VDC_MODE_MANUAL:
			scsw2.field.vdc_mode	= VDC_MODE_MANUAL;
			sysFbkPara.Vdc_filter_protect = sysFbkPara.Vdc_filter = sysFbkPara.Vdc_unfilter = sysCfgPara.Vdc = sysCfgPara.Vdc_man.value*SysBase.invudc;  //人工给定量为Q15格式
			break;
		default:
			break;
	}
	temp = sysCfgPara.Vdc;
	if(temp<_IQ(0.01))
	{
		temp = _IQ(0.01);
	}
	sysCfgPara.Vdc_INV          = _IQdiv(_IQ(1.0),temp);
}

void spdCmdConfig(void)
{
	switch (EV_MCU_Para.field.SpdCmd_SourceType)
	{
		case SPDCMD_SOURCE_AI:
			sysCfgPara.SpdCmd	= sysCfgPara.CANA_SpdCmd_man*SysBase.invspeed;
			break;
		case SPDCMD_SOURCE_ECANA:
			sysCfgPara.SpdCmd	= sysCfgPara.CANA_SpdCmd_man*SysBase.invspeed;
			break;
		default:
			break;
	}
}

void trqCmdConfig(void)
{
	switch (EV_MCU_Para.field.TrqCmd_SourceType)
	{
		case TRQCMD_SOURCE_AI:
			if((1 == scsw2.field.spdlmt_flag)
			 ||(2 == scsw2.field.spdlmt_flag))
			{
				spdlmt_trq_filter = spdlmt_trq_filter+_IQmpy(_IQ(0.5),(spdlmt_trq-spdlmt_trq_filter));
				sysCfgPara.TrqCmd = spdlmt_trq_filter;
			}
			else
			{
				sysCfgPara.TrqCmd_AIfilter = sysCfgPara.TrqCmd_AIfilter+_IQmpy(_IQ(0.1),(sysCfgPara.TrqCmd_AI-sysCfgPara.TrqCmd_AIfilter));
				sysCfgPara.TrqCmd = sysCfgPara.TrqCmd_AIfilter;   //trqLoopInput
			}
			break;
		case TRQCMD_SOURCE_ECANA:
			sysCfgPara.TrqCmd   = sysCfgPara.CANA_TrqCmd_man*SysBase.invtorque;
			break;
		default:
			break;
	}
}

void vdcctrl(void)  //母线电压控制
{
    if(sysCfgPara.Vdc>_IQ15toIQ(EV_MCU_Para.field.Motor_Protect_VdcLv))   //80V
    {
    	scsw2.field.vdc_state = 1;
    }
    else
    {
    	scsw2.field.vdc_state = 0;
    }
    sysCfgPara.Vdc_HMI = _IQtoIQ15(sysFbkPara.Vdc_filter);
    Vdc_faultDetect();
	sysCfgPara.Vdc	= min(sysCfgPara.Vdc, MAX_VDC);
	sysCfgPara.Vdc	= max(sysCfgPara.Vdc, 0);
	if((sysCfgPara.Vdc_HMI>_IQtoIQ15(_IQmpyI32(50,SysBase.invudc)))
	 &&(1 == FaultI2CIGBT_flag))
	{
		LimitI2CUpdate_en       =  1;
	}
	else
	{
		LimitI2CUpdate_en       =  0;
	}
}

#endif

