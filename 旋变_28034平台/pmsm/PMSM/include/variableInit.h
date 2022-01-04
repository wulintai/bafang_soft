//########################################################################################
//
//
//########################################################################################
#ifndef _VARIABLEINIT_H
#define _VARIABLEINIT_H
//========================================================================================
// 常量定义
//========================================================================================
// 系统常量
#define TRUE					1
#define FALSE					0
#define NULL					0

// 数据类型定义
#define DATA_TYPE_FLOAT			0
#define DATA_TYPE_INT16			1
#define DATA_TYPE_INT32			2

// 电机类型定义

// 逆变器类型定义

// 逆变器容量类型定义
// 编码器类型定义
#define NUM_ENCODER_TYPE		2
#define	ENC_TYPE_RESOLVER		0
#define	ENC_TYPE_ABZ_ENCODER	1

// 软件类型定义
#define NUM_SOFTWARE_TYPE		2
#define	SOFTWARE_TYPE_PRODUCT	0
#define SOFTWARE_TYPE_TEST		1

// 温度传感器类型定义
#define NUM_TEMPSENSOR_TYPE		7
#define	TEMPSENSOR_PT100		0
#define	TEMPSENSOR_PT1000		1
#define TEMPSENSOR_NTC_10K		2
#define TEMPSENSOR_NTC_30K		3
#define TEMPSENSOR_NTC_100K     4
#define TMP_MTR_NTC_15C054		5
#define TEMPSENSOR_KTY84_150    6

// 变量类型常量
#define VARIABLE_TYPE_INT		0
#define VARIABLE_TYPE_LONG		1
#define VARIABLE_TYPE_FLOAT		2
#define VARIABLE_TYPE_ULONG		3
#define VARIABLE_TYPE_DOUBLE	4
#define VARIABLE_TYPE_LONGLONG	5

// 相序类型常量
#define INV_POSITIVE_ORDER		0
#define INV_NEGATIVE_ORDER		1

// 电机正方向类型常量
#define MOTOR_COUNTER_CLOCKWISE	0
#define MOTOR_CLOCKWISE			1

// 力矩命令来源常量
#define TRQCMD_SOURCE_AI    	0
#define TRQCMD_SOURCE_ECANA	    1
#define TRQCMD_SOURCE_ECANB	    2

// 速度命令来源常量
#define SPDCMD_SOURCE_AI        0
#define SPDCMD_SOURCE_ECANA 	1
#define SPDCMD_SOURCE_ECANB		2

// 数学常量
#define SQRT2					_IQ(1.414214)				// SQRT(2)
#define	SQRT3					_IQ(1.732051)				// SQRT(3)
#define SQRT3DIV2				_IQ(0.866025)				// SQRT(3)/2
#define INVSQRT3				_IQ(0.57735)				// 1/SQRT(3)
#define SQRT2INV3				_IQ(0.8165)                 //SQRT(2/3)
#define INVSQRT2				_IQ(0.707107)             //SQRT(1/2)
#define PI						_IQ(3.141593)
#define INVPI					_IQ15(0.31831)
#define INV360					_IQ(0.002777778)			// 1/360
#define INVPOWERCAL				_IQ(0.104668)			// 1/9.550
#define EPSILON					_IQ(0.00001)               //_IQ(0.0001)

// 控制用常量
// SVPWM timer period (assume the pre-scale counter to divide by 1)
#define SVPWM_16KHZ				937		//(SYSTEM_FREQUENCY*1000/16/2)
#define SVPWM_12KHZ				1250		//(SYSTEM_FREQUENCY*1000/12/2)
#define SVPWM_10KHZ				1500		//(SYSTEM_FREQUENCY*1000/10/2)
#define SVPWM_8KHZ				1875		//(SYSTEM_FREQUENCY*1000/8/2)
#define SVPWM_6KHZ				2500		//(SYSTEM_FREQUENCY*1000/6.25/2) 6.25K
#define SVPWM_5KHZ				3000		//(SYSTEM_FREQUENCY*1000/5/2)
#define SVPWM_4KHZ				3750		//(SYSTEM_FREQUENCY*1000/4/2)
#define SVPWM_2KHZ				7500		//(SYSTEM_FREQUENCY*1000/4/2)

#define SVPWM_2KHZ_VALUE		2000
#define SVPWM_16KHZ_VALUE		16000

// Deadband time
#define PWM_DEADBAND_TIME		105						// 2.5us@100M
#define PWM_DEADBAND_CRT        _IQdiv(5,IPHASE_BASE)
#define PWM_DEADBAND_TD         PWM_DEADBAND_TIME
// Configure which ePWM timer interrupts are enabled at the PIE level:

#define EEPROM_START_ADDR					0
#define EEPROM_END_ADDR						239
#define EEPROM_SUM_LEN						240

#define EEPROM_START_ADDR1                  500
#define EEPROM_END_ADDR1                    739
#define EEPROM_SUM_LEN1						240

#define EEPROM_START_ADDR2                  800
#define EEPROM_END_ADDR2                    1039
#define EEPROM_SUM_LEN2						240

// 1 = enabled,  0 = disabled
#define PWM1_INT_ENABLE			1
#define PWM2_INT_ENABLE			1
#define PWM3_INT_ENABLE			1
#define PWM4_INT_ENABLE			1
#define PWM5_INT_ENABLE			1
#define PWM6_INT_ENABLE			1

#define PWM1_TZINT_ENABLE		1
#define PWM2_TZINT_ENABLE		1
#define PWM3_TZINT_ENABLE		1
#define PWM4_TZINT_ENABLE		1
#define PWM5_TZINT_ENABLE		1
#define PWM6_TZINT_ENABLE		1

// 角度模式定义
#define THETA_MODE_FEEDBACK		0
#define THETA_MODE_MANUAL		1
#define THETA_MODE_OMEGA		2


// 电压模式定义
#define VDC_MODE_FEEDBACK		0
#define VDC_MODE_MANUAL			1


// 人工给定物理量默认值
#define MANUAL_VDC				144// Unit: V
#define MANUAL_OMEGA_ELE		100				// Unit: Hz  10HZ
#define MANUAL_THETA			0				// Unit: 360°

// 物理量分辨率

//系统标幺值
//#define SPEED_BASE					6000
//#define OMEGA_BASE					100      // HZ SPEED_BASE/60
//#define TRQ_BASE					2500
//#define UDC_BASE					800
#define UPHASE_BASE					461.89 //((long)C_UDC_BASE*18919/32767)
//#define IPHASE_BASE					900
#define POWER_BASE                  10000   //kw


//电机参数扩大系数
#define MOTOR_PARA_COEFF			50000//

//========================================================================================
// 宏函数定义
//========================================================================================
#define LOBYTE(w)				((Uint16)(w)			& 0x0ff)
#define HIBYTE(w)				(((Uint16)(w) >> 8)	& 0x0ff)
#define max(a,b)				(((_iq)(a) > (_iq)(b)) ? (_iq)(a) : (_iq)(b))
#define min(a,b)				(((_iq)(a) < (_iq)(b)) ? (_iq)(a) : (_iq)(b))
#define MAKEWORD(low,high)		((Uint16)(low) | (((Uint16)(high)<<8)))

#define StartSVPWMTimer()		EPwm1Regs.ETSEL.bit.INTEN = PWM1_INT_ENABLE
#define StopSVPWMTimer()		EPwm1Regs.ETSEL.bit.INTEN = 0;

//========================================================================================
// 位操作掩码定义
//========================================================================================
#define B0_MSK					(0x1<<0)
#define B1_MSK					(0x1<<1)
#define B2_MSK					(0x1<<2)
#define B3_MSK					(0x1<<3)
#define B4_MSK					(0x1<<4)
#define B5_MSK					(0x1<<5)
#define B6_MSK					(0x1<<6)
#define B7_MSK					(0x1<<7)
#define B8_MSK					(0x1<<8)
#define B9_MSK					(0x1<<9)
#define B10_MSK					(0x1<<10)
#define B11_MSK					(0x1<<11)
#define B12_MSK					(0x1<<12)
#define B13_MSK					(0x1<<13)
#define B14_MSK					(0x1<<14)
#define B15_MSK					(0x1<<15)
#define B16_MSK					(0x1<<16)
#define B17_MSK					(0x1<<17)
#define B18_MSK					(0x1<<18)
#define B19_MSK					(0x1<<19)
#define B20_MSK					(0x1<<20)
#define B21_MSK					(0x1<<21)
#define B22_MSK					(0x1<<22)
#define B23_MSK					(0x1<<23)
#define B24_MSK					(0x1<<24)
#define B25_MSK					(0x1<<25)
#define B26_MSK					(0x1<<26)
#define B27_MSK					(0x1<<27)
#define B28_MSK					(0x1<<28)
#define B29_MSK					(0x1<<29)
#define B30_MSK					(0x1<<30)
#define B31_MSK					(0x1<<31)

//========================================================================================
// 数据结构定义
//========================================================================================
// 变量属性结构体

typedef union
{
	struct
	{
		Uint16	year			: 12;
		Uint16	month			: 4;
		Uint16	day				: 5;
		Uint16	reserved0		: 11;

		Uint16	hour			: 5;
		Uint16	minute			: 6;
		Uint16	second			: 6;
		Uint16	reserved1		: 15;
	} field;
	Uint32 wValue[2];
} tag_time;

typedef struct
{
	tag_time		value;
} timeVar;

#define SVPWM_PERIOD				_IQ24(0.125)         //8KHZ
#define OMEGA_CAL_PERIOD			((_iq)SVPWM_PERIOD)
#define THETACOMP_TIMEGAP			_IQ24mpy((_iq)SVPWM_PERIOD,_IQ24(1.35555))
#define THETACOMP_TIMEGAP_PERCENT	_IQ24(1.35555)

#ifdef _VARIABLEINIT_C
//===========================================================================
// 变量定义
//===========================================================================
// 系统控制全局变量
tag_sccw1			sccw1 = {0};
tag_sccw2			sccw2 = {0};
tag_scsw1			scsw1 = {0};
tag_scsw2			scsw2 = {0};
//tag_IO_state        IO_state = {0};
tag_emsw		    emsw = {0};
tag_emsw            emswold = {0};
tag_emsw1		    emsw1 = {0};
tag_emsw1           emsw1old = {0};
tag_emsw2		    emsw2 = {0};
tag_emsw2           emsw2old = {0};
tag_alarm           alarm = {0};
tag_EV_MCU_Para     EV_MCU_Para = {0};
mtrParaVars			mtrPara = {0};
invParaVars			invPara = {0};
SysBaseVars         SysBase = {0};
Uint16 eeprom_start_addr[3] = {0};
//Uint16 eeprom_crc_addr[3] = {0};
Uint16 brake_fault_cnt = 0;
Uint16 bms_fault_level = 0;
//===========================================================================
// 函数声明
//===========================================================================
void GlobalVar_Init(void);
void ClearManualCtrlCmd(void);
void SysCtrlVar_Init(void);
void SysCtrlBase_Init(void);
void SysCtrltrqtable_Init(void);
void ClearCtrlLoopPara(void);
void eepromPara_Wricall(void);
void eepromPara_Recall(unsigned int sector_addr);
void I2Cfault_Recall(void);
void Eeprom_ParaInt(void);
void SysCfgParaInt(void);
void Default_ParaInt(void);
void Default_ExParaInt(void);
void eepromPara_WricallAddr(unsigned int addr);

#else
//===========================================================================
// 外部引用接口声明
//===========================================================================
extern tag_sccw1				sccw1;
extern tag_sccw2				sccw2;
extern tag_scsw1				scsw1;
extern tag_scsw2				scsw2;
extern tag_emsw					emsw,emswold;
extern tag_emsw1				emsw1,emsw1old;
extern tag_emsw2				emsw2,emsw2old;
//extern tag_IO_state            	IO_state;
extern tag_alarm                alarm;
extern mtrParaVars				mtrPara;
extern invParaVars				invPara;
extern SysBaseVars              SysBase;
extern tag_EV_MCU_Para          EV_MCU_Para;
extern Uint16 eeprom_start_addr[];
//extern Uint16 eeprom_crc_addr[];
extern Uint16 brake_fault_cnt;
extern Uint16 bms_fault_level;

extern void GlobalVar_Init(void);
extern void ClearManualCtrlCmd(void);
extern void SysCtrlVar_Init(void);
extern void ClearCtrlLoopPara(void);
extern void eepromPara_Wricall(void);
extern void eepromPara_Recall(void);
extern void I2Cfault_Recall(void);
extern void Default_ParaInt(void);
extern void Default_ExParaInt(void);
extern void eepromPara_WricallAddr(unsigned int addr);

#endif	// end of #ifdef _VARIABLEINIT_C

#endif	// end of #ifndef _VARIABLEINIT_H

//===========================================================================
// No more.
//===========================================================================

