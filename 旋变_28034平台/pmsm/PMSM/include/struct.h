//########################################################################################

// File Name:		struct.h
//
// Version:			V1.0

// Input:
//
// Output:
//
// Description:
//
//########################################################################################
#ifndef _STRUCT_H
#define _STRUCT_H


typedef struct
{
	_iq			value;
} floatVar;

typedef struct
{
	long			value;
} longVar;

typedef struct
{
	long long		value;
} longlongVar;

//===========================================================================
// SCCW: System Control Command Word bit definitions:
//===========================================================================
typedef union
{
	struct
	{
		Uint16	run_enable			: 1;	//0:逆变器关管,		1:逆变器开管
		Uint16	runLoop_mode		: 3;	//0:电压环, 1:电流环, 2:力矩环, 3:速度环

		Uint16  vdc_mode            : 1;    //0:实际电压，1：人工电压
		Uint16  theta_mode          : 2;    //0:人工角度, 1:人工角速度, 2:实际反馈角度
		Uint16  preRelay            : 1;    //预充电

		Uint16  mainRelay           : 1;    //主继电器
		Uint16  ignition            : 1;
		Uint16  fault_clear         : 1;
		Uint16	fan_enable			: 1;	//0:不启动风扇,		1:启动风扇

		Uint16  eeprom_enable       : 1;    /*eeprom使能*/
		Uint16  fault_diague        : 1;    //诊断
		Uint16	shutoff			    : 1;    //下电
		Uint16  acdc_state          : 1;    //电压闭环

		Uint16  eeprom_mode         : 1;   //0:不可以写参数,	  1:可以写参数
		Uint16  error_clr           : 1;   /*0:不清除故障记录,1:清除故障记录*/
		Uint16  timer_clr			: 1;   /*0:不清除时间记录,1:清除时间记录*/
		Uint16	error_recall        : 1;   /*0:不读取故障记录,1:读取故障记录*/
		Uint16	timer_record		: 1;   /*0:不记录运行时间,1:记录运行时间*/
		Uint16	error_record		: 1;   /*0:不记录故障,1:记录故障*/
		Uint16	para_read  			: 1;    //0:不读取EEPROM中单个参数，1：读取EEPROM中单个参数
		Uint16  eeprom_recall		: 1;    //0:不恢复eeprom参数,1:恢复eeprom参数
		Uint16  para_record			: 1;    //0:不烧写单个参数 1：烧写单个参数
		Uint16  mtpa_study          : 1;   //0:禁止计时，1，开始
		Uint16  vdqhmi_mode         : 1;
		Uint16  zero_station        : 1;
		Uint16  speedlooplmt        : 1;
		Uint16	runLoop_mode_vehicle		: 3;
	} field;
	Uint32	wValue;
} tag_sccw1;



typedef union
{
	struct
	{
		Uint16	runLoop_mode			: 3;	//0:电压环, 1:电流环, 2:力矩环, 3:速度环
		Uint16	vdc_mode				: 1;	//0:实际采样直流电压, 1:人工给定直流电压
		Uint16	theta_mode				: 2;	//0:人工角度, 1:人工角速度, 2:实际反馈角度
		Uint16	run_flag				: 1;	//0:逆变器关管, 1:逆变器开管
		Uint16	fault_recover			: 1;	//0:不允许故障恢复, 1:允许故障恢复

		Uint16	lv_check				: 1;    //0:不检测欠压故障                          1：检测欠压故障
		Uint16  igbtFault_ChkMode       : 1;
		Uint16	fault_clear			    : 1;    //
		Uint16	crtFbkFault_ChkMode		: 1;    //
        Uint16  diag_enable             : 1;    //0:不进入电机自学习,	1:进入电机自学习
        Uint16  trqcmd_souremode        : 2;

		Uint16	crtFbk_ChkMode		    : 1;    //

		Uint16  diag_mode               : 4;    //
		Uint16	mtrtempFbk_ChkMode		: 1;
		Uint16  run_enable              : 1;
		Uint16  reserved2               :10;    //
	} field;
	Uint32	wValue;
} tag_sccw2;


//===========================================================================
// SCSW: System Control State Word bit definitions:
//===========================================================================
typedef union
{
	struct
	{
		Uint16	system_state		: 3;	//0:初始化, 1:上强电, 2:运行, 3:故障, 4:诊断 ,5:准备好
		Uint16	runLoop_state		: 3;	//0:电压环, 1:电流环, 2:力矩环, 3:速度环
		Uint16	fault_state			: 1;	//0:故障可恢复, 1:故障不可恢复
		Uint16	fault_clear         : 1;
		Uint16	fan_state			: 1;	//0:风扇关闭,	 1:风扇启动
		Uint16	mainRelay_state		: 1;	//0:主继电器断开,1:主继电器闭合
		Uint16	preRelay_state		: 1;	//0:主继电器断开,1:主继电器闭合
		Uint16	reserved1			: 5;
		Uint16	reserved2			: 16;
	} field;
	Uint32	wValue;
} tag_scsw1;

typedef union
{
	struct
	{
		Uint16	system_state		: 3;	//0:初始化, 1:上强电, 2:运行, 3:故障, 4:诊断 ,5:准备好
		Uint16	runLoop_state		: 3;	//0:电压环,1:电流环,2:力矩环,3:速度环
		Uint16	theta_state			: 2;	//0:人工角度, 1:人工角频率, 2:编码器实际角度

		Uint16	igbt_state			: 1;	//0:逆变器关管,		  1:逆变器开管
		Uint16	fault_flag			: 1;	//0:无故障,			  1:有故障
		Uint16	fault_state			: 1;	//0:故障不可恢复,	  1:故障可恢复
		Uint16	vdc_state			: 1;	//0:预充电未完成,	  1:预充电完成
		Uint16	vdc_mode			: 1;	//0:实际电压,		  1:人工电压
		Uint16  alarm_flag          : 1;
		Uint16	adCalib_state		: 1;
		Uint16  adCalib_state_finsh : 1;

		Uint16  diag_state          : 3;
		Uint16  stop_shake_flag     : 1;
		Uint16  deadtimecomp_state  : 1;    //0:不补偿                                   1:补偿
		Uint16  MTPA_mode           : 1;
		Uint16  error_rank          : 3;    //0:无故障，1：报警，2：降功率，3：零力矩，4：关管
		Uint16  SpdLmt_state        : 2;    //0:不限速    1：正向限速   2：负向限速
		Uint16  zero_state        	: 1;    //0:不驻坡    1：驻坡
		Uint16  Udcloop_state       : 1;
		Uint16  shake_flag          : 1;
		Uint16  spdlmt_flag         : 2;
	} field;
	Uint32	wValue;
} tag_scsw2;

//===========================================================================
// EMSW: Error Message State Word bit definitions:
//===========================================================================
/*故障标志状态字定义*/
typedef union
{
	struct
	{
		Uint16 igbt_fault			: 1;    /*bit0:IPM故障*/            //4
		Uint16 inv_overLoad			: 1;    /*bit1:过载*/               //4
		Uint16 vdc_overVolt			: 1;    /*bit2:直流过压*/            //4
		Uint16 vdc_lackVolt			: 1;    /*bit3:直流欠压*/            //4

		Uint16 over_speedFwd		: 1;    /*bit4:正向过速*/            //4
		Uint16 mdl_temp				: 1;    /*bit5:模块过温*/            //4
		Uint16 mtr_temp				: 1;   	/*bit6:电机过温*/            //4
		Uint16 software_fault		: 1; 	/*bit7:假中断故障*/          //4

		Uint16 hv_fault            : 1;    /*bit8:高压故障*/            //4
		Uint16 resolver_error		: 1;    /*bit9:旋变硬件故障*/         //4
		Uint16 selfcheck_fault		: 1; 	/*bit10:自检故障*/           //4
		Uint16 eeprom_fault			: 1; 	/*bit11:EEPROM故障*/         //4

		Uint16 over_speedRev		: 1;    /*bit12:反向过速*/          //4
		Uint16 ac_current			: 1;	/*bit13:12V弱电故障*/       //4
		Uint16 short_IGBT		    : 1;	/*bit14:电机短路故障*/         //4
		Uint16 relay_error          : 1;    /*bit15:预充电故障*/         //4
	}field;
	Uint16 wValue;
}tag_emsw;

typedef union
{
	struct
	{

		Uint16 APHsensor_error		: 1;                              //4
		Uint16 BPHsensor_error		: 1;                              //4
		Uint16 CPHsensor_error		: 1;                              //4
		Uint16 IDCsensor_error		: 1;                              //4

		Uint16  mdlsensor_short_error	  : 1;                              //4
		Uint16  mtrsensor_short_error     : 1;                              //4
		Uint16  open_phase                : 1;                              //~{H1O`~}
		Uint16  mtrsensor_open_error      : 1;                              //~{>xT5~}

		Uint16  Speed_detect_fault        : 1;                              //~{KY6H<l2b~}
		Uint16  Crt_detect_fault          : 1;                              //~{5gAw<l2b~}
		Uint16  HardFault_OU              : 1;
		Uint16  mdlsensor_open_error      : 1;

		Uint16  HardFault_OC              : 1;
		Uint16  HardFault_OCPWM           : 1;
		Uint16  V12_overVolt              : 1;
		Uint16  V12_lackVolt              : 1;
	}field;
	Uint16 wValue;
}tag_emsw1;


typedef union
{
	struct
	{
		Uint16  gear_fault       		    : 1;                              //3
		Uint16  Torque1_err           		: 1;                              //3
		Uint16  can_fault		            : 1;                              //3
		Uint16  can_fault_bms               : 1;
		Uint16  BMS_err_state               : 1;
		Uint16  Oilmtr_err                  : 1;
		Uint16  DCDC_err                    : 1;
		Uint16  HardFault_LU                : 1;

		Uint16  relay_error                 : 1;
		Uint16  break_error                 : 1;
		Uint16  zerostaion_error            : 1;
		Uint16  DIAG_ENCODER_error          : 1;
		Uint16  data_overflow               : 1;
		Uint16  Torque2_err			   	    : 1;
		Uint16  FO_V                        : 1;
		Uint16  FO_W                        : 1;
	}field;
	Uint16 wValue;
}tag_emsw2;

/*typedef union
{
	struct
	{
		Uint16	Relay_Fault			: 1;	//继电器吸合异常，
		Uint16	OH		            : 1;	//
		Uint16  FAIL                : 1;    //
		Uint16  RDCERR              : 1;    //
		Uint16  LU                  : 1;    //
		Uint16  OU                  : 1;    //
		Uint16  OC                  : 1;
		Uint16  OC_PWM              : 1;

		Uint16	reserved1			: 1;	//

		Uint16  X1_SPI              : 1;    //
		Uint16  X2_SPI              : 1;    //
		Uint16  X3_SPI              : 1;    //
	    Uint16  X4_SPI              : 1;    //
	    Uint16  X5_SPI              : 1;    //
	    Uint16  X6_SPI              : 1;    //
		Uint16	X7_SPI			    : 1;    //
	} field;
	Uint16	wValue;
} tag_IO_state;*/



typedef union
{
	struct
	{
		unsigned mdl_temp		    : 1;    /*模块温度报警*/
		unsigned mtr_temp		    : 1;   	/*电机温度报警*/
		unsigned udc_lv			    : 1;    /*UDC下限报警（电机允许运行时）*/
		unsigned udc_ov			    : 1;    /*UDC上限报警*/
		unsigned speed_ov           : 1;    //
		unsigned speed_rev          : 1;
		unsigned tbox_lose_fault    : 1;
		unsigned Brake_system_fault : 1;
		
		unsigned gear_fault         : 1;		
		unsigned InvOvLd_alarm      : 1;
		unsigned MtrOvLd_alarm      : 1;
		unsigned tbox_life_fault    : 1;
		unsigned break_error        : 1;
		unsigned V12_ov             : 1;
		unsigned I2C_alarm          : 1;
		unsigned can_fault     	    : 1;
	}field;
	Uint16 wValue;
}tag_alarm;


typedef struct
{
	//_iq           freq;
	_iq           speed;
	_iq           invspeed;
	_iq           omega;
	_iq           invomega;
	_iq           torque;
	_iq           invtorque;
	_iq           udc;
	_iq           invudc;
	_iq           iphase;
	_iq           inviphase;
	_iq           mtr_ratio;

	//_iq           uphase;
	//_iq           iphase_uphase_coeff;
	//_iq           omega_uphase_coeff;
	//_iq           ele_power;
}SysBaseVars;
//===========================================================================
// ~{Df1dFw2NJ}6(Re~}
//===========================================================================
typedef struct
{
	//longVar			PWM_NzTime;
	//longVar			PWM_Deadband;
	//longVar			PWM_Deadband_TD;
	floatVar		ThetaCmp_timeGap;
} invParaVars;

//===========================================================================
// ~{5g;z2NJ}6(Re~}
//===========================================================================
typedef struct
{
	//longVar			Encoder_type;
	longVar			Encoder_rollOver;
	//floatVar		Max_TorqueEle;
	//floatVar		Max_TorqueGen;
} mtrParaVars;

#endif	// end of #ifndef _STRUCT_H
//===========================================================================
// No more.
//===========================================================================

