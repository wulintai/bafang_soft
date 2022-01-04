/*
 * encoffsettune.c
 *
 *  Created on: 2017-4-17
 *      Author: ZGJ
 */

#ifndef _MTR_LEARNEXACTOFFSET_C
#define _MTR_LEARNEXACTOFFSET_C

#include "include_c.h"

void exactTuneinit(void)
{
	exactTune.SysBasespeed     =  SysBase.speed;
}

#if 0
void diagexactEncOffsetTuneAct_input()
{
	exactTune.Speed_fbk_HMI =  sysFbkPara.Speed_fbk_HMI;
	exactTune.Vd_ref_HMI    =  voltLoopPara.Vd_ref_HMI;
	exactTune.Vq_ref_HMI    =  voltLoopPara.Vq_ref_HMI;
}

void diagexactEncOffsetTuneAct_ctrl(tag_exactTune *v)
{
	}

void diagexactEncOffsetTuneAct_output()
{
	//Uint16 i = 0;
	//Uint16 test = 0;
	
	sccw2.field.run_enable   = exactTune.IGBT_run_enable;
	sccw2.field.runLoop_mode = exactTune.runLoop_mode;
	if(exactTune.DIAG_ENCODER_error>0)      emsw2.field.DIAG_ENCODER_error = 1;
	sysCfgPara.IdCmd_man                    = exactTune.IdCmd_man;
	sysCfgPara.IqCmd_man                    = exactTune.IqCmd_man;
	EV_MCU_Para.field.Motor_Encoder_offset  = exactTune.Motor_Encoder_offset;
    if(1 == exactTune.finsh_flag)
    {
    	if(0 == exactTune.DIAG_ENCODER_error)
		{

    		if(EV_MCU_Para.field.Motor_Encoder_offset>_IQ12(1)) EV_MCU_Para.field.Motor_Encoder_offset = epdw.field.Motor_Encoder_offset = exactTune.Motor_Encoder_offset-_IQ12(1);
    		else if(EV_MCU_Para.field.Motor_Encoder_offset<0) EV_MCU_Para.field.Motor_Encoder_offset = epdw.field.Motor_Encoder_offset = exactTune.Motor_Encoder_offset+_IQ12(1);
    		else EV_MCU_Para.field.Motor_Encoder_offset = epdw.field.Motor_Encoder_offset = exactTune.Motor_Encoder_offset;
#if 0

    		while(I2cWriteData(4,&epdw.value[4])){}
    		for(i=0;i<40000;i++) {}
    		while(I2cReadData(4,&test)) {}
			if(test==epdw.value[4]) alarm.field.I2C_alarm = 0;
			else alarm.field.I2C_alarm = 1;
#endif
		}
		sccw2.field.diag_enable     		= 0;
		sccw2.field.diag_mode 				= DIAG_NON_TEST;
		exactTune.finsh_flag                = 0;
    }
}

#else
void diagexactEncOffsetTuneAct_input()
{
	exactTune.IsFbkFilter            =  sysFbkPara.Is_fbk_HMI_fiter;//IQ15
	exactTune.Theta_fbk_Ele          =  sysFbkPara.Theta_fbk_Ele;//IQ24
	exactTune.Speed_fbk_HMI          =  sysFbkPara.Speed_fbk_HMI;//IQ15
	exactTune.speed_theta            =  spdLoopPara.Out_Theta;//IQ24
}

void diagexactEncOffsetTuneAct_ctrl(tag_exactTune *v)
{
	}

void diagexactEncOffsetTuneAct_output()
{
	sccw2.field.run_enable   = exactTune.IGBT_run_enable;
	sccw2.field.runLoop_mode = exactTune.runLoop_mode;
	sccw2.field.theta_mode   = exactTune.theta_mode;
	sysCfgPara.Theta_man.value = exactTune.Theta_man;
	sysCfgPara.VdCmd_man = exactTune.VdCmd_man;
	sysCfgPara.VqCmd_man = exactTune.VqCmd_man;
	if(exactTune.DIAG_ENCODER_error>0)
	{
		emsw2.field.DIAG_ENCODER_error = 1;
	}
	sysCfgPara.SpdCmd        = _IQ15toIQ(exactTune.SpeedCmd_man);//IQ24
	EV_MCU_Para.field.Motor_Encoder_offset      = exactTune.Motor_Encoder_offset>>12;
	EV_MCU_Para.field.Motor_Encoder_Direction   = exactTune.Encoder_direction;
	//Motor_StudyPara.field.Encoder_Offset_HMI          = EV_MCU_Para.field.Motor_Encoder_offset;
	//Motor_StudyPara.field.Encoder_Dir                 = EV_MCU_Para.field.Motor_Encoder_Direction;
    if(1 == exactTune.finsh_flag)
    {
    	if(0 == exactTune.DIAG_ENCODER_error)
		{
			exactTune.deta_offset = (exactTune.positive_offset-exactTune.negative_offset)>>1;
			if(1 == exactTune.Motor_Direction)
			{
				exactTune.deta_offset = 0-exactTune.deta_offset;
			}
			EV_MCU_Para.field.Motor_Encoder_offset = EV_MCU_Para.field.Motor_Encoder_offset+_IQtoIQ12(exactTune.deta_offset);
			if(EV_MCU_Para.field.Motor_Encoder_offset>_IQ12(1))
			{
				EV_MCU_Para.field.Motor_Encoder_offset = epdw.field.Motor_Encoder_offset = EV_MCU_Para.field.Motor_Encoder_offset-_IQ12(1);
			}
    		else if(EV_MCU_Para.field.Motor_Encoder_offset<0)
			{
				EV_MCU_Para.field.Motor_Encoder_offset = epdw.field.Motor_Encoder_offset = EV_MCU_Para.field.Motor_Encoder_offset+_IQ12(1);
    		}
			//Motor_StudyPara.field.Encoder_Offset_HMI          = EV_MCU_Para.field.Motor_Encoder_offset;
			//Motor_StudyPara.field.Encoder_Dir                 = EV_MCU_Para.field.Motor_Encoder_Direction;
			exactTune.write_exactune_flag = 1;
		}
		sccw2.field.diag_enable     		= 0;
		sccw2.field.diag_mode 				= DIAG_NON_TEST;
		exactTune.finsh_flag                = 0;
    }
	else
	{
		//Motor_StudyPara.field.Encoder_Offset_HMI          = 0;
		//Motor_StudyPara.field.Encoder_Dir                 = 0;
	}
}

#endif

#endif

