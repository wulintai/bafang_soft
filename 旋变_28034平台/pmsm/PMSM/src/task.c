#ifndef _TASK_C_
#define _TASK_C_

#include "include_c.h"

/**********************************************************************************
*Function name:				taskInit
*Parameter:					None
*Return value:				None
*Date:						2017-01-05
**********************************************************************************/
void taskInit(void)
{
	Uint16 task_index = 0;

	for(task_index=0;task_index<TASK_NUM;task_index++)
	{
		task_struct[task_index].task_cnt = 0;
		task_struct[task_index].task_flag = 0;
		task_struct[task_index].task_period = 0;
		task_struct[task_index].task_enable = TASK_DISABLE;
		task_struct[task_index].task_func = (TASK_FUNC_PTR)0;
	}
	for(task_index=0;task_index<TASK_NUM;task_index++)
	{
		switch(task_index)
		{
			case TASK500US:
				task_struct[TASK500US].task_cnt = 0;
				task_struct[TASK500US].task_flag = TASK_CLEAR_FLAG;
				task_struct[TASK500US].task_period = 2;
				task_struct[TASK500US].task_enable = TASK_ENABLE;
				task_struct[TASK500US].task_func = &task500usCtrl;
				break;
			case TASK1MS:
				task_struct[TASK1MS].task_cnt = 0;
				task_struct[TASK1MS].task_flag = TASK_CLEAR_FLAG;
				task_struct[TASK1MS].task_period = 4;
				task_struct[TASK1MS].task_enable = TASK_ENABLE;
				task_struct[TASK1MS].task_func = &task1msCtrl;
				break;
			case TASK2MS:
				task_struct[TASK2MS].task_cnt = 0;
				task_struct[TASK2MS].task_flag = TASK_CLEAR_FLAG;
				task_struct[TASK2MS].task_period = 8;
				task_struct[TASK2MS].task_enable = TASK_ENABLE;
				task_struct[TASK2MS].task_func = &task2msCtrl;
				break;
			case TASK5MS:
				task_struct[TASK5MS].task_cnt = 0;
				task_struct[TASK5MS].task_flag = TASK_CLEAR_FLAG;
				task_struct[TASK5MS].task_period = 20;
				task_struct[TASK5MS].task_enable = TASK_ENABLE;
				task_struct[TASK5MS].task_func = &task5msCtrl;
				break;
			case TASK10MS:
				task_struct[TASK10MS].task_cnt = 0;
				task_struct[TASK10MS].task_flag = TASK_CLEAR_FLAG;
				task_struct[TASK10MS].task_period = 40;
				task_struct[TASK10MS].task_enable = TASK_ENABLE;
				task_struct[TASK10MS].task_func = &task10msCtrl;
				break;
			case TASK20MS:
				task_struct[TASK20MS].task_cnt = 0;
				task_struct[TASK20MS].task_flag = TASK_CLEAR_FLAG;
				task_struct[TASK20MS].task_period = 80;
				task_struct[TASK20MS].task_enable = TASK_ENABLE;
				task_struct[TASK20MS].task_func = &task20msCtrl;
				break;
			case TASK50MS:
				task_struct[TASK50MS].task_cnt = 0;
				task_struct[TASK50MS].task_flag = TASK_CLEAR_FLAG;
				task_struct[TASK50MS].task_period = 200;
				task_struct[TASK50MS].task_enable = TASK_ENABLE;
				task_struct[TASK50MS].task_func = &task50msCtrl;
				break;
			case TASK100MS:
				task_struct[TASK100MS].task_cnt = 0;
				task_struct[TASK100MS].task_flag = TASK_CLEAR_FLAG;
				task_struct[TASK100MS].task_period = 400;
				task_struct[TASK100MS].task_enable = TASK_ENABLE;
				task_struct[TASK100MS].task_func = &task100msCtrl;
				break;
			case TASK500MS:
				task_struct[TASK500MS].task_cnt = 0;
				task_struct[TASK500MS].task_flag = TASK_CLEAR_FLAG;
				task_struct[TASK500MS].task_period = 2000;
				task_struct[TASK500MS].task_enable = TASK_ENABLE;
				task_struct[TASK500MS].task_func = &task500msCtrl;
				break;
			case TASK1000MS:
				task_struct[TASK1000MS].task_cnt = 0;
				task_struct[TASK1000MS].task_flag = TASK_CLEAR_FLAG;
				task_struct[TASK1000MS].task_period = 4000;
				task_struct[TASK1000MS].task_enable = TASK_ENABLE;
				task_struct[TASK1000MS].task_func = &task1000msCtrl;
				break;
			default:
				break;
		}
	}
}

/**********************************************************************************
*Function name:				taskSchedule
*Parameter:					None
*Return value:				None
*Date:						2018-04-23
*Update:						2017-04-23
**********************************************************************************/
//#pragma CODE_SECTION(taskSchedule, 	"ramfuncs");
void taskSchedule(void)
{
	Uint16 task_index = 0;

	for(task_index=0;task_index<TASK_NUM;task_index++)
	{
		if((TASK_ENABLE == task_struct[task_index].task_enable)
		 &&((TASK_FUNC_PTR)0 != task_struct[task_index].task_func))
		{
			task_struct[task_index].task_cnt++;
			if(task_struct[task_index].task_cnt>=task_struct[task_index].task_period)
			{
				task_struct[task_index].task_cnt = 0;
				task_struct[task_index].task_flag = TASK_SET_FLAG;
			}
			else
			{
				task_struct[task_index].task_cnt = task_struct[task_index].task_cnt;
				task_struct[task_index].task_flag = task_struct[task_index].task_flag;
			}
		}
		else
		{
			task_struct[task_index].task_cnt = 0;
			task_struct[task_index].task_flag = TASK_CLEAR_FLAG;
		}
	}
}

void task500usCtrl(void)
{
	task_struct[TASK500US].task_flag = TASK_CLEAR_FLAG;
	AdcSEQ2();  //读取adc的值
	HallDetectProtect();  //空
}

void task1msCtrl(void)
{
	task_struct[TASK1MS].task_flag = TASK_CLEAR_FLAG;
	vdcctrl();  //母线电压控制   还要再仔细看一下
	//PhaseUnbalanceProtect();
	CrtDetectProtect();  //电流错误检测
}

void task2msCtrl(void)
{
	_iq tooth_trq = 0;
	
	task_struct[TASK2MS].task_flag = TASK_CLEAR_FLAG;
	spdCmdConfig();   //速度环参数选择，根据速度命令源给定参数，目前只有根据CAN的参数来源
	trqCmdConfig();   //力矩环参数选择，根据力矩命令源给定参数，来自AI或者CAN，AI根据是否需要限速
	speedlooplmtcond();//转速限速模块，    删除
	zero_stationcond();//驻坡模块        田
	if(1 == zero_station_trqCmd_flag)
	{  
		tooth_trq = _IQ15toIQ(EV_MCU_Para.field.Tooth_trq);
		if((0 == Vehicle_cmd.cmdmode.data.gear_state)
		 ||(MTR_TRQ_LOOP != sccw1.field.runLoop_mode)
		 ||(1 == alarm.field.gear_fault))
		{
			sccw1.field.run_enable = 1;
			if(zero_station_trqCmd>0)
			{
				zero_station_trqCmd = zero_station_trqCmd-3000;
				if(zero_station_trqCmd<0)
				{
					sysCfgPara.TrqCmd_AIfilter = sysCfgPara.TrqCmd_AI = zero_station_trqCmd = 0;
					zero_station_trqCmd_flag = 0;
				}
			}
			else if(zero_station_trqCmd<0)
			{
				zero_station_trqCmd = zero_station_trqCmd+3000;
				if(zero_station_trqCmd>0)
				{
					sysCfgPara.TrqCmd_AIfilter = sysCfgPara.TrqCmd_AI = zero_station_trqCmd = 0;
					zero_station_trqCmd_flag = 0;
				}
			}
			else
			{
				sysCfgPara.TrqCmd_AIfilter = sysCfgPara.TrqCmd_AI = 0;
				zero_station_trqCmd_flag = 0;
			}
		}
		else if((1 == Vehicle_cmd.cmdmode.data.brake_state)
		 ||(2 == Vehicle_cmd.cmdmode.data.zero_station_free_flag))
		{
			if(_IQabs(zero_station_trqCmd)>tooth_trq)
			{
				if(zero_station_trqCmd>tooth_trq)
				{
					if((zero_station_trqCmd-3000)>tooth_trq)
					{
						zero_station_trqCmd = zero_station_trqCmd-3000;
					}
					else
					{
						zero_station_trqCmd = tooth_trq;
					}
				}
				else if(zero_station_trqCmd<-tooth_trq)
				{
					if((zero_station_trqCmd+3000)<-tooth_trq)
					{
						zero_station_trqCmd = zero_station_trqCmd+3000;
					}
					else
					{
						zero_station_trqCmd = -tooth_trq;
					}
				}
			}
			else
			{
				if(1 == Vehicle_cmd.cmdmode.data.gear_state)
				{
					zero_station_trqCmd = zero_station_trqCmd+300;
					if(zero_station_trqCmd>=tooth_trq)
					{
						sysCfgPara.TrqCmd_AIfilter = sysCfgPara.TrqCmd_AI = zero_station_trqCmd = tooth_trq;
						zero_station_trqCmd_flag = 0;
					}
				}
				else
				{
					zero_station_trqCmd = zero_station_trqCmd-300;
					if(zero_station_trqCmd<=-tooth_trq)
					{
						sysCfgPara.TrqCmd_AIfilter = sysCfgPara.TrqCmd_AI = zero_station_trqCmd = -tooth_trq;
						zero_station_trqCmd_flag = 0;
					}
				}
			}
		}
		else if(1 == Vehicle_cmd.cmdmode.data.zero_station_free_flag)
		{
			if(zero_station_trqCmd>0)
			{
				if(zero_station_trqCmd>tooth_trq)
				{
					if((zero_station_trqCmd-3000)>tooth_trq)
					{
						zero_station_trqCmd = zero_station_trqCmd-3000;
					}
					else
					{
						zero_station_trqCmd = tooth_trq;
					}
				}
				else
				{
					zero_station_trqCmd = tooth_trq;
				}
			}
			else if(zero_station_trqCmd<0)
			{
				if(zero_station_trqCmd<-tooth_trq)
				{
					if((zero_station_trqCmd+3000)<-tooth_trq)
					{
						zero_station_trqCmd = zero_station_trqCmd+3000;
					}
					else
					{
						zero_station_trqCmd = -tooth_trq;
					}
				}
				else
				{
					zero_station_trqCmd = -tooth_trq;
				}
			}
		}
		else if(3 == Vehicle_cmd.cmdmode.data.zero_station_free_flag)
		{
			if(1 == Vehicle_cmd.cmdmode.data.gear_state)
			{
				if(zero_station_trqCmd>tooth_trq)
				{
					if((zero_station_trqCmd-6800)>tooth_trq)
					{
						zero_station_trqCmd = zero_station_trqCmd-6800;
					}
					else
					{
						zero_station_trqCmd = tooth_trq;
					}
				}
				else if(zero_station_trqCmd<-tooth_trq)
				{
					if((zero_station_trqCmd+6800)<-tooth_trq)
					{
						zero_station_trqCmd = zero_station_trqCmd+6800;
					}
					else
					{
						zero_station_trqCmd = -tooth_trq;
					}
				}
				else
				{
					zero_station_trqCmd = zero_station_trqCmd+300;
					if(zero_station_trqCmd>=tooth_trq)
					{
						sysCfgPara.TrqCmd_AIfilter = sysCfgPara.TrqCmd_AI = zero_station_trqCmd = tooth_trq;
						zero_station_trqCmd_flag = 0;
					}
				}
			}
			else
			{
				if(zero_station_trqCmd>tooth_trq)
				{
					if((zero_station_trqCmd-6800)>tooth_trq)
					{
						zero_station_trqCmd = zero_station_trqCmd-6800;
					}
					else
					{
						zero_station_trqCmd = tooth_trq;
					}
				}
				else if(zero_station_trqCmd<-tooth_trq)
				{
					if((zero_station_trqCmd+6800)<-tooth_trq)
					{
						zero_station_trqCmd = zero_station_trqCmd+6800;
					}
					else
					{
						zero_station_trqCmd = -tooth_trq;
					}
				}
				else
				{
					zero_station_trqCmd = zero_station_trqCmd-300;
					if(zero_station_trqCmd<=-tooth_trq)
					{
						sysCfgPara.TrqCmd_AIfilter = sysCfgPara.TrqCmd_AI = zero_station_trqCmd = -tooth_trq;
						zero_station_trqCmd_flag = 0;
					}
				}
			}
		}
		else
		{
			sccw1.field.run_enable = 1;
			if(zero_station_trqCmd>0)
			{
				zero_station_trqCmd = zero_station_trqCmd-3000;
				if(zero_station_trqCmd<0)
				{
					sysCfgPara.TrqCmd_AIfilter = sysCfgPara.TrqCmd_AI = zero_station_trqCmd = 0;
					zero_station_trqCmd_flag = 0;
				}
			}
			else if(zero_station_trqCmd<0)
			{
				zero_station_trqCmd = zero_station_trqCmd+3000;
				if(zero_station_trqCmd>0)
				{
					sysCfgPara.TrqCmd_AIfilter = sysCfgPara.TrqCmd_AI = zero_station_trqCmd = 0;
					zero_station_trqCmd_flag = 0;
				}
			}
			else
			{
				sysCfgPara.TrqCmd_AIfilter = sysCfgPara.TrqCmd_AI = 0;
				zero_station_trqCmd_flag = 0;
			}
		}
	}
	//以上是驻坡模块

	sysFSMCtrl();            //系统状态机    李
	mtrFSMCtrl();            //电机状态机    李
	trqLmt();                //转矩限制      苏      仅设置参数、其余空
	trqLoscoeff();           //转矩限制      苏      空
	motorLookupOffsetTable();//电机编码器零偏查表计算
	if((scsw2.field.runLoop_state>=MTR_SPEED_LOOP)   //速度环或者状态数
	 &&(1 == scsw2.field.igbt_state))                //逆变器打开
	{
		spdLoopInput();      //速度环参数设置
		spdLoopCtrl(&spdLoopPara);  //速度环控制
	}
	if(scsw2.field.runLoop_state >= MTR_TRQ_LOOP     //力矩环
	 &&(1 == scsw2.field.igbt_state))
	{   
		diff_deal();  //空
		
		trqLoopInput();  //力矩环参数设置
		trqLoopCtrl();   //力矩环控制-计算值传入力矩环
		trqLoopOutput(); //没怎么用到
	}
	if((scsw2.field.runLoop_state >= MTR_CURRENT_LOOP)  //电流环
	 &&(1 == scsw2.field.igbt_state))
	{
		ctrcmdConfigslowin();     //电流环参数
		weakcrtLoopslowin();      //弱磁参数
		crtcmdConfigout();        //空
		mtrfwdslowin();           //参数
		crtLoopslowInput();       //电流参数
		crtLoopOutput();          //输出参数
	}
	if(1 == scsw2.field.igbt_state)    //电压环
	{
		voltLoopslowInput();
	}
	IGBT_Ctrl();                //逆变器控制
	trqFeedback();              //trqLoopPara.Torque_fbk_HMI   力矩反馈参数
	OverLoad_faultDetect();     //过载故障检测
	FaultProcess();             //12V弱点故障处理
}

void task5msCtrl(void)//庄
{
	task_struct[TASK5MS].task_flag = TASK_CLEAR_FLAG;
	canatransmit();     //can发送
	canareceive();      //can接收
}

void taskGetVehicleIoState(void)        //读io状态  吴
{
	Uint16 read_vehicle_io_state_index = 0;
	Uint16 io_state_new_bit_value = 0;
	Uint16 io_state_old_bit_value = 0;

	//Get the current io state value
	vehicle_io_state_new.data.DI1_WAKEUP_DSP   = GpioDataRegs.GPADAT.bit.GPIO25;  //钥匙开关输入
	vehicle_io_state_new.data.DI2_BRRAK_DSP    = GpioDataRegs.GPADAT.bit.GPIO26;  //刹车输入
	vehicle_io_state_new.data.DI3_DSP          = GpioDataRegs.GPADAT.bit.GPIO14;  //数字输入
	vehicle_io_state_new.data.DI4_DSP          = GpioDataRegs.AIODAT.bit.AIO14;   //数字输入 D档
	vehicle_io_state_new.data.DI5_DSP          = GpioDataRegs.AIODAT.bit.AIO12;   //数字输入 R档
	vehicle_io_state_new.data.DI6_DSP          = GpioDataRegs.GPADAT.bit.GPIO22;  //数字输入
	vehicle_io_state_new.data.DI7_DSP          = GpioDataRegs.GPADAT.bit.GPIO28;  //数字输入
	vehicle_io_state_new.data.DI8_DSP          = GpioDataRegs.GPADAT.bit.GPIO27;  //数字输入
	vehicle_io_state_new.data.LOT              = GpioDataRegs.GPBDAT.bit.GPIO42;
	vehicle_io_state_new.data.DOS              = GpioDataRegs.GPBDAT.bit.GPIO43;
	//数字信号滤波
	for(read_vehicle_io_state_index=0;read_vehicle_io_state_index<8;read_vehicle_io_state_index++)
	{
		io_state_new_bit_value = ((vehicle_io_state_new.wValue>>read_vehicle_io_state_index)&0x0001);
		io_state_old_bit_value = ((vehicle_io_state_old.wValue>>read_vehicle_io_state_index)&0x0001);
		if(io_state_new_bit_value != io_state_old_bit_value)
		{
			vehicle_io_state_count[read_vehicle_io_state_index]++;
		}
		else
		{
			vehicle_io_state_count[read_vehicle_io_state_index] = 0;
		}

		if(vehicle_io_state_count[read_vehicle_io_state_index]>5)
		{
			vehicle_io_state_count[read_vehicle_io_state_index] = 0;
			vehicle_io_state_old.wValue = vehicle_io_state_old.wValue&(0xFFFF-(1<<read_vehicle_io_state_index)); //清除旧值
			vehicle_io_state_old.wValue = vehicle_io_state_old.wValue|(io_state_new_bit_value<<read_vehicle_io_state_index);//赋新值
		}
	}
	vehicle_io_state.wValue = vehicle_io_state_old.wValue;//更新当前状态
}

void task10msCtrl(void)
{
	task_struct[TASK10MS].task_flag = TASK_CLEAR_FLAG;
	eeprom_Process();          //李
	taskGetVehicleIoState();   //吴 读IO状态
	AIFeedback();              //吴 油门反馈
	diagIO();                  //吴 给出力矩参数  sysCfgPara.TrqCmd_AI -> 力矩环
	if((1 == EV_MCU_Para.field.Error_rest)   //错误重置
	 &&(0 == sccw1.field.run_enable))
	{
		emsw.wValue   					= 0;							/*~{N^9JUO~}*/
		emsw1.wValue 					= 0;							/*~{N^9JUO~}*/
		emsw2.wValue   					= 0;							/*~{N^9JUO~}*/
		emswold.wValue   				= 0;							/*~{N^9JUO~}*/
		emsw1old.wValue 				= 0;							/*~{N^9JUO~}*/
		emsw2old.wValue   				= 0;							/*~{N^9JUO~}*/
	    alarm.wValue 				    = 0;							/*~{N^1(>/~}*/
		//sub_io_buzzer(SUB_IO_BUZZER_OFF);

	    FaultTrqLmtCoeff                = _IQ(1.0);
	    EV_MCU_Para.field.Error_rest    = 0;
	}

	errordisplay(); //空
}

void task20msCtrl(void)
{
	
	task_struct[TASK20MS].task_flag = TASK_CLEAR_FLAG;
	v12Feedback();       //12V的反馈
#if 0
	PWM_Period_lowspeed = EV_MCU_Para.field.PWM_Period_Swap_speed;
	PWM_Period_highspeed = EV_MCU_Para.field.PWM_Period_Swap_speed+_IQtoIQ15(_IQmpyI32(500,SysBase.invspeed));
	if(EV_MCU_Para.field.PWM_Lowspeed_Period<SVPWM_2KHZ_VALUE)
	{
		PWM_Period_low = SVPWM_2KHZ;
	}
	else if(EV_MCU_Para.field.PWM_Lowspeed_Period>SVPWM_16KHZ_VALUE)
	{
		PWM_Period_low = SVPWM_16KHZ;
	}
	else
	{
		PWM_Period_low  = (Uint16)((PWM_SYS_FRE/EV_MCU_Para.field.PWM_Lowspeed_Period)>>1);
	}
	if(EV_MCU_Para.field.PWM_Highspeed_Period>SVPWM_16KHZ_VALUE)
	{
		PWM_Period_high = SVPWM_16KHZ;
	}
	else if(EV_MCU_Para.field.PWM_Highspeed_Period<SVPWM_2KHZ_VALUE)
	{
		PWM_Period_high = SVPWM_2KHZ;
	}
	else
	{
		PWM_Period_high = (Uint16)((PWM_SYS_FRE/EV_MCU_Para.field.PWM_Highspeed_Period)>>1);
	}
	if(EV_MCU_Para.field.PWM_Period<SVPWM_2KHZ_VALUE)
	{
		PWM_Period = SVPWM_2KHZ;
	}
	else if(EV_MCU_Para.field.PWM_Period>SVPWM_16KHZ_VALUE)
	{
		PWM_Period = SVPWM_16KHZ;
	}
	else
	{
		PWM_Period      = (Uint16)((PWM_SYS_FRE/EV_MCU_Para.field.PWM_Period)>>1);
	}
#endif
}

void task50msCtrl(void)
{
	task_struct[TASK50MS].task_flag = TASK_CLEAR_FLAG;
	tempFeedback();     //温度反馈
	hmi_display();      //参数反馈 通过CAN传输
}

void task100msCtrl(void)
{
	task_struct[TASK100MS].task_flag = TASK_CLEAR_FLAG;
	if((1 == EV_MCU_Para.field.Protect_OvLdFlag)     //过载保护标志
	 &&(1 == first_adCalib_state_finsh))             //第一次AD校验完成
	{
		if(5 == OvLdTimeCnt)                         //过载计数=5
		{
			 Frq_Real = sysFbkPara.Speed_abs*10;
		     InvOverLoadProtect();
		     MtrOverLoadProtect();
		     OvLdProtectCoeff = (InvProtectCoeff<MtrProtectCoeff) ? InvProtectCoeff:MtrProtectCoeff;

		     if(OvLdProtectCoeff>_IQ15(1.0))   OvLdProtectCoeff = _IQ15(1.0);
		     else if (OvLdProtectCoeff< 0)     OvLdProtectCoeff = 0;
		     OvLdTimeCnt = 0;
		}
		OvLdTimeCnt++;
	}
	else
	{
		alarm.field.MtrOvLd_alarm = 0;
		alarm.field.InvOvLd_alarm = 0;
		OvLdProtectCoeff = _IQ15(1.0);
		InvOvLdTime = 0;
		MtrOvLdTime = 0;
	}
	if(0 == sccw1.field.run_enable)
	{
		InvOvLdTime = 0;
		MtrOvLdTime = 0;
	}

	if((1 == EV_MCU_Para.field.OpenPhaseCheck_En)
	 &&(1 == first_adCalib_state_finsh))
	{
		//MtrOpenPhaseProtect();
	}
#if 0
	error_delay_cnt++;
	if(error_delay_cnt>2)
	{
		error_delay_cnt = 0;
		errordisplay();
	}
#endif
}

void task500msCtrl(void)
{
	static Uint16 uscnt = 0;
	
	task_struct[TASK500MS].task_flag = TASK_CLEAR_FLAG;
	if(GpioDataRegs.GPBDAT.bit.GPIO34 == 1)                  //LED灯
	{
		GpioDataRegs.GPBCLEAR.bit.GPIO34 = 1; //OFF 500ms
		uscnt = 0;
	}
	else
	{
		uscnt++;
		if(uscnt>2)
		{
			GpioDataRegs.GPBSET.bit.GPIO34 = 1;//ON 1500ms
			uscnt = 0;
		}
	}
}

void task1000msCtrl(void)
{
	task_struct[TASK1000MS].task_flag = TASK_CLEAR_FLAG;
}

void hmi_display(void)
{
	_iq temp = 0;

	sysFbkPara.Iu_fbk_HMI = _IQtoIQ15(sysFbkPara.Iu_fbk);
	sysFbkPara.Iv_fbk_HMI = _IQtoIQ15(sysFbkPara.Iv_fbk);
	sysFbkPara.Iw_fbk_HMI = _IQtoIQ15(sysFbkPara.Iw_fbk);

	sysFbkPara.Power_ele_fbk = _IQmpy(sysFbkPara.Id_fbk_filter_0025,SysBase.iphase)*_IQ15mpy(voltLoopPara.Vd_ref_HMI,SysBase.udc)+
		                               _IQmpy(sysFbkPara.Iq_fbk_filter_0025,SysBase.iphase)*_IQ15mpy(voltLoopPara.Vq_ref_HMI,SysBase.udc);
	temp                        = sysFbkPara.Power_ele_fbk*SysBase.inviphase;
	temp                        = temp/_IQ15mpy(sysCfgPara.Vdc_HMI,SysBase.udc);
	sysFbkPara.Idc_fbk_filter = temp;

    sysFbkPara.Id_fbk_HMI = _IQtoIQ15(sysFbkPara.Id_fbk_filter);
    sysFbkPara.Iq_fbk_HMI = _IQtoIQ15(sysFbkPara.Iq_fbk_filter);
    sysFbkPara.Idc_fbk_HMI = _IQtoIQ15(sysFbkPara.Idc_fbk_filter);
}


#endif
