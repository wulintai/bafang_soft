/*
 * diagFSM.c
 *
 *  Created on: 2014-4-15
 *      Author: ThinkPad
 */
#ifndef _DIAGFSM_C
#define _DIAGFSM_C

#include "../include/include_c.h"

void diagFSMInit(void)
{
	int i = 0;

	for (i = 0; i < DIAGFSM_STATE_NUM; i++)
	{
		switch (i)
		{
			case DIAG_NON_TEST://
				diagFSMState[i].cond        = &diagNonTestCond;
				diagFSMState[i].action      = &diagNonTestAct;
				diagFSMState[i].entrance    = &diagNonTestEntrance;
				diagFSMState[i].export      = &diagNonTestExport;
				break;
			case DIAG_HALL_OFFSET://????????¡ä??D?¡Â¡Á??¡ì?¡ã-¡À¡ê¨¢?
				diagFSMState[i].cond        = &diagNonTestCond;
				diagFSMState[i].action      = &diagNonTestAct;
				diagFSMState[i].entrance    = &diagNonTestEntrance;
				diagFSMState[i].export      = &diagNonTestExport;
				break;
			case DIAG_ENCODER_OFFSET: //Dy¡À?¡Á??¡ì?¡ã
				diagFSMState[i].cond		= &diagEncOffsetTuneCond;
				diagFSMState[i].action		= &diagEncOffsetTuneAct;
				diagFSMState[i].entrance	= &diagEncOffsetTuneEntrance;
				diagFSMState[i].export		= &diagEncOffsetTuneExport;
				break;
			case DIAG_ABZ_OFFSET://??¨¢?¨º?¡À¨¤???¡Â¡Á??¡ì?¡ã-¡À¡ê¨¢?
				diagFSMState[i].cond        = &diagNonTestCond;
				diagFSMState[i].action      = &diagNonTestAct;
				diagFSMState[i].entrance    = &diagNonTestEntrance;
				diagFSMState[i].export      = &diagNonTestExport;
				break;
			case DIAG_MOTOR_INIT_POSITION://¡Áa¡Á¨®3?¨º??????¡ì?¡ã
				diagFSMState[i].cond		= &diagMotorPositionLearningCond;
				diagFSMState[i].action		= &diagMotorPositionLearningAct;
				diagFSMState[i].entrance	= &diagMotorPositionLearningEntrance;
				diagFSMState[i].export		= &diagMotorPositionLearningExport;
				break;
			case DIAG_MOTOR_FLUEX://?¡ì?¡ã¦Ì??¨²¡ä?¨¢¡ä
				diagFSMState[i].cond		= &diagMotorFluexLearningCond;
				diagFSMState[i].action		= &diagMotorFluexLearningAct;
				diagFSMState[i].entrance	= &diagMotorFluexLearningEntrance;
				diagFSMState[i].export		= &diagMotorFluexLearningExport;
				break;
			case DIAG_MOTOR_RS_LEARNING: //?¡ì?¡ã¦Ì??¨²¦Ì?¡Á¨¨Rs
				diagFSMState[i].cond		= &diagMotorRsLearningCond;
				diagFSMState[i].action		= &diagMotorRsLearningAct;
				diagFSMState[i].entrance	= &diagMotorRsLearningEntrance;
				diagFSMState[i].export		= &diagMotorRsLearningExport;
				break;
			case DIAG_MOTOR_Ld_LEARNING: //?¡ì?¡ã¦Ì??¨²?¡À?¨¢¦Ì??DLd
				diagFSMState[i].cond		= &diagMotorLdLearningCond;
				diagFSMState[i].action		= &diagMotorLdLearningAct;
				diagFSMState[i].entrance	= &diagMotorLdLearningEntrance;
				diagFSMState[i].export		= &diagMotorLdLearningExport;
				break;
			case DIAG_MOTOR_Lq_LEARNING: //?¡ì?¡ã¦Ì??¨²???¨¢¦Ì??DLq
				diagFSMState[i].cond		= &diagMotorLqLearningCond;
				diagFSMState[i].action		= &diagMotorLqLearningAct;
				diagFSMState[i].entrance	= &diagMotorLqLearningEntrance;
				diagFSMState[i].export		= &diagMotorLqLearningExport;
				break;
			case DIAG_ENCODER_EXACT_OFFSET: //??2a¡Á??¡ì?¡ã
				diagFSMState[i].cond		= &diagEncOffsetexactTuneCond;
				diagFSMState[i].action		= &diagEncOffsetexactTuneAct;
				diagFSMState[i].entrance	= &diagEncOffsetexactTuneEntrance;
				diagFSMState[i].export		= &diagEncOffsetexactTuneExport;
				break;
			default://¡À¡ê¨¢?
				diagFSMState[i].cond        = &diagNonTestCond;
				diagFSMState[i].action      = &diagNonTestAct;
				diagFSMState[i].entrance    = &diagNonTestEntrance;
				diagFSMState[i].export      = &diagNonTestExport;
				break;
		}
	}
	diagState = DIAG_NON_TEST;
	scsw2.field.diag_state = DIAG_NON_TEST;
}

void diagFSMCtrl(void)
{
	int i = 0;

	if ((*diagFSMState[diagState].cond)())
	{
		(*diagFSMState[diagState].action)();
	}
	else
	{
		for (i = 0; i < DIAGFSM_STATE_NUM; i++)
		{
			if ((*diagFSMState[i].cond)())
			{
				(*diagFSMState[diagState].export)();	//¨ª?3?¦Ì¡À?¡ã????¡Á¡ä¨¬?
				(*diagFSMState[i].entrance)();			//??¨¨?D?¦Ì?????¡Á¡ä¨¬?
				//(*diagFSMState[i].action)();				//¨¢¡é?¡ä?¡äDDD?¦Ì?????¡Á¡ä¨¬?
				break;
			}
		}
	}
}

//========================================================================================
// 2?????¡Á¡ä¨¬?
//========================================================================================
int diagNonTestCond(void)
{
	if(DIAG_NON_TEST == sccw2.field.diag_mode)
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

void diagNonTestAct(void)
{
}

void diagNonTestEntrance(void)
{
	diagState = DIAG_NON_TEST;
	scsw2.field.diag_state = DIAG_NON_TEST;
}

void diagNonTestExport(void)
{
	sccw2.field.run_enable      	= 0;
	ClearManualCtrlCmd();
}

//========================================================================================
//¡Á??¡ì?¡ã
//========================================================================================
int diagEncOffsetTuneCond(void)
{
	if (sccw2.field.diag_mode == DIAG_ENCODER_OFFSET)
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

void diagEncOffsetTuneAct(void)
{
	diagEncOffsetTuneAct_input();
	diagEncOffsetTuneAct_ctrl(&diagEncoffsetVar);
	diagEncOffsetTuneAct_output();
}

void diagEncOffsetTuneEntrance(void)
{
	diagState						       = DIAG_ENCODER_OFFSET;
	scsw2.field.diag_state			       = DIAG_ENCODER_OFFSET;
	diagEncoffsetVar.Ismax                 = EV_MCU_Para.field.Motor_study_MaxCrt;
	diagEncoffsetVar.Isstop                = diagEncoffsetVar.Ismax+_IQ15div(10,SysBase.iphase);

	diagEncoffsetVar.volmax		          = EV_MCU_Para.field.Motor_study_MaxVol;
	diagEncoffsetVar.volstep			  = 1;

	diagEncoffsetVar.delay_period         = 2000;
	diagEncoffsetVar.delay_cnt		      = 0;
	diagEncoffsetVar.quitcyle_period      = 4000;
	diagEncoffsetVar.TuneFlag	          = 0;
	diagEncoffsetVar.encTuneStep          = 0;
	diagEncoffsetVar.finshflag            = 0;
	diagEncoffsetVar.DIAG_ENCODER_error   = 0;
	diagEncoffsetVar.write_offset_flag    = 0;
}

void diagEncOffsetTuneExport(void)
{
	sccw2.field.run_enable					= 0;
	ClearManualCtrlCmd();
}


int diagEncOffsetexactTuneCond(void)
{
	if ((DIAG_ENCODER_EXACT_OFFSET == sccw2.field.diag_mode)
	/*&& (1 == diagEncoffsetVar.finshflag)*/)
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

void diagEncOffsetexactTuneEntrance(void)
{
	diagState						= DIAG_ENCODER_EXACT_OFFSET;
	scsw2.field.diag_state			= DIAG_ENCODER_EXACT_OFFSET;
	EV_MCU_Para.field.Weak_enable   = 0;
	EV_MCU_Para.field.TrqCmd_SourceType = TRQCMD_SOURCE_ECANB;
	EV_MCU_Para.field.SpdCmd_SourceType = TRQCMD_SOURCE_ECANB;
	EV_MCU_Para.field.VoltFeedfwd_Coeff = 0;

	exactTune.speed_keep_cnt        = 0;  
	exactTune.speed_cnt             = 0;
	exactTune.positive_offset       = 0;  
	exactTune.negative_offset       = 0;  
	exactTune.deta_offset           = 0;
	exactTune.write_exactune_flag   = 0;
	exactTune.Motor_Direction       = EV_MCU_Para.field.Motor_Direction;
	exactTune.Study_EncoderOffset_is = EV_MCU_Para.field.Study_EncoderOffset_is;
	exactTune.Study_EncoderOffset_speed = EV_MCU_Para.field.Study_EncoderOffset_speed;
	exactTune.finsh_flag            = 0;
	exactTune.DIAG_ENCODER_error    = 0;

	exactTune.Ismax                 = EV_MCU_Para.field.Motor_study_MaxCrt;//IQ15
	exactTune.Isstop                = exactTune.Ismax+_IQ15div(10,SysBase.iphase);//IQ15
	exactTune.volmax		        = EV_MCU_Para.field.Motor_study_MaxVol;//IQ15
	exactTune.volstep			    = 1;
	exactTune.delay_period          = 500;
	exactTune.delay_cnt		        = 0;
	exactTune.quitcyle_period       = 1000;

	exactTune.run_step              = 0;
    exactTune.Motor_Encoder_offset  =   epdw.field.Motor_Encoder_offset;
}

void diagEncOffsetexactTuneAct(void)
{
	diagexactEncOffsetTuneAct_input();
	diagexactEncOffsetTuneAct_ctrl(&exactTune);
	diagexactEncOffsetTuneAct_output();
}

void diagEncOffsetexactTuneExport(void)
{
	sccw2.field.run_enable					           = 0;
	//EV_MCU_Para.field.Motor_study_FM                   = _IQ15div(exactTune.EMF_600RPM,(EV_MCU_Para.field.Motor_Np*_IQ15(0.18128267)));
	//0.18128267=2*pi*1.732/60
}

//¦Ì??¨²RS?¡ì?¡ã
int diagMotorRsLearningCond(void)
{
	//¦Ì??¨²¦Ì?¡Á¨¨¡Á??¡ì?¡ã¡ê?¦Ì?¡Á¨¨?¡ì?¡ã¨º??¨¤¨¢¡é¦Ì?¡ê?2?¨º??¡§¨¢¡é?¨²?????¡ì?¡ã¦Ì??¨´¡ä?¨¦?¡ê?¨¬¨ª?¨®??¨ªa¨¬??t?D?????¨¨???¡ì?¡ã¨ª¨º3¨¦¡À¨º??
	if(DIAG_MOTOR_RS_LEARNING == sccw2.field.diag_mode)
	{
		return 1;
	}
	else
	{
		return 0;
	}

}

void diagMotorRsLearningEntrance(void)
{
#if 0
	//Rs?¡ì?¡ã¨ª¨º3¨¦¡À¨º????¨¢?
	diagRsStr.RsLearning_finshflag 		                 = 	0;
	diagState											 = 	DIAG_MOTOR_RS_LEARNING;
	scsw2.field.diag_state						         = 	DIAG_MOTOR_RS_LEARNING;
	EV_MCU_Para.field.Weak_enable                        =  0;//1?¦Ì?¨¨?¡ä?
	//??¨ºy?¡Â??¨¢?
	diagRsStr.RsLearning_cnt 				             = 	0;
	diagRsStr.RsLearning_Id_set                          =	_IQ15div(25,SysBase.iphase); //5A¦Ì?¨¢¡Â???¡§
	diagRsStr.RsLearning_Id_cmd                          =  0;
	diagRsStr.RsLearning_rs                              =  0;
	diagRsStr.RsLearning_speedbase                       = SysBase.speed;
	diagRsStr.RsLearning_Id_fbk_sum		                 =	0; 				// Id¡¤¡ä¨¤?¦Ì?¨¢¡Â?¦Ì
	diagRsStr.RsLearning_ud_fbk_sum	                     =	0; 				// ud¡¤¡ä¨¤?¦Ì??1?¦Ì
	diagRsStr.RsLearning_Id_fbk_point1                   =	0; 			// Id¡¤¡ä¨¤?¦Ì?¨¢¡Â?¦Ì1
	diagRsStr.RsLearning_ud_fbk_point1                   =	0; 			// ud¡¤¡ä¨¤?¦Ì??1?¦Ì1
	diagRsStr.RsLearning_Id_fbk_point2	                 =	0; 			// Id¡¤¡ä¨¤?¦Ì?¨¢¡Â?¦Ì2
	diagRsStr.RsLearning_ud_fbk_point2                   =	0; 			// ud¡¤¡ä¨¤?¦Ì??1?¦Ì2
	diagRsStr.RsLearning_err                             =  0;
	diagRsStr.RsLearning_write_flag                      =  0;
	diagRsStr.RsLearning_step                            =  0;
	diagRsStr.IGBT_run_enable                            =  0;
	diagRsStr.RsLearning_runloop                         =  MTR_CURRENT_LOOP;
	EV_MCU_Para.field.Motor_study_Rs	                 =	_IQ15(0.1);//?3?¦Ì??¨¨?¡Á¨¨?¦Ì
	EV_MCU_Para.field.Weak_enable  = 0;
	EV_MCU_Para.field.TrqCmd_SourceType = TRQCMD_SOURCE_ECANB;
	EV_MCU_Para.field.SpdCmd_SourceType = TRQCMD_SOURCE_ECANB;
#endif
}

void diagMotorRsLearningAct(void)
{
#if 0
	diagMotorRsLearningAct_in();
	diagMotorRsLearningAct_ctrl(&diagRsStr);
	diagMotorRsLearningAct_out();
#else
	sccw2.field.diag_enable     		= 0;
	sccw2.field.diag_mode 				= DIAG_NON_TEST;
#endif
}

void diagMotorRsLearningExport(void)
{	//¨ª¡ê?1??DD
	sccw2.field.run_enable					    = 	 0;
	ClearManualCtrlCmd();
}
//=====================================================================//


//=====================================================================//
int diagMotorLdLearningCond(void)
{
	if (DIAG_MOTOR_Ld_LEARNING == sccw2.field.diag_mode)
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

void diagMotorLdLearningEntrance(void)
{
#if 0
	diagState							   = DIAG_MOTOR_Ld_LEARNING;
	scsw2.field.diag_state				   = DIAG_MOTOR_Ld_LEARNING;
	diagEncoffsetVar.finshflag			   = 0;
	high_fre_str.finshflag                 = 0;
	high_fre_str.high_fre_get_nspole_flag  = 0;
	high_fre_str.diag_initangle_finishflag = 0;
	high_fre_str.is_id_positive_current_filter		   = 0;
	high_fre_str.is_iq_positive_current_filter		   = 0;
	high_fre_str.alpha_current_filter				   = 0;
	high_fre_str.beta_current_filter 				   = 0;
	high_fre_str.high_fre_id_lp_filter	   = 0;
	high_fre_str.high_fre_iq_lp_filter	   = 0;
	high_fre_str.Ui                        = 0;
	high_fre_str.Up                        = 0;
	diagEncoffsetVar.write_dq_flag         = 0;
	highFrequency_clear();
#endif
}

void diagMotorLdLearningAct(void)
{
	diagMotorInitPositionLearningAct_input();
	diagMotorInitPositionLearningAct_ctrl();
	diagMotorInitPositionLearningAct_out();

}

void diagMotorLdLearningExport(void)
{
	sccw2.field.run_enable					= 0;
	ClearManualCtrlCmd();
}
//=====================================================================//

//=====================================================================//
int diagMotorLqLearningCond(void)
{
	if (DIAG_MOTOR_Lq_LEARNING == sccw2.field.diag_mode)
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

void diagMotorLqLearningEntrance(void)
{
#if 0
	diagState						       = DIAG_MOTOR_Lq_LEARNING;
	scsw2.field.diag_state			       = DIAG_MOTOR_Lq_LEARNING;
	diagEncoffsetVar.finshflag             = 0;
	high_fre_str.finshflag                 = 0;
	high_fre_str.high_fre_get_nspole_flag  = 0;
	high_fre_str.diag_initangle_finishflag = 0;
	high_fre_str.is_id_positive_current_filter          = 0;
	high_fre_str.is_iq_positive_current_filter          = 0;
	high_fre_str.alpha_current_filter                   = 0;
	high_fre_str.beta_current_filter                    = 0;
	high_fre_str.high_fre_id_lp_filter     = 0;
	high_fre_str.high_fre_iq_lp_filter     = 0;
	high_fre_str.Ui                        = 0;
	high_fre_str.Up                        = 0;
	diagEncoffsetVar.write_dq_flag         = 0;
	highFrequency_clear();
#endif
}

void diagMotorLqLearningAct(void)
{
	diagMotorInitPositionLearningAct_input();
	diagMotorInitPositionLearningAct_ctrl();
	diagMotorInitPositionLearningAct_out();
}

void diagMotorLqLearningExport(void)
{
	sccw2.field.run_enable					= 0;
	ClearManualCtrlCmd();
}

//=====================================================================//
int diagMotorFluexLearningCond(void)
{
	if(DIAG_MOTOR_FLUEX == sccw2.field.diag_mode)
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

void diagMotorFluexLearningEntrance(void)
{
	diagState						       = DIAG_MOTOR_FLUEX;
	scsw2.field.diag_state			       = DIAG_MOTOR_FLUEX;
	/*diagFluexStr.diagFluex_finishflag      = 0;
	diagFluexStr.diagFluex_writeflag       = 0;
	diagFluexStr.diagFluex_cnt = 0;
	diagFluexStr.diagFluex_err = 0;
	diagFluexStr.diagFluex_fluex = 0;
	diagFluexStr.diagFluex_fluex_HMI = 0;
	diagFluexStr.diagFluex_is = 0;
	diagFluexStr.diagFluex_iscmd1 = EV_MCU_Para.field.Motor_studyFluex_is1;//IQ15
	diagFluexStr.diagFluex_iscmd2 = EV_MCU_Para.field.Motor_studyFluex_is2;//IQ15
	diagFluexStr.diagFluex_speedcmd = _IQ15toIQ(EV_MCU_Para.field.Motor_studyFluex_speed);//IQ24
	diagFluexStr.diagFluex_np = EV_MCU_Para.field.Motor_Np;
	diagFluexStr.diagFluex_runloop = MTR_SPEED_LOOP;
	diagFluexStr.diagFluex_speedbase = SysBase.speed;
	diagFluexStr.diagFluex_udcbase = SysBase.udc;
	diagFluexStr.diagFluex_step = 0;
	diagFluexStr.IGBT_run_enable = 0;
	diagFluexStr.diagFluex_keepflag = 0;
	diagFluexStr.diagFluex_motorDir = EV_MCU_Para.field.Motor_Direction;
	diagFluexStr.diagFluex_current_step = 1;//_IQtoIQ15(SysBase.inviphase);//IQ15
	EV_MCU_Para.field.Weak_enable  = 0;
	EV_MCU_Para.field.TrqCmd_SourceType = TRQCMD_SOURCE_ECANB;
	EV_MCU_Para.field.SpdCmd_SourceType = TRQCMD_SOURCE_ECANB;*/
}

void diagMotorFluexLearningAct(void)
{
#if 0
	diagMotorFluexLearningAct_input();
	diagMotorFluexLearningAct_ctrl(&diagFluexStr);
	diagMotorFluexLearningAct_out();
#else
	sccw2.field.diag_enable = 0;
	sccw2.field.diag_mode   = DIAG_NON_TEST;
#endif
}

void diagMotorFluexLearningExport(void)
{
	sccw2.field.run_enable					= 0;
	ClearManualCtrlCmd();
}

//=====================================================================//
int diagMotorPositionLearningCond(void)
{
	if (DIAG_MOTOR_INIT_POSITION == sccw2.field.diag_mode)
	{
		return 1;
	}
	else
	{
		return 0;
	}
}


void diagMotorPositionLearningEntrance(void)
{
#if 0
	diagState						       = DIAG_MOTOR_INIT_POSITION;
	scsw2.field.diag_state			       = DIAG_MOTOR_INIT_POSITION;
	diagEncoffsetVar.finshflag             = 0;
	high_fre_str.finshflag                 = 0;
	high_fre_str.high_fre_get_nspole_flag  = 0;
	high_fre_str.diag_initangle_finishflag = 0;
	high_fre_str.is_id_positive_current_filter          = 0;
	high_fre_str.is_iq_positive_current_filter          = 0;
	high_fre_str.alpha_current_filter                   = 0;
	high_fre_str.beta_current_filter                    = 0;
	high_fre_str.high_fre_id_lp_filter     = 0;
	high_fre_str.high_fre_iq_lp_filter     = 0;
	high_fre_str.Ui                        = 0;
	high_fre_str.Up                        = 0;
	diagEncoffsetVar.write_dq_flag         = 0;
	//high_fre_str.diagIsAlphaBpf = 0;
	//high_fre_str.diagIsBetaBpf = 0;
	highFrequency_clear();
#endif
}

void diagMotorPositionLearningAct(void)
{
	diagMotorInitPositionLearningAct_input();
	diagMotorInitPositionLearningAct_ctrl();
	diagMotorInitPositionLearningAct_out();
}

void diagMotorPositionLearningExport(void)
{
	sccw2.field.run_enable					= 0;
	ClearManualCtrlCmd();
}


#endif




