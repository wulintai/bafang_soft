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
	mtrfwdvars.motor_Np                   = EV_MCU_Para.field.Motor_Np;//���������
	mtrfwdvars.motor_Ld                   = _IQ15toIQ(EV_MCU_Para.field.Ld_Stator);//����ֱ����
	mtrfwdvars.motor_Fm                   = _IQ15toIQ(EV_MCU_Para.field.Flux_Rotor);//ת�Ӵ���
	mtrfwdvars.motor_Lq                   = _IQ15toIQ(EV_MCU_Para.field.Lq_Stator);//���������

	if(SYS_DIAGNOSIS == scsw1.field.system_state)//��ѧϰʱ����Ҫǰ������
	{
		mtrfwdvars.fwd_coeff = 0;//ǰ������ϵ��Ϊ0
	}
	else
	{
		mtrfwdvars.fwd_coeff              = _IQ12toIQ(EV_MCU_Para.field.VoltFeedfwd_Coeff);//ǰ������ϵ��sqrt(3)
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

