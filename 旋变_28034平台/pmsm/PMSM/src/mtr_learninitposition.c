#ifndef _MTR_LEARNINITPOSITION_C_
#define _MTR_LEARNINITPOSITION_C_

#include "include_c.h"

void diagMotorInitPositionLearningAct_input(void)
{
}

void diagMotorInitPositionLearningAct_ctrl(void)
{
}

void diagMotorInitPositionLearningAct_out(void)
{
#if 0
#if 0
	if(1 == diagEncoffsetVar.finshflag)
	{
		sccw2.field.diag_enable = 0;
		sccw2.field.diag_mode   = DIAG_NON_TEST;
		sccw2.field.run_enable = 0;
		sccw2.field.runLoop_mode = MTR_VOLT_LOOP;
		sccw2.field.theta_mode = THETA_MODE_MANUAL;
		diagEncoffsetVar.diag_initangle_finishflag = 1;
	}
	else
	{
		sccw2.field.run_enable = 1;
		sccw2.field.runLoop_mode = MTR_VOLT_LOOP;
		sccw2.field.theta_mode = THETA_MODE_MANUAL;
	}
#else
	if(1 == diagEncoffsetVar.finshflag)
	{
		sccw2.field.diag_enable = 0;
		sccw2.field.diag_mode   = DIAG_NON_TEST;
		sccw2.field.run_enable = 0;
		sccw2.field.runLoop_mode = MTR_VOLT_LOOP;
		sccw2.field.theta_mode = THETA_MODE_MANUAL;
		//sysCfgPara.Omega_man.value = 0;
		//diagEncoffsetVar.diag_initangle_finishflag = 1;
		EV_MCU_Para.field.Motor_study_Ld = epdw.field.Motor_study_Ld = high_fre_str.high_fre_ld_value;
		EV_MCU_Para.field.Motor_study_Lq = epdw.field.Motor_study_Lq = high_fre_str.high_fre_lq_value;
		Motor_StudyPara.field.Ld_HMI = _IQtoIQ15(EV_MCU_Para.field.Motor_study_Ld);
		Motor_StudyPara.field.Lq_HMI = _IQtoIQ15(EV_MCU_Para.field.Motor_study_Lq);
		Motor_StudyPara.field.Rotor_Initangle_HMI = high_fre_str.Theta_init_detect_angle;
		diagEncoffsetVar.write_dq_flag = 1;
	}
	else
	{
		sccw2.field.run_enable = 1;
		sccw2.field.runLoop_mode = MTR_VOLT_LOOP;
		sccw2.field.theta_mode = THETA_MODE_MANUAL;
		Motor_StudyPara.field.Ld_HMI = _IQtoIQ15(EV_MCU_Para.field.Motor_study_Ld);
		Motor_StudyPara.field.Lq_HMI = _IQtoIQ15(EV_MCU_Para.field.Motor_study_Lq);
		Motor_StudyPara.field.Rotor_Initangle_HMI = high_fre_str.Theta_init_detect_angle;
		//sysCfgPara.Omega_man.value = 10*EV_MCU_Para.field.high_fre_frequency;
	}
#endif
#endif
}

#endif
