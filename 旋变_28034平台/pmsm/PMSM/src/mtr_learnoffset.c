/*
 * encoffsettune.c
 *
 *  Created on: 2017-4-17
 *      Author: ZGJ
 */

#ifndef _MTR_LEARNOFFSET_C
#define _MTR_LEARNOFFSET_C

#include "include_c.h"


void diagEncOffsetTuneAct_input()
{
	diagEncoffsetVar.Speed_fbk_abs    	    =  _IQabs(_IQ15mpy(sysFbkPara.Speed_fbk_HMI,SysBase.speed));
	diagEncoffsetVar.IsFbkFilter            =  sysFbkPara.Is_fbk_HMI_fiter;
	diagEncoffsetVar.Theta_fbk_Ele          =  sysFbkPara.Theta_fbk_Ele;
}


void diagEncOffsetTuneAct_output()
{
	sccw2.field.run_enable                      = diagEncoffsetVar.IGBT_run_enable;
	sccw2.field.theta_mode                      = diagEncoffsetVar.theta_mode;
	sysCfgPara.Theta_man.value                  = diagEncoffsetVar.Theta_man;
	sccw2.field.runLoop_mode                    = diagEncoffsetVar.runLoop_mode;
	sysCfgPara.VdCmd_man                        = diagEncoffsetVar.VdCmd_man;
	sysCfgPara.VqCmd_man                        = diagEncoffsetVar.VqCmd_man;
	EV_MCU_Para.field.Motor_Encoder_offset      = diagEncoffsetVar.Motor_Encoder_offset>>12;
	EV_MCU_Para.field.Motor_Encoder_Direction   = diagEncoffsetVar.Encoder_direction;
	//Motor_StudyPara.field.Encoder_Offset_HMI          = EV_MCU_Para.field.Motor_Encoder_offset;
	//Motor_StudyPara.field.Encoder_Dir                 = EV_MCU_Para.field.Motor_Encoder_Direction;
	if(1 == diagEncoffsetVar.TuneFlag)
	{
		if(diagEncoffsetVar.DIAG_ENCODER_error>0)
		{
			emsw2.field.DIAG_ENCODER_error = 1;
		}
		else
		{
			EV_MCU_Para.field.Motor_Encoder_Direction	= epdw.field.Motor_Encoder_Direction = diagEncoffsetVar.Encoder_direction;
			EV_MCU_Para.field.Motor_Encoder_offset      = epdw.field.Motor_Encoder_offset    = diagEncoffsetVar.Motor_Encoder_offset>>12;
			//Motor_StudyPara.field.Encoder_Offset_HMI          = EV_MCU_Para.field.Motor_Encoder_offset;
			//Motor_StudyPara.field.Encoder_Dir                 = EV_MCU_Para.field.Motor_Encoder_Direction;
			diagEncoffsetVar.write_offset_flag          = 1;
			diagEncoffsetVar.finshflag                  = 1;
		}
		sccw2.field.diag_enable     		= 0;
		sccw2.field.diag_mode 				= DIAG_NON_TEST;
	}
}

void diagEncOffsetTuneAct_ctrl(tag_diagEncOffset *v)
{
	if (v->TuneFlag == 0)
	{
		switch (v->encTuneStep)
		{
			case 0:                                                                //初始化
				if(v->Speed_fbk_abs>10)
				{
					v->IGBT_run_enable          = 0;
				}
				else
				{
					v->IGBT_run_enable          = 1;
					v->runLoop_mode		        = MTR_VOLT_LOOP;
					v->theta_mode			    = THETA_MODE_MANUAL;
					v->Motor_Encoder_offset	    = 0;
					v->Theta_man           	    = 0;
					v->encTuneStep              = 1;
					v->keepflag		            = 0;
					v->theta_Ud				    = 0;
					v->theta_Uq				    = 0;
					v->VdCmd_man			    = 0;
					v->VqCmd_man			    = 0;
					v->delay_cnt                = 0;
					v->Encoder_direction        = MOTOR_COUNTER_CLOCKWISE;
				}
				break;
			case 1:																	//Vq    确定相序
				if(0 == v->keepflag)
				{
					if(v->IsFbkFilter <	v->Ismax)
					{
						if(v->VqCmd_man < v->volmax)
						{
							v->VqCmd_man+=v->volstep;
						}
						else
						{
							v->VqCmd_man = v->volmax;
							v->keepflag = 1;
						}
					}
					else
					{
						v->keepflag = 1;
					}
				}
				else
				{
					v->delay_cnt++;
					if(v->delay_cnt<v->delay_period)
					{
						v->theta_Uq = v->Theta_fbk_Ele;
					}
					else if(v->delay_cnt< v->quitcyle_period)
					{
						v->VqCmd_man = 0 ;
					}
					else
					{
						v->encTuneStep		= 2;
						v->delay_cnt	        = 0;
						v->keepflag           = 0;
					}
				}
				if(v->IsFbkFilter>v->Isstop)
				{
					if(v->VqCmd_man>0)
					{
						v->VqCmd_man-=v->volstep;
					}
					else
					{
						v->DIAG_ENCODER_error   = 1;
						v->IGBT_run_enable      = 0;
						v->TuneFlag	= 1;
					}
				}
				break;

			case 2:																	//Vd    确定相序
				v->VqCmd_man = 0;
				if(v->keepflag			== 0)
				{
					if(v->IsFbkFilter	<	v->Ismax)
					{
						if(v->VdCmd_man < v->volmax)
						{
							v->VdCmd_man += v->volstep;
						}
						else
						{
							v->VdCmd_man = v->volmax;
							v->keepflag = 1;
						}
					}
					else
					{
						v->keepflag = 1;
					}
				}
				else
				{
					v->delay_cnt++;
					if(v->delay_cnt<v->delay_period)
					{
						v->theta_Ud = v->Theta_fbk_Ele;
					}
					else if(v->delay_cnt<v->quitcyle_period)
					{
						v->VdCmd_man = 0 ;
					}
					else
					{
						v->encTuneStep		= 3;
						v->delay_cnt	        = 0;
						v->keepflag           = 0;
					}
				}
				if(v->IsFbkFilter>v->Isstop)
				{
					if(v->VdCmd_man>0)
					{
						v->VdCmd_man-=v->volstep;
					}
					else
					{
						v->DIAG_ENCODER_error = 1;
						v->IGBT_run_enable    = 0;
						v->TuneFlag	        = 1;
					}
				}
				break;

			case 3:																	//Vq     确定相序
				v->VdCmd_man = 0;
				if(v->keepflag			== 0)
				{
					if(v->IsFbkFilter	<	v->Ismax)
					{
						if(v->VqCmd_man < v->volmax)
						{
							v->VqCmd_man+= v->volstep;
						}
						else
						{
							v->VqCmd_man = v->volmax;
							v->keepflag = 1;
						}
					}
					else
					{
						v->keepflag = 1;
					}
				}
				else
				{
					v->delay_cnt++;
					if(v->delay_cnt<v->delay_period)
					{
						v->theta_Uq = v->Theta_fbk_Ele;
					}
					else if(v->delay_cnt<v->quitcyle_period)
					{
						v->VqCmd_man = 0 ;
					}
					else
					{
						v->encTuneStep		= 4;
						v->delay_cnt	= 0;
						v->keepflag = 0;
					}
				}
				if(v->IsFbkFilter>v->Isstop)
				{
					if(v->VqCmd_man>0)
					{
						v->VqCmd_man-=v->volstep;
					}
				}
				break;

			case 4:                                                                 //确定相序
				if(v->theta_Uq>v->theta_Ud)
				{
					v->thetatemp=v->theta_Uq-v->theta_Ud;
				}
				else
				{
					v->thetatemp=v->theta_Uq-v->theta_Ud+_IQ(1.0);
				}

				if((v->thetatemp>_IQ(0.125))&&(v->thetatemp<_IQ(0.4)))
				{
					v->Encoder_direction = MOTOR_COUNTER_CLOCKWISE;
				}
				else if((v->thetatemp>_IQ(0.6))&&(v->thetatemp<_IQ(0.875)))
				{
					v->Encoder_direction = MOTOR_CLOCKWISE;
				}
				else
				{
					v->DIAG_ENCODER_error = 2;
					v->IGBT_run_enable    = 0;
					v->TuneFlag	        = 1;
				}
				v->VdCmd_man		= 0;
				v->VqCmd_man		= 0;
				v->encTuneStep	= 5;
				v->delay_cnt		= 0;
				v->keepflag 		= 0;
				break;
			case 5:                                                              //重新Vq  ，确定旋变角度
				if(v->keepflag == 0)
				{
					if(v->IsFbkFilter	<	v->Ismax)
					{
						if(v->VqCmd_man < v->volmax)
						{
							v->VqCmd_man+= v->volstep;
						}
						else
						{
							v->VqCmd_man = v->volmax;
							v->keepflag = 1;
						}
					}
					else
					{
						v->keepflag = 1;
					}
				}
				else
				{
					v->delay_cnt++;
					if(v->delay_cnt<v->delay_period)
					{
						v->theta_Uq = v->Theta_fbk_Ele;
					}
					else if(v->delay_cnt<v->quitcyle_period)
					{
						v->VqCmd_man = 0 ;
					}
					else
					{
						v->encTuneStep		= 6;
						v->delay_cnt	= 0;
						v->keepflag = 0;
					}
				}
				if(v->IsFbkFilter>v->Isstop)
				{
					if(v->VqCmd_man>0)
					{
						v->VqCmd_man-=v->volstep;
					}
				}
				break;
			case 6:																	//Vd   确定旋变角度
				v->VqCmd_man = 0;
				if(v->keepflag			== 0)
				{
					if(v->IsFbkFilter	<	v->Ismax)
					{
						if(v->VdCmd_man < v->volmax)
						{
							v->VdCmd_man += v->volstep;
						}
						else
						{
							v->VdCmd_man = v->volmax;
							v->keepflag = 1;
						}
					}
					else
					{
						v->keepflag = 1;
					}
				}
				else
				{
					v->delay_cnt++;
					if(v->delay_cnt<v->delay_period)
					{
						v->theta_Ud = v->Theta_fbk_Ele;
					}
					else if(v->delay_cnt<v->quitcyle_period)
					{
						v->VdCmd_man = 0 ;
					}
					else
					{
						v->encTuneStep		= 7;
						v->delay_cnt	        = 0;
						v->keepflag           = 0;
						v->IGBT_run_enable	 	= 	0;
					}
				}
				if(v->IsFbkFilter>v->Isstop)
				{
					if(v->VdCmd_man>0)
					{
						v->VdCmd_man-=v->volstep;
					}
				}
				break;
			case 7:
				v->Motor_Encoder_offset	=	v->theta_Ud;
				v->IGBT_run_enable	 	= 	0;
				v->TuneFlag	            =   1;  //完成测试
				break;
			default:
				break;
		}
	}
}
#endif
