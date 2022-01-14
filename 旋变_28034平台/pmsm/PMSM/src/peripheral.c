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
	GpioCtrlRegs.GPADIR.bit.GPIO20  	= 1;  //倒车灯控制
	GpioDataRegs.GPASET.bit.GPIO20      = 1;  //倒车等控制
	GpioCtrlRegs.GPADIR.bit.GPIO22      = 1;  //PTC,R1
	GpioDataRegs.GPASET.bit.GPIO22      = 1;  //PTC,R1
	GpioCtrlRegs.GPADIR.bit.GPIO23      = 1;  //水泵,R2
	GpioDataRegs.GPASET.bit.GPIO23      = 1;  //水泵,R2
	GpioCtrlRegs.GPADIR.bit.GPIO27      = 1;  //空调,short
	GpioDataRegs.GPASET.bit.GPIO27      = 1;  //空调,short
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
	if ((1 == sccw2.field.run_flag)                //需要打开逆变器
	  &&(1 == scsw2.field.adCalib_state_finsh))    //AD校验完成
	{
		if (0 == scsw2.field.igbt_state)           //逆变器未打开
		{
			IGBT_ON();                             //打开逆变器
			if(0 == IGBT_Clock_Enable)             //逆变器时钟未使能 - 逆变器未打开
			{
				scsw2.field.igbt_state		= 1;
				FaultI2CIGBT_flag           = 1;   //电压写入I2C标志
				ClearCtrlLoopPara();               //清除各环参数
				ClearManualCtrlCmd();              //清除控制参数
			}
		}
	}
	else
	{
		IGBT_OFF();                                //关闭逆变器
		scsw2.field.igbt_state = 0;                //逆变器状态未关
		ClearCtrlLoopPara();                       //清除各环参数
		ClearManualCtrlCmd();			           //清除控制参数
	}
}

void EPWM_Disable(void)
{
//2.设置比较控制寄存器,关闭比较,关闭PWM输出
	EPwm1Regs.DBCTL.bit.OUT_MODE = 0;
	EPwm2Regs.DBCTL.bit.OUT_MODE = 0;
	EPwm3Regs.DBCTL.bit.OUT_MODE = 0;

	EALLOW;
	EPwm1Regs.AQCSFRC.all=0x0005;         //强制上下桥臂封锁输出
	EPwm2Regs.AQCSFRC.all=0x0005;
	EPwm3Regs.AQCSFRC.all=0x0005;
	EDIS;
	//3.初始化比较寄存器
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
	//2.设置比较控制寄存器,开启比较,开启PWM输出
	EPwm1Regs.DBCTL.bit.OUT_MODE = 3;
	EPwm2Regs.DBCTL.bit.OUT_MODE = 3;
	EPwm3Regs.DBCTL.bit.OUT_MODE = 3;
	EALLOW;
    EPwm1Regs.AQCSFRC.all=0x0000;         //强制上下桥臂
    EPwm2Regs.AQCSFRC.all=0x0000;
    EPwm3Regs.AQCSFRC.all=0x0000;
	//清除T5报警位子程序
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
		EPWM_Enable();           //使能PWM输出
	}
}

#endif
