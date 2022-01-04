#ifndef _MTR_LEARNFLUEX_C_
#define _MTR_LEARNFLUEX_C_

#if 0

#include "include_c.h"

void diagMotorFluexLearningAct_input(void)
{
	diagFluexStr.diagFluex_speed_fbk = sysFbkPara.Speed_fbk_Filter;//IQ24
	diagFluexStr.diagFluex_id_fbk    = spdLoopPara.Idcmd_real;//IQ24
	//diagFluexStr.diagFluex_vd_fbk = _IQdiv(voltLoopPara.Vd_ref,SQRT3);
	diagFluexStr.diagFluex_vq_fbk = _IQdiv(voltLoopPara.Vq_ref,SQRT3);//IQ24
	//diagFluexStr.diagFluex_vdq_fbk = _IQdiv(voltLoopPara.Vdq,SQRT3);
}

void diagMotorFluexLearningAct_ctrl(DIAG_FLUEX_STRUCT_PTR v)
{
	//_iq tmp1 = 0;
	//_iq tmp2 = 0;
	//_iq fluex = 0;

	if(0 == v->diagFluex_finishflag)
	{
		switch(v->diagFluex_step)
		{
			case 0://3?????
				v->IGBT_run_enable = 1;//?a1?
				v->diagFluex_runloop = MTR_SPEED_LOOP;//??αχ??
				v->diagFluex_is = 0;//??αχ
				v->diagFluex_speed = 0;
				v->diagFluex_cnt = 0;//??κy
				v->diagFluex_err = 0;//'???
				v->diagFluex_keepflag = 0;
				v->diagFluex_basevalue = _IQdiv(v->diagFluex_udcbase,v->diagFluex_speedbase);//????
				v->diagFluex_step++;//2???
				break;
			case 1:
				if(0 == v->diagFluex_keepflag)
				{
					if(1 == v->diagFluex_motorDir)
					{
						v->diagFluex_is = v->diagFluex_is-v->diagFluex_current_step;
						if(v->diagFluex_is<(-v->diagFluex_iscmd1))
						{
							v->diagFluex_is = 0-v->diagFluex_iscmd1;//IQ15
							v->diagFluex_keepflag = 1;
						}
						v->diagFluex_speed = 0-v->diagFluex_speedcmd;//IQ24
					}
					else
					{
						v->diagFluex_is = v->diagFluex_is+v->diagFluex_current_step;
						if(v->diagFluex_is>v->diagFluex_iscmd1)
						{
							v->diagFluex_is = v->diagFluex_iscmd1;//IQ15
							v->diagFluex_keepflag = 1;
						}
						v->diagFluex_speed = v->diagFluex_speedcmd;//IQ24
					}
				}
				else
				{
					if(_IQabs(_IQmpy(v->diagFluex_speed,v->diagFluex_speedbase)-_IQmpy(v->diagFluex_speed_fbk,v->diagFluex_speedbase))<50)//IQ15
					{
						v->diagFluex_keep_cnt++;
						if(v->diagFluex_keep_cnt>=2500)
						{
							v->diagFluex_keep_cnt = 0;
							v->diagFluex_cnt = 0;
							v->diagFluex_vq1 = v->diagFluex_vq_fbk;
							v->diagFluex_id1 = v->diagFluex_id_fbk;
							v->diagFluex_keepflag = 0;
							v->diagFluex_step++;
						}
					}
					else
					{
						v->diagFluex_keep_cnt = 0;
					}
					v->diagFluex_cnt++;
					if(v->diagFluex_cnt>5000)
					{
						v->diagFluex_cnt = 0;
						v->IGBT_run_enable = 0;
						v->diagFluex_err = 1;
						v->diagFluex_finishflag = 1;
					}
				}
				break;
			case 2:
				v->diagFluex_is = 0;//IQ15
				v->diagFluex_speed = 0;//IQ24
				if(_IQabs(_IQmpy(v->diagFluex_speed_fbk,v->diagFluex_speedbase))<100)
				{
					v->diagFluex_keep_cnt = 0;
					v->diagFluex_cnt = 0;
					v->diagFluex_step++;
				}
				v->diagFluex_cnt++;
				if(v->diagFluex_cnt>50000)
				{
					v->diagFluex_cnt = 0;
					v->IGBT_run_enable = 0;
					v->diagFluex_err = 1;
					v->diagFluex_finishflag = 1;
				}
				break;
			case 3:
				if(0 == v->diagFluex_keepflag)
				{
					if(1 == v->diagFluex_motorDir)
					{
						v->diagFluex_is = v->diagFluex_is-v->diagFluex_current_step;
						if(v->diagFluex_is<(-v->diagFluex_iscmd2))
						{
							v->diagFluex_is = 0-v->diagFluex_iscmd2;//IQ15
							v->diagFluex_keepflag = 1;
						}
						v->diagFluex_speed = 0-v->diagFluex_speedcmd;//IQ24
					}
					else
					{
						v->diagFluex_is = v->diagFluex_is+v->diagFluex_current_step;
						if(v->diagFluex_is>v->diagFluex_iscmd2)
						{
							v->diagFluex_is = v->diagFluex_iscmd2;//IQ15
							v->diagFluex_keepflag = 1;
						}
						v->diagFluex_speed = v->diagFluex_speedcmd;//IQ24
					}
				}
				else
				{
					if(_IQabs(_IQmpy(v->diagFluex_speed,v->diagFluex_speedbase)-_IQmpy(v->diagFluex_speed_fbk,v->diagFluex_speedbase))<50)//IQ15
					{
						v->diagFluex_keep_cnt++;
						if(v->diagFluex_keep_cnt>=2500)
						{
							v->diagFluex_keep_cnt = 0;
							v->diagFluex_cnt = 0;
							v->diagFluex_vq2 = v->diagFluex_vq_fbk;
							v->diagFluex_id2 = v->diagFluex_id_fbk;
							v->diagFluex_keepflag = 0;
							v->diagFluex_step++;
						}
					}
					else
					{
						v->diagFluex_keep_cnt = 0;
					}
					v->diagFluex_cnt++;
					if(v->diagFluex_cnt>5000)
					{
						v->diagFluex_cnt = 0;
						v->IGBT_run_enable = 0;
						v->diagFluex_err = 1;
						v->diagFluex_finishflag = 1;
					}
				}
				break;
			case 4:
				v->diagFluex_is = 0;//IQ15
				v->diagFluex_speed = 0;//IQ24
				if(_IQabs(_IQmpy(v->diagFluex_speed_fbk,v->diagFluex_speedbase))<100)
				{
					v->diagFluex_keep_cnt = 0;
					v->diagFluex_cnt = 0;
					v->diagFluex_step++;
				}
				v->diagFluex_cnt++;
				if(v->diagFluex_cnt>50000)
				{
					v->diagFluex_cnt = 0;
					v->IGBT_run_enable = 0;
					v->diagFluex_err = 1;
					v->diagFluex_finishflag = 1;
				}
				break;
			default:
				v->IGBT_run_enable = 0;
				v->diagFluex_finishflag = 1;
				break;
		}
	}
}

void diagMotorFluexLearningAct_out(void)
{
	_iq tmp1 = 0;
	_iq tmp2 = 0;
	_iq tmp  = 0;
	
	sccw2.field.run_enable                      = diagFluexStr.IGBT_run_enable;
	sccw2.field.runLoop_mode                    = diagFluexStr.diagFluex_runloop;
	sysCfgPara.SpdCmd                           = diagFluexStr.diagFluex_speed;//IQ24
	diagFluexStr.diagFluex_fluex_HMI            = _IQtoIQ15(diagFluexStr.diagFluex_fluex);//IQ15
	EV_MCU_Para.field.Motor_study_FM            = diagFluexStr.diagFluex_fluex_HMI;//IQ15
	//Motor_StudyPara.field.Rotor_Fluex_HMI       = EV_MCU_Para.field.Motor_study_FM;//IQ15
	if(1 == diagFluexStr.diagFluex_finishflag)
	{
		sccw2.field.diag_enable = 0;
		sccw2.field.diag_mode   = DIAG_NON_TEST;
		if(1 == diagFluexStr.diagFluex_err)
		{
			emsw2.field.DIAG_ENCODER_error = 1;
		}
		else
		{
			tmp1 = _IQmpy(diagFluexStr.diagFluex_vq1,diagFluexStr.diagFluex_id2)-_IQmpy(diagFluexStr.diagFluex_vq2,diagFluexStr.diagFluex_id1);//IQ24
			tmp  = _IQdiv(SysBase.speed,(60*SysBase.udc));//IQ24
			tmp2 = _IQmpy((diagFluexStr.diagFluex_speedcmd*EV_MCU_Para.field.Motor_Np),tmp);//IQ24
			tmp2 = _IQmpy(tmp2,2*PI);//IQ24
			tmp2 = _IQmpy(tmp2,(diagFluexStr.diagFluex_id2-diagFluexStr.diagFluex_id1));//IQ24
			diagFluexStr.diagFluex_fluex  = _IQdiv(tmp1,tmp2);//IQ24
			diagFluexStr.diagFluex_fluex_HMI = _IQtoIQ15(diagFluexStr.diagFluex_fluex);//IQ15
			EV_MCU_Para.field.Motor_study_FM = epdw.field.Motor_study_FM = diagFluexStr.diagFluex_fluex_HMI;//IQ15
			//Motor_StudyPara.field.Rotor_Fluex_HMI  = EV_MCU_Para.field.Motor_study_FM;//IQ15
			tmp = _IQdiv((1000*EV_MCU_Para.field.Motor_Np),(60*SysBase.udc));//IQ24
			tmp = _IQmpy(tmp,2*PI);//IQ24
			tmp = _IQmpy(tmp,diagFluexStr.diagFluex_fluex);//IQ24
			//tmp = _IQmpy(tmp,SQRT3);
			//tmp = _IQdiv(tmp,SQRT2);
			//Motor_StudyPara.field.Back_EMF_HMI = _IQtoIQ15(tmp);//IQ15
			diagFluexStr.diagFluex_writeflag = 1;
		}
	}
	else
	{
		//Motor_StudyPara.field.Back_EMF_HMI = 0;
	}
}
#endif


#endif
