/*
 * mtrfwd.c
 *
 *  Created on: 2017-4-19
 *      Author: ZGJ
 */
#ifndef _MTRFWD_C
#define _MTRFWD_C

#include "include_c.h"

#pragma	CODE_SECTION(mtrfwdin,		"ramfuncs");
//#pragma	CODE_SECTION(mtrfwdclc,	    "ramfuncs");
#pragma	CODE_SECTION(mtrfwdctrl,	"ramfuncs");

void mtrfwdslowin(void)
{
	mtrfwdvars.motor_Np                   = EV_MCU_Para.field.Motor_Np;//电机极对数
	mtrfwdvars.motor_Ld                   = _IQ15toIQ(EV_MCU_Para.field.Ld_Stator);//定子直轴电感
	mtrfwdvars.motor_Fm                   = _IQ15toIQ(EV_MCU_Para.field.Flux_Rotor);//转子磁链
	mtrfwdvars.motor_Lq                   = _IQ15toIQ(EV_MCU_Para.field.Lq_Stator);//电机交轴电感

	if(SYS_DIAGNOSIS == scsw1.field.system_state)//自学习时不需要前馈补偿
	{
		mtrfwdvars.fwd_coeff = 0;//前馈补偿系数为0
	}
	else
	{
		mtrfwdvars.fwd_coeff              = _IQ12toIQ(EV_MCU_Para.field.VoltFeedfwd_Coeff);//前馈补偿系数sqrt(3)
	}
 
	mtrfwdvars.udcbase                    = SysBase.udc;
	mtrfwdvars.speedbase_div_udcbase      = _IQmpyI32(SysBase.speed,SysBase.invudc);
	mtrfwdvars.iphasebase_div_udcbase     = _IQmpyI32(SysBase.iphase,SysBase.invudc);
	mtrfwdvars.Motor_Direction            = EV_MCU_Para.field.Motor_Direction;
}

void mtrfwdin(void)
{
	mtrfwdvars.id_cmd                     = crtLoopPara.IdPid_Ref;//IQ24
	mtrfwdvars.iq_cmd                     = crtLoopPara.IqPid_Ref;//IQ24
	//mtrfwdvars.id_fbk                     = sysFbkPara.Id_fbk_filter;//IQ24
	//mtrfwdvars.iq_fbk                     = sysFbkPara.Iq_fbk_filter;//IQ24
	//mtrfwdvars.vdc                        = sysCfgPara.Vdc;//IQ24
	mtrfwdvars.speedfbk                   = sysFbkPara.Speed_fbk_Filter;//IQ24
}

void mtrfwdctrl(tag_mtrfwdvars *v)
{
	}

void mtrfwdout(void)
{
	
}

void mtrfwdclc(void)
{
	mtrfwdvars.IdPid_Out_fwd = 0;
	mtrfwdvars.IqPid_Out_fwd = 0;
}

#endif

