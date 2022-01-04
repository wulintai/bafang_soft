#ifndef _MTR_LEARNRS_H_
#define _MTR_LEARNRS_H_

#if 0

typedef struct
{
	_iq RsLearning_speedbase;
	_iq RsLearning_Speed_fbk;
	_iq RsLearning_Id_set;
	_iq RsLearning_Id_cmd;
	_iq RsLearning_Id_fbk_sum;
	_iq RsLearning_ud_fbk_sum;
	_iq RsLearning_Id_fbk_point1;
	_iq RsLearning_ud_fbk_point1;
	_iq RsLearning_Id_fbk_point2;
	_iq RsLearning_ud_fbk_point2;
	_iq RsLearning_rs;
	Uint16 RsLearning_runloop;
	Uint16 RsLearning_step;
	Uint16 RsLearning_err;
	Uint16 RsLearning_cnt;
	Uint16 IGBT_run_enable;
	Uint16 RsLearning_finshflag;
	Uint16 RsLearning_write_flag;
}DIAG_RS_STRUCT,* DIAG_RS_STRUCT_PTR;

#ifdef _MTR_LEARNRS_C_
	DIAG_RS_STRUCT diagRsStr = {0};
	void diagMotorRsLearningAct_in(void);
	void diagMotorRsLearningAct_ctrl(DIAG_RS_STRUCT_PTR v);
	void diagMotorRsLearningAct_out(void);
#else
	extern DIAG_RS_STRUCT diagRsStr;
	extern void diagMotorRsLearningAct_in(void);
	extern void diagMotorRsLearningAct_ctrl(DIAG_RS_STRUCT_PTR v);
	extern void diagMotorRsLearningAct_out(void);
#endif

#endif

#endif
