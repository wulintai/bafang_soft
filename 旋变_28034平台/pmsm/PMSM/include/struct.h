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
		Uint16	run_enable			: 1;	//0:������ع�,		1:���������
		Uint16	runLoop_mode		: 3;	//0:��ѹ��, 1:������, 2:���ػ�, 3:�ٶȻ�

		Uint16  vdc_mode            : 1;    //0:ʵ�ʵ�ѹ��1���˹���ѹ
		Uint16  theta_mode          : 2;    //0:�˹��Ƕ�, 1:�˹����ٶ�, 2:ʵ�ʷ����Ƕ�
		Uint16  preRelay            : 1;    //Ԥ���

		Uint16  mainRelay           : 1;    //���̵���
		Uint16  ignition            : 1;
		Uint16  fault_clear         : 1;
		Uint16	fan_enable			: 1;	//0:����������,		1:��������

		Uint16  eeprom_enable       : 1;    /*eepromʹ��*/
		Uint16  fault_diague        : 1;    //���
		Uint16	shutoff			    : 1;    //�µ�
		Uint16  acdc_state          : 1;    //��ѹ�ջ�

		Uint16  eeprom_mode         : 1;   //0:������д����,	  1:����д����
		Uint16  error_clr           : 1;   /*0:��������ϼ�¼,1:������ϼ�¼*/
		Uint16  timer_clr			: 1;   /*0:�����ʱ���¼,1:���ʱ���¼*/
		Uint16	error_recall        : 1;   /*0:����ȡ���ϼ�¼,1:��ȡ���ϼ�¼*/
		Uint16	timer_record		: 1;   /*0:����¼����ʱ��,1:��¼����ʱ��*/
		Uint16	error_record		: 1;   /*0:����¼����,1:��¼����*/
		Uint16	para_read  			: 1;    //0:����ȡEEPROM�е���������1����ȡEEPROM�е�������
		Uint16  eeprom_recall		: 1;    //0:���ָ�eeprom����,1:�ָ�eeprom����
		Uint16  para_record			: 1;    //0:����д�������� 1����д��������
		Uint16  mtpa_study          : 1;   //0:��ֹ��ʱ��1����ʼ
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
		Uint16	runLoop_mode			: 3;	//0:��ѹ��, 1:������, 2:���ػ�, 3:�ٶȻ�
		Uint16	vdc_mode				: 1;	//0:ʵ�ʲ���ֱ����ѹ, 1:�˹�����ֱ����ѹ
		Uint16	theta_mode				: 2;	//0:�˹��Ƕ�, 1:�˹����ٶ�, 2:ʵ�ʷ����Ƕ�
		Uint16	run_flag				: 1;	//0:������ع�, 1:���������
		Uint16	fault_recover			: 1;	//0:��������ϻָ�, 1:������ϻָ�

		Uint16	lv_check				: 1;    //0:�����Ƿѹ����                          1�����Ƿѹ����
		Uint16  igbtFault_ChkMode       : 1;
		Uint16	fault_clear			    : 1;    //
		Uint16	crtFbkFault_ChkMode		: 1;    //
        Uint16  diag_enable             : 1;    //0:����������ѧϰ,	1:��������ѧϰ
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
		Uint16	system_state		: 3;	//0:��ʼ��, 1:��ǿ��, 2:����, 3:����, 4:��� ,5:׼����
		Uint16	runLoop_state		: 3;	//0:��ѹ��, 1:������, 2:���ػ�, 3:�ٶȻ�
		Uint16	fault_state			: 1;	//0:���Ͽɻָ�, 1:���ϲ��ɻָ�
		Uint16	fault_clear         : 1;
		Uint16	fan_state			: 1;	//0:���ȹر�,	 1:��������
		Uint16	mainRelay_state		: 1;	//0:���̵����Ͽ�,1:���̵����պ�
		Uint16	preRelay_state		: 1;	//0:���̵����Ͽ�,1:���̵����պ�
		Uint16	reserved1			: 5;
		Uint16	reserved2			: 16;
	} field;
	Uint32	wValue;
} tag_scsw1;

typedef union
{
	struct
	{
		Uint16	system_state		: 3;	//0:��ʼ��, 1:��ǿ��, 2:����, 3:����, 4:��� ,5:׼����
		Uint16	runLoop_state		: 3;	//0:��ѹ��,1:������,2:���ػ�,3:�ٶȻ�
		Uint16	theta_state			: 2;	//0:�˹��Ƕ�, 1:�˹���Ƶ��, 2:������ʵ�ʽǶ�

		Uint16	igbt_state			: 1;	//0:������ع�,		  1:���������
		Uint16	fault_flag			: 1;	//0:�޹���,			  1:�й���
		Uint16	fault_state			: 1;	//0:���ϲ��ɻָ�,	  1:���Ͽɻָ�
		Uint16	vdc_state			: 1;	//0:Ԥ���δ���,	  1:Ԥ������
		Uint16	vdc_mode			: 1;	//0:ʵ�ʵ�ѹ,		  1:�˹���ѹ
		Uint16  alarm_flag          : 1;
		Uint16	adCalib_state		: 1;
		Uint16  adCalib_state_finsh : 1;

		Uint16  diag_state          : 3;
		Uint16  stop_shake_flag     : 1;
		Uint16  deadtimecomp_state  : 1;    //0:������                                   1:����
		Uint16  MTPA_mode           : 1;
		Uint16  error_rank          : 3;    //0:�޹��ϣ�1��������2�������ʣ�3�������أ�4���ع�
		Uint16  SpdLmt_state        : 2;    //0:������    1����������   2����������
		Uint16  zero_state        	: 1;    //0:��פ��    1��פ��
		Uint16  Udcloop_state       : 1;
		Uint16  shake_flag          : 1;
		Uint16  spdlmt_flag         : 2;
	} field;
	Uint32	wValue;
} tag_scsw2;

//===========================================================================
// EMSW: Error Message State Word bit definitions:
//===========================================================================
/*���ϱ�־״̬�ֶ���*/
typedef union
{
	struct
	{
		Uint16 igbt_fault			: 1;    /*bit0:IPM����*/            //4
		Uint16 inv_overLoad			: 1;    /*bit1:����*/               //4
		Uint16 vdc_overVolt			: 1;    /*bit2:ֱ����ѹ*/            //4
		Uint16 vdc_lackVolt			: 1;    /*bit3:ֱ��Ƿѹ*/            //4

		Uint16 over_speedFwd		: 1;    /*bit4:�������*/            //4
		Uint16 mdl_temp				: 1;    /*bit5:ģ�����*/            //4
		Uint16 mtr_temp				: 1;   	/*bit6:�������*/            //4
		Uint16 software_fault		: 1; 	/*bit7:���жϹ���*/          //4

		Uint16 hv_fault            : 1;    /*bit8:��ѹ����*/            //4
		Uint16 resolver_error		: 1;    /*bit9:����Ӳ������*/         //4
		Uint16 selfcheck_fault		: 1; 	/*bit10:�Լ����*/           //4
		Uint16 eeprom_fault			: 1; 	/*bit11:EEPROM����*/         //4

		Uint16 over_speedRev		: 1;    /*bit12:�������*/          //4
		Uint16 ac_current			: 1;	/*bit13:12V�������*/       //4
		Uint16 short_IGBT		    : 1;	/*bit14:�����·����*/         //4
		Uint16 relay_error          : 1;    /*bit15:Ԥ������*/         //4
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
		Uint16	Relay_Fault			: 1;	//�̵��������쳣��
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
		unsigned mdl_temp		    : 1;    /*ģ���¶ȱ���*/
		unsigned mtr_temp		    : 1;   	/*����¶ȱ���*/
		unsigned udc_lv			    : 1;    /*UDC���ޱ����������������ʱ��*/
		unsigned udc_ov			    : 1;    /*UDC���ޱ���*/
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

