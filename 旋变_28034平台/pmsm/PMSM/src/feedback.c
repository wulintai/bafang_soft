#ifndef _FEEDBACK_C_
#define _FEEDBACK_C_

#include "include_c.h"

void AdcSEQ2(void)
{
	sysFbkPara.AI1_fbk_ad = AdcResult.ADCRESULT6;//油门1
	sysFbkPara.AI2_fbk_ad = AdcResult.ADCRESULT7;//油门2
	sysFbkPara.Temp_mdl_ad =   AdcResult.ADCRESULT4;//T_MOS_AD 
	sysFbkPara.Temp_mtr_ad =   AdcResult.ADCRESULT3;//TEMP_AD
	sysFbkPara.V12_fbk_ad = AdcResult.ADCRESULT8; //V12_AD
}

void diagIO(void)
{
	_iq speed = 0;
	_iq trq = 0;
	_iq k_tmp = 0;
	_iq tooth_trq = 0;
	_iq x_value = 0;
	_iq trq_lmt = 0;
	_iq trq_deta = 0;
	_iq trq_step = 0;
	_iq trq_n_step_updown = 0;
	_iq trq_step_second = 0;
	_iq trq_step_third = 0;
	//_iq trq_gear_step = 0;
	//_iq pre_trq_value = 0;
	_iq tooth_trq_step = 0;
	_iq max_fwd_up_step = 0;
	_iq max_fwd_down_step = 0;
	_iq max_feedback_up_step = 0;
	_iq max_feedback_down_step = 0;
	_iq min_feedback_up_step = 0;
	_iq min_feedback_down_step = 0;
	_iq max_feedback_enter_down_step = 0;
	_iq gear_n_step = 0;
	_iq max_rev_down_step = 0;
	_iq max_rev_up_step = 0;

	if(TRQCMD_SOURCE_AI == EV_MCU_Para.field.TrqCmd_SourceType)  //力矩命令来源 = 0 AI1
	{
		speed = sysFbkPara.Speed;  //当前转速
		tooth_trq = _IQ15toIQ(EV_MCU_Para.field.Tooth_trq);  //贴齿力矩（最大贴齿力矩 2/500）<< 9位 = 2.048
		//pre_trq_value = _IQmpy(_IQ(0.5),SysBase.invtorque);
		trq_lmt = _IQmpyI32(10,SysBase.invtorque);  // 1/50 = 0.02
		trq_step = _IQmpy(_IQ(0.01),SysBase.invtorque);  // 1/50000    最小步长
		trq_step_second = _IQmpy(_IQ(0.02125),SysBase.invtorque);  // 0.0000425
		trq_step_third = _IQmpy(_IQ(0.025),SysBase.invtorque);  // 1/20000 = 0.00005
		//trq_gear_step = _IQmpy(_IQ(0.015),SysBase.invtorque);
		tooth_trq_step = _IQmpy(_IQ(0.2),SysBase.invtorque); // 1/2500
		max_fwd_up_step = _IQmpy(_IQ(0.5),SysBase.invtorque);  // 1/1000
		max_fwd_down_step = _IQmpy(_IQ(1.05),SysBase.invtorque);  // 1.05/500
		max_feedback_up_step = _IQmpy(_IQ(3.5),SysBase.invtorque); // 3.5/500
		max_feedback_down_step = _IQmpy(_IQ(0.1),SysBase.invtorque);  // 0.1/500
		min_feedback_up_step = _IQmpy(_IQ(0.05),SysBase.invtorque);   // 0.05/500
		min_feedback_down_step = _IQmpy(_IQ(0.05),SysBase.invtorque);  // 0.05/500
		max_feedback_enter_down_step = _IQmpy(_IQ(3.5),SysBase.invtorque);  // 3.5/500
		gear_n_step = _IQmpy(_IQ(0.5),SysBase.invtorque); // 0.5/500
		max_rev_down_step = _IQmpy(_IQ(0.25),SysBase.invtorque);  // 0.25/500
		max_rev_up_step = _IQmpy(_IQ(1.5),SysBase.invtorque);  // 1.5/500
		trq_n_step_updown = _IQmpy(_IQ(0.1),SysBase.invtorque);  // 0.1/500
		
		sccw1.field.runLoop_mode = MTR_TRQ_LOOP;  //运行模式设为力矩环

		//刹车信号采集
		if(1 == vehicle_io_state.data.DI2_BRRAK_DSP)
		{
			Vehicle_cmd.cmdmode.data.brake_state = 1;
		}
		else
		{
			Vehicle_cmd.cmdmode.data.brake_state = 0;
		}

		//档位信号采集
		if((1 == vehicle_io_state.data.DI4_DSP)//？
		 &&(0 == vehicle_io_state.data.DI5_DSP))
		{
			VCU_IOstate.field.In_fwdrev_new = 1;//D档
		}
		else if((0 == vehicle_io_state.data.DI4_DSP)
		      &&(1 == vehicle_io_state.data.DI5_DSP))
		{
			VCU_IOstate.field.In_fwdrev_new = 2;//R档
		}
		else
		{
			VCU_IOstate.field.In_fwdrev_new = 0;//N档
		}

		//以下是挂档策略
		if(VCU_IOstate.field.In_fwdrev_new != VCU_IOstate.field.In_fwdrev)//挂档动作
		{
			if(1 == VCU_IOstate.field.In_fwdrev_new)//前进档
			{
				if(((0 == VCU_IOstate.field.In_fwdrev)||(2 == VCU_IOstate.field.In_fwdrev))  //速度<50 && 非前进状态
				 &&(speed<50))
				{
					Vehicle_cmd.cmdmode.data.gear_swap_trq_flag = 1;   //  制动状态
				}
				VCU_IOstate.field.In_fwdrev = VCU_IOstate.field.In_fwdrev_new;  //更新档位值
				Vehicle_cmd.cmdmode.data.gear_swap_zero_flag = 1;  //  有手刹
				if(speed<0)
				{
					zero_speed_throad = 50-speed;   //设置驻坡速度
				}
				else if(speed<50)
				{
					zero_speed_throad = 50;
				}
				else
				{
					zero_speed_throad = 15;
				}
			}
			else if(2 == VCU_IOstate.field.In_fwdrev_new)//后退档
			{
				if(((0 == VCU_IOstate.field.In_fwdrev)||(1 == VCU_IOstate.field.In_fwdrev))  //速度>-50 && 非后退状态
				 &&(speed>-50))
				{
					Vehicle_cmd.cmdmode.data.gear_swap_trq_flag = 1;  //  制动状态
				}
				VCU_IOstate.field.In_fwdrev = VCU_IOstate.field.In_fwdrev_new;   //更新档位值
				Vehicle_cmd.cmdmode.data.gear_swap_zero_flag = 1;  //  有手刹
				if(speed>0)
				{
					zero_speed_throad = 50+speed;  //驻坡速度
				}
				else if(speed>-50)
				{
					zero_speed_throad = 50;
				}
				else
				{
					zero_speed_throad = 15;
				}
			}
			else//空档
			{
				VCU_IOstate.field.In_fwdrev = VCU_IOstate.field.In_fwdrev_new;  //更新档位值
			}
		}

		switch(VCU_IOstate.field.In_fwdrev)  //更新档位值
		{
			case 1:
				Vehicle_cmd.cmdmode.data.gear_state = 1;
				break;
			case 2:
				Vehicle_cmd.cmdmode.data.gear_state = 2;
				break;
			default:
				Vehicle_cmd.cmdmode.data.gear_state = 0;
				break;
		}

		//判断滑行回馈
		if(0 == Vehicle_cmd.cmdmode.data.slip_feedback_flag)  //滑行反馈标志 = 0 不复位
		{
			if((0 == Vehicle_cmd.cmdmode.data.brake_state)  //无刹车信号
			 &&(speed>=500)  //速度>500
			 &&(canarev_cmd.soc_ref<95)  // 剩余电量<95%  通过CAN传输
			 &&(1 == Vehicle_cmd.cmdmode.data.gear_state)  // 前进状态    0:空挡        1：前进        2：后退
			 &&(0 == Vehicle_cmd.torque_ref)  //无输出力矩
			 &&(sysCfgPara.TrqCmd_AI<=tooth_trq)  //   AI力矩命令值  <=  贴齿力矩
			 )
			{
				Vehicle_cmd.cmdmode.data.slip_feedback_flag = 0; //滑行反馈标志 = 0 不复位  非滑行状态     此处应为1
			}
		}
		else
		{
			if((1 == Vehicle_cmd.cmdmode.data.brake_state) //有刹车信号
			 ||(speed<450) //速度<450
			 ||(canarev_cmd.soc_ref>96)  //剩余电量>96%  通过CAN传输
			 ||(1 != Vehicle_cmd.cmdmode.data.gear_state) //不是前进状态
			 ||(0 != Vehicle_cmd.torque_ref))  //输出力矩不为0
			{
				Vehicle_cmd.cmdmode.data.slip_feedback_flag = 0; //滑行反馈标志 = 0 不复位  非滑行状态
			}
		}
		
		//判断制动回馈
		if(0 == Vehicle_cmd.cmdmode.data.brake_feedback_flag)  //制动时无强断电
		{
			if((1 == Vehicle_cmd.cmdmode.data.brake_state) //刹车信号
			 &&(speed>=500) //速度>500
			 &&(canarev_cmd.soc_ref<95) // 剩余电量<95%  通过CAN传输
			 &&(1 == Vehicle_cmd.cmdmode.data.gear_state) // 前进状态    0:空挡        1：前进        2：后退
			 &&(0 == Vehicle_cmd.torque_ref) //无输出力矩
			 &&(sysCfgPara.TrqCmd_AI<=tooth_trq)) //  AI力矩命令值  <=  贴齿力矩
			{
				Vehicle_cmd.cmdmode.data.brake_feedback_flag = 0; //刹车反馈标志 = 0 制动时无强断电
			}
		}
		else
		{
			if((0 == Vehicle_cmd.cmdmode.data.brake_state)  //无刹车信号
			 ||(speed<450) //速度<450
			 ||(canarev_cmd.soc_ref>96)  //剩余电量>96%  通过CAN传输
			 ||(1 != Vehicle_cmd.cmdmode.data.gear_state)  //不是前进状态
			 ||(0 != Vehicle_cmd.torque_ref))  //输出力矩不为0
			{
				Vehicle_cmd.cmdmode.data.brake_feedback_flag = 0;  //刹车反馈标志 = 0 制动时无强断电
			}
		}
		
		//逆变器开关管指令
		if((0 != Vehicle_cmd.cmdmode.data.gear_state)  //非空挡
		 &&(0 == scsw2.field.fault_flag)  //无故障
		 &&(1 == canarev_cmd.cmdmode2.data.main_relay_state)  //总继电器状态打开
		 &&(0 == canarev_cmd.cmdmode2.data.charge_relay_state)  //充电继电器关闭
		 &&(0 == canarev_cmd.cmdmode2.data.charge_state)  //非充电状态
		 &&(0 == canarev_cmd.cmdmode2.data.charge_line_state)  //充电线状态为0
		 &&(1 == canarev_cmd.cmdmode2.data.BMS_self_state)) //BMS自检正常
		{
			sccw1.field.run_enable              = 1;//逆变器开管
			motor_turn_off_cnt = 0;      //计数清零
		}
		else
		{
			if((0 == Vehicle_cmd.cmdmode.data.gear_state)  //空挡
			 &&(_IQabs(speed) < 100)  //速度的绝对值<100
			 &&(0 == zero_station_trqCmd)  //非驻坡
			 &&(0 == diff_trq_filter)  //未用到
			 &&(0 == sysCfgPara.TrqCmd_AI))  //AI力矩命令值 = 0
			{
				motor_turn_off_cnt++;
				if(motor_turn_off_cnt>400)  //4s
				{
					motor_turn_off_cnt = 400;
					sccw1.field.run_enable = 0;  //逆变器关管
				}
			}
			else
			{
				motor_turn_off_cnt = 0;
			}
		}	

		//力矩给定指令
		if(1 == sccw1.field.run_enable)
		{
			if(1 == Vehicle_cmd.cmdmode.data.gear_state)//前进档
			{
				if(1 == Vehicle_cmd.cmdmode.data.slip_feedback_flag)//滑行回馈  处于滑行状态
				{
					if(speed>=3000)//15kw回馈功率        根据转速确定反馈功率
					{
						trq = 143250/speed;  //计算力矩
						sysCfgPara.TrqCmd_NEW = 0-_IQmpyI32(trq,SysBase.invtorque);  //力矩命令值
					}
					else if(speed>=1500)//47N.m--->10N.m
					{
						trq = _IQmpyI32(37,SysBase.invtorque);  //  37/500
						k_tmp = _IQdiv(trq,_IQmpyI32(1500,SysBase.invspeed));  //    1500/12000
						k_tmp = _IQmpy(_IQmpyI32((speed-1500),SysBase.invspeed),k_tmp);
						trq = _IQmpyI32(10,SysBase.invtorque)+k_tmp;
						sysCfgPara.TrqCmd_NEW = 0-trq;  //力矩命令值
					}
					else if(speed>=500)//10N.m--->0N.m
					{
						trq = _IQmpyI32(10,SysBase.invtorque);
						k_tmp = _IQdiv(trq,_IQmpyI32(1000,SysBase.invspeed));
						k_tmp = _IQmpy(_IQmpyI32((speed-500),SysBase.invspeed),k_tmp);
						sysCfgPara.TrqCmd_NEW = 0-k_tmp;  //力矩命令值
					}
					else
					{
						sysCfgPara.TrqCmd_NEW = 0;
					}
					if(sysCfgPara.TrqCmd_NEW<-trqLoopPara.Max_TorqueGen)   //力矩命令值 < -发电峰值力矩
					{
						sysCfgPara.TrqCmd_NEW = -trqLoopPara.Max_TorqueGen;  //力矩命令值 = -发电峰值力矩
					}
				}
				else if(1 == Vehicle_cmd.cmdmode.data.brake_feedback_flag)// 制动回馈
				{
					if(speed>=3000)//15kw回馈功率
					{
						trq = 143250/speed;
						sysCfgPara.TrqCmd_NEW = 0-_IQmpyI32(trq,SysBase.invtorque);  //力矩命令值
					}
					else if(speed>=1500)//47N.m--->10N.m
					{
						trq = _IQmpyI32(37,SysBase.invtorque);
						k_tmp = _IQdiv(trq,_IQmpyI32(1500,SysBase.invspeed));
						k_tmp = _IQmpy(_IQmpyI32((speed-1500),SysBase.invspeed),k_tmp);  //力矩命令值
						trq = _IQmpyI32(10,SysBase.invtorque)+k_tmp;
						sysCfgPara.TrqCmd_NEW = 0-trq;
					}
					else if(speed>=500)//10N.m--->0N.m
					{
						trq = _IQmpyI32(10,SysBase.invtorque);
						k_tmp = _IQdiv(trq,_IQmpyI32(1000,SysBase.invspeed));
						k_tmp = _IQmpy(_IQmpyI32((speed-500),SysBase.invspeed),k_tmp);
						sysCfgPara.TrqCmd_NEW = 0-k_tmp;  //力矩命令值
					}
					else
					{
						sysCfgPara.TrqCmd_NEW = 0;
					}
					if(sysCfgPara.TrqCmd_NEW<-trqLoopPara.Max_TorqueGen)  //力矩命令值 < -发电峰值力矩
					{
						sysCfgPara.TrqCmd_NEW = -trqLoopPara.Max_TorqueGen;  //力矩命令值 = -发电峰值力矩
					}
				}
				else if(Vehicle_cmd.torque_ref>=0)//前进驱动
				{
					if(1 == Vehicle_cmd.cmdmode.data.brake_state)//有刹车信号
					{
						if(1 == Vehicle_cmd.cmdmode.data.gear_swap_trq_flag)  //制动状态  0:驱动，    1：制动
						{
							sysCfgPara.TrqCmd_NEW = 0;  //力矩命令值 = 0
						}
						else
						{
							sysCfgPara.TrqCmd_NEW = tooth_trq;  //力矩命令值 = 贴齿力矩
						}
					}
					else
					{
						if(3 == VCU_IOstate.field.exit_zero_add_trq_flag)  // 未驻坡
						{
							sysCfgPara.TrqCmd_NEW = _IQ15toIQ(Vehicle_cmd.torque_ref)+zero_station_trqCmd_store; // 力矩命令值 = 前进力矩 + 驻坡力矩
						}
						else  //驻坡
						{
							sysCfgPara.TrqCmd_NEW = _IQ15toIQ(Vehicle_cmd.torque_ref); // 力矩命令值 = 前进力矩
						}
						if(1 != Vehicle_cmd.cmdmode.data.gear_swap_trq_flag)  //驱动状态
						{
							if(sysCfgPara.TrqCmd_NEW<=tooth_trq)  // 力矩命令值 < 贴齿力矩
							{
								sysCfgPara.TrqCmd_NEW = tooth_trq;  // 力矩命令值 = 贴齿力矩
							}
						}
					}
					if(sysCfgPara.TrqCmd_NEW>trqLoopPara.Max_TorqueEle)    // 力矩命令值 > 最大驱动力矩
					{
						sysCfgPara.TrqCmd_NEW = trqLoopPara.Max_TorqueEle; // 力矩命令值 = 最大驱动力矩
					}
				}
				else  //其他情况
				{   
					sysCfgPara.TrqCmd_NEW = 0;
				}
			}
			else if(2 == Vehicle_cmd.cmdmode.data.gear_state)//后退档
			{
				if(1 == Vehicle_cmd.cmdmode.data.brake_state) // 有刹车信号
				{
					if(1 == Vehicle_cmd.cmdmode.data.gear_swap_trq_flag)  //制动状态
					{
						sysCfgPara.TrqCmd_NEW = 0;  //  力矩控制值为0
					}
					else
					{
						sysCfgPara.TrqCmd_NEW = -tooth_trq;  //  力矩控制值 = -贴齿力矩
					}
				}
				else  //无刹车信号
				{
					if(Vehicle_cmd.torque_ref>=0)  //前进驱动
					{
						if(3 == VCU_IOstate.field.exit_zero_add_trq_flag)  //  未驻坡状态
						{
							sysCfgPara.TrqCmd_NEW = 0-_IQ15toIQ(Vehicle_cmd.torque_ref)+zero_station_trqCmd_store;  // 力矩命令值 = -前进力矩 + 驻坡力矩
						}
						else  //驻坡
						{
							sysCfgPara.TrqCmd_NEW = 0-_IQ15toIQ(Vehicle_cmd.torque_ref);  // 力矩命令值 = -前进力矩
						}
						if(1 != Vehicle_cmd.cmdmode.data.gear_swap_trq_flag)  //驱动状态
						{
							if(sysCfgPara.TrqCmd_NEW>=(-tooth_trq))  // 力矩命令值 > 最大贴齿力矩
							{
								sysCfgPara.TrqCmd_NEW = -tooth_trq;  // 力矩命令值 = 最大贴齿力矩
							}
						}
					}
					else
					{
						sysCfgPara.TrqCmd_NEW = 0;
					}
				}
				if(sysCfgPara.TrqCmd_NEW<-trqLoopPara.Max_TorqueEle)  // 力矩命令值 < -最大驱动力矩
				{
					sysCfgPara.TrqCmd_NEW = -trqLoopPara.Max_TorqueEle;  // 力矩命令值 = 最大驱动力矩
				}
			}
			else//空档
			{
				if(_IQabs(speed)<450)  // 速度绝对值 < 450
				{
					sysCfgPara.TrqCmd_NEW = 0;
				}
				else if(_IQabs(speed)>500) // 速度绝对值 > 500
				{
					if(speed>500)//正转
					{
						sysCfgPara.TrqCmd_NEW = 0-_IQmpyI32(5,SysBase.invtorque);  //  设定力矩命令值
					}
					else//反转
					{
						sysCfgPara.TrqCmd_NEW = _IQmpyI32(5,SysBase.invtorque);  //  设定力矩命令值
					}
				}
			}

			if(1 == alarm.field.break_error)  //刹车故障
			{
				sysCfgPara.TrqCmd_NEW = 0;  // 力矩设定值 = 0
			}

			if(1 == Vehicle_cmd.cmdmode.data.gear_state)//前进档
			{
				if(1 == Vehicle_cmd.cmdmode.data.slip_feedback_flag)//滑行能量回馈
				{
					if(_IQabs(sysCfgPara.TrqCmd_AI)<=tooth_trq)   //AI力矩命令值的绝对值 < 贴齿力矩     驻坡模块 -> sysCfgPara.TrqCmd_AI -> 力矩环
					{
						if(sysCfgPara.TrqCmd_AI<=0)  // AI力矩命令值 <= 0
						{
							if((sysCfgPara.TrqCmd_AI == -tooth_trq)  // AI力矩命令值 = -贴齿力矩
							 ||(up_step>tooth_trq_step))  //上升步长 > 贴齿力矩步长
							{
								up_step = tooth_trq_step;  //上升步长 = 贴齿力矩步长
							}
							else
							{
								up_step = up_step-trq_step; //上升步长 = 上升步长 - 最小步长
								if(up_step<trq_step)  //  上升步长 < 最小步长
								{
									up_step = trq_step;  //  上升步长 = 最小步长
								}
							}
							if((sysCfgPara.TrqCmd_AI+up_step)>=0)  // AI力矩命令值 + 上升步长 >= 0
							{
								up_step = _IQabs(sysCfgPara.TrqCmd_AI)+1;  //上升步长 = AI力矩命令值的绝对值 + 1
							}
							
							if((down_step<trq_step)  //下降步长 < 最小步长
							 ||(0 == sysCfgPara.TrqCmd_AI))  // AI力矩命令值 = 0
							{
								down_step = trq_step;  //下降步长 = 最小步长
							}
							else
							{
								down_step = down_step+trq_step;   //  下降步长 = 下降步长 + 最小步长
								if(down_step>max_feedback_down_step)
								{
									down_step = max_feedback_down_step;
								}
							}
						}
						else   // AI力矩命令值 > 0
						{
							up_step = trq_step;  //上升步长 = 最小步长 = 1/50000
							
							if((sysCfgPara.TrqCmd_AI == tooth_trq)  // AI力矩命令值 = 贴齿力矩
							 ||(down_step>tooth_trq_step))  // 下降步长 > 1/2500
							{
								down_step = tooth_trq_step;  // 下降步长 = 1/2500
							}
							else
							{
								down_step = down_step-trq_step;  // 下降步长 - 1/50000
								if(down_step<trq_step)  //下降步长 < 最小步长
								{
									down_step = trq_step;  //下降步长 = 最小步长
								}
							}
							if((sysCfgPara.TrqCmd_AI-down_step)<=0)  //  AI力矩命令值 < 下降步长
							{
								down_step = _IQabs(sysCfgPara.TrqCmd_AI);  //下降步长 =  AI力矩命令值的绝对值
							}
						}
					}
					else  //AI力矩命令值的绝对值 > 贴齿力矩
					{
						if(sysCfgPara.TrqCmd_AI>tooth_trq) // AI力矩命令值 > 贴齿力矩  (+)
						{
							up_step = _IQmpy(_IQ(0.1),SysBase.invtorque);  //上升步长
							down_step = _IQmpy(_IQ(1.0),SysBase.invtorque);  //下降步长
							if((sysCfgPara.TrqCmd_AI-down_step)<=tooth_trq)
							{
								down_step = _IQabs(sysCfgPara.TrqCmd_AI-tooth_trq);
							}
						}
						else  // AI力矩命令值 < -贴齿力矩 (-)
						{
							trq_deta = _IQabs(sysCfgPara.TrqCmd_NEW-sysCfgPara.TrqCmd_AI); //力矩命令值 - AI力矩命令值
							if(sysCfgPara.TrqCmd_AI>=-trq_lmt)  // 力矩命令值 > -力矩极限值
							{
								x_value = _IQabs(sysCfgPara.TrqCmd_AI);
								x_value = -_IQmpy(_IQ(6.25),x_value)+_IQ(0.125);
								up_step = -_IQmpy(_IQ(0.069),x_value)+_IQmpy(_IQ(3.5),SysBase.invtorque);   //上升步长
							
								x_value = _IQabs(sysCfgPara.TrqCmd_AI);
								x_value = _IQmpy(_IQ(6.25),x_value)-_IQ(0.025);
								down_step = _IQmpy(_IQ(0.001),x_value)+_IQmpy(_IQ(0.05),SysBase.invtorque); //下降步长
							}
							else if(trq_deta<trq_lmt)  // 力矩命令值 < 0.02
							{
								x_value = trq_deta;
								x_value = -_IQmpy(_IQ(5.0),x_value)+_IQ(0.1);
								up_step = -_IQmpy(_IQ(0.069),x_value)+_IQmpy(_IQ(3.5),SysBase.invtorque);  //上升步长
								
								x_value = trq_deta;
								x_value = -_IQmpy(_IQ(5.0),x_value)+_IQ(0.1);
								down_step = -_IQmpy(_IQ(0.001),x_value)+_IQmpy(_IQ(0.1),SysBase.invtorque);  //下降步长
							}
							else
							{
								up_step = max_feedback_up_step;  //上升步长
								down_step = max_feedback_down_step;  //下降步长
							}
							//上升步长限制
							if(up_step>max_feedback_up_step)
							{
								up_step = max_feedback_up_step;
							}
							else if(up_step<min_feedback_up_step)
							{
								up_step = min_feedback_up_step;
							}

							if((sysCfgPara.TrqCmd_AI+up_step)>=-tooth_trq)
							{
								up_step = _IQabs(_IQabs(sysCfgPara.TrqCmd_AI)-tooth_trq);
							}
							//下降步长限制
							if(down_step>max_feedback_down_step)
							{
								down_step = max_feedback_down_step;
							}
							else if(down_step<min_feedback_down_step)
							{
								down_step = min_feedback_down_step;
							}
						}
					}
				}
				else if(1 == Vehicle_cmd.cmdmode.data.brake_feedback_flag)//制动能量回馈
				{

					if(_IQabs(sysCfgPara.TrqCmd_AI)<=tooth_trq)  //AI力矩命令值的绝对值 <= 贴齿力矩
					{
						if(sysCfgPara.TrqCmd_AI<=0) //AI力矩命令值 <= 0
						{
							if((sysCfgPara.TrqCmd_AI == -tooth_trq)  // AI力矩命令值 = -贴齿力矩
							 ||(up_step>tooth_trq_step))  //下降步长 > 1/2500
							{
								up_step = tooth_trq_step;  //下降步长 = 1/2500
							}
							else
							{
								up_step = up_step-trq_step;
								if(up_step<trq_step)
								{
									up_step = trq_step;
								}
							}
							if((sysCfgPara.TrqCmd_AI+up_step)>=0)
							{
								up_step = _IQabs(sysCfgPara.TrqCmd_AI)+1;
							}
							
							if((down_step<trq_step)
							 ||(0 == sysCfgPara.TrqCmd_AI))
							{
								down_step = trq_step;
							}
							else
							{
								down_step = down_step+trq_step;
								if(down_step>max_feedback_down_step)
								{
									down_step = max_feedback_down_step;
								}
							}
						}
						else  //AI力矩命令值 > 0
						{
							up_step = trq_step;
							
							if((sysCfgPara.TrqCmd_AI == tooth_trq)
							 ||(down_step>tooth_trq_step))
							{
								down_step = tooth_trq_step;
							}
							else
							{
								down_step = down_step-trq_step;
								if(down_step<trq_step)
								{
									down_step = trq_step;
								}
							}
							if((sysCfgPara.TrqCmd_AI-down_step)<=0)
							{
								down_step = _IQabs(sysCfgPara.TrqCmd_AI);
							}
						}
					}
					else
					{
						if(sysCfgPara.TrqCmd_AI>tooth_trq)
						{
							up_step = _IQmpy(_IQ(0.1),SysBase.invtorque);
							down_step = max_feedback_enter_down_step;
							if((sysCfgPara.TrqCmd_AI-down_step)<=tooth_trq)
							{
								down_step = _IQabs(sysCfgPara.TrqCmd_AI-tooth_trq);
							}
						}
						else
						{
							trq_deta = _IQabs(sysCfgPara.TrqCmd_NEW-sysCfgPara.TrqCmd_AI);
							if(sysCfgPara.TrqCmd_AI>=-trq_lmt)
							{
								x_value = _IQabs(sysCfgPara.TrqCmd_AI);
								x_value = -_IQmpy(_IQ(6.25),x_value)+_IQ(0.125);
								up_step = -_IQmpy(_IQ(0.069),x_value)+_IQmpy(_IQ(3.5),SysBase.invtorque);
							
								x_value = _IQabs(sysCfgPara.TrqCmd_AI);
								x_value = _IQmpy(_IQ(6.25),x_value)-_IQ(0.025);
								down_step = _IQmpy(_IQ(0.001),x_value)+_IQmpy(_IQ(0.05),SysBase.invtorque);
							}
							else if(trq_deta<trq_lmt)
							{
								x_value = trq_deta;
								x_value = -_IQmpy(_IQ(5.0),x_value)+_IQ(0.1);
								up_step = -_IQmpy(_IQ(0.069),x_value)+_IQmpy(_IQ(3.5),SysBase.invtorque);
								
								x_value = trq_deta;
								x_value = -_IQmpy(_IQ(5.0),x_value)+_IQ(0.1);
								down_step = -_IQmpy(_IQ(0.001),x_value)+_IQmpy(_IQ(0.1),SysBase.invtorque);
							}
							else
							{
								up_step = max_feedback_up_step;
								down_step = max_feedback_down_step;
							}
							if(up_step>max_feedback_up_step)
							{
								up_step = max_feedback_up_step;
							}
							else if(up_step<min_feedback_up_step)
							{
								up_step = min_feedback_up_step;
							}
							if((sysCfgPara.TrqCmd_AI+up_step)>=-tooth_trq)
							{
								up_step = _IQabs(_IQabs(sysCfgPara.TrqCmd_AI)-tooth_trq);
							}
							if(down_step>max_feedback_down_step)
							{
								down_step = max_feedback_down_step;
							}
							else if(down_step<min_feedback_down_step)
							{
								down_step = min_feedback_down_step;
							}
						}
					}

				}
				else//前进驱动
				{

					if(_IQabs(sysCfgPara.TrqCmd_AI)<=tooth_trq)
					{
						if(sysCfgPara.TrqCmd_AI<0)
						{
							//200ms
							up_step = tooth_trq_step;
							if((sysCfgPara.TrqCmd_AI+up_step)>=0)
							{
								up_step = _IQabs(sysCfgPara.TrqCmd_AI);
							}

							down_step = trq_step;
						}
						else
						{
							//200ms
							if((up_step<trq_step)   //上升步长<最小步长
							 ||(0 == sysCfgPara.TrqCmd_AI)
							 ||(1 == Vehicle_cmd.cmdmode.data.brake_state)  //刹车信号
							 ||((0 == Vehicle_cmd.torque_ref)&&(sysCfgPara.TrqCmd_NEW <= tooth_trq))
							 ||(1 == scsw2.field.zero_state)  //驻坡
							 ||(1 == Vehicle_cmd.cmdmode.data.gear_swap_trq_flag))  //驻坡
							{
								up_step = trq_step;
							}
							else
							{
								up_step = up_step+trq_step;
								if(up_step>tooth_trq_step)
								{
									up_step = tooth_trq_step;
								}
							}

							//200ms
							if((sysCfgPara.TrqCmd_AI == tooth_trq)
							 ||(down_step>tooth_trq_step))
							{
								down_step = tooth_trq_step;
							}
							else
							{
								down_step = down_step-trq_step;
								if(down_step<trq_step)
								{
									down_step = trq_step;
								}
							}
							if((sysCfgPara.TrqCmd_AI-down_step)<=0)
							{
								down_step = _IQabs(sysCfgPara.TrqCmd_AI)+1;
							}
						}
					}
					else if(sysCfgPara.TrqCmd_AI>tooth_trq)  //sysCfgPara.TrqCmd_AI > 0
					{
						trq_deta = _IQabs(sysCfgPara.TrqCmd_NEW-sysCfgPara.TrqCmd_AI);
						if(sysCfgPara.TrqCmd_AI<=trq_lmt)
						{
							//200ms
							if(up_step<trq_step)
							{
								up_step = trq_step;
							}
							else if(up_step<tooth_trq_step)
							{
								up_step = up_step+trq_step;
								if(up_step>tooth_trq_step)
								{
									up_step = tooth_trq_step;
								}
							}
							else
							{
								up_step = up_step+trq_step_second;
								if(up_step>max_fwd_up_step)
								{
									up_step = max_fwd_up_step;
								}
							}

							//100ms
							x_value = _IQabs(sysCfgPara.TrqCmd_AI);
							x_value = -_IQmpy(_IQ(1.25),x_value)+_IQ(0.025);
							down_step = -_IQmpy(_IQ(0.0925),x_value)+_IQmpy(_IQ(1.05),SysBase.invtorque);
						}
						else if(trq_deta<=trq_lmt)//终点阶段
						{
							x_value = trq_deta;
							x_value = -_IQmpy(_IQ(5.0),x_value)+_IQ(0.1);
							up_step = -_IQmpy(_IQ(0.02),x_value)+_IQmpy(_IQ(1.05),SysBase.invtorque);

							x_value = trq_deta;
							x_value = -_IQmpy(_IQ(5.0),x_value)+_IQ(0.1);
							down_step = -_IQmpy(_IQ(0.02),x_value)+_IQmpy(_IQ(1.05),SysBase.invtorque);
						}
						else if((trq_deta>trq_lmt)
							  &&(sysCfgPara.TrqCmd_AI<sysCfgPara.TrqCmd_NEW))//加速阶段
						{
							if(up_step<trq_step)
							{
								up_step = trq_step;
							}
							else if(up_step<tooth_trq_step)
							{
								up_step = up_step+trq_step;
								if(up_step>tooth_trq_step)
								{
									up_step = tooth_trq_step;
								}
							}
							else
							{
								up_step = up_step+trq_step_third;
								if(up_step>max_fwd_up_step)
								{
									up_step = max_fwd_up_step;
								}
							}

							down_step = trq_step;
						}
						else if((trq_deta>trq_lmt)
							  &&(sysCfgPara.TrqCmd_AI>sysCfgPara.TrqCmd_NEW))//减速阶段
						{
							up_step = trq_step;

							if(down_step<max_fwd_down_step)
							{
								down_step = down_step+_IQmpy(_IQ(0.05),SysBase.invtorque);
							}
							else
							{
								down_step = max_fwd_down_step;
							}
						}
						else
						{
							up_step = max_fwd_up_step;
							down_step = max_fwd_down_step;
						}
						if(up_step>max_fwd_up_step)
						{
							up_step = max_fwd_up_step;
						}
						else if(up_step<trq_step)
						{
							up_step = trq_step;
						}
						if(down_step>max_fwd_down_step)
						{
							down_step = max_fwd_down_step;
						}
						else if(down_step<trq_step)
						{
							down_step = trq_step;
						}
						if((sysCfgPara.TrqCmd_AI-down_step)<=tooth_trq)
						{
							down_step = _IQabs(sysCfgPara.TrqCmd_AI-tooth_trq);
						}
					}
					else  //sysCfgPara.TrqCmd_AI < 0
					{
						up_step = _IQmpy(_IQ(1.5),SysBase.invtorque);
						down_step = trq_step;
						if((sysCfgPara.TrqCmd_AI+up_step)>=-tooth_trq)
						{
							up_step = _IQabs(_IQabs(sysCfgPara.TrqCmd_AI)-tooth_trq);
						}
					}	



				}
			}
			else if(2 == Vehicle_cmd.cmdmode.data.gear_state)//后退档
			{


				if(_IQabs(sysCfgPara.TrqCmd_AI)<=tooth_trq)
				{
					if(sysCfgPara.TrqCmd_AI>0)
					{
						up_step = trq_step;

						//200ms
						if((sysCfgPara.TrqCmd_AI == tooth_trq)
						 ||(down_step > tooth_trq_step))
						{
							down_step = tooth_trq_step;
						}
						else
						{
							down_step = down_step-trq_step;
							if(down_step<trq_step)
							{
								down_step = trq_step;
							}
						}
						if((sysCfgPara.TrqCmd_AI-down_step)<=0)
						{
							down_step = sysCfgPara.TrqCmd_AI;
						}
					}
					else  // sysCfgPara.TrqCmd_AI < 0
					{
						//200ms
						if((sysCfgPara.TrqCmd_AI == -tooth_trq)
						 ||(up_step>tooth_trq_step))
						{
							up_step = tooth_trq_step;
						}
						else
						{
							up_step = up_step-trq_step;
							if(up_step<trq_step)
							{
								up_step = trq_step;
							}
						}
						if((sysCfgPara.TrqCmd_AI+up_step)>=0)
						{
							down_step = _IQabs(sysCfgPara.TrqCmd_AI);
						}
						
						//200ms
						if((down_step<trq_step)
						 ||(0 == sysCfgPara.TrqCmd_AI)
						 ||(1 == Vehicle_cmd.cmdmode.data.brake_state)
						 ||((0 == Vehicle_cmd.torque_ref)&&(sysCfgPara.TrqCmd_NEW >= -tooth_trq))
						 ||(1 == scsw2.field.zero_state)
						 ||(1 == Vehicle_cmd.cmdmode.data.gear_swap_trq_flag))
						{
							down_step = trq_step;
						}
						else
						{
							down_step = down_step+trq_step;
							if(down_step>tooth_trq_step)
							{
								down_step = tooth_trq_step;
							}
						}
					}
				}
				else if(sysCfgPara.TrqCmd_AI>tooth_trq)
				{
					up_step = trq_step;
					
					if(sysCfgPara.TrqCmd_AI>trq_lmt)
					{
						down_step = _IQmpy(_IQ(1.5),SysBase.invtorque);
					}
					else
					{
						if(down_step>_IQmpy(_IQ(1.5),SysBase.invtorque))
						{
							down_step = _IQmpy(_IQ(1.5),SysBase.invtorque);
						}
						else
						{
							down_step = down_step-_IQmpy(_IQ(0.065),SysBase.invtorque);
							if(down_step<tooth_trq_step)
							{
								down_step = tooth_trq_step;
							}
						}
					}
					if((sysCfgPara.TrqCmd_AI-down_step)<=tooth_trq)
					{
						down_step = _IQabs(sysCfgPara.TrqCmd_AI-tooth_trq);
					}
				}
				else
				{
					trq_deta = _IQabs(sysCfgPara.TrqCmd_NEW-sysCfgPara.TrqCmd_AI);
					if(sysCfgPara.TrqCmd_AI>=-trq_lmt)
					{
						//200ms
						if(up_step>max_rev_up_step)
						{
							up_step = max_rev_up_step;
						}
						else
						{
							up_step = up_step-_IQmpy(_IQ(0.06875),SysBase.invtorque);
							if(up_step<tooth_trq_step)
							{
								up_step = tooth_trq_step;
							}
						}

						//200ms
						if(down_step<trq_step)
						{
							down_step = trq_step;
						}
						else if(down_step<tooth_trq_step)
						{
							down_step = down_step+trq_step;
							if(down_step>tooth_trq_step)
							{
								down_step = tooth_trq_step;
							}
						}
						else
						{
							down_step = down_step+_IQmpy(_IQ(0.00625),SysBase.invtorque);
							if(down_step>max_rev_down_step)
							{
								down_step = max_rev_down_step;
							}
						}
					}
					else if(trq_deta<trq_lmt)
					{
						//500ms
						if(up_step>max_rev_up_step)
						{
							up_step = max_rev_up_step;
						}
						else
						{
							up_step = up_step-_IQmpy(_IQ(0.03),SysBase.invtorque);
							if(up_step<trq_step)
							{
								up_step = trq_step;
							}
						}

						//500ms
						if(down_step>max_rev_down_step)
						{
							down_step = max_rev_down_step;
						}
						else
						{
							down_step = down_step-_IQmpy(_IQ(0.0048),SysBase.invtorque);
							if(down_step<trq_step)
							{
								down_step = trq_step;
							}
						}
					}
					else
					{
						if(sysCfgPara.TrqCmd_AI>sysCfgPara.TrqCmd_NEW)//加速阶段
						{
							up_step = trq_step;
							
							if(down_step<max_rev_down_step)
							{
								down_step = down_step+_IQmpy(_IQ(0.0125),SysBase.invtorque);
							}
							else
							{
								down_step = max_rev_down_step;
							}
						}
						else//减速阶段
						{
							if(up_step<max_rev_up_step)
							{
								up_step = up_step+_IQmpy(_IQ(0.075),SysBase.invtorque);
							}
							else
							{
								up_step = max_rev_up_step;
							}
							
							down_step = trq_step;
						}
					}
					if(up_step>max_rev_up_step)
					{
						up_step = max_rev_up_step;
					}
					else if(up_step<trq_step)
					{
						up_step = trq_step;
					}
					if((sysCfgPara.TrqCmd_AI+up_step)>=-tooth_trq)
					{
						up_step = _IQabs(_IQabs(sysCfgPara.TrqCmd_AI)-tooth_trq);
					}
					if(down_step>max_rev_down_step)
					{
						down_step = max_rev_down_step;
					}
					else if(down_step<trq_step)
					{
						down_step = trq_step;
					}
				}

			}
			else//空档
			{
				if(_IQabs(sysCfgPara.TrqCmd_AI)<=tooth_trq)
				{
					up_step = trq_n_step_updown;
					down_step = trq_n_step_updown;
				}
				else
				{
					if(sysCfgPara.TrqCmd_AI>tooth_trq)
					{
						up_step = trq_n_step_updown;
						down_step = gear_n_step;
						if((sysCfgPara.TrqCmd_AI-down_step)<=tooth_trq)
						{
							down_step = sysCfgPara.TrqCmd_AI-tooth_trq;
						}
					}
					else
					{
						up_step = gear_n_step;
						if((sysCfgPara.TrqCmd_AI+up_step)>=-tooth_trq)
						{
							up_step = _IQabs(sysCfgPara.TrqCmd_AI)-tooth_trq;
						}
						down_step = trq_n_step_updown;
					}
				}
			}
			
			if(sysCfgPara.TrqCmd_NEW>sysCfgPara.TrqCmd_AI)
			{
				sysCfgPara.TrqCmd_AI = sysCfgPara.TrqCmd_AI+up_step;
				if(sysCfgPara.TrqCmd_AI>sysCfgPara.TrqCmd_NEW)
				{
					sysCfgPara.TrqCmd_AI = sysCfgPara.TrqCmd_NEW;
				}
			}
			else if(sysCfgPara.TrqCmd_NEW<sysCfgPara.TrqCmd_AI)
			{
				sysCfgPara.TrqCmd_AI = sysCfgPara.TrqCmd_AI-down_step;
				if(sysCfgPara.TrqCmd_AI<sysCfgPara.TrqCmd_NEW)
				{
					sysCfgPara.TrqCmd_AI = sysCfgPara.TrqCmd_NEW;
				}
			}
		}
		else
		{
			sysCfgPara.TrqCmd_NEW	               = 0;
			sysCfgPara.TrqCmd_AI                   = 0;
		}
	}
	else
	{
		VCU_IOstate.field.In_fwdrev 	           = 0;
		VCU_IOstate.field.In_fwdrev_new            = 0;
		sysCfgPara.TrqCmd_NEW                      = 0;
		sysCfgPara.TrqCmd_AI                       = 0;
	}
}

void AIFeedback(void)//油门反馈
{
	int16 i = 0;
    static int16 ConversionCnt = 0;
    _iq AI1_ad  = 0;
	_iq AI1_ad_offset = 0;
	_iq tmp = 0;
	_iq ai_deta = 0;
	_iq AI1_ad_tmp = 0;

	if(TRQCMD_SOURCE_AI == EV_MCU_Para.field.TrqCmd_SourceType)   //力矩命令来源 = 0 AI1
	{
		if(sysFbkPara.AI1_fbk_ad>EV_MCU_Para.field.AI1_offset_off)  // 油门的AD采样值 > 2350 = 100%
		{
			AI1_ad_tmp = EV_MCU_Para.field.AI1_offset_off+1; // 2350 + 1 = 100%
		}
		else
		{
			AI1_ad_tmp = sysFbkPara.AI1_fbk_ad;  // 反之赋值为当前采样值
		}
		if(0 == AD_AISelf_Flag)
		{
			AD_AISelf_Cnt++;
			AD_AI1_Sum += AI1_ad_tmp;   //AD采样累加值
			if(AD_AISelf_Cnt>=128)  //1280ms
			{
				AD_AISelf_Flag = 1;
				AD_AISelf_Cnt = 0;
				AI1_ad_offset = AD_AI1_Sum>>7;  //计算平均ad采样值
				if(AI1_ad_offset > (EV_MCU_Para.field.AI1_offset_on-20))  //550  (<550 认为是0)
				{
					emsw2.field.Torque1_err = 1;  // 置位错误标志
				}
				for (i = 0; i < AI_BUFFER_LENGTH; i++)  // AI_BUFFER_LENGTH = 8
				{
					AI1_Ad_Buffer[i]  = AI1_ad_offset;  //将平均AD采样值放入缓存数组
				}
			}
		}
		else
		{
			if(1 == emsw2.field.Torque1_err)    // 错误标志为1
			{
				AD_AI1_Sum = AD_AI1_Sum-(AD_AI1_Sum>>7)+AI1_ad_tmp;  //AD采样累加值 = AD采样累加值 - 平均值 + 当前值  (滤波)
				AI1_ad_offset = AD_AI1_Sum>>7; // 最新的平均值
				if(AI1_ad_offset<(EV_MCU_Para.field.AI1_offset_on-30))  // 平均值 < 550 -30
				{
					emsw2.field.Torque1_err = 0;  // 清除错误标志
				}
				for (i = 0; i < AI_BUFFER_LENGTH; i++) // AI_BUFFER_LENGTH = 8
				{
					AI1_Ad_Buffer[i]  = AI1_ad_offset; //将平均AD采样值放入缓存数组
				}
			}
			else // 错误标志为0
			{
				AI1_Ad_Buffer[ConversionCnt] = AI1_ad_tmp;   //将当前AD采样值放入 缓存数组
				for (i = 0; i < AI_BUFFER_LENGTH; i++ )
				{
					AI1_ad += AI1_Ad_Buffer[i];  //将缓存累加
				}
				AI1_ad  = AI1_ad>>3;  //缓存平均值
				ConversionCnt++;
				ConversionCnt = ConversionCnt%AI_BUFFER_LENGTH;  // ConversionCnt 0-7 循环

				if(AI1_ad<EV_MCU_Para.field.AI1_offset_on)   // 缓存平均值 < 550
				{
					AI1_ad = EV_MCU_Para.field.AI1_offset_on;  // 缓存平均值 = 550
				}
				if(AI1_ad>EV_MCU_Para.field.AI1_offset_off)  // 缓存平均值 > 2350
				{
					AI1_ad = EV_MCU_Para.field.AI1_offset_off;  // 缓存平均值 = 2350
				}
				AI1_ad = AI1_ad-EV_MCU_Para.field.AI1_offset_on;  // 缓存平均值 - 550

				ai_deta = (EV_MCU_Para.field.AI1_offset_off-EV_MCU_Para.field.AI1_offset_on);  //2350 - 550
				tmp = _IQ15div(AI1_ad,ai_deta);  // 线性比例
				sysFbkPara.AI1_fbk = _IQ15mpy(tmp,EV_MCU_Para.field.Motor_PeakTorqueEle);//IQ15   //线性比例 * 峰值扭矩  150
				Vehicle_cmd.torque_ref = sysFbkPara.AI1_fbk;//IQ15
				
				if((1 == Vehicle_cmd.cmdmode.data.brake_state) //刹车状态
				 &&(Vehicle_cmd.torque_ref>0))  //力矩值>0
				{
					AD_AISelf_Cnt++;
					if(AD_AISelf_Cnt > 400)   //4S
					{
						alarm.field.break_error = 1;//刹车故障
						AD_AISelf_Cnt         = 500;
					}
				}
				else
				{
					AD_AISelf_Cnt = 0;
					alarm.field.break_error = 0;
				}
			}
		}
	}
	else
	{
		emsw2.field.Torque1_err = 0;
		emsw2.field.Torque2_err = 0;
	}
}

void motorParameterInit(void)
{
	sysFbkPara.V12_fbk = sysFbkPara.V12_fbk_filter = _IQmpyI32(12,SysBase.invudc);
	sysFbkPara.V12_fbk_HMI = _IQtoIQ15(sysFbkPara.V12_fbk_filter);
	Motor_Encoder_offset_current = Motor_Encoder_offset = (EV_MCU_Para.field.Motor_Encoder_offset<<3);
	Motor_Encoder_offset_coeff = EV_MCU_Para.field.Motor_Encoder_offset_elecoeff;
}

void motorLookupOffsetTable(void)
{
	_iq speed = 0;
	_iq encoder_index_pre = 0;
	_iq encoder_index_nex = 0;
	_iq encoder_deta_speed = 0;
	_iq encoder_speed_abs = 0;
	_iq encoder_current_value = 0;
	_iq deta_speed = 0;
	_iq deta_encoder = 0;
	_iq k_encoder = 0;
	_iq Motor_Encoder_offset_base = 0;
	int16 *table = (int *)0;

	speed = _IQ15mpy(sysFbkPara.Speed_fbk_HMI,SysBase.speed);
	if(1 == EV_MCU_Para.field.Motor_Direction)
	{
		speed = 0-speed;
	}
	if(speed<-500)
	{
		Motor_Encoder_offset_base = (EV_MCU_Para.field.Motor_Encoder_offset<<3);
		table = (int *)(&encoder_offsetrev[0]);
	}
	else
	{
		Motor_Encoder_offset_base = (EV_MCU_Para.field.Motor_Encoder_offset<<3);
		table = (int *)(&encoder_offsetfwd[0]);
	}
	if(4 == EV_MCU_Para.field.Weak_enable)
	{
		Motor_Encoder_offset_current = Motor_Encoder_offset_base;
	}
	else
	{
		encoder_speed_abs = _IQabs(sysFbkPara.Speed_fbk_Filter);
		if(encoder_speed_abs<TRQ_Table.encoder_minspeed)
		{
			Motor_Encoder_offset_current = Motor_Encoder_offset_base;
		}
		else if(encoder_speed_abs>=TRQ_Table.encoder_maxspeed)
		{
			encoder_index_pre = encoder_index_nex = TRQ_Table.encoder_num-1;
			Motor_Encoder_offset_current = Motor_Encoder_offset_base+table[encoder_index_nex];
		}
		else
		{
			encoder_deta_speed = encoder_speed_abs-TRQ_Table.encoder_minspeed;
			encoder_index_pre  = encoder_deta_speed/TRQ_Table.encoder_detaspeed;
			encoder_index_nex  = encoder_index_pre+1;
			deta_speed = encoder_deta_speed-_IQmpyI32(encoder_index_pre,TRQ_Table.encoder_detaspeed);
			deta_encoder = table[encoder_index_nex]-table[encoder_index_pre];
			k_encoder = _IQdiv(deta_speed,TRQ_Table.encoder_detaspeed);
			k_encoder = _IQmpy(k_encoder,deta_encoder);
			encoder_current_value = table[encoder_index_pre]+k_encoder;
			Motor_Encoder_offset_current = Motor_Encoder_offset_base+encoder_current_value;
		}
		while((Motor_Encoder_offset_current<0)||(Motor_Encoder_offset_current>=_IQ15(1.0)))
		{
			if(Motor_Encoder_offset_current<0)
			{
				Motor_Encoder_offset_current = Motor_Encoder_offset_current+_IQ15(1.0);
			}
			else
			{
				Motor_Encoder_offset_current = Motor_Encoder_offset_current-_IQ15(1.0);
			}
		}
	}

	if(_IQmpy(trqLoop_trqRampref ,sysFbkPara.Speed_fbk ) >= 0)
	{
		Motor_Encoder_offset_coeff = EV_MCU_Para.field.Motor_Encoder_offset_elecoeff;
	}
	else
	{
		Motor_Encoder_offset_coeff = EV_MCU_Para.field.Motor_Encoder_offset_gencoeff;
	}
}

void v12Feedback(void)
{
	sysFbkPara.V12_fbk = _IQmpyI32(sysFbkPara.V12_fbk_ad,EV_MCU_Para.field.V12_adjustCoeff);//IQ24-BASE
	LPFilter(sysFbkPara.V12_fbk,&sysFbkPara.V12_fbk_filter,_IQ(0.3),_IQ(0.7));//IQ24-BASE
	sysFbkPara.V12_fbk_HMI = _IQtoIQ15(sysFbkPara.V12_fbk_filter);//IQ15-BASE

	motor_sys_init_cnt++;
	if(motor_sys_init_cnt>250)
	{
		motor_sys_init_cnt = 250;
	}
	if(motor_sys_init_cnt>=250)
	{
		if(sysFbkPara.V12_fbk_HMI > EV_MCU_Para.field.Motor_Protect_V12Ov)
		{
			emsw1.field.V12_overVolt = 1;
		}
		else
		{
			if((1 == emsw1.field.V12_overVolt)
			 &&(1 == sccw2.field.fault_recover))
			{
				emsw1.field.V12_overVolt = 0;
			}
		}

		if(sysFbkPara.V12_fbk_HMI < EV_MCU_Para.field.Motor_Protect_V12Lv)
		{
			emsw1.field.V12_lackVolt = 1;
		}
		else
		{
			if((1 == emsw1.field.V12_lackVolt)
			 &&(1 == sccw2.field.fault_recover))
			{
				emsw1.field.V12_lackVolt = 0;
			}
		}
	}
}

void trqFeedback(void)
{
	if(1 == scsw2.field.igbt_state)
	{
		trqLoopPara.Torque_fbk_HMI = trqLoopPara.Torque_ref_HMI;
	}
	else
	{
		trqLoopPara.Torque_fbk_HMI = 0;
	}
}

_iq tempLookupMtrTable(Uint16 temp_type,_iq temp_ad,const int* table)
{
	int  i = 0;
	_iq tempa = 0;
	_iq tempc = 0;
	_iq tempd = 0;
	_iq tempe = 0;
	
	switch(temp_type)
	{
		case TEMPSENSOR_PT100:
			tempa = temp_ad;//????AD
			tempd = table[52];//210???AD-???AD?
			if(tempa>tempd)//????210?
			{
				i = 51;//205????
				while(0 != i)
				{
					tempc = table[i];//???????AD?
					tempe = i*50-500;//???????(???10?)
					if(tempc<tempa)
					{
						tempd = tempc;
						i--;
					}
					else if(tempc>tempa)
					{
						tempa = tempa-tempc;
						tempc = tempd-tempc;
						tempa = (tempa*50)/tempc+tempe;
						return tempa;
					}
					else
					{
						tempa = tempe;
						return tempa;
					}
				}
				tempa = -500;//??-50?(???10?)
				return tempa;
			}
			else
			{
				tempa = 2100;
				return tempa;//??210?(???10?)
			}
			//break;
		case TEMPSENSOR_PT1000:
			tempa = temp_ad;//????AD
			tempd = table[52];//210???AD-???AD?
			if(tempa>tempd)//????210?
			{
				i = 51;//205????
				while(0 != i)
				{
					tempc = table[i];//???????AD?
					tempe = i*50-500;//???????(???10?)
					if(tempc<tempa)
					{
						tempd = tempc;
						i--;
					}
					else if(tempc>tempa)
					{
						tempa = tempa-tempc;
						tempc = tempd-tempc;
						tempa = (tempa*50)/tempc+tempe;
						return tempa;
					}
					else
					{
						tempa = tempe;
						return tempa;
					}
				}
				tempa = -500;//??-500?(???10?)
				return tempa;
			}
			else
			{
				tempa = 2100;
				return tempa;//??210?(???10?)
			}
			//break;
		case TEMPSENSOR_NTC_10K:
			tempa = temp_ad;//????AD
			tempd = table[250];//210???AD-???AD?
			if(tempa<tempd)//????210?
			{
				i = 249;//205????
				while(0 != i)
				{
					tempc = table[i];//???????AD?
					tempe = i*10-400;//???????(???10?)
					if(tempc>tempa)
					{
						tempd = tempc;
						i--;
					}
					else if(tempc<tempa)
					{
						tempa = tempa-tempc;
						tempc = tempd-tempc;
						tempa = (tempa*10)/tempc+tempe;
						return tempa;
					}
					else
					{
						tempa = tempe;
						return tempa;
					}
				}
				tempa = -400;//??-50?(???10?)
				return tempa;
			}
			else
			{
				tempa = 2100;
				return tempa;//??210?(???10?)
			}
			//break;
		case TEMPSENSOR_NTC_30K:
			tempa = temp_ad;//????AD
			tempd = table[50];//210???AD-???AD?
			if(tempa<tempd)//????210?
			{
				i = 49;//205????
				while(0 != i)
				{
					tempc = table[i];//???????AD?
					tempe = i*50-400;//???????(???10?)
					if(tempc>tempa)
					{
						tempd = tempc;
						i--;
					}
					else if(tempc<tempa)
					{
						tempa = tempa-tempc;
						tempc = tempd-tempc;
						tempa = (tempa*50)/tempc+tempe;
						return tempa;
					}
					else
					{
						tempa = tempe;
						return tempa;
					}
				}
				tempa = -400;//??-500?(???10?)
				return tempa;
			}
			else
			{
				tempa = 2100;
				return tempa;//??210?(???10?)
			}
			//break;
		case TEMPSENSOR_NTC_100K:
			tempa = temp_ad;//????AD
			tempd = table[151];//210???AD-???AD?
			if(tempa<tempd)//????210?
			{
				i = 150;//205????
				while(0 != i)
				{
					tempc = table[i];//???????AD?
					tempe = i*10-300;//???????(???10?)
					if(tempc>tempa)
					{
						tempd = tempc;
						i--;
					}
					else if(tempc<tempa)
					{
						tempa = tempa-tempc;
						tempc = tempd-tempc;
						tempa = (tempa*10)/tempc+tempe;
						return tempa;
					}
					else
					{
						tempa = tempe;
						return tempa;
					}
				}
				tempa = -300;//??-500?(???10?)
				return tempa;
			}
			else
			{
				tempa = 1250;
				return tempa;//??210?(???10?)
			}
			//break;
		case TMP_MTR_NTC_15C054:
			tempa = temp_ad;//????AD
			tempd = table[250];//210???AD-???AD?
			if(tempa<tempd)//????210?
			{
				i = 249;//205????
				while(0 != i)
				{
					tempc = table[i];//???????AD?
					tempe = i*10-400;//???????(???10?)
					if(tempc>tempa)
					{
						tempd = tempc;
						i--;
					}
					else if(tempc<tempa)
					{
						tempa = tempa-tempc;
						tempc = tempd-tempc;
						tempa = (tempa*10)/tempc+tempe;
						return tempa;
					}
					else
					{
						tempa = tempe;
						return tempa;
					}
				}
				tempa = -400;//??-50?(???10?)
				return tempa;
			}
			else
			{
				tempa = 2100;
				return tempa;//??210?(???10?)
			}
			//break;
		case TEMPSENSOR_KTY84_150:
			tempa = temp_ad;//????AD
			tempd = table[24];//210???AD-???AD?
			if(tempa>tempd)//????210?
			{
				i = 23;//205????
				while(0 != i)
				{
					tempc = table[i];//???????AD?
					tempe = i*100-400;//???????(???10?)
					if(tempc<tempa)
					{
						tempd = tempc;
						i--;
					}
					else if(tempc>tempa)
					{
						tempa = tempa-tempc;
						tempc = tempd-tempc;
						tempa = (tempa*100)/tempc+tempe;
						return tempa;
					}
					else
					{
						tempa = tempe;
						return tempa;
					}
				}
				tempa = -400;//??-50?(???10?)
				return tempa;
			}
			else
			{
				tempa = 1900;
				return tempa;//??210?(???10?)
			}
		default:
			tempa = 2100;
			return tempa;//??210?(???10?)
	}
}

_iq tempLookupTable(_iq temp,const int* table)
{
	int  i = 0;
	_iq tempa = 0;
	_iq tempc = 0;
	_iq tempd = 0;
	_iq tempe = 0;
	
	tempa = temp;//????AD
	tempd = table[165];//295???AD-???AD?
	if(tempa>tempd)//????295?
	{
		i = 164;//115????
		while(0 != i)
		{
			tempc = table[i];//???????AD?
			tempe = i*10-400;//???????(???10?)
			if(tempc<tempa)
			{
				tempd = tempc;
				i--;
			}
			else if(tempc>tempa)
			{
				tempa = tempc-tempa;
				tempc = tempc-tempd;
				tempa = (tempa*10)/tempc+tempe;
				return tempa;
			}
			else
			{
				tempa = tempe;
				return tempa;
			}
		}
		tempa = -400;//??-30?(???10?)
		return tempa;
	}
	else
	{
		tempa = 1250;
		return tempa;//??1200?(???10?)
	}
}

void tempFeedback(void)
{
	_iq temp = 0;
	
	sysFbkPara.Temp_mtr_ad          = (sysFbkPara.Temp_mtr_ad*EV_MCU_Para.field.Mtrtemp_adjustCoeff)>>10;
	sysFbkPara.Temp_mdl_ad_filter	= (sysFbkPara.Temp_mdl_ad*28+sysFbkPara.Temp_mdl_ad_filter*100)>>7;
	sysFbkPara.Temp_mtr_ad_filter	= (sysFbkPara.Temp_mtr_ad*28+sysFbkPara.Temp_mtr_ad_filter*100)>>7;
	sysFbkPara.TA_ad_filter         = (sysFbkPara.TA_ad*28+sysFbkPara.TA_ad_filter*100)>>7;
	
	temp = tempLookupTable(sysFbkPara.Temp_mdl_ad_filter,&TMP_NTC_TAB[0]);
	sysFbkPara.Temp_igbt = temp/10;
	temp = tempLookupTable(sysFbkPara.TA_ad_filter,&TMP_NTC_TAB[0]);
	sysFbkPara.Temp_ta = temp/10;
	sysFbkPara.Temp_mdl = sysFbkPara.Temp_igbt;//max(sysFbkPara.Temp_igbt,sysFbkPara.Temp_ta);
	
	if(motor_sys_init_cnt>=250)
	{
		if(sysFbkPara.Temp_mdl_ad_filter<1140)
		{
			emsw1.field.mdlsensor_short_error = 1;
		}
		else if(sysFbkPara.Temp_mdl_ad_filter>3354)
		{
			emsw1.field.mdlsensor_open_error = 1;
		}
		else
		{
			if(sysFbkPara.Temp_mdl > EV_MCU_Para.field.Motor_Protect_OverTempCtr)
			{
				emsw.field.mdl_temp = 1;
			}
			else
			{
				if((1 == emsw.field.mdl_temp)
				 &&(1 == sccw2.field.fault_recover))
				{
					emsw.field.mdl_temp = 0;
				}
			}
		}
	}

    switch (EV_MCU_Para.field.Motor_TempSensor_type)
	{
		case TEMPSENSOR_PT100:
			temp = tempLookupMtrTable(TEMPSENSOR_PT100,sysFbkPara.Temp_mtr_ad_filter,&TMP_MTR_PT100_TAB[0]);
			sysFbkPara.Temp_mtr = temp/10;
			break;
		case TEMPSENSOR_PT1000:
			temp = tempLookupMtrTable(TEMPSENSOR_PT1000,sysFbkPara.Temp_mtr_ad_filter,&TMP_MTR_PT1000_TAB[0]);
			sysFbkPara.Temp_mtr = temp/10;
			break;
		case TEMPSENSOR_NTC_10K:
			temp = tempLookupMtrTable(TEMPSENSOR_NTC_10K,sysFbkPara.Temp_mtr_ad_filter,&TMP_MTR_NTC_10K_TAB[0]);
			sysFbkPara.Temp_mtr = temp/10;
			break;
		case TEMPSENSOR_NTC_30K:
			temp = tempLookupMtrTable(TEMPSENSOR_NTC_30K,sysFbkPara.Temp_mtr_ad_filter,&TMP_MTR_NTC_30K_TAB[0]);
			sysFbkPara.Temp_mtr = temp/10;
			break;
		case TEMPSENSOR_NTC_100K:
			temp = tempLookupMtrTable(TEMPSENSOR_NTC_100K,sysFbkPara.Temp_mtr_ad_filter,&TMP_MTR_NTC_100K_TAB[0]);
			sysFbkPara.Temp_mtr = temp/10;
			break;
		case TMP_MTR_NTC_15C054:
			temp = tempLookupMtrTable(TMP_MTR_NTC_15C054,sysFbkPara.Temp_mtr_ad_filter,&TMP_MTR_NTC_15C054_TAB[0]);
			sysFbkPara.Temp_mtr = temp/10;
			break;
		case TEMPSENSOR_KTY84_150:
			temp = tempLookupMtrTable(TEMPSENSOR_KTY84_150,sysFbkPara.Temp_mtr_ad_filter,&TMP_MTR_KTY84_150_TAB[0]);
			sysFbkPara.Temp_mtr = temp/10;
			break;
		default:
			sysFbkPara.Temp_mtr=sysFbkPara.Temp_mdl+3;
			break;
	}

	if((1 == sccw2.field.mtrtempFbk_ChkMode)
	 &&(motor_sys_init_cnt>=250))
	{
		if(sysFbkPara.Temp_mtr_ad_filter > 3480)//电机温度短路故障
		{
			sysFbkPara.Temp_mtr         = -50;
			emsw1.field.mtrsensor_open_error = 1;
		}
		else if(sysFbkPara.Temp_mtr_ad_filter<880)//电机温度断路故障
		{
			sysFbkPara.Temp_mtr         = 210;
			emsw1.field.mtrsensor_short_error = 1;
		}
		else
		{
			if (sysFbkPara.Temp_mtr > EV_MCU_Para.field.Motor_Protect_OverTempMtr)
			{
				emsw.field.mtr_temp = 1;
			}
			else
			{
				if((1 == emsw.field.mtr_temp)
				 &&(1 == sccw2.field.fault_recover))
				{
					emsw.field.mtr_temp = 0;
					emsw1.field.mtrsensor_short_error=0;
					emsw1.field.mtrsensor_open_error=0;
				}
			}
			
			if(sysFbkPara.Temp_mtr>EV_MCU_Para.field.Motor_Protect_AlarmTempMtr)
			{
				GpioDataRegs.GPACLEAR.bit.GPIO9 = 1;
			}
			else if(sysFbkPara.Temp_mtr<(EV_MCU_Para.field.Motor_Protect_AlarmTempMtr-5))
			{
				GpioDataRegs.GPASET.bit.GPIO9 = 1;
			}
		}
	}
	else
	{
		emsw.field.mtr_temp = 0;
		emsw1.field.mtrsensor_short_error=0;
		emsw1.field.mtrsensor_open_error=0;
	}
	if(LimitI2CUpdate_en)
	{
		if(I2CValueLimit_Record.field.mdl_temp < sysFbkPara.Temp_mdl)
		{
			I2CValueLimit_Record.field.mdl_temp = sysFbkPara.Temp_mdl;
			eeprom_valuelimit_index[5]    = 1;
			LimitI2CRecord_en             = 1;
		}
		if(I2CValueLimit_Record.field.mtr_temp < sysFbkPara.Temp_mtr)
		{
			I2CValueLimit_Record.field.mtr_temp = sysFbkPara.Temp_mtr;
			eeprom_valuelimit_index[6]    = 1;
			LimitI2CRecord_en             = 1;
		}
	}
}

#pragma CODE_SECTION(Resolver_read, 	"ramfuncs");
void Resolver_read(void)
{	
}

#pragma CODE_SECTION(ThetaCal_Resolver, 	"ramfuncs");
void ThetaCal_Resolver()
{
	_iq fTemp = 0;
	_iq fTemp1 = 0;
	_iq dir_encoder = 0;
	_iq Tmp = 0;
	
	if(MOTOR_CLOCKWISE == EV_MCU_Para.field.Motor_Encoder_Direction)
	{
		sysFbkPara.Theta_fbk_raw = mtrPara.Encoder_rollOver.value - sysFbkPara.Theta_fbk_raw;
	}
	fTemp = _IQmpy((sysFbkPara.Theta_fbk_raw<<12),SysBase.mtr_ratio);
	Theta_fbk_raw_n_1 = Theta_fbk_raw_n;
	Theta_fbk_raw_n = fTemp;
	Theta_fbk_raw_deta_n_1 = Theta_fbk_raw_deta_n;
	Theta_fbk_raw_deta_n = Theta_fbk_raw_n-Theta_fbk_raw_n_1;
	if(1 == resolver_fault_type)
	{
		Tmp = Theta_fbk_raw_deta_n-Theta_fbk_raw_deta_n_1;//IQ24
		if(Tmp>=_IQ(0.5))
		{
			Tmp = Tmp-_IQ(1.0);
		}
		else if(Tmp<=-_IQ(0.5))
		{
			Tmp = Tmp+_IQ(1.0);
		}
		Theta_fbk_raw_deta_err = _IQabs(Tmp);//IQ24abs	
		if(Theta_fbk_raw_deta_err>_IQ12toIQ(EV_MCU_Para.field.resolver_fault_theta_maxerr))
		{
			Theta_fbk_fault_cnt++;
		}
		Theta_fbk_sample_cnt++;
		if(Theta_fbk_sample_cnt>=100)
		{
			Theta_fbk_sample_cnt = 0;
			if(Theta_fbk_fault_cnt>=10)
			{
				emsw.field.resolver_error = 1;
			}
			Theta_fbk_fault_cnt = 0;
		}
	}
	else
	{
		Theta_fbk_sample_cnt = 0;
		Theta_fbk_fault_cnt = 0;
	}

	if((MTR_VOLT_LOOP != scsw2.field.runLoop_state)
	 &&(SYS_DIAGNOSIS != sysState))
	{
		dir_encoder = Motor_Encoder_offset-Motor_Encoder_offset_current;//IQ15
		if(Motor_Encoder_offset<Motor_Encoder_offset_current)
		{
			if(_IQabs(dir_encoder)<_IQ15(0.5))
			{
				Motor_Encoder_cnt++;
				if(Motor_Encoder_cnt>=1)
				{
					Motor_Encoder_cnt = 0;
					Motor_Encoder_offset = Motor_Encoder_offset+8;
				}
			}
			else
			{
				Motor_Encoder_cnt++;
				if(Motor_Encoder_cnt>=1)
				{
					Motor_Encoder_cnt = 0;
					Motor_Encoder_offset = Motor_Encoder_offset-8;
				}
			}
		}
		else if(Motor_Encoder_offset>Motor_Encoder_offset_current)
		{
			if(_IQabs(dir_encoder)<_IQ15(0.5))
			{
				Motor_Encoder_cnt++;
				if(Motor_Encoder_cnt>=1)
				{
					Motor_Encoder_cnt = 0;
					Motor_Encoder_offset = Motor_Encoder_offset-8;
				}
			}
			else
			{
				Motor_Encoder_cnt++;
				if(Motor_Encoder_cnt>=1)
				{
					Motor_Encoder_cnt = 0;
					Motor_Encoder_offset = Motor_Encoder_offset+8;
				}
			}
		}
		else
		{
			Motor_Encoder_cnt = 0;
		}
		fTemp1 = Motor_Encoder_offset+((Motor_Encoder_offset_coeff-2048)<<3);//IQ15		
		sysFbkPara.Theta_fbk_Ele = fTemp - (fTemp1<<9);//IQ24
	}
	else
	{
		fTemp1 = EV_MCU_Para.field.Motor_Encoder_offset+(Motor_Encoder_offset_coeff-2048);
		sysFbkPara.Theta_fbk_Ele = fTemp - (fTemp1<<12);//IQ24
	}
	while((sysFbkPara.Theta_fbk_Ele >= _IQ(1))
		||(sysFbkPara.Theta_fbk_Ele < 0))
	{
		if (sysFbkPara.Theta_fbk_Ele >= _IQ(1))
		{
			sysFbkPara.Theta_fbk_Ele -= _IQ(1);
		}
		else if (sysFbkPara.Theta_fbk_Ele < 0)
		{
			sysFbkPara.Theta_fbk_Ele += _IQ(1);
		}
	}
	sysFbkPara.Theta_fbk_Ele_HMI = sysFbkPara.Theta_fbk_Ele>>12;//IQ12
}

//#pragma CODE_SECTION(SpeedCal_Resolver, 	"ramfuncs");
void SpeedCal_Resolver()
{
	_iq	fTemp1 = 0;

	SpeedCal_Count++;
	if(SpeedCal_Count>=EV_MCU_Para.field.SpdLoop_Cycle)
	{
		SpeedCal_Count = 0;
		SpeedCal_ThetaEle = sysFbkPara.Theta_fbk_Ele;
		if(0 == SpeedCal_First_Flag)
		{
			SpeedCal_First_Flag = 1;
			SpeedCal_ThetaEleOld = SpeedCal_ThetaEle;
		}
		SpeedCal_deltaTime = _IQmpyI32(sysCfgPara.Omega_period.value,EV_MCU_Para.field.SpdLoop_Cycle);
		SpeedCal_deltaTheta     = SpeedCal_ThetaEle - SpeedCal_ThetaEleOld;
		SpeedCal_ThetaEleOld    = SpeedCal_ThetaEle;
		if (SpeedCal_deltaTheta > _IQ(0.5))
		{
			SpeedCal_deltaTheta -= _IQ(1);
		}
		else if (SpeedCal_deltaTheta < -_IQ(0.5))
		{
			SpeedCal_deltaTheta += _IQ(1);
		}

		fTemp1			= _IQmpyI32(SysBase.invomega,1000);  //时间是以ms为最小单位，故需要乘1000
		SpeedCal_spdFbk = _IQdiv(_IQmpy(SpeedCal_deltaTheta,fTemp1),(SpeedCal_deltaTime*EV_MCU_Para.field.Motor_Np)); //速度计算

		if(MOTOR_CLOCKWISE == EV_MCU_Para.field.Motor_Direction)
	    {
	    	SpeedCal_spdFbk = 0-SpeedCal_spdFbk;
	    }
		sysFbkPara.Speed_fbk = SpeedCal_spdFbk;
		sysFbkPara.Omega_fbk_Mech = sysFbkPara.Speed_fbk;
		sysFbkPara.Omega_fbk_Ele = _IQmpyI32(sysFbkPara.Omega_fbk_Mech , (long)EV_MCU_Para.field.Motor_Np);
		LPFilter(sysFbkPara.Omega_fbk_Mech,&sysFbkPara.Speed_fbk_Filter,_IQ(0.1),_IQ(0.9));
		if(_IQabs(sysFbkPara.Speed_fbk_Filter) < SysBase.invspeed)
		{
			sysFbkPara.Speed_fbk_Filter = 0;
		}
		if(_IQabs(sysFbkPara.Speed_fbk_Filter)>(_IQ15toIQ(EV_MCU_Para.field.Motor_PeakSpeedFwd)+_IQ(0.05)))
		{
			emsw2.field.data_overflow = 1;
		}
		else
		{
			if((emsw2.field.data_overflow == 1)&&(sccw2.field.fault_recover == 1))
			{
				emsw2.field.data_overflow = 0;
			}
		}
	    sysFbkPara.Speed_fbk_HMI = _IQtoIQ15(sysFbkPara.Speed_fbk_Filter);
	    sysFbkPara.Speed_fbk_HMI_abs = _IQ15abs(sysFbkPara.Speed_fbk_HMI);
		sysFbkPara.Speed = _IQ15mpy(sysFbkPara.Speed_fbk_HMI,SysBase.speed);
		sysFbkPara.Speed_abs = _IQabs(sysFbkPara.Speed);
	    SpeedFbk_FaultChecking();
	    SpeedDetectProtect();
		if(LimitI2CUpdate_en)
		{
			fTemp1 = sysFbkPara.Speed_abs;
			if(I2CValueLimit_Record.field.speed < fTemp1)
			{
				I2CValueLimit_Record.field.speed = fTemp1;
				eeprom_valuelimit_index[3]    = 1;
				LimitI2CRecord_en             = 1;
			}
		}
#if 0
		LPFilter(sysFbkPara.Speed_fbk,&sysFbkPara.Speed_fbk_Filter_Lowspeed,_IQ(0.005),_IQ(0.995));
#else
		LPFilter(sysFbkPara.Speed_fbk,&sysFbkPara.Speed_fbk_Filter_Lowspeed,_IQ(0.3),_IQ(0.7));
		LPFilter(sysFbkPara.Speed_fbk,&Speed_fbk_Filter_Stop_Lowspeed,_IQ(0.005),_IQ(0.995));
#endif
	}
}

#endif

