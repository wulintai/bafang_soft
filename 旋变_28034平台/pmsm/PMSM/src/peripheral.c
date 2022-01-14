/*
 * peripheral.c
 *
 *  Created on: 2014-3-19
 *      Author: ThinkPad
 */
#ifndef _PERIPHERAL_C
#define _PERIPHERAL_C

#include "include_c.h"

//#pragma	CODE_SECTION(IGBT_Ctrl,			"ramfuncs");
//#pragma	CODE_SECTION(IGBT_Clock,		"ramfuncs");

void InitIOPeripheral(void)
{
	EALLOW;

	GpioCtrlRegs.GPADIR.bit.GPIO11  	= 1;  //Fan_out
	GpioDataRegs.GPASET.bit.GPIO11      = 1;  //Fan_out
	GpioCtrlRegs.GPADIR.bit.GPIO20  	= 1;  //�����ƿ���
	GpioDataRegs.GPASET.bit.GPIO20      = 1;  //�����ȿ���
	GpioCtrlRegs.GPADIR.bit.GPIO22      = 1;  //PTC,R1
	GpioDataRegs.GPASET.bit.GPIO22      = 1;  //PTC,R1
	GpioCtrlRegs.GPADIR.bit.GPIO23      = 1;  //ˮ��,R2
	GpioDataRegs.GPASET.bit.GPIO23      = 1;  //ˮ��,R2
	GpioCtrlRegs.GPADIR.bit.GPIO27      = 1;  //�յ�,short
	GpioDataRegs.GPASET.bit.GPIO27      = 1;  //�յ�,short
	EDIS;
	GpioDataRegs.GPACLEAR.bit.GPIO21    = 1;

	//VCU_IOstate.field.In_fwdrev_new     = 0;
	//VCU_IOstate.field.In_fwdrev         = 0;
}

void IGBT_Clock(void)
{
	if(IGBT_Clock_Enable == 1)
	{
		IGBT_Clock_Cnt++;
	}
	else
	{
		IGBT_Clock_Cnt  = 0;
	}
}

void IGBT_Ctrl(void)
{
	if ((1 == sccw2.field.run_flag)                //��Ҫ�������
	  &&(1 == scsw2.field.adCalib_state_finsh))    //ADУ�����
	{
		if (0 == scsw2.field.igbt_state)           //�����δ��
		{
			IGBT_ON();                             //�������
			if(0 == IGBT_Clock_Enable)             //�����ʱ��δʹ�� - �����δ��
			{
				scsw2.field.igbt_state		= 1;
				FaultI2CIGBT_flag           = 1;   //��ѹд��I2C��־
				ClearCtrlLoopPara();               //�����������
				ClearManualCtrlCmd();              //������Ʋ���
			}
		}
	}
	else
	{
		IGBT_OFF();                                //�ر������
		scsw2.field.igbt_state = 0;                //�����״̬δ��
		ClearCtrlLoopPara();                       //�����������
		ClearManualCtrlCmd();			           //������Ʋ���
	}
}

void EPWM_Disable(void)
{
//2.���ñȽϿ��ƼĴ���,�رձȽ�,�ر�PWM���
	EPwm1Regs.DBCTL.bit.OUT_MODE = 0;
	EPwm2Regs.DBCTL.bit.OUT_MODE = 0;
	EPwm3Regs.DBCTL.bit.OUT_MODE = 0;

	EALLOW;
	EPwm1Regs.AQCSFRC.all=0x0005;         //ǿ�������ű۷������
	EPwm2Regs.AQCSFRC.all=0x0005;
	EPwm3Regs.AQCSFRC.all=0x0005;
	EDIS;
	//3.��ʼ���ȽϼĴ���
	EPwm1Regs.CMPA.half.CMPA = SVPWM_8KHZ>>1;//>>1);// 0x0ffff; // adjust duty for output EPWM1A-5000
	EPwm2Regs.CMPA.half.CMPA = SVPWM_8KHZ>>1;//>>1);// 0x0ffff; // adjust duty for output EPWM2A
	EPwm3Regs.CMPA.half.CMPA = SVPWM_8KHZ>>1;//>>1);// 0x0ffff; // adjust duty for output EPWM3A

#if 0
	if(triphase_Short.step == 0)
	{
		GpioDataRegs.GPACLEAR.bit.GPIO21    = 1;
		EALLOW;
		GpioCtrlRegs.GPADIR.bit.GPIO21  	= 1;
		EDIS;
	}
#endif
}

void IGBT_OFF(void)
{
	GpioDataRegs.GPACLEAR.bit.GPIO15 = 1;
	//GpioDataRegs.GPACLEAR.bit.GPIO31=1;
	EPWM_Disable();
}

void EPWM_Enable(void)
{
	if(EPwm1Regs.DBCTL.bit.OUT_MODE == 0 )
	{
	//2.���ñȽϿ��ƼĴ���,�����Ƚ�,����PWM���
	EPwm1Regs.DBCTL.bit.OUT_MODE = 3;
	EPwm2Regs.DBCTL.bit.OUT_MODE = 3;
	EPwm3Regs.DBCTL.bit.OUT_MODE = 3;
	EALLOW;
    EPwm1Regs.AQCSFRC.all=0x0000;         //ǿ�������ű�
    EPwm2Regs.AQCSFRC.all=0x0000;
    EPwm3Regs.AQCSFRC.all=0x0000;
	//���T5����λ�ӳ���
	EPwm1Regs.TZCLR.all = 0x0005;	//;
    EPwm2Regs.TZCLR.all = 0x0005;	//;
    EPwm3Regs.TZCLR.all = 0x0005;	//;
    EDIS;
	}
}

void IGBT_ON(void)
{
	IGBT_Clock_Enable = 1;
#if 1
	GpioDataRegs.GPASET.bit.GPIO15 = 1;
	//GpioDataRegs.GPASET.bit.GPIO31=1;
#endif

	if(IGBT_Clock_Cnt>=3)
	{
		IGBT_Clock_Enable = 0;
		asm("	rpt		#250	|| nop");
		EPWM_Enable();           //ʹ��PWM���
	}
}

#endif
