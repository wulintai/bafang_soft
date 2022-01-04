/*
 * sysConfig.h
 *
 *  Created on: 2014-3-19
 *      Author: ThinkPad
 */

#ifndef SYSCONFIG_H_
#define SYSCONFIG_H_

// ??㊣??¯???T?米?“辰?
#define MAX_VDC						_IQ(1.05)	// Unit: V   BASE

typedef struct
{
	// ???豕2?那y
	floatVar		Theta_man;
	_iq			    Theta_ele;
	Uint16		    Theta_ele_HMI;

	// ???米?那2?那y
	floatVar		Omega_man;
	_iq			    Omega_ele;
	floatVar		Omega_period;

	// 米??12?那y
	_iq			    Vdc;
	_iq             Vdc_INV;
	floatVar		Vdc_man;
	_iq			    VdCmd_man;
	_iq			    VqCmd_man;
	Uint16          Vdc_HMI;

	// 米?芍¯2?那y
	_iq			    IdCmd_man;
	_iq			    IqCmd_man;

	//MTPA
	_iq             IsCmd_man;
	_iq             MTPAtheta_man;

	// 芍|??2?那y
	_iq			    CANA_TrqCmd_man;
	_iq             TrqCmd_AI;
	_iq             TrqCmd_Zero;
	_iq             TrqCmd_AIfilter;
	_iq             TrqCmd_NEW;
	_iq			    TrqCmd;
	// ?迄?豕2?那y
	_iq             CANA_SpdCmd_man;
	//_iq			    SpdCmd_AIfilter;
	_iq			    SpdCmd;
	//_iq             SpdCmd_NEW;
} sysCfgVars;


#ifdef _SYSCONFIG_C
	sysCfgVars sysCfgPara = {0};
	void thetaConfig(void);
	void vdcConfig(void);
	void vdcctrl(void);
	void spdCmdConfig(void);
	void trqCmdConfig(void);
#else
	extern sysCfgVars sysCfgPara;
	extern void thetaConfig(void);
	extern void vdcConfig(void);
	extern void vdcctrl(void);
	extern void spdCmdConfig(void);
	extern void trqCmdConfig(void);

#endif

#endif /* SYSCONFIG_H_ */

