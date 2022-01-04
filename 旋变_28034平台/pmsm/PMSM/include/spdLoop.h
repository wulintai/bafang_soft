#ifndef _SPDLOOP_H_
#define _SPDLOOP_H_

typedef struct
{
	// 速度环输入参数
	_iq				Speed_cmd;
	_iq				Speed_fbk;

	// 速度控制参数
	_iq				Speed_ref;
	_iq         	Speed_ref_HMI;

	// 速度环输出参数
	_iq				TrqCmd_real;
	_iq             Idcmd_real;
	_iq				Iqcmd_real;
	_iq             Study_EncoderOffset_is;
	_iq             Out_Theta;

	_iq             Ref;
	_iq             Fdb;
	_iq             Err;
	_iq             Err_old;
	_iq             LimitState;
	_iq             Out;
	_iq             OutFilter;
	_iq             Up;
	_iq             Ui;
	_iq             Ud;
	_iq             Kp;
	_iq             Ki;
	_iq             Kd;
	_iq             OutMin;
	_iq             OutMax;
	_iq             ErrMax;
	_iq             ErrMin;
	//_iq             Out_fwd;
	_iq             spdLoop_spdRef;
	_iq             Speed_fbk_Filter;
	_iq             spdLoop_trqCmd;
	Uint16          diagState;
	Uint16          Weak_enable;
} spdLoopVars;

#ifdef _SPDLOOP_C_
	//PIDREG3 pid_spd = {0};
	PIDREG3 pid_station = {0};
	spdLoopVars spdLoopPara = {0};
	_iq	spdLoop_spdCmd = 0;
	_iq	spdLoop_spdRef = 0;
	_iq spdLoop_deltaspdCmd = 0;
	_iq spdLoop_spdCalstep = 0;
	_iq spdLoop_StepCaltimeSlice = 0;
	_iq spdLoop_mincalspdstep = 0;
	_iq	spdLoop_spdRamp_upStep = 0;
	//_iq spdLoop_spd_error = 0;
	_iq	spdLoop_spdRamp_downStep = 0;
	_iq	spdLoop_spdMaxPos = 0;
	_iq	spdLoop_spdMaxNeg = 0;
	//_iq spdLoop_trqCmd = 0;
	_iq	spdLoop_trqupLimit = 0;
	_iq	spdLoop_trqdownLimit = 0;
	void spdLoopCtrl(spdLoopVars *v);
	void spdLoopCalstep(void);
	void spdRamp(void);
	void spdLoopInput(void);
	void spdLoopParaClr(void);
#else
	//extern PIDREG3 pid_spd;
	extern PIDREG3 pid_station;
	extern spdLoopVars spdLoopPara;
	//extern _iq spdLoop_trqCmd;
	extern void spdLoopCtrl(spdLoopVars *v);
	extern void spdRamp(void);
	extern void spdLoopCalstep(void);
	extern void spdLoopInput(void);
	extern void spdLoopParaClr(void);
#endif

#endif

