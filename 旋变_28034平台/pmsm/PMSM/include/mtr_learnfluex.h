#ifndef _MTR_LEARNFLUEX_H_
#define _MTR_LEARNFLUEX_H_

#if 0
typedef struct
{
	_iq diagFluex_speedcmd;
	_iq diagFluex_iscmd1;
	_iq diagFluex_iscmd2;
	_iq diagFluex_np;
	_iq diagFluex_speedbase;
	_iq diagFluex_udcbase;
	_iq diagFluex_basevalue;
	_iq diagFluex_speed_fbk;
	_iq diagFluex_id_fbk;
	_iq diagFluex_vq_fbk;
	_iq diagFluex_runloop;
	_iq diagFluex_is;
	_iq diagFluex_speed;
	_iq diagFluex_fluex;
	_iq diagFluex_fluex_HMI;
	_iq diagFluex_vq1;
	_iq diagFluex_vq2;
	_iq diagFluex_id1;
	_iq diagFluex_id2;
	_iq diagFluex_current_step;
	Uint16 diagFluex_step;
	Uint16 diagFluex_cnt;
	Uint16 diagFluex_keep_cnt;
	Uint16 diagFluex_err;
	Uint16 diagFluex_finishflag;
	Uint16 diagFluex_writeflag;
	Uint16 IGBT_run_enable;
	Uint16 diagFluex_motorDir;
	Uint16 diagFluex_keepflag;
}DIAG_FLUEX_STRUCT,* DIAG_FLUEX_STRUCT_PTR;

#ifdef _MTR_LEARNFLUEX_C_
	DIAG_FLUEX_STRUCT diagFluexStr = {0};
	void diagMotorFluexLearningAct_input(void);
	void diagMotorFluexLearningAct_ctrl(DIAG_FLUEX_STRUCT_PTR v);
	void diagMotorFluexLearningAct_out(void);
#else
	extern DIAG_FLUEX_STRUCT diagFluexStr;
	extern void diagMotorFluexLearningAct_input(void);
	extern void diagMotorFluexLearningAct_ctrl(DIAG_FLUEX_STRUCT_PTR v);
	extern void diagMotorFluexLearningAct_out(void);
#endif

#endif

#endif
