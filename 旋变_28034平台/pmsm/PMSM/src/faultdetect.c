#ifndef _FAULTDETECT_C
#define _FAULTDETECT_C

#include "include_c.h"

void IGBT_faultProcess(void)
{
	// ~{9X9\~}
	/*if(_IQ15mpy(sysCfgPara.Vdc_HMI,SysBase.udc)>80)
	{
		if (sccw2.field.igbtFault_ChkMode == 1)
		{
			emsw.field.igbt_fault	= 1;
		}
	}
	else
	{
		emsw.field.igbt_fault	= 0;
	}*/
	emsw1.field.HardFault_OC	= 1;
	scsw2.field.fault_flag = 1;
	sccw2.field.run_flag = 0;
	//~{9X9\~}
	IGBT_OFF();

	EALLOW;
	EPwm1Regs.TZCLR.all				= 0x07;					// clear all int flag
	EPwm2Regs.TZCLR.all				= 0x07;					// clear all int flag
	EPwm3Regs.TZCLR.all				= 0x07;					// clear all int flag
	EDIS;
}

void IGBT_pwmProcess(void)//硬件限流
{
	// ~{9X9\~}
	if(_IQ15mpy(sysCfgPara.Vdc_HMI,SysBase.udc)>80)
	{
		emsw1.field.HardFault_OCPWM	= 1;
	}
	else
	{
		emsw1.field.HardFault_OCPWM	= 0;
	}
	scsw2.field.fault_flag = 1;
	sccw2.field.run_flag = 0;
	//~{9X9\~}
	IGBT_OFF();

	EALLOW;
	EPwm1Regs.TZCLR.all				= 0x07;					// clear all int flag
	EPwm2Regs.TZCLR.all				= 0x07;					// clear all int flag
	EPwm3Regs.TZCLR.all				= 0x07;					// clear all int flag
	EDIS;
}

void IGBT_vceProcess(void)//短路故障
{
	emsw.field.igbt_fault = 1;//FO fault-短路故障
	scsw2.field.fault_flag = 1;
	sccw2.field.run_flag = 0;
	//~{9X9\~}
	IGBT_OFF();
	/*if(0 == GpioDataRegs.GPADAT.bit.GPIO8)//OUT1
	{
		emsw2.field.FO_U = 1;
	}
	else if(0 == GpioDataRegs.GPADAT.bit.GPIO9)//OUT2
	{
		emsw2.field.FO_V = 1;
	}
	else//OUT3
	{
		emsw2.field.FO_W = 1;
	}*/

	EALLOW;
	EPwm1Regs.TZCLR.all				= 0x07;					// clear all int flag
	EPwm2Regs.TZCLR.all				= 0x07;					// clear all int flag
	EPwm3Regs.TZCLR.all				= 0x07;					// clear all int flag
	EDIS;
}

void OverLoad_faultDetect(void)
{
	sysFbkPara.Is_square_fbk = _IQmpy(sysFbkPara.Id_fbk,sysFbkPara.Id_fbk )+ _IQmpy(sysFbkPara.Iq_fbk,sysFbkPara.Iq_fbk);
	sysFbkPara.Is_fbk	     = _IQsqrt(sysFbkPara.Is_square_fbk);
	sysFbkPara.Is_fbk_HMI    = _IQtoIQ15(sysFbkPara.Is_fbk);
	LPFilter(sysFbkPara.Is_fbk_HMI,&sysFbkPara.Is_fbk_HMI_fiter,_IQ(0.1),_IQ(0.9));
	if(1 == first_adCalib_state_finsh)
	{
		if(sysFbkPara.Is_fbk_HMI_fiter > EV_MCU_Para.field.Protect_overLoadInv)
		{
			emsw.field.ac_current	= 1;
		}
		else
		{
			if((1 == emsw.field.ac_current)
			 &&(1 == sccw2.field.fault_recover))
			{
				emsw.field.ac_current	= 0;
			}
		}
	}
	else
	{
		emsw.field.ac_current	= 0;
	}
}

void InvOverLoadProtectInt(void)
{
	InvOvLdRatedCrt     = _IQ15mpy(EV_MCU_Para.field.Motor_Protect_InvCrt,_IQ15(1.414));
	InvOvLdTime         = 0;
	InvOvLdTimeRamp     = -240;
	IpercentInv         = 0;
	IpercentCoeffInv    = EV_MCU_Para.field.Motor_Protect_InvCrtCoeff<<3;
	InvProtectCoeff     = _IQ15(1.0);
}

void MtrOverLoadProtectInt(void)
{
	MtrOvLdRatedCrt   = _IQ15mpy(EV_MCU_Para.field.Motor_Protect_MtrCrt,_IQ15(1.414));
	MtrOvLdTime       = 0;
	MtrOvLdTimeRamp   = -450;
	IpercentMtr       = 0;
	IpercentCoeffMtr  = EV_MCU_Para.field.Motor_Protect_MtrCrtCoeff<<3;
	MtrProtectCoeff   = _IQ15(1.0);
	OvLdTimeCnt       = 5;
	OvLdProtectCoeff  = _IQ15(1.0);
	Frq_Real          = 0;
}

void Vdc_faultDetect(void)
{
	if(1 == canarev_cmd.cmdmode2.data.main_relay_state)
	{
		motor_main_relay_cnt++;
		if(motor_main_relay_cnt>1000)
		{
			motor_main_relay_cnt = 1000;
		}
	}
	if((motor_sys_init_cnt>=250)
	 &&(motor_main_relay_cnt>=1000))
	{
		if(_IQtoIQ15(sysFbkPara.Vdc_filter_protect) > EV_MCU_Para.field.Motor_Protect_VdcOv)
		{
			emsw.field.vdc_overVolt = 1;
		}
		else
		{
			if((1 == emsw.field.vdc_overVolt)
			 &&(1 == sccw2.field.fault_recover))
			{
				emsw.field.vdc_overVolt = 0;
			}
		}

		if (_IQtoIQ15(sysFbkPara.Vdc_filter_protect) < EV_MCU_Para.field.Motor_Protect_VdcLv)
		{
			if(1 == sccw1.field.run_enable)
			{
				emsw.field.vdc_lackVolt = 1;
			}
			else
			{
				emsw.field.vdc_lackVolt = 0;
			}
		}
		else
		{
			if((1 == emsw.field.vdc_lackVolt)
			 &&(1 == sccw2.field.fault_recover))
			{
				emsw.field.vdc_lackVolt = 0;
			}
		}
	}
}

void FaultProcess(void)
{
	if(1 == scsw2.field.fault_flag)
	{
		if(0 == scsw2.field.igbt_state)   
		{
			ClearManualCtrlCmd();				
			ClearCtrlLoopPara();				
		}
	}
}

//#pragma CODE_SECTION(FaultExpress, 	"ramfuncs");
void FaultExpress(void)
{
	if(0 == (emsw.wValue||emsw1.wValue||emsw2.wValue||alarm.wValue))
	{
		scsw2.field.error_rank = 0;
	}
	else if(0 != (emsw.wValue||emsw1.wValue))
	{
		scsw2.field.error_rank = 4;
	}
	else if(0 != emsw2.wValue)
	{
		scsw2.field.error_rank = 3;
	}
	else if(0 != alarm.wValue)
	{
		scsw2.field.error_rank = 2;
	}
	else
	{
		scsw2.field.error_rank = 1;
	}

	if(scsw2.field.error_rank>=3)
	{
		if(1 == scsw2.field.igbt_state)
		{
			if((4 == scsw2.field.error_rank)
			 ||(_IQ15abs(sysFbkPara.Speed_fbk_HMI)<_IQ15div(700,SysBase.speed)))
			{
				IGBT_OFF();
				scsw2.field.igbt_state = 0;
			}
		}
		scsw2.field.fault_flag	= 1;

		if((FaultDataRecord_en==1)&&(FaultI2CRecord_en==0)&& !emsw.field.vdc_lackVolt && LimitI2CUpdate_en
			&&(emsw.wValue != emswold.wValue || emsw1.wValue != emsw1old.wValue || emsw2.wValue != emsw2old.wValue))
	    {
	    	I2C_FaultDataRecord();
			emswold.wValue = emsw.wValue;
			emsw1old.wValue= emsw1.wValue;
			emsw2old.wValue= emsw2.wValue;
	    }
	}
	else
	{
		FaultTrqLmtCoeff        = _IQ(1.0);
		scsw2.field.fault_flag	= 0;
		FaultDataRecord_en      = 1;     
		FaultI2CRecord_en       = 0;     
		emswold.wValue = emsw.wValue;
		emsw1old.wValue= emsw1.wValue;
		emsw2old.wValue= emsw2.wValue;
	}
}

void SpeedDetectProtectint(void)
{
	SpdEn_SpeedDetectFault   = _IQdiv(200,SysBase.speed); //~{C?~}500us~{1d;/~}200rpm~{<41(9JUO~}
	SpdOld_SpeedDetectFault  = 0;
	SpdDiff_SpeedDetectFault = 0;
	Cnt_SpeedDetectFault     = 0;
	CntTime_SpeedDetectFault = 0;
}

void SpeedFbk_FaultChecking(void)
{
	if (sysFbkPara.Speed_fbk_HMI > EV_MCU_Para.field.Motor_Protect_OverSpdFwd)
	{
		emsw.field.over_speedFwd = 1;
	}
	else
	{
		if((emsw.field.over_speedFwd == 1)&&(sccw2.field.fault_recover == 1))
		{
			emsw.field.over_speedFwd = 0;
		}
	}
	if (sysFbkPara.Speed_fbk_HMI < -((_iq)EV_MCU_Para.field.Motor_Protect_OverSpdRev))
	{
		emsw.field.over_speedRev = 1;
	}
	else
	{
		if((emsw.field.over_speedRev == 1)&&(sccw2.field.fault_recover == 1))
		{
			emsw.field.over_speedRev = 0;
		}
	}
}

void SpeedDetectProtect(void)
{
    if(1 == EV_MCU_Para.field.SpeedDetectFault_En)
    {
    	if(1 == scsw2.field.igbt_state)
    	{
    		CntTime_SpeedDetectFault++;
    		SpdDiff_SpeedDetectFault = _IQabs(SpeedCal_spdFbk - SpdOld_SpeedDetectFault);
    		SpdOld_SpeedDetectFault  = SpeedCal_spdFbk;
    		if(SpdDiff_SpeedDetectFault > SpdEn_SpeedDetectFault)     Cnt_SpeedDetectFault++;
    		if(CntTime_SpeedDetectFault >= 120000)                   //1min~{DZ3vOVH}4N<41(KY6H<l2b9JUO~}
    		{
    			if(Cnt_SpeedDetectFault >= 3)   emsw1.field.Speed_detect_fault = 1;
    			CntTime_SpeedDetectFault        = 0;
    			Cnt_SpeedDetectFault            = 0;
    		}
    	}
    	else
    	{
    		Cnt_SpeedDetectFault     = 0;
    		CntTime_SpeedDetectFault = 0;
    		SpdOld_SpeedDetectFault  = SpeedCal_spdFbk;
    	}
    }
	if((emsw1.field.Speed_detect_fault == 1)&&(sccw2.field.fault_recover == 1))
	{
		emsw1.field.Speed_detect_fault = 0;
	}
}

void HallDetectProtect(void)
{
}

void I2C_FaultDataRecord(void)
{
	FaultDataRecord_num = I2CErr_Record.field.I2C_fault_num + 1;   //~{=xHkPB9JUOJ}>]<GB<#,9JUOWiJ}<S~}1
	if(FaultDataRecord_num > 4)  FaultDataRecord_num = 0;
	I2CErr_Record.field.I2CErr_Data[FaultDataRecord_num].emsw    = emsw.wValue;
	I2CErr_Record.field.I2CErr_Data[FaultDataRecord_num].emsw1   = emsw1.wValue;
	I2CErr_Record.field.I2CErr_Data[FaultDataRecord_num].emsw2   = emsw2.wValue;
	I2CErr_Record.field.I2CErr_Data[FaultDataRecord_num].alarm   = alarm.wValue;
	I2CErr_Record.field.I2CErr_Data[FaultDataRecord_num].speed   = _IQ15mpy(sysFbkPara.Speed_fbk_HMI,SysBase.speed);
	I2CErr_Record.field.I2CErr_Data[FaultDataRecord_num].torque  = _IQ15mpy(trqLoopPara.Torque_fbk_HMI,SysBase.torque);
	I2CErr_Record.field.I2CErr_Data[FaultDataRecord_num].mdl_temp  = sysFbkPara.Temp_mdl;
	I2CErr_Record.field.I2CErr_Data[FaultDataRecord_num].mtr_temp  = sysFbkPara.Temp_mtr;
	I2CErr_Record.field.I2CErr_Data[FaultDataRecord_num].udc       = _IQ15mpy(sysCfgPara.Vdc_HMI,SysBase.udc);
	I2CErr_Record.field.I2CErr_Data[FaultDataRecord_num].udq       = _IQ15mpy(voltLoopPara.Vdq_HMI,SysBase.udc);
	I2CErr_Record.field.I2CErr_Data[FaultDataRecord_num].id        = _IQ15mpy(crtLoopPara.Id_ref_HMI,SysBase.iphase);
	I2CErr_Record.field.I2CErr_Data[FaultDataRecord_num].iq        = _IQ15mpy(crtLoopPara.Iq_ref_HMI,SysBase.iphase);
	I2CErr_Record.field.I2CErr_Data[FaultDataRecord_num].ia        = _IQ15mpy(sysFbkPara.Iu_fbk_HMI,SysBase.iphase);
	I2CErr_Record.field.I2CErr_Data[FaultDataRecord_num].ib        = _IQ15mpy(sysFbkPara.Iv_fbk_HMI,SysBase.iphase);
	I2CErr_Record.field.I2CErr_Data[FaultDataRecord_num].ic        = _IQ15mpy(sysFbkPara.Iw_fbk_HMI,SysBase.iphase);
	I2CErr_Record.field.I2CErr_Data[FaultDataRecord_num].reseverd  = sccw1.field.run_enable;

	FaultDataRecord_en  = 0;    //~{9JUOJ}>]N4P4Hk~}I2C~{G0={V9TY4N8|PB~}
	FaultI2CRecord_en   = 1;    //I2C~{P4Hk2YWwJ9D\#,5H4}P4Hk~}
}

void CrtDetectProtect(void)
{
	if(1 == EV_MCU_Para.field.CrtDetectFault_En)
	{
		if(scsw2.field.adCalib_state_finsh && !scsw2.field.igbt_state )
		{
			if(_IQ15abs(sysFbkPara.Speed_fbk_HMI)<_IQ15div(30,SysBase.speed))
			{
				if(sysFbkPara.Is_fbk_HMI_fiter > CrtEn_CrtDetectFault)
				{
					Cnt_CrtDetectFault ++;
				}
				if(Cnt_CrtDetectFault >= 1000)
				{
					emsw1.field.Crt_detect_fault = 1;
					Cnt_CrtDetectFault           = 0;
				}
			}
			else
			{
				Cnt_CrtDetectFault = 0;
			}
		}
		else
		{
			Cnt_CrtDetectFault = 0;
		}
	}
	else
	{
		Cnt_CrtDetectFault = 0;
	}
	if((1 == emsw1.field.Crt_detect_fault)
	 &&(1 == sccw2.field.fault_recover))
	{
		emsw1.field.Crt_detect_fault = 0;
	}
}

void InvOverLoadProtect(void)   //~{1dF5Fw9}TX1#;$~},500ms~{VP6O~}
{
	_iq temp  = 0;
	_iq temp1 = 0;
	
	InvOvLdRatedCrt = _IQ15mpy(EV_MCU_Para.field.Motor_Protect_InvCrt,_IQ15(1.414));//IQ15
	IpercentCoeffInv = EV_MCU_Para.field.Motor_Protect_InvCrtCoeff<<3;//IQ15
	IpercentInv = _IQ15div(_IQ15div(sysFbkPara.Is_fbk_HMI_fiter,InvOvLdRatedCrt),IpercentCoeffInv);

	if(Frq_Real < 2000)//电机转速小于200转
	{
	   temp        = _IQ15div(1000,550+(Frq_Real*450/2000));
	   IpercentInv = _IQ15mpy(IpercentInv,temp);
	}

	temp1= (sysFbkPara.Temp_mdl-60)<<1;
	if(temp1 < -5)  temp1 = -5;
	if(temp1 > 30)  temp1 = 30;
	temp = _IQ15(1.0)+(_IQ15(0.30)*temp1/30);
	IpercentInv = _IQ15mpy(IpercentInv,temp);

	if(IpercentInv >= InvOvLd400P)
	{
        InvOvLdTime = 3600000;
	}
	else if(IpercentInv <= InvOvLd110P)
	{
		InvOvLdTime  -= 20000;        //2min~{SI~}2400000~{=5N*~}0~{#,~}3min~{SI~}3600000~{=+N*~}0
		if(InvOvLdTime<0)      InvOvLdTime = 0;
	}
	else
	{
        if(IpercentInv<=InvOvLd120P)       InvOvLdTimeRamp = 17280-_IQ15mpy(IpercentInv-InvOvLd110P,_IQ15(2.636719));
    	else if(IpercentInv<=InvOvLd130P)  InvOvLdTimeRamp = 8640-_IQ15mpy(IpercentInv-InvOvLd120P,_IQ15(1.318359));
    	else if(IpercentInv<=InvOvLd140P)  InvOvLdTimeRamp = 4320-_IQ15mpy(IpercentInv-InvOvLd130P,_IQ15(0.878906));
    	else if(IpercentInv<=InvOvLd150P)  InvOvLdTimeRamp = 1440-_IQ15mpy(IpercentInv-InvOvLd140P,_IQ15(0.219727));
    	else if(IpercentInv<=InvOvLd165P)  InvOvLdTimeRamp = 720-_IQ15mpy(IpercentInv-InvOvLd150P,_IQ15(0.073242));
    	else if(IpercentInv<=InvOvLd180P)  InvOvLdTimeRamp = 360-_IQ15mpy(IpercentInv-InvOvLd165P,_IQ15(0.036621));
    	else if(IpercentInv<=InvOvLd190P)  InvOvLdTimeRamp = 180-_IQ15mpy(IpercentInv-InvOvLd180P,_IQ15(0.027466));
    	else if(IpercentInv<=InvOvLd200P)  InvOvLdTimeRamp = 90-_IQ15mpy(IpercentInv-InvOvLd190P,_IQ15(0.018311));
        else if(IpercentInv<=InvOvLd240P)  InvOvLdTimeRamp = 30-_IQ15mpy(IpercentInv-InvOvLd200P,_IQ15(0.001984));
        else                               InvOvLdTimeRamp = 2;
        InvOvLdTime += (1800000/InvOvLdTimeRamp);
        /*~{1#;$J1<dK5Cw~}
         * <1.1,IpercentInv,1.2>:17280~~8640,   ~{?I3VPxTKPP~}192~{!+~}96min~{#,;y1>N^1#;$~}
         * <1.2,IpercentInv,1.3>:8640~~4320,    ~{?I3VPxTKPP~}96min~{!+~}48min~{#,;y1>N^1#;$~}
         * <1.3,IpercentInv,1.4>:4320~{!+~}1440~{#,~}     ~{?I3VPxTKPP~}48min~{!+~}16min~{#,;y1>N^1#;$~}
         * <1.4,IpercentInv,1.5>:1440~{!+~}720~{#,~}       ~{?I3VPxTKPP~}16min~{!+~}8min
         * <1.5,IpercentInv,1.65>:720~{!+~}360~{#,~}       ~{?I3VPxTKPP~}8min~{!+~}4min
         * <1.65,IpercentInv,1.8>:360~{!+~}180~{#,~}       ~{?I3VPxTKPP~}4min~{!+~}2min
         * <1.8,IpercentInv,1.9>:180~{!+~}90~{#,~}           ~{?I3VPxTKPP~}2min~{!+~}1min
         * <1.9,IpercentInv,2.0>:90~{!+~}30~{#,~}             ~{?I3VPxTKPP~}1min~{!+~}20s
         * <2.0,IpercentInv,2.4>:30~~4,         ~{?I3VPxTKPP~}20s~~2.66s
         * <2.4,IpercentInv,4.0>:2,            ~{?I3VPxTKPP~}1.33s
         */
	}
	if(InvOvLdTime > 2400000)
	{
		alarm.field.InvOvLd_alarm = 1;  //~{9}TX1(>/~}
		if(InvOvLdTime >= 3600000)
		{
			emsw.field.inv_overLoad	= 1;  //~{9}TX9JUO#,M#;z~}
		    InvProtectCoeff = _IQ15(0.4); //~{7eV5E$>X=5VA~}0.5
		    InvOvLdTime = 3600000;
		}
		else
		{
			InvProtectCoeff = _IQ15(1.0)-_IQ15mpy(_IQ15div(InvOvLdTime-2400000,2400000),_IQ15(1.2)); //~{O^VF7eV5E$>X#:~}0.4~{!+~}1.0
		}
	}
	else
	{
		alarm.field.InvOvLd_alarm = 0;
		InvProtectCoeff           = _IQ15(1.0);
	}
	if((emsw.field.inv_overLoad	== 1)&&(sccw2.field.fault_recover == 1))
	{
		emsw.field.inv_overLoad	= 0;
	}
}

void MtrOverLoadProtect(void)     //~{5g;z9}TX1#;$~},500ms~{V\FZ~}
{
	_iq temp  = 0;
	_iq temp1 = 0;
	
	MtrOvLdRatedCrt   = _IQ15mpy(EV_MCU_Para.field.Motor_Protect_MtrCrt,_IQ15(1.414));
	IpercentCoeffMtr  = EV_MCU_Para.field.Motor_Protect_MtrCrtCoeff<<3;
	IpercentMtr = _IQ15div(_IQ15div(sysFbkPara.Is_fbk_HMI_fiter,MtrOvLdRatedCrt),IpercentCoeffMtr);

	if(Frq_Real <= 2000)
	{
	   temp        = _IQ15div(1000,550+(Frq_Real*450/2000));
	   IpercentMtr = _IQ15mpy(IpercentMtr,temp);
	}
    
	temp1 = (sysFbkPara.Temp_mtr-120)<<1;
	if(temp1 < -7)  temp1 = -7;
	if(temp1 >  42)  temp1 = 42;
	temp = _IQ15(1.0)+(_IQ15(0.30)*temp1/42);
	IpercentMtr = _IQ15mpy(IpercentMtr,temp);

    if(IpercentMtr >= MtrOvLd400P)
    {
        MtrOvLdTime = 1440000;
    }
    else if(IpercentMtr <= MtrOvLd116P)
    {
    	MtrOvLdTime -= 6000;     //1.5min~{74J1O^<d8t~}
    	if(MtrOvLdTime < 0)      MtrOvLdTime = 0;
    }
    else
    {
        if(IpercentMtr<=MtrOvLd137P)       MtrOvLdTimeRamp = 18000-_IQ15mpy(IpercentMtr-MtrOvLd116P,_IQ15(1.307845));
    	else if(IpercentMtr<=MtrOvLd158P)  MtrOvLdTimeRamp = 9000-_IQ15mpy(IpercentMtr-MtrOvLd137P,_IQ15(0.871897));
    	else if(IpercentMtr<=MtrOvLd179P)  MtrOvLdTimeRamp = 3000-_IQ15mpy(IpercentMtr-MtrOvLd158P,_IQ15(0.348772));
    	else if(IpercentMtr<=MtrOvLd201P)  MtrOvLdTimeRamp = 600-_IQ15mpy(IpercentMtr-MtrOvLd179P,_IQ15(0.072661));
    	else if(IpercentMtr<=MtrOvLd222P)  MtrOvLdTimeRamp = 100-_IQ15mpy(IpercentMtr-MtrOvLd201P,_IQ15(0.007266));
    	else if(IpercentMtr<=MtrOvLd243P)  MtrOvLdTimeRamp = 50-_IQ15mpy(IpercentMtr-MtrOvLd222P,_IQ15(0.005813));
        else                               MtrOvLdTimeRamp = 5;
        MtrOvLdTime += (1800000/MtrOvLdTimeRamp);
        /*
	    *<1.16,IpercentMtr,1.37>:18000~~9000,100~~200~{#,?I3VPxTKPP~}60~{!+~}30min
	    *<1.37,IpercentMtr,1.58>:9000~~3000,200~~600~{#,~}  ~{?I3VPxTKPP~}30min~{!+~}10min
	    *<1.58,IpercentMtr,1.79>:3000~~600,600~~3000~{#,~}  ~{?I3VPxTKPP~}10min~{!+~}2min
	    *<1.79,IpercentMtr,2.01>:600~~100,3000~~18000~{#,?I3VPxTKPP~}2min~{!+~}20s
	    *<2.01,IpercentMtr,2.22>:100~~50,18000~~36000~{#,?I3VPxTKPP~}20s~{!+~}10s
	    *<2.22,IpercentMtr,2.43>:50~~10,36000~~180000~{#,?I3VPxTKPP~}10s~{!+~}2s
	    *<2.43,IpercentMtr,4.00>:5,360000~{#,~}                    ~{?I3VPxTKPP~}1s
	    */
    }
	if(MtrOvLdTime > 720000)
	{
		alarm.field.MtrOvLd_alarm = 1;  //~{9}TX1(>/~}
		if(MtrOvLdTime >= 1440000)
		{
			emsw.field.inv_overLoad	= 1;  //~{9}TX9JUO#,M#;z~}
		    MtrProtectCoeff = _IQ15(0.4); //~{7eV5E$>X=5VA~}0.4
		    MtrOvLdTime     = 1440000;
		}
		else
		{
			MtrProtectCoeff = _IQ15(1.0)-_IQ15mpy(_IQ15div(MtrOvLdTime-720000,720000),_IQ15(0.6));//~{O^VF7eV5E$>X#:~}0.4~{!+~}1.0
		}
	}
	else
	{
		alarm.field.MtrOvLd_alarm = 0;
		MtrProtectCoeff           = _IQ15(1.0);
	}
	if((emsw.field.inv_overLoad	== 1)&&(sccw2.field.fault_recover == 1))
	{
		emsw.field.inv_overLoad	= 0;
	}
}

#if 0

void MtrOpenPhaseProtect(void)          //~{Jd3vH1O`<l2b~}
{
	_iq temp;

	Crt_OpenPhaseMin  = _IQ15mpy(EV_MCU_Para.field.Motor_Protect_MtrCrt,_IQ15mpy(EV_MCU_Para.field.OpenPhaseCheck_CrtCoeff<<3,_IQ15(1.414)));

	if(Cnt_OpenPhase == 350000)
	{
		  if(!(Ia_checksum >= Ib_checksum))
		  {
			temp = Ia_checksum;
			Ia_checksum = Ib_checksum;
			Ib_checksum = temp;
		  }
		  if(!(Ia_checksum >= Ic_checksum))
		  {
			temp = Ia_checksum;
			Ia_checksum = Ic_checksum;
			Ic_checksum = temp;
		  }
		  if(Ib_checksum < Ic_checksum)
		  {
			  Ic_checksum = Ib_checksum;
		  }
		  if(Ia_checksum > 3*Ic_checksum)
		  {
			  emsw1.field.open_phase = 1;
		  }
		  Ia_checksum   = 0;
		  Ib_checksum   = 0;
		  Ic_checksum   = 0;
		  Cnt_OpenPhase = 0;
	}
	if((emsw1.field.open_phase == 1)&&(sccw2.field.fault_recover == 1))
	{
		emsw1.field.open_phase = 0;
	}
}


void MtrOpenPhaseProtectInt(void)
{
    Ia_checksum       = 0;
    Ib_checksum       = 0;
    Ic_checksum       = 0;
    Cnt_OpenPhase     = 0;
	Spd_OpenPhaseMin  = _IQ15div(10,SysBase.speed);
	Crt_OpenPhaseMin  = _IQ15mpy(EV_MCU_Para.field.Motor_Protect_MtrCrt,_IQ15mpy(EV_MCU_Para.field.OpenPhaseCheck_CrtCoeff<<3,_IQ15(1.414)));
}

void PhaseUnbalanceProtect(void)          //~{Jd3v2;F=:b<l2b~}
{
	if(1 == EV_MCU_Para.field.PhaseUnbalanceCheck_En)
	{
		if(1 == scsw2.field.igbt_state)
		{
			Crt_PhaseUnbalance    = _IQtoIQ15(_IQabs(sysFbkPara.Iu_fbk_real+sysFbkPara.Iv_fbk_real+sysFbkPara.Iw_fbk_real));
			CrtPer_PhaseUnbalance = _IQ15div(Crt_PhaseUnbalance,_IQ15mpy(EV_MCU_Para.field.Motor_Protect_MtrCrt,_IQ15(1.414)));
			CrtPer_PhaseUnbalance = _IQ15div(CrtPer_PhaseUnbalance,EV_MCU_Para.field.PhaseUnbalanceCheck_CrtCoeff<<3);

			if(CrtPer_PhaseUnbalance >= MtrUBl120P)       Cnt_PhaseUnbalance += 60;  //1s
			else if(CrtPer_PhaseUnbalance >= MtrUBl100P)  Cnt_PhaseUnbalance += 30;  //2s
			else if(CrtPer_PhaseUnbalance >= MtrUBl080P)  Cnt_PhaseUnbalance += 10;  //6s
			else if(CrtPer_PhaseUnbalance >= MtrUBl060P)  Cnt_PhaseUnbalance += 4;   //15s
			else if(CrtPer_PhaseUnbalance >= MtrUBl040P)  Cnt_PhaseUnbalance += 2;   //30s
			else if(CrtPer_PhaseUnbalance >= MtrUBl020P)  Cnt_PhaseUnbalance += 1;   //60s
			else
			{
				if(Cnt_PhaseUnbalance < 100)              Cnt_PhaseUnbalance  = 100;
				else                                      Cnt_PhaseUnbalance -= 6;
			}
            if(Cnt_PhaseUnbalance > 60000 )
			{
				emsw1.field.Insulation_fault = 1;
				Cnt_PhaseUnbalance           = 0;
				CrtPer_PhaseUnbalance        = 0;
			}
		}
		else
		{
			Cnt_PhaseUnbalance    = 0;
			CrtPer_PhaseUnbalance = 0;
		}
	}
	if((emsw1.field.Insulation_fault == 1)&&(sccw2.field.fault_recover == 1))
	{
		emsw1.field.Insulation_fault = 0;
	}
}


void PhaseUnbalanceProtectint(void)
{
	Cnt_PhaseUnbalance    = 0;
	CrtPer_PhaseUnbalance = 0;
}
#endif


void CrtDetectProtectint(void)
{
	CrtEn_CrtDetectFault = _IQ15div(10,SysBase.iphase);
	Cnt_CrtDetectFault   = 0;
}

#endif

