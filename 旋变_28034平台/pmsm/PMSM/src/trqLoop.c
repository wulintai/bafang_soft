#ifndef _TRQLOOP_C
#define _TRQLOOP_C

#include "include_c.h"

void trqLoop_Init(void)
{
	trqLoop_trqRampcmd	= 0;
	trqLoop_trqRampref	= 0;
	trqLoopPara.Max_TorqueEle_HMI = EV_MCU_Para.field.Motor_PeakTorqueEle;
	trqLoopPara.Max_TorqueGen_HMI = EV_MCU_Para.field.Motor_PeakTorqueGen;
}

void trqLoopInput(void)
{
	if(MTR_TRQ_LOOP == scsw2.field.runLoop_state)//力矩环
	{
		if(1 == zero_station_trqCmd_flag)   //删除
		{
			trqLoop_trqCmd = zero_station_trqCmd;
		}
		else
		{
			trqLoop_trqCmd = sysCfgPara.TrqCmd;
		}
	}
	else if(MTR_SPEED_LOOP == scsw2.field.runLoop_state)//速度环
	{
		trqLoop_trqCmd = spdLoopPara.TrqCmd_real;
	}
	else//其它环
	{
		trqLoop_trqCmd = 0;
	}
	
	trqLoop_EleGenMode  = (_IQmpy(trqLoop_trqRampref ,sysFbkPara.Speed_fbk_Filter ) >= 0) ? ELECTRIC_MODE : GENERATE_MODE; //判断是电动还是发电
	if(GENERATE_MODE == trqLoop_EleGenMode)
	{
		trqLoop_trqMax	= trqLoopPara.Max_TorqueGen;//发电
	}
	else
	{
		trqLoop_trqMax	= trqLoopPara.Max_TorqueEle;//电动
	}

    trqLoop_absSpdFbk = _IQabs(sysFbkPara.Speed_fbk_Filter);//IQ24
    trqidqmap_absSpdFbk = _IQmpy(_IQmpy(trqLoop_absSpdFbk,_IQ15toIQ(EV_MCU_Para.field.Motor_UDCMAP)),sysCfgPara.Vdc_INV);//IQ24
    trqidqmap_absSpdFbk = (trqidqmap_absSpdFbk>>13)<<13;//IQ24

	if(1 == EV_MCU_Para.field.Test_Mode)
	{
		trqLoop_absSpdFbk = 0;
		trqidqmap_absSpdFbk = 0;
	}
}

void trqLoopCtrl(void)
{
	if(MTR_TRQ_LOOP == scsw2.field.runLoop_state)//力矩环
	{
		if(SYS_ZEROSTOP == scsw1.field.system_state)  //零位置力矩环
		{
			trqLoop_trqRampstep				=	_IQ15toIQ(EV_MCU_Para.field.Zero_Sation_Trq_Ramp);  //10 驻坡力矩步长
			trqLoop_trqRampstep_down		=	trqLoop_trqRampstep;
		}
		else
		{
			trqLoop_trqRampstep				=	_IQ15toIQ(EV_MCU_Para.field.Trqloop_UpTrqRamp);     //1  转矩环上升的步长
			trqLoop_trqRampstep_down		=	_IQ15toIQ(EV_MCU_Para.field.Trqloop_DownTrqRamp);   //2  转矩环下降的步长
		}

		if(1 == zero_station_trqCmd_flag)   //使用驻坡力矩
		{
			trqLoop_trqRampstep				=	SysBase.invtorque;
			trqLoop_trqRampstep_down		=	trqLoop_trqRampstep;
			if(_IQabs(sysCfgPara.TrqCmd) > _IQmpyI32(10,SysBase.invtorque))    //给定力矩值 > 10
			{
				trqLoop_trqRampstep		    =	_IQ15toIQ(EV_MCU_Para.field.Trqloop_UpTrqRamp);
				trqLoop_trqRampstep_down    =	trqLoop_trqRampstep;
			}
		}
		
	}
	else if(MTR_SPEED_LOOP == scsw2.field.runLoop_state)//速度环
	{
		trqLoop_trqRampstep=trqLoop_trqMax;
		trqLoop_trqRampstep_down=trqLoop_trqMax;
	}
	else//其它
	{
		trqLoop_trqRampstep = 0;
		trqLoop_trqRampstep_down = 0;
	}
	trqLos();   //失效变量
	trqRamp();  //计算trqLoop_trqRampref
	trqidqmap();   //空
}
void trqidqmap(void)
{
	//Torque value
	}

void trqLoopOutput(void)
{
	trqLoopPara.IdCmd_real	 = trqidqmap_idref;
	trqLoopPara.IqCmd_real	 = trqidqmap_iqref;
	trqLoopPara.Torque_ref_HMI  =_IQtoIQ15(trqLoop_trqRampref);
}

void trqLoscoeff(void)
{
}

void trqLos()
{
	trqLoop_Lostrqcmd = trqLoop_trqCmd;   //失效变量
	if(GENERATE_MODE == trqLoop_EleGenMode)  //发电模式
	{
		trqLoop_trqAvl = trqLoopPara.Max_TorqueGen;
	}
	else
	{
		trqLoop_trqAvl = trqLoopPara.Max_TorqueEle;
	}
	if(trqLoop_Lostrqcmd<-trqLoop_trqAvl)
	{
		trqLoop_LostrqRef=-trqLoop_trqAvl;
	}
	else if(trqLoop_Lostrqcmd>trqLoop_trqAvl)
    {
		trqLoop_LostrqRef=trqLoop_trqAvl;
    }
	else
	{
		trqLoop_LostrqRef=trqLoop_Lostrqcmd;
	}
}

void trqRamp()
{
	trqLoop_trqRampcmd=trqLoop_LostrqRef;
	LinearRamp(trqLoop_trqRampcmd,&trqLoop_trqRampref,                    //trqLoop_trqRampcmd -> trqLoop_trqRampref -> 力矩环
			trqLoop_trqRampstep,trqLoop_trqRampstep_down,
			trqLoop_trqAvl,-trqLoop_trqAvl);
}

void trqLoopParaClr(void)
{
	FaultTrqLmtCoeff            = _IQ(1.0);  //外部故障限制扭矩系数
	trqlmtcoeff                 = _IQ(1.0);  //内部故障限制扭矩系数

	sysCfgPara.TrqCmd           = 0;
	trqLoopPara.Torque_fbk_HMI  = 0;
	trqLoop_trqCmd              = 0;
	trqLoop_EleGenMode			= ELECTRIC_MODE;

	trqLoop_trqRampref          = 0;
	trqidqmap_trq               = 0;


	trqidqmap_iqref             = 0;
	trqidqmap_idref             = 0;
	trqLoopPara.IdCmd_real      = trqidqmap_idref;
	trqLoopPara.IqCmd_real      = trqidqmap_iqref;

	trqLmt_eletrqmax            = _IQ15toIQ(EV_MCU_Para.field.Motor_PeakTorqueEle);
	trqLmt_gentrqmax            = _IQ15toIQ(EV_MCU_Para.field.Motor_PeakTorqueGen);
	trqLoopPara.Max_TorqueEle   = trqLoop_trqMaxelecoeff 	  = trqLmt_eletrqmax;
    trqLoopPara.Max_TorqueGen   = trqLoop_trqMaxgencoeff      = trqLmt_gentrqmax;
	
	trqLoopPara.Torque_ref_HMI  = _IQtoIQ15(trqLoop_trqRampref);
}

#endif

