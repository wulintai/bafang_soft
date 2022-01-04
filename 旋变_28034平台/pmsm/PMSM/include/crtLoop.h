#ifndef _CRTLOOP_H_
#define _CRTLOOP_H_

typedef struct
{
	// 电流调节器参数
	_iq			Id_ref_HMI;     //Software packaging
	_iq			Iq_ref_HMI;     //Software packaging

	Uint16   	Motor_Direction;

	_iq 		IdCmd;
	_iq 		IqCmd;
	_iq 		Vdc;
	_iq 		Id_PI_Vd;
	_iq 		Iq_PI_Vq;

	_iq 		IdPid_Ref;
	_iq 		IdPid_Fdk;
	_iq 		IdPid_Err;
	_iq 		IdPid_Err_old;
	_iq 		IdPid_Kp;
	_iq 		IdPid_Ki;
	//_iq 		IdPid_Kd;
	_iq 		IdPid_Up;
	_iq 		IdPid_Ui;
	_iq 		IdPid_Out;
	_iq 		IdPid_LimitState;
	_iq 		IdPid_OutMax;
	_iq 		IdPid_OutMin;

	_iq 		IqPid_Ref;
	_iq 		IqPid_Fdk;
	_iq 		IqPid_Err;
	_iq 		IqPid_Err_old;
	_iq 		IqPid_Kp;
	_iq 		IqPid_Ki;
	//_iq 		IqPid_Kd;
	_iq 		IqPid_Up;
	_iq 		IqPid_Ui;
	_iq 		IqPid_Out;
	_iq 		IqPid_LimitState;
	_iq 		IqPid_OutMax;
	_iq 		IqPid_OutMin;
} crtLoopVars;

#ifdef _CRTLOOP_C_
	crtLoopVars	crtLoopPara = {0};
	void crtLoopInput(void);
	void crtLoopslowInput(void);
	void crtLoopCtrl(crtLoopVars *v);
	void crtLoopOutput(void);
	void crtLoopParaClr(void);
#else
	extern crtLoopVars	crtLoopPara;
	extern void crtLoopInput(void);
	extern void crtLoopslowInput(void);
	extern void crtLoopCtrl(crtLoopVars *v);
	extern void crtLoopOutput(void);
	extern void crtLoopParaClr(void);
#endif

#endif
