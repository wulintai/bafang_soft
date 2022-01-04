#ifndef _MTR_LEARNINITPOSITION_H_
#define _MTR_LEARNINITPOSITION_H_

typedef union
{
	struct
	{
		Uint16 DI;
		Uint16 IU_AD;
		Uint16 IV_AD;
		Uint16 IW_AD;
		Uint16 VPN_AD;
		Uint16 OV_REF;
		Uint16 T_IGBT_AD;
		Uint16 AI3_AD;
		Uint16 TEMP_M2_AD;
		Uint16 TEMP_M1_AD;

		Uint16 V12_ADC;
		Uint16 AI2_AD;
		Uint16 V5_ADC;
		Uint16 AI1_AD;
		Uint16 V5_fbk_HMI;
		Uint16 V12_fbk_HMI;
		Uint16 AI4_AD;
		Uint16 TA_AD;
		Uint16 I_DC_AD;
		Uint16 AI_fbk_offset;
	}field;
	Uint16 value[20];
}MOTOR_VAR_PARAMETER,* MOTOR_VAR_PARAMETER_PTR;

#ifdef _MTR_LEARNINITPOSITION_C_
	MOTOR_VAR_PARAMETER Motor_VarPara = {0};
	void diagMotorInitPositionLearningAct_input(void);
	void diagMotorInitPositionLearningAct_ctrl(void);
	void diagMotorInitPositionLearningAct_out(void);
#else
	extern MOTOR_VAR_PARAMETER Motor_VarPara;
	extern void diagMotorInitPositionLearningAct_input(void);
	extern void diagMotorInitPositionLearningAct_ctrl(void);
	extern void diagMotorInitPositionLearningAct_out(void);
#endif

#endif
