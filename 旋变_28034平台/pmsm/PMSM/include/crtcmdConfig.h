#ifndef CRTCMDCONFIG_H_
#define CRTCMDCONFIG_H_

typedef struct
{
	// ‰»Î
	Uint16 runLoop_state;
	Uint16 MTPA_mode;
	Uint16 weak_mode;
	Uint16 diagState;
	_iq    IdCmd_man;
	_iq    IqCmd_man;
	_iq    trqLoop_Idcmd_real;
	_iq    trqLoop_Iqcmd_real;
	_iq    spdLoop_Idcmd_real;
	_iq    spdLoop_Iqcmd_real;

    // ‰≥ˆ
    _iq    IdCmd;
    _iq    IqCmd;

	//==================MTPA============
	Uint16 ThetaMTPA_man;
	_iq    IsMTPACmd_man;
	_iq    Current_step;
}tag_crtCmd_vars;

#ifdef _CRTCMDCONFIG_C
	tag_crtCmd_vars crtCmd_vars = {0};
	void ctrcmdConfigslowin(void);
	void crtcmdConfigin(void);
	void crtcmdConfigctrl(tag_crtCmd_vars *v);
	void crtcmdConfigout(void);
	void crtcmdConfigclc(void);
#else
	extern tag_crtCmd_vars crtCmd_vars;
	extern void ctrcmdConfigslowin(void);
	extern void crtcmdConfigin(void);
	extern void crtcmdConfigctrl(tag_crtCmd_vars *v);
	extern void crtcmdConfigout(void);
	extern void crtcmdConfigclc(void);
#endif

#endif

