/*
 * ctrlsysfuntion.c
 *
 *  Created on: 2014-11-14
 *      Author: ThinkPad
 */

#ifndef _SYSCTRLFUN_C_
#define _SYSCTRLFUN_C_

#include "include_c.h"

void errordisplay(void)
{
	/*Uint16 i = 0;
	Uint16 temp = 0;
	
	error_toltalvalue                   =alarm.wValue;
	error_toltalvalue                   =error_toltalvalue<<16;
	error_toltalvalue                   =(error_toltalvalue|emsw2.wValue)<<16;
	error_toltalvalue                   =(error_toltalvalue|emsw1.wValue)<<16;
	error_toltalvalue                   =error_toltalvalue|emsw.wValue;
	if(0 != error_toltalvalue)
	{
		for(i=0;i<64;i++)
		{
			temp =(error_toltalvalue>>i)&0x01;
			if(1 == temp)
			{
				error_display = i;
				if((error_display != error_display_old)
				 ||(ALARM_BUZZER_INIT_STATE == error_alarm_state))
				{
					error_display_old = error_display;
					if(error_display<10)
					{
						error_alarm_state = ALARM_BUZZER_SHORT_ALARM_STATE;
					}
					else
					{
						error_alarm_state = ALARM_BUZZER_LONG_ALARM_STATE;
					}
				}
				else
				{
					error_alarm_state = error_alarm_state;
					error_display_old = error_display;
				}
				break;
			}
		}
	}
	else
	{
		error_display = 0;
		error_display_old = 0;
		error_alarm_cnt = 0;
		error_alarm_flag = 0;
		error_alarm_state = ALARM_BUZZER_INIT_STATE;
	}
	switch(error_alarm_state)
	{
		case ALARM_BUZZER_INIT_STATE:
			error_alarm_cnt = 0;
			error_alarm_flag = 0;
			error_alarm_state = error_alarm_state;
			sub_io_buzzer(SUB_IO_BUZZER_OFF);
			break;
		case ALARM_BUZZER_SHORT_ALARM_STATE:
			error_alarm_cnt++;
			if(error_alarm_cnt>(2*(error_display%10)+2))
			{
				error_alarm_cnt = 0;
				error_alarm_flag = 0;
				error_alarm_state = ALARM_BUZZER_DELAY_STATE;
				sub_io_buzzer(SUB_IO_BUZZER_OFF);
			}
			else
			{
				error_alarm_cnt = error_alarm_cnt;
				error_alarm_state = error_alarm_state;
				if(0 == error_alarm_flag)
				{
					sub_io_buzzer(SUB_IO_BUZZER_OFF);
					error_alarm_flag = 1;
				}
				else
				{
					sub_io_buzzer(SUB_IO_BUZZER_ON);
					error_alarm_flag = 0;
				}
			}
			break;
		case ALARM_BUZZER_LONG_ALARM_STATE:
			error_alarm_cnt++;
			if(error_alarm_cnt>=3*(error_display/10))
			{
				error_alarm_cnt = 0;
				error_alarm_flag = 0;
				error_alarm_state = ALARM_BUZZER_SHORT_ALARM_STATE;
				sub_io_buzzer(SUB_IO_BUZZER_OFF);
			}
			else if(0 == error_alarm_cnt%3)
			{
				error_alarm_cnt = error_alarm_cnt;
				error_alarm_flag = error_alarm_flag;
				error_alarm_state = error_alarm_state;
				sub_io_buzzer(SUB_IO_BUZZER_OFF);
			}
			else
			{
				error_alarm_cnt = error_alarm_cnt;
				error_alarm_flag = 0;
				error_alarm_state = error_alarm_state;
				sub_io_buzzer(SUB_IO_BUZZER_ON);
			}
			break;
		case ALARM_BUZZER_DELAY_STATE:
			error_alarm_cnt++;
			if(error_alarm_cnt>=5)
			{
				error_alarm_cnt = 0;
				error_alarm_flag = 0;
				if(error_display<10)
				{
					error_alarm_state = ALARM_BUZZER_SHORT_ALARM_STATE;
				}
				else
				{
					error_alarm_state = ALARM_BUZZER_LONG_ALARM_STATE;
				}
			}
			else
			{
				error_alarm_cnt = error_alarm_cnt;
				error_alarm_flag = error_alarm_flag;
				error_alarm_state = error_alarm_state;
			}
			break;
		default:
			break;
	}*/
}

void zero_stationcond()             //驻坡功能
{
	_iq speed = 0;
	//_iq pre_trq_value = 0;

	speed = sysFbkPara.Speed;
	//pre_trq_value = _IQmpy(_IQ(0.5),SysBase.invtorque)-3000;
	
	if((1 == EV_MCU_Para.field.Zero_Sation_Enable)
	 &&(1 == scsw2.field.igbt_state))
	{
		if(0 == scsw2.field.zero_state)//进入驻坡
		{
			if((((1 == Vehicle_cmd.cmdmode.data.gear_state)&&(speed<-zero_speed_throad))
			  ||((2 == Vehicle_cmd.cmdmode.data.gear_state)&&(speed > zero_speed_throad)))
			 &&((0 == Vehicle_cmd.cmdmode.data.brake_state)&&(0 == alarm.field.gear_fault)&&(0 == Vehicle_cmd.torque_ref)))//进入驻坡
			{
				scsw2.field.zero_state = 1;
				VCU_IOstate.field.zero_disable_flag = 0;
				VCU_IOstate.field.zero_or_start_flag = 0;
			}
			else if((1 == Vehicle_cmd.cmdmode.data.gear_swap_trq_flag)
				  &&(0 == Vehicle_cmd.cmdmode.data.brake_state)
				  &&(0 != Vehicle_cmd.torque_ref)
				  &&(0 != Vehicle_cmd.cmdmode.data.gear_state)
				  /*&&(_IQabs(trqLoop_trqRampref)>=pre_trq_value)*/)//前后换档松刹车踩油门起步
			{
				scsw2.field.zero_state = 1;
				VCU_IOstate.field.zero_disable_flag = 0;
				VCU_IOstate.field.zero_or_start_flag = 1;
			}

			if(1 == Vehicle_cmd.cmdmode.data.gear_swap_zero_flag)
			{
				if((1 == scsw2.field.zero_state)
				 ||((1 == Vehicle_cmd.cmdmode.data.gear_state)&&(speed> 50))
				 ||((2 == Vehicle_cmd.cmdmode.data.gear_state)&&(speed<-50))
				 ||(0 == Vehicle_cmd.cmdmode.data.gear_state))
				{
					Vehicle_cmd.cmdmode.data.gear_swap_zero_flag = 0;
					zero_speed_throad = 15;
				}
			}

			if(1 == zero_station_trqCmd_flag)
			{
				if(((0 != Vehicle_cmd.cmdmode.data.gear_state) && (0 != Vehicle_cmd.torque_ref))
				 ||(1 == scsw2.field.zero_state))
				{
					zero_station_trqCmd_flag = 0;
					Vehicle_cmd.cmdmode.data.zero_station_free_flag = 0;
				}
			}
			if(0 != VCU_IOstate.field.exit_zero_add_trq_flag)
			{
				if((0 == Vehicle_cmd.cmdmode.data.gear_state)
				 ||(1 == Vehicle_cmd.cmdmode.data.brake_state)
				 ||(1 == scsw2.field.zero_state)
				 ||(0 == Vehicle_cmd.torque_ref))
				{
					VCU_IOstate.field.exit_zero_add_trq_flag = 0;
				}
			}
		}
		else//退出驻坡
		{
			if(0 == VCU_IOstate.field.zero_or_start_flag)
			{
				if(0 == VCU_IOstate.field.exit_zero_add_trq_flag)
				{
					if((0 != Vehicle_cmd.torque_ref)
					 &&(0 != Vehicle_cmd.cmdmode.data.gear_state))
					{
						VCU_IOstate.field.exit_zero_add_trq_flag = 1;
						zero_station_trqCmd_store = zero_station_trqCmd;
					}
				}
				else if(1 == VCU_IOstate.field.exit_zero_add_trq_flag)
				{
					if((0 == Vehicle_cmd.torque_ref)
					 &&(0 != Vehicle_cmd.cmdmode.data.gear_state))
					{
						VCU_IOstate.field.exit_zero_add_trq_flag = 0;
					}
				}
				if((MTR_TRQ_LOOP != sccw1.field.runLoop_mode)
				 ||(0 == Vehicle_cmd.cmdmode.data.gear_state)
				 ||(1 == Vehicle_cmd.cmdmode.data.brake_state)
				 ||((1 == Vehicle_cmd.cmdmode.data.gear_state) &&(((0 != Vehicle_cmd.torque_ref)&&(2 == VCU_IOstate.field.exit_zero_add_trq_flag))||(sysFbkPara.Speed_fbk_HMI>500)))
				 ||((2 == Vehicle_cmd.cmdmode.data.gear_state) &&(((0 != Vehicle_cmd.torque_ref)&&(2 == VCU_IOstate.field.exit_zero_add_trq_flag))||(sysFbkPara.Speed_fbk_HMI<-500)))
				 ||(1 == alarm.field.gear_fault))
				{
					scsw2.field.zero_state = 0;
					zero_station_trqCmd_flag = 1;
					if((MTR_TRQ_LOOP != sccw1.field.runLoop_mode)
					  ||(0 == Vehicle_cmd.cmdmode.data.gear_state)
					  ||(1 == Vehicle_cmd.cmdmode.data.brake_state)
					  ||(1 == alarm.field.gear_fault))
					{
						Vehicle_cmd.cmdmode.data.zero_station_free_flag = 0;
					}
					else if((Vehicle_cmd.cmdmode.data.gear_state==1 &&(((0 != Vehicle_cmd.torque_ref)&&(2 == VCU_IOstate.field.exit_zero_add_trq_flag))||(sysFbkPara.Speed_fbk_HMI>500)))
					      ||(Vehicle_cmd.cmdmode.data.gear_state==2 &&(((0 != Vehicle_cmd.torque_ref)&&(2 == VCU_IOstate.field.exit_zero_add_trq_flag))||(sysFbkPara.Speed_fbk_HMI<-500))))
					{
						if(((1 == Vehicle_cmd.cmdmode.data.gear_state) && (0 != Vehicle_cmd.torque_ref) && (2 == VCU_IOstate.field.exit_zero_add_trq_flag))
					     ||((2 == Vehicle_cmd.cmdmode.data.gear_state) && (0 != Vehicle_cmd.torque_ref) && (2 == VCU_IOstate.field.exit_zero_add_trq_flag)))
						{
							VCU_IOstate.field.exit_zero_add_trq_flag = 3;
							if((1 == Vehicle_cmd.cmdmode.data.gear_state)
							 &&(trqLoop_trqRampref<_IQmpyI32(2,SysBase.invtorque)))
							{
								Vehicle_cmd.cmdmode.data.zero_station_free_flag = 3;
							}
							else if((2 == Vehicle_cmd.cmdmode.data.gear_state)
								  &&(trqLoop_trqRampref>-_IQmpyI32(3,SysBase.invtorque)))
							{
								Vehicle_cmd.cmdmode.data.zero_station_free_flag = 3;
							}
							else
							{
								Vehicle_cmd.cmdmode.data.zero_station_free_flag = 0;
								zero_station_trqCmd_flag = 0;
								zero_station_trqCmd_store = zero_station_trqCmd = 0;
							}
							Vehicle_cmd.cmdmode.data.gear_swap_trq_flag = 0;
						}
						else
						{
							Vehicle_cmd.cmdmode.data.zero_station_free_flag = 2;
						}
					}
					else
					{
						Vehicle_cmd.cmdmode.data.zero_station_free_flag = 1;
					}
				}
			}
			else
			{
				if((0 == Vehicle_cmd.cmdmode.data.gear_swap_trq_flag)
				 ||(1 == Vehicle_cmd.cmdmode.data.brake_state)
				 ||(0 == Vehicle_cmd.torque_ref)
				 ||(0 == Vehicle_cmd.cmdmode.data.gear_state))
				{
					scsw2.field.zero_state = 0;
					VCU_IOstate.field.exit_zero_add_trq_flag = 3;
					zero_station_trqCmd_store = zero_station_trqCmd;
					sysCfgPara.TrqCmd_AIfilter = sysCfgPara.TrqCmd_AI = zero_station_trqCmd;
					
				}
				
			}
		}
	}
	else
	{
		scsw2.field.zero_state = 0;
		zero_station_trqCmd_flag = 0;
		zero_station_trqCmd_store = 0;
		Vehicle_cmd.cmdmode.data.zero_station_free_flag = 0;
		VCU_IOstate.field.exit_zero_add_trq_flag = 0;
		Vehicle_cmd.cmdmode.data.gear_swap_zero_flag = 0;
		VCU_IOstate.field.zero_disable_flag = 0;
		VCU_IOstate.field.zero_or_start_flag = 0;
	}
}

void udcloop_cond(void)
{
	if((_IQtoIQ15(sysFbkPara.Vdc_filter_protect)>EV_MCU_Para.field.Udcloop_UdcUplmt) || (_IQtoIQ15(sysFbkPara.Vdc_filter_protect)<EV_MCU_Para.field.Udcloop_UdcDownlmt))
	{
		scsw2.field.Udcloop_state = 1;
	}
	if(sysFbkPara.Speed_fbk_HMI_abs<EV_MCU_Para.field.Udcloop_Speedmin)
	{
		scsw2.field.Udcloop_state = 0;
	}
}

void speedlooplmtcond()
{
	//_iq factor = 0;
	_iq speed_up = 0;
	_iq speed_lmt = 0;
	_iq speed_exit = 0;
	_iq tooth_trq = 0;
	_iq tooth_trq_exit = 0;
	_iq spd_lmt_up_step = 0;
	_iq spd_lmt_down_step = 0;
	_iq x_value= 0;
	_iq trq_lmt = 0;
	_iq spd_lmt_exit_trq_deta = 0;
	_iq trq_step = 0;
	_iq max_unload_trq_step = 0;
	
	if((1 == EV_MCU_Para.field.Spd_Over_Limt_enable)
	 &&(1 == scsw2.field.igbt_state))
	{
		tooth_trq = _IQ15toIQ(EV_MCU_Para.field.Tooth_trq);
		tooth_trq_exit = tooth_trq+SysBase.invtorque;
		trq_lmt = _IQmpyI32(10,SysBase.invtorque);
		spd_lmt_exit_trq_deta = _IQmpyI32(5,SysBase.invtorque);
		trq_step = _IQmpy(_IQ(0.01),SysBase.invtorque);
		max_unload_trq_step = _IQmpy(_IQ(0.03),SysBase.invtorque);
		
		if(1 == VCU_IOstate.field.In_fwdrev)
		{
			if(1 == vehicle_io_state.data.DI6_DSP)//低速档
			{
				speed_up = EV_MCU_Para.field.Spd_FWD_L_Limt_Spd-_IQ15div(300,SysBase.speed);
				speed_exit = EV_MCU_Para.field.Spd_FWD_L_Limt_Spd-_IQ15div(1000,SysBase.speed);
				if(speed_up<0)
				{
					speed_up = EV_MCU_Para.field.Spd_FWD_L_Limt_Spd;
				}
				if(speed_exit<0)
				{
					speed_exit = 0;
				}
				speed_lmt = EV_MCU_Para.field.Spd_FWD_L_Limt_Spd;
			}
			else//高速档
			{
				speed_up = EV_MCU_Para.field.Spd_FWD_Limt_Spd-_IQ15div(300,SysBase.speed);
				speed_exit = EV_MCU_Para.field.Spd_FWD_Limt_Spd-_IQ15div(1000,SysBase.speed);
				if(speed_up<0)
				{
					speed_up = EV_MCU_Para.field.Spd_FWD_Limt_Spd;
				}
				if(speed_exit<0)
				{
					speed_exit = 0;
				}
				speed_lmt = EV_MCU_Para.field.Spd_FWD_Limt_Spd;
			}
			
		}
		else if(2 == VCU_IOstate.field.In_fwdrev)//后退档
		{
			if(1 == vehicle_io_state.data.DI6_DSP)//低速档
			{
				speed_up = (-EV_MCU_Para.field.Spd_FWD_E_Limt_Spd)+_IQ15div(300,SysBase.speed);
				speed_exit = (-EV_MCU_Para.field.Spd_FWD_E_Limt_Spd)+_IQ15div(600,SysBase.speed);
				if(speed_up>0)
				{
					speed_up = 0-EV_MCU_Para.field.Spd_FWD_E_Limt_Spd;
				}
				if(speed_exit>0)
				{
					speed_exit = 0;
				}
				speed_lmt = 0-EV_MCU_Para.field.Spd_FWD_E_Limt_Spd;
			}
			else//高速档
			{
				speed_up = (-EV_MCU_Para.field.Spd_REV_Limt_Spd)+_IQ15div(300,SysBase.speed);
				speed_exit = (-EV_MCU_Para.field.Spd_REV_Limt_Spd)+_IQ15div(1200,SysBase.speed);
				if(speed_up>0)
				{
					speed_up = 0-EV_MCU_Para.field.Spd_REV_Limt_Spd;
				}
				if(speed_exit>0)
				{
					speed_exit = 0;
				}
				speed_lmt = 0-EV_MCU_Para.field.Spd_REV_Limt_Spd;
			}
		}
		else
		{
			speed_up = (-EV_MCU_Para.field.Spd_REV_Limt_Spd)+_IQ15div(300,SysBase.speed);
			speed_exit = (-EV_MCU_Para.field.Spd_REV_Limt_Spd)+_IQ15div(1200,SysBase.speed);
			if(speed_up>0)
			{
				speed_up = 0-EV_MCU_Para.field.Spd_REV_Limt_Spd;
			}
			if(speed_exit>0)
			{
				speed_exit = 0;
			}
			speed_lmt = 0-EV_MCU_Para.field.Spd_REV_Limt_Spd;
		}

		if(1 == VCU_IOstate.field.limit_speed_flag)
		{
			if(1 == Vehicle_cmd.cmdmode.data.gear_state)
			{
				if(sysFbkPara.Speed_fbk_HMI<speed_lmt+_IQ15div(50,SysBase.speed))
				{
					VCU_IOstate.field.limit_speed_flag = 0;
				}
			}
			else
			{
				if(sysFbkPara.Speed_fbk_HMI>speed_lmt-_IQ15div(50,SysBase.speed))
				{
					VCU_IOstate.field.limit_speed_flag = 0;
				}
			}
		}
		else
		{
			if(1 == Vehicle_cmd.cmdmode.data.gear_state)
			{
				if(sysFbkPara.Speed_fbk_HMI>speed_lmt+_IQ15div(100,SysBase.speed))
				{
					VCU_IOstate.field.limit_speed_flag = 1;
				}
			}
			else
			{
				if(sysFbkPara.Speed_fbk_HMI<speed_lmt-_IQ15div(100,SysBase.speed))
				{
					VCU_IOstate.field.limit_speed_flag = 1;
				}
			}
		}
		
		if(0 == scsw2.field.spdlmt_flag)
		{
			if(0 != scsw2.field.SpdLmt_state)
			{
				if(1 == scsw2.field.SpdLmt_state)           //正向过速
				{
					if(((sysCfgPara.TrqCmd_AIfilter+spd_lmt_exit_trq_deta)<spdLoopPara.TrqCmd_real)
					 ||((_IQabs(sysCfgPara.TrqCmd_AIfilter)<=tooth_trq_exit)&&(0 == Vehicle_cmd.torque_ref))
					 ||(sysFbkPara.Speed_fbk_HMI<speed_exit))//(给定+10Nm)＜速度闭环力矩
					{
						scsw2.field.SpdLmt_state    					=  0;
						spdlmt_cmd                                      =  0;
					}
				}
				else                                        //反向过速
				{
					if(((sysCfgPara.TrqCmd_AIfilter-spd_lmt_exit_trq_deta)>spdLoopPara.TrqCmd_real)
					 ||((_IQabs(sysCfgPara.TrqCmd_AIfilter)<=tooth_trq_exit)&&(0 == Vehicle_cmd.torque_ref))
					 ||(sysFbkPara.Speed_fbk_HMI>speed_exit))//(给定-10Nm)>速度闭环力矩
					{
						scsw2.field.SpdLmt_state    					=  0;
						spdlmt_cmd                                      = 0;
					}
				}
			}
			else
			{
				if((1 == Vehicle_cmd.cmdmode.data.gear_state)
				  &&(sysFbkPara.Speed_fbk_HMI>speed_up)
				  &&(0 == VCU_IOstate.field.limit_speed_flag)
				  &&(0 != Vehicle_cmd.torque_ref))
				{
					scsw2.field.spdlmt_flag                         =  1;
					spdlmt_trq_filter = spdlmt_trq                                      =  trqLoop_trqRampref;
				}
				else if((2 == Vehicle_cmd.cmdmode.data.gear_state)
					  &&(sysFbkPara.Speed_fbk_HMI<speed_up)
					  &&(0 == VCU_IOstate.field.limit_speed_flag)
					  &&(0 != Vehicle_cmd.torque_ref))
				{
					scsw2.field.spdlmt_flag                         =  2;
					spdlmt_trq_filter = spdlmt_trq                                      =  trqLoop_trqRampref;
				}
			}
		}
		else
		{
			if(1 == scsw2.field.spdlmt_flag)
			{
				if(spdlmt_trq>tooth_trq)
				{
					if(spdlmt_trq<=trq_lmt)
					{
						x_value = _IQabs(spdlmt_trq);
						x_value = -_IQmpy(_IQ(31.25),x_value)+_IQ(0.625);
						spd_lmt_down_step = -_IQmpy(_IQ(0.00008),x_value)+_IQmpy(_IQ(0.03),SysBase.invtorque);
					}
					else
					{
						spd_lmt_down_step = max_unload_trq_step;
					}
					if(spd_lmt_down_step>max_unload_trq_step)
					{
						spd_lmt_down_step = max_unload_trq_step;
					}
					else if(spd_lmt_down_step<trq_step)
					{
						spd_lmt_down_step = trq_step;
					}
					
					if((spdlmt_trq-spd_lmt_down_step)>tooth_trq)
					{
						spdlmt_trq = spdlmt_trq-spd_lmt_down_step;
					}
					else
					{
						spdlmt_trq = tooth_trq;
					}
				}
				else if(spdlmt_trq<-tooth_trq)
				{
					if(spdlmt_trq>=-trq_lmt)
					{
						x_value = _IQabs(spdlmt_trq);
						x_value = -_IQmpy(_IQ(31.25),x_value)+_IQ(0.625);
						spd_lmt_up_step = -_IQmpy(_IQ(0.00008),x_value)+_IQmpy(_IQ(0.03),SysBase.invtorque);
					}
					else
					{
						spd_lmt_up_step = max_unload_trq_step;
					}
					if(spd_lmt_up_step>max_unload_trq_step)
					{
						spd_lmt_up_step = max_unload_trq_step;
					}
					else if(spd_lmt_up_step<trq_step)
					{
						spd_lmt_up_step = trq_step;
					}
					
					if((spdlmt_trq+spd_lmt_up_step)<-tooth_trq)
					{
						spdlmt_trq = spdlmt_trq+spd_lmt_up_step;
					}
					else
					{
						spdlmt_trq = -tooth_trq;
					}
				}
				else
				{
					spd_lmt_up_step = trq_step;
					spdlmt_trq = spdlmt_trq+spd_lmt_up_step;
					if(spdlmt_trq>tooth_trq)
					{
						spdlmt_trq = tooth_trq;
					}
				}

				if(1 == VCU_IOstate.field.limit_speed_flag)
				{
					spdlmt_trq = tooth_trq;
				}

				if(spdlmt_trq == tooth_trq)
				{
					scsw2.field.SpdLmt_state = 1;
					scsw2.field.spdlmt_flag = 0;
					spdlmt_cmd = sysFbkPara.Speed_fbk_Filter+_IQdiv(50,SysBase.speed);
					if(spdlmt_cmd>_IQ15toIQ(speed_lmt))
					{
						spdlmt_cmd = _IQ15toIQ(speed_lmt);
					}
				}
			}
			else
			{
				if(spdlmt_trq>tooth_trq)
				{
					if(spdlmt_trq<=trq_lmt)
					{
						x_value = _IQabs(spdlmt_trq);
						x_value = -_IQmpy(_IQ(62.5),x_value)+_IQ(1.25);
						spd_lmt_down_step = -_IQmpy(_IQ(0.00008),x_value)+_IQmpy(_IQ(0.03),SysBase.invtorque);
					}
					else
					{
						spd_lmt_down_step = max_unload_trq_step;
					}
					if(spd_lmt_down_step>max_unload_trq_step)
					{
						spd_lmt_down_step = max_unload_trq_step;
					}
					else if(spd_lmt_down_step<trq_step)
					{
						spd_lmt_down_step = trq_step;
					}
					
					if((spdlmt_trq-spd_lmt_down_step)>tooth_trq)
					{
						spdlmt_trq = spdlmt_trq-spd_lmt_down_step;
					}
					else
					{
						spdlmt_trq = tooth_trq;
					}
				}
				else if(spdlmt_trq<-tooth_trq)
				{
					if(spdlmt_trq>=-trq_lmt)
					{
						x_value = _IQabs(spdlmt_trq);
						x_value = -_IQmpy(_IQ(62.5),x_value)+_IQ(1.25);
						spd_lmt_up_step = -_IQmpy(_IQ(0.00036),x_value)+_IQmpy(_IQ(0.19),SysBase.invtorque);
					}
					else
					{
						spd_lmt_up_step = _IQmpy(_IQ(0.19),SysBase.invtorque);
					}
					if(spd_lmt_up_step>_IQmpy(_IQ(0.19),SysBase.invtorque))
					{
						spd_lmt_up_step = _IQmpy(_IQ(0.19),SysBase.invtorque);
					}
					else if(spd_lmt_up_step<trq_step)
					{
						spd_lmt_up_step = trq_step;
					}
					
					if((spdlmt_trq+spd_lmt_up_step)<-tooth_trq)
					{
						spdlmt_trq = spdlmt_trq+spd_lmt_up_step;
					}
					else
					{
						spdlmt_trq = -tooth_trq;
					}
				}
				else
				{
					spd_lmt_down_step = trq_step;
					spdlmt_trq = spdlmt_trq-spd_lmt_down_step;
					if(spdlmt_trq<-tooth_trq)
					{
						spdlmt_trq = -tooth_trq;
					}
				}

				if(1 == VCU_IOstate.field.limit_speed_flag)
				{
					spdlmt_trq = -tooth_trq;
				}

				if(spdlmt_trq == -tooth_trq)
				{
					scsw2.field.SpdLmt_state = 2;
					scsw2.field.spdlmt_flag = 0;
					spdlmt_cmd = sysFbkPara.Speed_fbk_Filter-_IQdiv(50,SysBase.speed);
					if(spdlmt_cmd<_IQ15toIQ(speed_lmt))
					{
						spdlmt_cmd = _IQ15toIQ(speed_lmt);
					}
				}
			}
		}
		
		if(((0 == scsw2.field.igbt_state)||(MTR_TRQ_LOOP != sccw1.field.runLoop_mode)||(0 == Vehicle_cmd.cmdmode.data.gear_state))
		 &&((0 != scsw2.field.SpdLmt_state)||(0 != scsw2.field.spdlmt_flag)))
		{
			sysCfgPara.TrqCmd_AIfilter = sysCfgPara.TrqCmd_AI = trqLoop_trqRampref;
			scsw2.field.SpdLmt_state = 0;
			scsw2.field.spdlmt_flag = 0;
		}
	}
	else
	{
		scsw2.field.SpdLmt_state = 0;
		scsw2.field.spdlmt_flag = 0;
	}
}

void diff_deal(void)
{
	}

void diff_deal_clear(void)
{
	/*diff_limit_flag = PI_NO_LIMIT;
	scsw2.field.shake_flag = 0;
	diff_spd_err_old = diff_spd_err = sysFbkPara.Speed_fbk_Filter_Lowspeed-sysFbkPara.Speed_fbk;
	diff_trq = diff_trq_ui = diff_trq_filter = 0;*/

	scsw2.field.stop_shake_flag = 0;
	diff_stop_spd_err_old = diff_stop_spd_err = Speed_fbk_Filter_Stop_Lowspeed-sysFbkPara.Speed_fbk;
	diff_stop_trq = diff_stop_trq_ui = diff_stop_trq_filter = 0;
}

void sub_io_yb(Uint16 buzzer_io_state)
{
	switch(buzzer_io_state)
	{
		case SUB_IO_BUZZER_OFF:
			GpioDataRegs.GPACLEAR.bit.GPIO10 = 1;
			break;
		case SUB_IO_BUZZER_ON:
			GpioDataRegs.GPASET.bit.GPIO10 = 1;
			break;
		default:
			GpioDataRegs.GPACLEAR.bit.GPIO10 = 1;
			break;
	}
}

void sub_io_buzzer(Uint16 buzzer_io_state)
{
	switch(buzzer_io_state)
	{
		case SUB_IO_BUZZER_OFF:
			GpioDataRegs.GPACLEAR.bit.GPIO11 = 1;
			break;
		case SUB_IO_BUZZER_ON:
			GpioDataRegs.GPASET.bit.GPIO11 = 1;
			break;
		default:
			GpioDataRegs.GPACLEAR.bit.GPIO11 = 1;
			break;
	}
}

void sub_io_ad2s1205_fs1(Uint16 fan_io_state)
{
	switch(fan_io_state)
	{
		case SUB_IO_LOW:
			GpioDataRegs.GPACLEAR.bit.GPIO6 = 1;
			break;
		case SUB_IO_HIGH:
			GpioDataRegs.GPASET.bit.GPIO6 = 1;
			break;
		default:
			GpioDataRegs.GPACLEAR.bit.GPIO6 = 1;
			break;
	}
}

void sub_io_ad2s1205_fs2(Uint16 fan_io_state)
{
	switch(fan_io_state)
	{
		case SUB_IO_LOW:
			GpioDataRegs.GPACLEAR.bit.GPIO7 = 1;
			break;
		case SUB_IO_HIGH:
			GpioDataRegs.GPASET.bit.GPIO7 = 1;
			break;
		default:
			GpioDataRegs.GPACLEAR.bit.GPIO7 = 1;
			break;
	}
}

void sub_io_ad2s1205_rst(Uint16 fan_io_state)
{
	switch(fan_io_state)
	{
		case SUB_IO_LOW:
			GpioDataRegs.GPACLEAR.bit.GPIO8 = 1;
			break;
		case SUB_IO_HIGH:
			GpioDataRegs.GPASET.bit.GPIO8 = 1;
			break;
		default:
			GpioDataRegs.GPACLEAR.bit.GPIO8 = 1;
			break;
	}
}

void sub_io_relay(Uint16 fan_io_state)
{
	switch(fan_io_state)
	{
		case SUB_IO_LOW:
			GpioDataRegs.GPBCLEAR.bit.GPIO40 = 1;
			break;
		case SUB_IO_HIGH:
			GpioDataRegs.GPBSET.bit.GPIO40 = 1;
			break;
		default:
			GpioDataRegs.GPBSET.bit.GPIO40 = 1;
			break;
	}
}

void sub_io_re_cs(Uint16 fan_io_state)
{
	switch(fan_io_state)
	{
		case SUB_IO_LOW:
			GpioDataRegs.GPBCLEAR.bit.GPIO39 = 1;
			break;
		case SUB_IO_HIGH:
			GpioDataRegs.GPBSET.bit.GPIO39 = 1;
			break;
		default:
			GpioDataRegs.GPBSET.bit.GPIO39 = 1;
			break;
	}
}

#endif

