#ifndef _WEAKCRTLOOP_H_
#define _WEAKCRTLOOP_H_

typedef struct
{
	Uint16 weak_mode;
	Uint16 sub_weak_mode;
	//Uint16 weakIsLimt_Enable;
	Uint16 weak_flag;
	Uint16 weak_swap_flag;
	_iq weakrunLoop_state;
	//_iq omegaEle;
	_iq Vdc;
	_iq Weak_Vol_Utilization;
	_iq Vdq_Filter;
	_iq CCP_Motor_UDCMAP;
	_iq weak_idmax;
	_iq WeakIdPid_Limit;
	_iq weak_ismax;
	_iq weak_thetamax;
	//_iq trqLoop_Idcmd_real;
	//_iq trqLoop_Iqcmd_real;


	//_iq IdCmd_temp;
	//_iq IqCmd_temp;
	//_iq weakismap;
	//_iq weakismap_iqref;
	_iq IdCmd;
	_iq IqCmd;
	_iq IdCmd_in;
	_iq IqCmd_in;
	//_iq iphase;

	_iq weakIdPid_Ref;
	_iq weakIdPid_Fdk;
	_iq weakIdPid_Err;
	_iq weakIdPid_Err_old;
	_iq weakIdPid_Kp;
	_iq weakIdPid_Ki;
	//_iq weakIdPid_Kd;
	_iq weakIdPid_Up;
	_iq weakIdPid_Ui;
	//_iq weakIdPid_Ud;
	_iq weakIdPid_Out;
	_iq weakIdPid_LimitState;
	_iq weakIdPid_OutMax;
	_iq weakIdPid_OutMin;
	_iq weakIdPid_Outfilter;

	_iq weakIqPid_Ref;
	_iq weakIqPid_Fdk;
	_iq weakIqPid_Err;
	_iq weakIqPid_Err_old;
	_iq weakIqPid_Kp;
	_iq weakIqPid_Ki;
	//_iq weakIqPid_Kd;
	_iq weakIqPid_Up;
	_iq weakIqPid_Ui;
	//_iq weakIqPid_Ud;
	_iq weakIqPid_Out;
	_iq weakIqPid_LimitState;
	_iq weakIqPid_OutMax;
	_iq weakIqPid_OutMin;
	_iq WeakIqPid_Limit;
	_iq weakIqPid_Outfilter;

	//Uint16 iderr_alarm;
}tag_weakcrtLoopvars;

extern void weakcrtLoopctrlclc(void);

#ifdef _WEAKCRTLOOP_C_
	tag_weakcrtLoopvars weakcrtLoopvars = {0};
	void weakcrtLoopin(void);
	void weakcrtLoopslowin(void);
	void weakcrtLoopctrl(tag_weakcrtLoopvars *v);
	void weakcrtLoopout(void);
	//void weakcrtLoopctrlclc(void);
#else
	extern tag_weakcrtLoopvars weakcrtLoopvars;
	extern void weakcrtLoopin(void);
	extern void weakcrtLoopslowin(void);
	extern void weakcrtLoopctrl(tag_weakcrtLoopvars *v);
	extern void weakcrtLoopout(void);
	//extern void weakcrtLoopctrlclc(void);
#endif

#endif
