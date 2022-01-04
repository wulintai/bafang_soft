#ifndef _MTR_LEARNRS_C_
#define _MTR_LEARNRS_C_

#if 0

#include "include_c.h"

void diagMotorRsLearningAct_in(void)
{

	diagRsStr.RsLearning_Speed_fbk = sysFbkPara.Speed_fbk_HMI_abs;
	diagRsStr.RsLearning_ud_fbk_sum = voltLoopPara.Vd_ref_Filter;
	diagRsStr.RsLearning_Id_fbk_sum = sysFbkPara.Id_fbk_filter;
}

void diagMotorRsLearningAct_ctrl(DIAG_RS_STRUCT_PTR v)
{
	if(0 == v->RsLearning_finshflag)
	{
		switch(v->RsLearning_step)
		{
			case 0://3?¨º??¡¥
				if(_IQ15mpy(v->RsLearning_Speed_fbk,v->RsLearning_speedbase)>50)
				{
					v->IGBT_run_enable = 0;
				}
				else
				{
					v->RsLearning_cnt = 0;
					v->RsLearning_err = 0;
					v->RsLearning_Id_fbk_point1 = 0;
					v->RsLearning_ud_fbk_point1 = 0;
					v->RsLearning_Id_fbk_point2 = 0;
					v->RsLearning_ud_fbk_point2 = 0;
					v->IGBT_run_enable = 1;
					v->RsLearning_runloop = MTR_CURRENT_LOOP;
					v->RsLearning_Id_cmd = 0;
					v->RsLearning_step++;
				}
				break;
			case 1:
				v->RsLearning_cnt++;
				if(v->RsLearning_cnt<1000)
				{
					v->RsLearning_Id_cmd = v->RsLearning_Id_set;//d?¨¢¦Ì?¨¢¡Â
					v->RsLearning_Id_fbk_point1 = v->RsLearning_Id_fbk_sum;
					v->RsLearning_ud_fbk_point1 = v->RsLearning_ud_fbk_sum;
				}
				else if(v->RsLearning_cnt<2000)
				{
					v->RsLearning_Id_cmd = 0;//d?¨¢¦Ì?¨¢¡Â
				}
				else
				{
					v->RsLearning_cnt = 0;
					v->RsLearning_step++;
				}
				if(_IQ15mpy(v->RsLearning_Speed_fbk,v->RsLearning_speedbase)>50)
				{
					v->IGBT_run_enable = 0;
					v->RsLearning_err = 1;
					v->RsLearning_finshflag = 1;
				}
				break;
			case 2:
				v->RsLearning_cnt++;
				if(v->RsLearning_cnt<1000)
				{
					v->RsLearning_Id_cmd = 2*v->RsLearning_Id_set;//d?¨¢¦Ì?¨¢¡Â
					v->RsLearning_Id_fbk_point2 = v->RsLearning_Id_fbk_sum;
					v->RsLearning_ud_fbk_point2 = v->RsLearning_ud_fbk_sum;
				}
				else if(v->RsLearning_cnt<2000)
				{
					v->RsLearning_Id_cmd = 0;//d?¨¢¦Ì?¨¢¡Â
				}
				else
				{
					v->RsLearning_cnt = 0;
					v->RsLearning_step++;
				}
				if(_IQ15mpy(v->RsLearning_Speed_fbk,v->RsLearning_speedbase)>50)
				{
					v->IGBT_run_enable = 0;
					v->RsLearning_err = 1;
					v->RsLearning_finshflag = 1;
				}
				break;
			default:
				v->IGBT_run_enable = 0;
				v->RsLearning_finshflag = 1;
				break;
		}
	}
}

void diagMotorRsLearningAct_out(void)
{
	_iq deta_ud = 0;
	_iq deta_id = 0;
	
	sccw2.field.run_enable = diagRsStr.IGBT_run_enable;//?a1¨¹
	sccw2.field.runLoop_mode = diagRsStr.RsLearning_runloop;//¦Ì?¨¢¡Â?¡¤
	sysCfgPara.IdCmd_man = diagRsStr.RsLearning_Id_cmd;//IQ15
	sysCfgPara.IqCmd_man = 0;//IQ15
	if(1 == diagRsStr.RsLearning_finshflag)
	{
		if(1 == diagRsStr.RsLearning_err)
		{
			emsw2.field.DIAG_ENCODER_error = 1;//¡Á??¡ì?¡ã1¨º??
		}
		else
		{
			deta_ud = diagRsStr.RsLearning_ud_fbk_point2-diagRsStr.RsLearning_ud_fbk_point1;
			deta_id = diagRsStr.RsLearning_Id_fbk_point2-diagRsStr.RsLearning_Id_fbk_point1;
			if((deta_ud<=0)||(deta_id<=0))
			{
				emsw2.field.DIAG_ENCODER_error = 1;//¡Á??¡ì?¡ã1¨º??
			}
			else
			{
				diagRsStr.RsLearning_rs = _IQdiv(deta_ud,deta_id);
				EV_MCU_Para.field.Motor_study_Rs = epdw.field.Motor_study_Rs = diagRsStr.RsLearning_rs>>9;
				//Motor_StudyPara.field.Stator_Rs_HMI = EV_MCU_Para.field.Motor_study_Rs;
				diagRsStr.RsLearning_write_flag = 1;
			}
		}
		sccw2.field.diag_enable     		= 0;
		sccw2.field.diag_mode 				= DIAG_NON_TEST;
		diagRsStr.RsLearning_finshflag = 0;
	}
}

#endif

#endif

