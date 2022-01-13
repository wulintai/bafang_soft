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
	
	if((1 == EV_MCU_Para.field.Zero_Sation_Enable)  //驻坡使能
	 &&(1 == scsw2.field.igbt_state))               //逆变器开管    使能驱动芯片
	{
		if(0 == scsw2.field.zero_state)// 未驻坡的情况下
		{
			if((((1 == Vehicle_cmd.cmdmode.data.gear_state)&&(speed<-zero_speed_throad))     //前进情况下，速度向后达到阈值
			  ||((2 == Vehicle_cmd.cmdmode.data.gear_state)&&(speed > zero_speed_throad)))   //后退情况下，速度向前达到阈值
			 &&((0 == Vehicle_cmd.cmdmode.data.brake_state)&&(0 == alarm.field.gear_fault)&&(0 == Vehicle_cmd.torque_ref)))// &&未刹车&&驻坡失败警告=0&&油门力矩=0
			{
				scsw2.field.zero_state = 1;   //进入驻坡
				VCU_IOstate.field.zero_disable_flag = 0;
				VCU_IOstate.field.zero_or_start_flag = 0;   //驻坡启动
			}
			else if((1 == Vehicle_cmd.cmdmode.data.gear_swap_trq_flag)  //制动
				  &&(0 == Vehicle_cmd.cmdmode.data.brake_state)         //未刹车
				  &&(0 != Vehicle_cmd.torque_ref)                       //油门力矩≠0
				  &&(0 != Vehicle_cmd.cmdmode.data.gear_state)          //不是空挡
				  /*&&(_IQabs(trqLoop_trqRampref)>=pre_trq_value)*/)//前后换档松刹车踩油门起步
			{
				scsw2.field.zero_state = 1;  //进入驻坡
				VCU_IOstate.field.zero_disable_flag = 0;
				VCU_IOstate.field.zero_or_start_flag = 1;   //驻坡启动
			}

			if(1 == Vehicle_cmd.cmdmode.data.gear_swap_zero_flag)   //有手刹
			{
				if((1 == scsw2.field.zero_state)    //驻坡状态
				 ||((1 == Vehicle_cmd.cmdmode.data.gear_state)&&(speed> 50))   //前进状态&&速度>50
				 ||((2 == Vehicle_cmd.cmdmode.data.gear_state)&&(speed<-50))   //后退状态&&速度<-50
				 ||(0 == Vehicle_cmd.cmdmode.data.gear_state))  //空挡
				{
					Vehicle_cmd.cmdmode.data.gear_swap_zero_flag = 0;       //不需要手刹
					zero_speed_throad = 15;                  //驻坡速度阈值
				}
			}

			if(1 == zero_station_trqCmd_flag)    //使用驻坡力矩标志
			{
				if(((0 != Vehicle_cmd.cmdmode.data.gear_state) && (0 != Vehicle_cmd.torque_ref))  //非空挡有前进力矩
				 ||(1 == scsw2.field.zero_state))                                                 //处于驻坡状态
				{
					zero_station_trqCmd_flag = 0;                                                 //不使用驻坡力矩
					Vehicle_cmd.cmdmode.data.zero_station_free_flag = 0;                          //力矩环
				}
			}
			if(0 != VCU_IOstate.field.exit_zero_add_trq_flag)    //离开驻坡时需要增加力矩
			{
				if((0 == Vehicle_cmd.cmdmode.data.gear_state)    //空挡
				 ||(1 == Vehicle_cmd.cmdmode.data.brake_state)   //有刹车
				 ||(1 == scsw2.field.zero_state)                 //驻坡
				 ||(0 == Vehicle_cmd.torque_ref))                //油门无力矩
				{
					VCU_IOstate.field.exit_zero_add_trq_flag = 0;//离开驻坡时不需要增加力矩
				}
			}
		}
		else//退出驻坡
		{
			if(0 == VCU_IOstate.field.zero_or_start_flag)    //非驻坡启动
			{
				if(0 == VCU_IOstate.field.exit_zero_add_trq_flag)  //离开驻坡时不需要增加力矩
				{
					if((0 != Vehicle_cmd.torque_ref)         //油门无力矩
					 &&(0 != Vehicle_cmd.cmdmode.data.gear_state)) //非空挡
					{
						VCU_IOstate.field.exit_zero_add_trq_flag = 1; //离开驻坡时需要增加力矩
						zero_station_trqCmd_store = zero_station_trqCmd;   //
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
	_iq speed_lmt = 0;    //速度限制
	_iq speed_exit = 0;   //离开限速的阈值
	_iq tooth_trq = 0;
	_iq tooth_trq_exit = 0;  //贴齿力矩阈值
	_iq spd_lmt_up_step = 0;
	_iq spd_lmt_down_step = 0;
	_iq x_value= 0;
	_iq trq_lmt = 0;
	_iq spd_lmt_exit_trq_deta = 0;
	_iq trq_step = 0;
	_iq max_unload_trq_step = 0;
	
	if((1 == EV_MCU_Para.field.Spd_Over_Limt_enable)   //限速使能
	 &&(1 == scsw2.field.igbt_state)) //逆变器开管
	{
		tooth_trq = _IQ15toIQ(EV_MCU_Para.field.Tooth_trq);//贴齿力矩（最大贴齿力矩 2/500）<< 9位 = 2.048
		tooth_trq_exit = tooth_trq+SysBase.invtorque;//贴齿力矩阈值 2.048 +1/500 = 2.05
		trq_lmt = _IQmpyI32(10,SysBase.invtorque);// 1/50 = 0.02
		spd_lmt_exit_trq_deta = _IQmpyI32(5,SysBase.invtorque);// 1/100
		trq_step = _IQmpy(_IQ(0.01),SysBase.invtorque);// 1/50000
		max_unload_trq_step = _IQmpy(_IQ(0.03),SysBase.invtorque);  // 3/50000
		
		if(1 == VCU_IOstate.field.In_fwdrev)//当前的档位为D档  前进档    1 = D  2 = R  0 = N
		{
			if(1 == vehicle_io_state.data.DI6_DSP)//刹车\低速档  未赋值
			{
				speed_up = EV_MCU_Para.field.Spd_FWD_L_Limt_Spd-_IQ15div(300,SysBase.speed); //(L档前进速度限制 4500/12000) - 300/12000  = 4200/12000 = 0.35
				speed_exit = EV_MCU_Para.field.Spd_FWD_L_Limt_Spd-_IQ15div(1000,SysBase.speed);//(L档前进速度限制 4500/12000) - 1000/12000  = 3500/12000 ≈ 0.292
				if(speed_up<0)//N
				{
					speed_up = EV_MCU_Para.field.Spd_FWD_L_Limt_Spd;
				}
				if(speed_exit<0)//N
				{
					speed_exit = 0;
				}
				speed_lmt = EV_MCU_Para.field.Spd_FWD_L_Limt_Spd;// 4500/12000 = 0.375 = 3/8
			}
			else//高速档
			{
				speed_up = EV_MCU_Para.field.Spd_FWD_Limt_Spd-_IQ15div(300,SysBase.speed); //(前进速度限制 4500/12000) - 300/12000  = 4200/12000 = 0.35
				speed_exit = EV_MCU_Para.field.Spd_FWD_Limt_Spd-_IQ15div(1000,SysBase.speed);//(前进速度限制 4500/12000) - 1000/12000  = 3500/12000 ≈ 0.292
				if(speed_up<0)
				{
					speed_up = EV_MCU_Para.field.Spd_FWD_Limt_Spd;// 4500/12000 = 0.375 = 3/8
				}
				if(speed_exit<0)
				{
					speed_exit = 0;
				}
				speed_lmt = EV_MCU_Para.field.Spd_FWD_Limt_Spd;// 4500/12000 = 0.375 = 3/8
			}
			
		}
		else if(2 == VCU_IOstate.field.In_fwdrev)//后退档
		{
			if(1 == vehicle_io_state.data.DI6_DSP)//刹车\低速档  未赋值
			{
				speed_up = (-EV_MCU_Para.field.Spd_FWD_E_Limt_Spd)+_IQ15div(300,SysBase.speed);// -(E档前进速度限制 1600/12000) + 300/12000 = -1300/12000 ≈ -0.108
				speed_exit = (-EV_MCU_Para.field.Spd_FWD_E_Limt_Spd)+_IQ15div(600,SysBase.speed);// -(E档前进速度限制 1600/12000) + 600/12000 = -1000/12000 ≈ -0.083
				if(speed_up>0)//N
				{
					speed_up = 0-EV_MCU_Para.field.Spd_FWD_E_Limt_Spd;
				}
				if(speed_exit>0)//Y
				{
					speed_exit = 0;
				}
				speed_lmt = 0-EV_MCU_Para.field.Spd_FWD_E_Limt_Spd;//  -(E档前进速度限制 1600/12000) ≈ -0.133
			}
			else//高速档
			{
				speed_up = (-EV_MCU_Para.field.Spd_REV_Limt_Spd)+_IQ15div(300,SysBase.speed);// -(后退速度限制 750/12000) + 300/12000  = -450/12000 = -0.0875 = -7/80
				speed_exit = (-EV_MCU_Para.field.Spd_REV_Limt_Spd)+_IQ15div(1200,SysBase.speed);// -(后退速度限制 750/12000) + 1200/12000 = 450/12000 = 0.0875 = 7/80
				if(speed_up>0)//N
				{
					speed_up = 0-EV_MCU_Para.field.Spd_REV_Limt_Spd;
				}
				if(speed_exit>0)//Y
				{
					speed_exit = 0;
				}
				speed_lmt = 0-EV_MCU_Para.field.Spd_REV_Limt_Spd;//  -(后退速度限制 750/12000) = -0.0625 = -5/80
			}
		}
		else // 空挡
		{
			speed_up = (-EV_MCU_Para.field.Spd_REV_Limt_Spd)+_IQ15div(300,SysBase.speed);// -(后退速度限制 750/12000) + 300/12000 = -450/12000 = -0.0875 = -7/80
			speed_exit = (-EV_MCU_Para.field.Spd_REV_Limt_Spd)+_IQ15div(1200,SysBase.speed);// -(后退速度限制 750/12000) + 1200/12000 = 450/12000 = 0.0875 = 7/80
			if(speed_up>0)//N
			{
				speed_up = 0-EV_MCU_Para.field.Spd_REV_Limt_Spd;
			}
			if(speed_exit>0)//Y
			{
				speed_exit = 0;
			}
			speed_lmt = 0-EV_MCU_Para.field.Spd_REV_Limt_Spd;//  -(后退速度限制 750/12000) = -0.0625 = -5/80
		}

		//速度小于一定值后清除VCU限速标志
		if(1 == VCU_IOstate.field.limit_speed_flag)// VCU限速标志
		{
			if(1 == Vehicle_cmd.cmdmode.data.gear_state)//前进档 - 取自VCU_IOstate.field.In_fwdrev的值
			{
				if(sysFbkPara.Speed_fbk_HMI<speed_lmt+_IQ15div(50,SysBase.speed))//(滤波后的速度反馈/瞬时速度)<速度限制(0.375 4500rpm)+ (50/12000≈0.0042) = 0.3792
				{
					VCU_IOstate.field.limit_speed_flag = 0;// 清除VCU限速标志
				}
			}
			else//后退档 - 取自VCU_IOstate.field.In_fwdrev的值
			{
				if(sysFbkPara.Speed_fbk_HMI>speed_lmt-_IQ15div(50,SysBase.speed))//(滤波后的速度反馈/瞬时速度)> 速度限制(-0.0625 -750rpm) - (50/12000≈0.0042) = -0.0067
				{
					VCU_IOstate.field.limit_speed_flag = 0;// 清除VCU限速标志
				}
			}
		}
		else//速度大于一定值后置位限速标志
		{
			if(1 == Vehicle_cmd.cmdmode.data.gear_state)//前进档 - 取自VCU_IOstate.field.In_fwdrev的值
			{
				if(sysFbkPara.Speed_fbk_HMI>speed_lmt+_IQ15div(100,SysBase.speed))//(滤波后的速度反馈/瞬时速度)>速度限制(0.375 4500rpm)+ (50/12000≈0.0042) = 0.3792
				{
					VCU_IOstate.field.limit_speed_flag = 1;//置位VCU限速标志
				}
			}
			else//后退档 - 取自VCU_IOstate.field.In_fwdrev的值
			{
				if(sysFbkPara.Speed_fbk_HMI<speed_lmt-_IQ15div(100,SysBase.speed))//(滤波后的速度反馈/瞬时速度)< 速度限制(-0.0625 -750rpm) - (50/12000≈0.0042) = -0.0067
				{
					VCU_IOstate.field.limit_speed_flag = 1;//置位VCU限速标志
				}
			}
		}
		

		if(0 == scsw2.field.spdlmt_flag) //  scsw2限速标志为0
		{
			if(0 != scsw2.field.SpdLmt_state) //有限速           0:不限速    1：正向限速   2：反向限速
			{
				if(1 == scsw2.field.SpdLmt_state)           //正向限速状态
				{
					if(((sysCfgPara.TrqCmd_AIfilter+spd_lmt_exit_trq_deta)<spdLoopPara.TrqCmd_real)//(给定+10Nm(0.01))＜速度闭环力矩
					 ||((_IQabs(sysCfgPara.TrqCmd_AIfilter)<=tooth_trq_exit)&&(0 == Vehicle_cmd.torque_ref))//给定力矩<贴齿力矩阈值&&驱动力矩=0
					 ||(sysFbkPara.Speed_fbk_HMI<speed_exit))// 转速 < 离开限速的阈值 = 0.292 3500rpm
					{
						scsw2.field.SpdLmt_state    					=  0;//限速状态设为不限速
						spdlmt_cmd                                      =  0;//限速控制清零
					}
				}
				else                                        //反向限速状态
				{
					if(((sysCfgPara.TrqCmd_AIfilter-spd_lmt_exit_trq_deta)>spdLoopPara.TrqCmd_real)//(给定-10Nm(0.01))>速度闭环力矩
					 ||((_IQabs(sysCfgPara.TrqCmd_AIfilter)<=tooth_trq_exit)&&(0 == Vehicle_cmd.torque_ref))//给定力矩<贴齿力矩阈值&&驱动力矩=0
					 ||(sysFbkPara.Speed_fbk_HMI>speed_exit))// 转速 > 离开限速的阈值 = -0.0625  -1950rpm
					{
						scsw2.field.SpdLmt_state    					=  0;//限速状态设为不限速
						spdlmt_cmd                                      = 0;//限速控制清零
					}
				}
			}
			else //无限速
			{
				if((1 == Vehicle_cmd.cmdmode.data.gear_state)  //前进档 - 取自VCU_IOstate.field.In_fwdrev的值
				  &&(sysFbkPara.Speed_fbk_HMI>speed_up)//(滤波后的速度反馈/瞬时速度)>上升速度(0.35 4200rpm)
				  &&(0 == VCU_IOstate.field.limit_speed_flag)//限速标志为0/未限速
				  &&(0 != Vehicle_cmd.torque_ref))//驱动力矩≠0
				{
					scsw2.field.spdlmt_flag                         =  1; //正向限速
					spdlmt_trq_filter = spdlmt_trq                                      =  trqLoop_trqRampref;
			      //限速扭矩滤波          限速扭矩                                                扭矩指令
				}
				else if((2 == Vehicle_cmd.cmdmode.data.gear_state) //后退档 - 取自VCU_IOstate.field.In_fwdrev的值
					  &&(sysFbkPara.Speed_fbk_HMI<speed_up)//(滤波后的速度反馈/瞬时速度)<反向速度(-0.0875 -1050rpm)
					  &&(0 == VCU_IOstate.field.limit_speed_flag)//限速标志为0/未限速
					  &&(0 != Vehicle_cmd.torque_ref))//驱动力矩≠0
				{
					scsw2.field.spdlmt_flag                         =  2; //反向限速
					spdlmt_trq_filter = spdlmt_trq                                      =  trqLoop_trqRampref;
				   //限速扭矩滤波          限速扭矩                                                扭矩指令
				}
			}
		}
		else //scsw2限速标志为非0
		{
			if(1 == scsw2.field.spdlmt_flag)  //正向限速标志
			{
				if(spdlmt_trq>tooth_trq)//限速扭矩 > 贴齿扭矩 = 2.048
				{
					if(spdlmt_trq<=trq_lmt) //限速扭矩 <= 扭矩限制 = 0.02
					{
						x_value = _IQabs(spdlmt_trq); //限速扭矩的绝对值
						x_value = -_IQmpy(_IQ(31.25),x_value)+_IQ(0.625); // 0.625-(限速扭矩的绝对值*31.25)
						spd_lmt_down_step = -_IQmpy(_IQ(0.00008),x_value)+_IQmpy(_IQ(0.03),SysBase.invtorque);//限速扭矩下降的步长
					}
					else //限速扭矩 > 扭矩限制 = 0.02
					{
						spd_lmt_down_step = max_unload_trq_step; //限速扭矩下降的步长 = 无负载最大扭矩下降步长 = 3/50000
					}
					if(spd_lmt_down_step>max_unload_trq_step)//限速扭矩下降的步长 > 无负载最大扭矩下降步长 = 3/50000
					{
						spd_lmt_down_step = max_unload_trq_step;//限速扭矩下降的步长 = 无负载最大扭矩下降步长 = 3/50000
					}
					else if(spd_lmt_down_step<trq_step)//限速扭矩下降的步长 < 最小扭矩下降步长 = 1/50000
					{
						spd_lmt_down_step = trq_step;//限速扭矩下降的步长 = 最小扭矩下降步长 = 1/50000
					}
					
					if((spdlmt_trq-spd_lmt_down_step)>tooth_trq)//限速扭矩 - 限速扭矩下降的步长 > 贴齿扭矩
					{
						spdlmt_trq = spdlmt_trq-spd_lmt_down_step;//限速扭矩 = 限速扭矩 - 限速扭矩下降的步长
					}
					else//限速扭矩 - 限速扭矩下降的步长 < 贴齿扭矩
					{
						spdlmt_trq = tooth_trq;//限速扭矩 = 贴齿扭矩
					}
				}
				else if(spdlmt_trq<-tooth_trq)//限速扭矩 < -贴齿扭矩 = -2.048
				{
					if(spdlmt_trq>=-trq_lmt)//限速扭矩 >= -扭矩限制 = -0.02
					{
						x_value = _IQabs(spdlmt_trq); //限速扭矩的绝对值
						x_value = -_IQmpy(_IQ(31.25),x_value)+_IQ(0.625); // 0.625-(限速扭矩的绝对值*31.25)
						spd_lmt_up_step = -_IQmpy(_IQ(0.00008),x_value)+_IQmpy(_IQ(0.03),SysBase.invtorque); //限速扭矩下降的步长
					}
					else//限速扭矩 < -扭矩限制 = -0.02
					{
						spd_lmt_up_step = max_unload_trq_step; //限速扭矩下降的步长 = 无负载最大扭矩下降步长 = 3/50000
					}
					if(spd_lmt_up_step>max_unload_trq_step)//限速扭矩下降的步长 > 无负载最大扭矩下降步长 = 3/50000
					{
						spd_lmt_up_step = max_unload_trq_step; //限速扭矩下降的步长 = 无负载最大扭矩下降步长 = 3/50000
					}
					else if(spd_lmt_up_step<trq_step)//限速扭矩下降的步长 < 最小扭矩下降步长 = 1/50000
					{
						spd_lmt_up_step = trq_step;//限速扭矩下降的步长 = 最小扭矩下降步长 = 1/50000
					}
					
					if((spdlmt_trq+spd_lmt_up_step)<-tooth_trq)//限速扭矩 + 限速扭矩下降的步长 < -贴齿扭矩
					{
						spdlmt_trq = spdlmt_trq+spd_lmt_up_step;//限速扭矩 = 限速扭矩 + 限速扭矩下降的步长
					}
					else//限速扭矩 + 限速扭矩下降的步长 > -贴齿扭矩
					{
						spdlmt_trq = -tooth_trq;//限速扭矩 = -贴齿扭矩
					}
				}
				else// -2.048 = -贴齿扭矩 < 限速扭矩 < 贴齿扭矩 = 2.048
				{
					spd_lmt_up_step = trq_step; //限速扭矩下降的步长 = 最小扭矩下降步长 = 1/50000
					spdlmt_trq = spdlmt_trq+spd_lmt_up_step;//限速扭矩 = 限速扭矩 + 限速扭矩下降的步长
					if(spdlmt_trq>tooth_trq)//限速扭矩 > 贴齿扭矩
					{
						spdlmt_trq = tooth_trq;//限速扭矩 = 贴齿扭矩
					}
				}

				if(1 == VCU_IOstate.field.limit_speed_flag)//VCU限速标志为1 = 限速
				{
					spdlmt_trq = tooth_trq;//限速扭矩 = 贴齿扭矩
				}

				if(spdlmt_trq == tooth_trq)//限速扭矩 = 贴齿扭矩
				{
					scsw2.field.SpdLmt_state = 1;//scsw2限速状态置为1 正向限速
					scsw2.field.spdlmt_flag = 0;//scsw2限速标志清零
					spdlmt_cmd = sysFbkPara.Speed_fbk_Filter+_IQdiv(50,SysBase.speed);//限速控制 = (速度反馈/平均速度) + (50/12000)
					if(spdlmt_cmd>_IQ15toIQ(speed_lmt))//限速控制 > 速度限制<<9位 = (0.375*512) = 192
					{
						spdlmt_cmd = _IQ15toIQ(speed_lmt);//限速控制 = 速度限制<<9位 = (0.375*512) = 192
					}
				}
			}
			else //反向限速标志
			{
				if(spdlmt_trq>tooth_trq)//限速扭矩 > 贴齿扭矩 = 2.048
				{
					if(spdlmt_trq<=trq_lmt)//限速扭矩 <= 扭矩限制 = 0.02
					{
						x_value = _IQabs(spdlmt_trq);//限速扭矩的绝对值
						x_value = -_IQmpy(_IQ(62.5),x_value)+_IQ(1.25);// 1.25-(限速扭矩的绝对值*62.5)
						spd_lmt_down_step = -_IQmpy(_IQ(0.00008),x_value)+_IQmpy(_IQ(0.03),SysBase.invtorque);//限速扭矩下降的步长
					}
					else//限速扭矩 > 扭矩限制 = 0.02
					{
						spd_lmt_down_step = max_unload_trq_step;//限速扭矩下降的步长 = 无负载最大扭矩下降步长 = 3/50000
					}
					if(spd_lmt_down_step>max_unload_trq_step)//限速扭矩下降的步长 > 无负载最大扭矩下降步长 = 3/50000
					{
						spd_lmt_down_step = max_unload_trq_step;//限速扭矩下降的步长 = 无负载最大扭矩下降步长 = 3/50000
					}
					else if(spd_lmt_down_step<trq_step)//限速扭矩下降的步长 < 最小扭矩下降步长 = 1/50000
					{
						spd_lmt_down_step = trq_step;//限速扭矩下降的步长 = 最小扭矩下降步长 = 1/50000
					}
					
					if((spdlmt_trq-spd_lmt_down_step)>tooth_trq)//限速扭矩 - 限速扭矩下降的步长 > 贴齿扭矩
					{
						spdlmt_trq = spdlmt_trq-spd_lmt_down_step;//限速扭矩 = 限速扭矩 - 限速扭矩下降的步长
					}
					else//限速扭矩 + 限速扭矩下降的步长 > -贴齿扭矩
					{
						spdlmt_trq = tooth_trq;//限速扭矩 = 贴齿扭矩
					}
				}
				else if(spdlmt_trq<-tooth_trq)//限速扭矩 < -贴齿扭矩 = -2.048
				{
					if(spdlmt_trq>=-trq_lmt)//限速扭矩 >= -扭矩限制 = -0.02
					{
						x_value = _IQabs(spdlmt_trq);//限速扭矩的绝对值
						x_value = -_IQmpy(_IQ(62.5),x_value)+_IQ(1.25);// 1.25-(限速扭矩的绝对值*62.5)
						spd_lmt_up_step = -_IQmpy(_IQ(0.00036),x_value)+_IQmpy(_IQ(0.19),SysBase.invtorque);//限速扭矩下降的步长
					}
					else//限速扭矩 < -扭矩限制 = -0.02
					{
						spd_lmt_up_step = _IQmpy(_IQ(0.19),SysBase.invtorque);//限速扭矩下降的步长 = 0.19*1/500 = 19/50000
					}
					if(spd_lmt_up_step>_IQmpy(_IQ(0.19),SysBase.invtorque))//限速扭矩下降的步长 > 0.19*1/500 = 19/50000
					{
						spd_lmt_up_step = _IQmpy(_IQ(0.19),SysBase.invtorque);//限速扭矩下降的步长 = 0.19*1/500 = 19/50000
					}
					else if(spd_lmt_up_step<trq_step)//限速扭矩下降的步长 < 最小扭矩下降步长 = 1/50000
					{
						spd_lmt_up_step = trq_step;//限速扭矩下降的步长 = 最小扭矩下降步长 = 1/50000
					}
					
					if((spdlmt_trq+spd_lmt_up_step)<-tooth_trq)//限速扭矩 + 限速扭矩下降的步长 < -贴齿扭矩
					{
						spdlmt_trq = spdlmt_trq+spd_lmt_up_step;//限速扭矩 = 限速扭矩 + 限速扭矩下降的步长
					}
					else//限速扭矩 + 限速扭矩下降的步长 > -贴齿扭矩
					{
						spdlmt_trq = -tooth_trq;//限速扭矩 = -贴齿扭矩
					}
				}
				else
				{
					spd_lmt_down_step = trq_step;  //限速扭矩下降的步长 = 最小扭矩下降步长 = 1/50000
					spdlmt_trq = spdlmt_trq-spd_lmt_down_step;//限速扭矩 = 限速扭矩 - 限速扭矩下降的步长
					if(spdlmt_trq<-tooth_trq)//限速扭矩<-贴齿扭矩
					{
						spdlmt_trq = -tooth_trq;//限速扭矩 = -贴齿扭矩
					}
				}

				if(1 == VCU_IOstate.field.limit_speed_flag)//VCU限速标志为1 = 限速
				{
					spdlmt_trq = -tooth_trq;//限速扭矩 = -贴齿扭矩
				}

				if(spdlmt_trq == -tooth_trq)//限速扭矩 = -贴齿扭矩
				{
					scsw2.field.SpdLmt_state = 2;//scsw2限速状态置为2 反向限速
					scsw2.field.spdlmt_flag = 0;//scsw2限速标志清零
					spdlmt_cmd = sysFbkPara.Speed_fbk_Filter-_IQdiv(50,SysBase.speed);//限速控制 = (速度反馈/平均速度) - (50/12000)
					if(spdlmt_cmd<_IQ15toIQ(speed_lmt))//限速控制 < 速度限制<<9位 = (0.375*512) = 192
					{
						spdlmt_cmd = _IQ15toIQ(speed_lmt);//限速控制 = 速度限制<<9位 = (0.375*512) = 192
					}
				}
			}
		}
		
		if(((0 == scsw2.field.igbt_state)//逆变器关管
		  ||(MTR_TRQ_LOOP != sccw1.field.runLoop_mode)//不为力矩环
		  ||(0 == Vehicle_cmd.cmdmode.data.gear_state)) //空档
		 &&((0 != scsw2.field.SpdLmt_state)||(0 != scsw2.field.spdlmt_flag)))//scsw2为限速状态||scsw2限速标志位置位
		{
			sysCfgPara.TrqCmd_AIfilter = sysCfgPara.TrqCmd_AI = trqLoop_trqRampref;//给定力矩 = 当前力矩 = 扭矩指令
			scsw2.field.SpdLmt_state = 0;//scsw2限速状态清零
			scsw2.field.spdlmt_flag = 0;//清除scsw2标志位
		}
	}
	else
	{
		scsw2.field.SpdLmt_state = 0;//scsw2限速状态清零
		scsw2.field.spdlmt_flag = 0;//清除scsw2标志位
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

