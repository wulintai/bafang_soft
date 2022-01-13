/*
 * sysFSM.c
 *
 *  Created on: 2014-3-19
 *      Author: ThinkPad
 */

#ifndef _SYSFSM_C
#define _SYSFSM_C

#include "include_c.h"

//static Uint16 speed_ref_cnt = 0;

void sysFSMInit()
{
	int i = 0;
	
	sysState = SYS_POWERDOWN;
	for(i=0;i<SYSFSM_STATE_NUM;i++)
	{
		switch(i)
		{
		case SYS_INIT:     //系统初始化
			sysFSMState[i].cond=&sysInitCond;    //弱点自检
			sysFSMState[i].action=&sysInitAct;   //系统初始化，参数设置
			sysFSMState[i].entrance=&sysInitEntrance;  //参数初始化/清零
			sysFSMState[i].export=&sysInitExport;  //空
			break;
		case SYS_READY:    //准备状态
			sysFSMState[i].cond=&sysReadyCond;   //强电自检
			sysFSMState[i].action=&sysReadyAct;  //同上的系统初始化
			sysFSMState[i].entrance=&sysReadyEntrance; //参数初始化/清零  同上除了设置系统状态
			sysFSMState[i].export=&sysReadyExport; //空
			break;
		case SYS_RUN:      //运行
			sysFSMState[i].cond=&sysRunCond;     //系统自检
			sysFSMState[i].action=&sysRunAct;    //设置参数
			sysFSMState[i].entrance=&sysRunEntrance;  //设置运行状态
			sysFSMState[i].export=&sysRunExport; //空
			break;
		case SYS_SPDLMT:   //限速
			sysFSMState[i].cond=&sysSpdLmtCond;  //限速判断
			sysFSMState[i].action=&sysSpdLmtAct; //
			sysFSMState[i].entrance=&sysSpdLmtEntrance;
			sysFSMState[i].export=&sysSpdLmtExport;
			break;
		case SYS_ZEROSTOP: //驻坡
			sysFSMState[i].cond=&sysZeroStopCond;
			sysFSMState[i].action=&sysZeroStopAct;
			sysFSMState[i].entrance=&sysZeroStopEntrance;
			sysFSMState[i].export=&sysZeroStopExport;
			break;
		case SYS_POWERDOWN://下电
			sysFSMState[i].cond=&sysPowerdownCond;
			sysFSMState[i].action=&sysPowerdownAct;
			sysFSMState[i].entrance=&sysPowerdownEntrance;
			sysFSMState[i].export=&sysPowerdownExport;
			break;
		case SYS_DIAGNOSIS://自检
			sysFSMState[i].cond= &sysDiagnosisCond;
			sysFSMState[i].action= &sysDiagnosisAct;
			sysFSMState[i].entrance= &sysDiagnosisEntrance;
			sysFSMState[i].export= &sysDiagnosisExport;
			break;
		case SYS_FAULT:    //故障
			sysFSMState[i].cond=&sysFaultCond;
			sysFSMState[i].action=&sysFaultAct;
			sysFSMState[i].entrance=&sysFaultEntrance;
			sysFSMState[i].export=&sysFaultExport;
			break;
		case SYS_UDCLOOP:  //电压环
			sysFSMState[i].cond=&sysUdcloopCond;
			sysFSMState[i].action=&sysUdcloopAct;
			sysFSMState[i].entrance=&sysUdcloopEntrance;
			sysFSMState[i].export=&sysUdcloopExport;
			break;
		default:           //默认
			sysFSMState[i].cond=&sysPowerdownCond;
			sysFSMState[i].action=&sysPowerdownAct;
			sysFSMState[i].entrance=&sysPowerdownEntrance;
			sysFSMState[i].export=&sysPowerdownExport;
			break;
		}
	}
}


void sysFSMCtrl()
{
	int i = 0;
	
	if ((*sysFSMState[sysState].cond)())
	{
		(*sysFSMState[sysState].action)();
	}
	else
	{
		for (i = 0; i < SYSFSM_STATE_NUM; i++)
		{
			if ((*sysFSMState[i].cond)())
			{
				(*sysFSMState[sysState].export)();	//退出当前系统状态
				(*sysFSMState[i].entrance)();		//进入新系统状态
				(*sysFSMState[i].action)();			//立即执行
				break;
			}
		}
	}
}

//系统弱电自检完成
int sysInitCond()
{
	if (
		scsw2.field.adCalib_state       	  && !scsw2.field.fault_flag
		&& !sccw1.field.shutoff 			  && !scsw2.field.vdc_state
		)
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

void sysInitAct()
{
	sccw2.field.run_enable	    = 0;                //运行使能
	sccw2.field.runLoop_mode    = sccw1.field.runLoop_mode    = MTR_TRQ_LOOP;    //力矩环
}

void sysInitEntrance()
{
	ClearManualCtrlCmd();							// 清除人工命令给定
	sccw2.field.run_enable	    = 0;
	sccw2.field.runLoop_mode    = sccw1.field.runLoop_mode    = MTR_TRQ_LOOP;
	scsw1.field.system_state	= SYS_INIT;
	sysState					= SYS_INIT;
}

void sysInitExport()
{

}


//系统强电上电完成，未开管
int sysReadyCond()
{
	if (
		!sccw2.field.diag_enable              && !scsw2.field.fault_flag     //未进入电机自学习&&无故障
		&& !sccw1.field.shutoff 			  &&  scsw2.field.vdc_state      //上电&&预充电完成
		&& scsw2.field.adCalib_state          && !sccw1.field.run_enable     //AD校验完成&&逆变器未开管
		)
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

void sysReadyAct()
{
	sccw2.field.run_enable		= 0;  //运行使能
	sccw2.field.runLoop_mode    = sccw1.field.runLoop_mode    = MTR_TRQ_LOOP;      //力矩环
}

void sysReadyEntrance()
{
	ClearManualCtrlCmd();							// 清除人工命令给定

	sccw2.field.run_enable		= 0;
	sccw2.field.runLoop_mode    = sccw1.field.runLoop_mode    = MTR_TRQ_LOOP;
	scsw1.field.system_state	= SYS_READY;
	sysState					= SYS_READY;
}

void sysReadyExport()
{

}

//系统运行状态
int sysRunCond()
{
	if (
		!sccw2.field.diag_enable	&& !scsw2.field.fault_flag       //未进入自学习&&无故障
		&& !sccw1.field.shutoff		&& sccw1.field.run_enable        //上电&&逆变器打开
		&& !scsw2.field.SpdLmt_state&& !scsw2.field.zero_state       //未限速&&未驻坡
		&& scsw2.field.vdc_state	&& scsw2.field.adCalib_state     //预充电完成&&AD校验完成
		&& !scsw2.field.Udcloop_state                                //不是电压环
		)
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

void sysRunAct()
{
	sccw2.field.runLoop_mode    = sccw1.field.runLoop_mode;     //设置运行环
	sccw2.field.run_enable      = sccw1.field.run_enable;       //逆变器状态
}

void sysRunEntrance()
{
	sccw2.field.run_enable      = sccw1.field.run_enable;       //逆变器状态
	scsw1.field.system_state	= SYS_RUN;
	sysState					= SYS_RUN;
}

void sysRunExport()
{

}
//电压闭环
int sysUdcloopCond()
{
	if(
	sccw1.field.run_enable 	  && !scsw2.field.fault_flag
	&& !sccw1.field.shutoff	  && !sccw2.field.diag_enable
	&& !scsw2.field.zero_state && !scsw2.field.SpdLmt_state
	&& scsw2.field.Udcloop_state
	)
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

void sysUdcloopAct()
{
#if 0
	udcloop.udcloop_udcspeed = sysFbkPara.Speed_abs;
	udcloop.udcloop_powerlmt = EV_MCU_Para.field.Udcloop_PowerLimt;
	udcloop.udcloop_trqlmt   = udcloop.udcloop_powerlmt*9550/udcloop.udcloop_udcspeed;
	udcloop.udcloop_trqlmt   = udcloop.udcloop_trqlmt*SysBase.invtorque;
	pid_udcloop.Fdb     	 = sysFbkPara.Vdc_filter_protect;
	pid_udcloop.Ref          = _IQ15toIQ(EV_MCU_Para.field.Udcloop_Udcref);
	pid_udcloop.Kp           = _IQ15toIQ(EV_MCU_Para.field.Udcloop_kp);
	pid_udcloop.Ki           = _IQ15toIQ(EV_MCU_Para.field.Udcloop_ki);
	pid_udcloop.Kd           = 0;
	pid_udcloop.Out_fwd      = 0;
	pid_udcloop.OutMax       = udcloop.udcloop_trqlmt;
	pid_udcloop.OutMin       = -udcloop.udcloop_trqlmt;
	pidreg3_calc(&pid_udcloop);
	LPFilter(pid_udcloop.Out,&udcloop.udcloop_trqCmd,_IQ(0.5),_IQ(0.5));
	if(sysFbkPara.Speed_fbk_HMI>0)
	{
		sysCfgPara.TrqCmd  			= -udcloop.udcloop_trqCmd;
	}
	else
	{
		sysCfgPara.TrqCmd  			= udcloop.udcloop_trqCmd;
	}
#endif

	sccw1.field.runLoop_mode 	= MTR_TRQ_LOOP;
	sccw2.field.run_enable      = sccw1.field.run_enable;
}

void sysUdcloopEntrance()
{
	sccw2.field.runLoop_mode 	= MTR_TRQ_LOOP;
	sccw2.field.run_enable      = sccw1.field.run_enable;
	scsw1.field.system_state	= SYS_UDCLOOP;
	sysState					= SYS_UDCLOOP;
	//pidreg3_init(&pid_udcloop);
	//udcloop.udcloop_trqCmd      = pid_udcloop.Ui              = trqLoop_trqRampref;
}

void sysUdcloopExport()
{
	sccw2.field.runLoop_mode 	= MTR_TRQ_LOOP;
	scsw2.field.Udcloop_state   = 0;
}

//超速速度环限速
int sysSpdLmtCond()
{
	if (
		sccw1.field.run_enable 	&& !scsw2.field.fault_flag         //逆变器打开&&无故障
		&& !sccw1.field.shutoff	&&!sccw2.field.diag_enable         //上电状态&&未自学习
		&&  scsw2.field.vdc_state&& scsw2.field.adCalib_state      //预充电完成&&AD校验完成
		&&  scsw2.field.SpdLmt_state                               //有限速标志
		)
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

void sysSpdLmtAct()
{
	_iq speed_limit = 0;
	static Uint16 speed_deta_count = 0;

	if(scsw2.field.SpdLmt_state == 1)   //正向限速
	{
		if(1 == VCU_IOstate.field.In_fwdrev)     //正转
		{
			//sysCfgPara.SpdCmd =_IQ15toIQ(EV_MCU_Para.field.Spd_FWD_E_Limt_Spd);

			if(1 == vehicle_io_state.data.DI6_DSP)//低速档
			{
				speed_limit = _IQ15toIQ(EV_MCU_Para.field.Spd_FWD_L_Limt_Spd);   //4500转
			}
			else//高速档
			{
				speed_limit = _IQ15toIQ(EV_MCU_Para.field.Spd_FWD_Limt_Spd);     //4500转
			}
			speed_deta_count++;
			if(speed_deta_count>50)//Delay 100ms
			{
				speed_deta_count = 0;
				if(spdlmt_cmd>speed_limit)   //速度环的速度命令值>限速值
				{
					spdlmt_cmd = spdlmt_cmd-_IQmpyI32(10,SysBase.invspeed);   //速度环的速度命令值-10
					if(spdlmt_cmd<speed_limit)
					{
						spdlmt_cmd = speed_limit;    //速度环的速度命令值不小于限速值
					}
				}
				else if(spdlmt_cmd<speed_limit)
				{
					spdlmt_cmd = spdlmt_cmd+_IQmpyI32(10,SysBase.invspeed);
					if(spdlmt_cmd>speed_limit)
					{
						spdlmt_cmd = speed_limit;
					}
				}
			}
			sysCfgPara.SpdCmd = spdlmt_cmd;    //设置速度命令值
		}
		else
		{
			//sysCfgPara.SpdCmd =_IQ15toIQ(EV_MCU_Para.field.Spd_FWD_Limt_Spd);

			speed_limit = _IQ15toIQ(EV_MCU_Para.field.Spd_FWD_Limt_Spd);   //4500转
			speed_deta_count++;
			if(speed_deta_count>50)//Delay 100ms
			{
				speed_deta_count = 0;
				if(spdlmt_cmd>speed_limit)
				{
					spdlmt_cmd = spdlmt_cmd-_IQmpyI32(10,SysBase.invspeed);
					if(spdlmt_cmd<speed_limit)
					{
						spdlmt_cmd = speed_limit;
					}
				}
				else if(spdlmt_cmd<speed_limit)
				{
					spdlmt_cmd = spdlmt_cmd+_IQmpyI32(10,SysBase.invspeed);
					if(spdlmt_cmd>speed_limit)
					{
						spdlmt_cmd = speed_limit;
					}
				}
			}
			sysCfgPara.SpdCmd = spdlmt_cmd;//设置速度命令值
		}
	}
	else if(scsw2.field.SpdLmt_state == 2)   //反转
	{
		if(1 == vehicle_io_state.data.DI6_DSP)//低速档
		{
			speed_limit = 0-_IQ15toIQ(EV_MCU_Para.field.Spd_FWD_E_Limt_Spd);  //1600转
		}
		else//高速档
		{
			speed_limit = 0-_IQ15toIQ(EV_MCU_Para.field.Spd_REV_Limt_Spd);    //750转
		}
		speed_deta_count++;
		if(speed_deta_count>50)//Delay 200ms
		{
			speed_deta_count = 0;
			if(spdlmt_cmd>speed_limit)
			{
				spdlmt_cmd = spdlmt_cmd-_IQmpyI32(10,SysBase.invspeed);
				if(spdlmt_cmd<speed_limit)
				{
					spdlmt_cmd = speed_limit;
				}
			}
			else if(spdlmt_cmd<speed_limit)
			{
				spdlmt_cmd = spdlmt_cmd+_IQmpyI32(10,SysBase.invspeed);
				if(spdlmt_cmd>speed_limit)
				{
					spdlmt_cmd = speed_limit;
				}
			}
		}
		sysCfgPara.SpdCmd = spdlmt_cmd;  //设置速度命令值
	}
	sccw2.field.run_enable     = sccw1.field.run_enable;    //逆变器状态
	sccw2.field.runLoop_mode   = MTR_SPEED_LOOP;            //设置为速度环

}
void sysSpdLmtEntrance()
{
	sccw2.field.run_enable     = sccw1.field.run_enable;
	sccw2.field.runLoop_mode   = MTR_SPEED_LOOP;
	scsw1.field.system_state	= SYS_SPDLMT;
	sysState					= SYS_SPDLMT;
}

void sysSpdLmtExport()
{
	sccw2.field.runLoop_mode 	= MTR_TRQ_LOOP;
	scsw2.field.SpdLmt_state    = 0;

	sysCfgPara.TrqCmd_AIfilter = sysCfgPara.TrqCmd_AI = trqLoop_trqRampref;
	up_step = down_step = _IQmpy(_IQ(0.01),SysBase.invtorque);
}


//驻坡
int sysZeroStopCond()
{
	if (
		sccw1.field.run_enable 	  && !scsw2.field.fault_flag
		&& !sccw1.field.shutoff	  && !sccw2.field.diag_enable
		&& scsw2.field.zero_state && !scsw2.field.SpdLmt_state
		)
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

void sysZeroStopAct()
{
	//_iq tooth_trq = 0;
	_iq speed_exit_max = 0;

	//tooth_trq = _IQ15toIQ(EV_MCU_Para.field.Tooth_trq);
	speed_exit_max = _IQmpyI32(100,SysBase.invspeed);
	if(2 == Vehicle_cmd.cmdmode.data.gear_state)
	{
		speed_exit_max = 0-speed_exit_max;
	}
	LPFilter(SpeedCal_spdFbk,&zero_station_SpeedCal_spdFbk,_IQ(0.5),_IQ(0.5));
	if(0 == VCU_IOstate.field.zero_or_start_flag)
	{
		if(1 == VCU_IOstate.field.exit_zero_add_trq_flag)
		{
			if(pid_station.Ref>speed_exit_max)
			{
				pid_station.Ref = pid_station.Ref-zero_speed_step;
				if(pid_station.Ref<=speed_exit_max)
				{
					pid_station.Ref = speed_exit_max;
					VCU_IOstate.field.exit_zero_add_trq_flag = 2;
				}
			}
			else if(pid_station.Ref<speed_exit_max)
			{
				pid_station.Ref = pid_station.Ref+zero_speed_step;
				if(pid_station.Ref>=speed_exit_max)
				{
					pid_station.Ref = speed_exit_max;
					VCU_IOstate.field.exit_zero_add_trq_flag = 2;
				}
			}
			else
			{
				VCU_IOstate.field.exit_zero_add_trq_flag = 2;
			}
		}
		else
		{
			if(pid_station.Ref>0)
			{
				pid_station.Ref = pid_station.Ref-zero_speed_step;
				if(pid_station.Ref<0)
				{
					pid_station.Ref = 0;
				}
			}
			else if(pid_station.Ref<0)
			{
				pid_station.Ref = pid_station.Ref+zero_speed_step;
				if(pid_station.Ref>0)
				{
					pid_station.Ref = 0;
				}
			}
		}
	}
	else
	{
		if(pid_station.Ref>speed_start_up)
		{
			pid_station.Ref = pid_station.Ref-zero_speed_step;
			if(pid_station.Ref<=speed_start_up)
			{
				pid_station.Ref = speed_start_up;
				Vehicle_cmd.cmdmode.data.gear_swap_trq_flag = 0;
			}
		}
		else if(pid_station.Ref<speed_start_up)
		{
			pid_station.Ref = pid_station.Ref+zero_speed_step;
			if(pid_station.Ref>=speed_start_up)
			{
				pid_station.Ref = speed_start_up;
				Vehicle_cmd.cmdmode.data.gear_swap_trq_flag = 0;
			}
		}
		else
		{
			Vehicle_cmd.cmdmode.data.gear_swap_trq_flag = 0;
		}
	}

	pid_station.Fdb       		= zero_station_SpeedCal_spdFbk;
	if(0 == VCU_IOstate.field.zero_or_start_flag)
	{
		pid_station.Kp		  		= _IQ12toIQ(EV_MCU_Para.field.Zero_Sation_kp);  //IQ12
		pid_station.Ki			  	= _IQ12toIQ(EV_MCU_Para.field.Zero_Sation_ki);
	}
	else
	{
		pid_station.Kp		  		= _IQ12toIQ(EV_MCU_Para.field.EliJitter_Stop_Kp);  //IQ12
		pid_station.Ki			  	= _IQ12toIQ(EV_MCU_Para.field.EliJitter_Stop_Kd);
	}
	pid_station.Kd            	= 0;
	pid_station.Out_fwd         = 0;

#if 0
	if(1 == Vehicle_cmd.cmdmode.data.gear_state)//前进档
	{
		pid_station.OutMax		  	= trqLoopPara.Max_TorqueEle;  //IQ12
		if(zero_station_trqLmt>tooth_trq)
		{
			pid_station.OutMin = tooth_trq;
		}
		else
		{
			pid_station.OutMin		  	= zero_station_trqLmt;
		}
	}
	else//后退档
	{
		if(zero_station_trqLmt<-tooth_trq)
		{
			pid_station.OutMax = -tooth_trq;
		}
		else
		{
			pid_station.OutMax		  	= zero_station_trqLmt;
		}
		pid_station.OutMin		  	= -trqLoopPara.Max_TorqueEle;
	}
#else
	pid_station.OutMax		  	= trqLoopPara.Max_TorqueEle;  //IQ12
	pid_station.OutMin		  	= -trqLoopPara.Max_TorqueEle;
#endif
	pidreg3_calc(&pid_station);
	if(0 == VCU_IOstate.field.zero_or_start_flag)
	{
		LPFilter(pid_station.Out,&zero_station_trqCmd,_IQ(0.5),_IQ(0.5));
	}
	else
	{
		LPFilter(pid_station.Out,&zero_station_trqCmd,_IQ(0.1),_IQ(0.9));
	}

	sccw2.field.run_enable      = sccw1.field.run_enable;
	sccw1.field.runLoop_mode 	= MTR_TRQ_LOOP;
    sysCfgPara.TrqCmd  			= zero_station_trqCmd;

	if(1 == VCU_IOstate.field.zero_disable_flag)
	{
		zero_station_deltaTheta     = SpeedCal_deltaTheta;
		if(MOTOR_CLOCKWISE == EV_MCU_Para.field.Motor_Direction)
		{
			zero_station_deltaTheta = 0-SpeedCal_deltaTheta;
		}
		zero_station_deltaThetasum = zero_station_deltaThetasum+zero_station_deltaTheta;
		
		if(1 == Vehicle_cmd.cmdmode.data.gear_state)
		{
			if(zero_station_deltaThetasum < -_IQ(1.0)*EV_MCU_Para.field.Zero_Sation_lengthlmt)
	        {
				sysCfgPara.TrqCmd            = 0;
				emsw2.field.zerostaion_error = 1;
	        }
		}
		else if(2 == Vehicle_cmd.cmdmode.data.gear_state)
		{
			if(zero_station_deltaThetasum > _IQ(1.0)*EV_MCU_Para.field.Zero_Sation_lengthlmt)
			{
				sysCfgPara.TrqCmd            = 0;
			    emsw2.field.zerostaion_error = 1;
			}
		}
	}
}

void sysZeroStopEntrance()
{
	sccw2.field.runLoop_mode 	= MTR_TRQ_LOOP;
	sccw2.field.run_enable      = sccw1.field.run_enable;
	scsw1.field.system_state	= SYS_ZEROSTOP;
	sysState					= SYS_ZEROSTOP;
	zero_station_SpeedCal_spdFbk= SpeedCal_spdFbk;
	//zero_station_deltaTheta     = SpeedCal_deltaTheta;
	zero_station_deltaThetasum  = 0;
	pidreg3_init(&pid_station);
	zero_station_trqCmd         = pid_station.Ui   = pid_station.Out = trqidqmap_trq;//IQ24
	diff_trq_filter = diff_stop_trq_filter = 0;
	zero_station_trqLmt = trqidqmap_trq;
	zero_speed_step = _IQmpyI32(1,SysBase.invspeed);
	pid_station.Ref = sysFbkPara.Speed_fbk_Filter;//IQ24
	if(1 == Vehicle_cmd.cmdmode.data.gear_state)
	{
		if(sysFbkPara.Speed_fbk_Filter<0)
		{
			speed_start_up = _IQmpyI32(50,SysBase.invspeed);
		}
		else
		{
			speed_start_up = sysFbkPara.Speed_fbk_Filter+_IQmpyI32(50,SysBase.invspeed);
		}
	}
	else
	{
		if(sysFbkPara.Speed_fbk_Filter>0)
		{
			speed_start_up = -_IQmpyI32(50,SysBase.invspeed);
		}
		else
		{
			speed_start_up = sysFbkPara.Speed_fbk_Filter-_IQmpyI32(50,SysBase.invspeed);
		}
	}
	//speed_ref_cnt = 0;
}

void sysZeroStopExport()
{
	sccw2.field.runLoop_mode 	= MTR_TRQ_LOOP;
	scsw2.field.zero_state    	= 0;
	zero_station_deltaThetasum  = 0;

	sysCfgPara.TrqCmd_AIfilter = sysCfgPara.TrqCmd_AI = trqLoop_trqRampref;
}

//系统下电
int sysPowerdownCond()
{
	if (!sccw1.field.fault_diague && !scsw2.field.fault_flag
		&& sccw1.field.shutoff    && !sccw1.field.acdc_state
		&&!sccw2.field.diag_enable)
	{
		return 1;
	}
		return 0;
}
void sysPowerdownAct()
{
	sccw2.field.run_flag=0;
   	sccw2.field.runLoop_mode=MTR_TRQ_LOOP;
}

void sysPowerdownEntrance()
{
	sccw2.field.lv_check=0;
	scsw1.field.system_state	= SYS_POWERDOWN;
	sysState=SYS_POWERDOWN;
}

void sysPowerdownExport()
{
	sccw2.field.lv_check=1;
}


//系统故障状态
int sysFaultCond()
{
	if (scsw2.field.fault_flag)
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

void sysFaultAct()
{
	if((scsw2.field.error_rank == 4) || (sysFbkPara.Speed_abs<700))
	{
		sccw2.field.run_enable		= 0;
	}
	else
	{
		sccw2.field.run_enable		= sccw1.field.run_enable;
	}

	if(emsw.field.hv_fault)
	{
		scsw1.field.fault_state=0;            //0:不允许恢复故障
	}
	else
	{
		if(sccw1.field.run_enable || scsw2.field.igbt_state)
		{
			scsw1.field.fault_state=0;        //0:不允许恢复故障
		}
		else
		{
			scsw1.field.fault_state=1;        //1:允许恢复
			scsw1.field.fault_state=0;        //1:允许恢复
		}
	}
	sccw2.field.fault_recover=scsw1.field.fault_state;
	FaultTrqLmtCoeff = 0;
	sccw2.field.runLoop_mode=MTR_TRQ_LOOP;
}

void sysFaultEntrance()
{
	scsw1.field.system_state	= SYS_FAULT;
	sysState					= SYS_FAULT;
}

void sysFaultExport()
{
	sccw2.field.fault_recover		= 0;              //0:不允许恢复故障
}

//自学习
int sysDiagnosisCond()
{
	if(	sccw2.field.diag_enable && !scsw2.field.fault_flag
		&&scsw2.field.vdc_state) //
	{
		return(1);
	}
	return 0;
}
void sysDiagnosisAct()
{
	//diagEncoffsetVar.finshflag = 1;//test
	diagFSMCtrl();
}
void sysDiagnosisEntrance()
{
	ClearManualCtrlCmd();                           // 清除人工命令给定
	sccw2.field.fault_recover = 0;			        //不允许恢复故障
	sccw1.field.run_enable = 0;
	sccw2.field.run_enable = 0;
	scsw1.field.system_state  	= SYS_DIAGNOSIS;
	sysState                    = SYS_DIAGNOSIS;

	diagFSM_runLoopModeOld       = scsw2.field.runLoop_state;
	diagFSM_thetaModeOld         = sccw2.field.theta_mode;
}

void sysDiagnosisExport()
{
	(*diagFSMState[diagState].export)();  //退出当前诊断状态
	ClearManualCtrlCmd();
	sccw1.field.run_enable      = 0;
	sccw2.field.run_enable      = 0;
	sccw2.field.diag_enable     = 0;
	scsw2.field.runLoop_state	= diagFSM_runLoopModeOld;
	sccw2.field.theta_mode		= diagFSM_thetaModeOld;
	sccw2.field.diag_mode		   = DIAG_NON_TEST;
	diagState					   = DIAG_NON_TEST;
	EV_MCU_Para.field.Weak_enable  = epdw.field.Weak_enable;
	EV_MCU_Para.field.TrqCmd_SourceType = epdw.field.TrqCmd_SourceType;
	EV_MCU_Para.field.SpdCmd_SourceType = epdw.field.SpdCmd_SourceType;
	EV_MCU_Para.field.VoltFeedfwd_Coeff = epdw.field.VoltFeedfwd_Coeff;
}

#endif /* SYSFSM_C_ */

