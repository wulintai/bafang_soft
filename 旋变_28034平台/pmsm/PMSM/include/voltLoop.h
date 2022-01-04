#ifndef _VOLTLOOP_H_
#define _VOLTLOOP_H_

#define SVPWM_SMALL_PULSE	30

#define SVPWM_DEAD_TIME_0_5US 	_IQ(0.0025)
#define SVPWM_DEAD_TIME_1US		_IQ(0.005)
#define SVPWM_DEAD_TIME_1_5US 	_IQ(0.0075)
#define SVPWM_DEAD_TIME_2US 	_IQ(0.01)
#define SVPWM_DEAD_TIME_2_5US 	_IQ(0.0125)
#define SVPWM_DEAD_TIME_3US 	_IQ(0.015)
#define SVPWM_DEAD_TIME_3_5US 	_IQ(0.0175)
#define SVPWM_DEAD_TIME_4US 	_IQ(0.02)
#define SVPWM_DEAD_TIME_4_5US 	_IQ(0.0225)
#define SVPWM_DEAD_TIME_5US 	_IQ(0.025)
#define SVPWM_DEAD_TIME_5_5US 	_IQ(0.0275)
#define SVPWM_DEAD_TIME_6US 	_IQ(0.03)
#define SVPWM_DEAD_TIME_6_5US 	_IQ(0.0325)
#define SVPWM_DEAD_TIME_7US 	_IQ(0.035
#define SVPWM_DEAD_TIME 		SVPWM_DEAD_TIME_3_5US

#define DEADZONE_30_DEGREE		1398101
#define DEADZONE_90_DEGREE		4194304
#define DEADZONE_150_DEGREE		6990506
#define DEADZONE_210_DEGREE		9786709
#define DEADZONE_270_DEGREE		12582912
#define DEADZONE_330_DEGREE		15379114
#define DEADZONE_DETA			233016

#define SVPWM_MI_30_DEGREE		1398101
#define SVPWM_MI_60_DEGREE		2796202
#define SVPWM_MI_120_DEGREE		5592405
#define SVPWM_MI_180_DEGREE		8388608
#define SVPWM_MI_240_DEGREE		11184810
#define SVPWM_MI_300_DEGREE		13981013
#define SVPWM_MI_360_DEGREE     16777216
#define DEADZONE_7US_T			105
#define DEADZONE_6_5US_T		97
#define DEADZONE_6US_T			90
#define DEADZONE_5_5US_T		82
#define DEADZONE_5US_T			75
#define DEADZONE_4_5US_T		67
#define DEADZONE_4US_T			60
#define DEADZONE_3_5US_T		52
#define DEADZONE_3US_T			45
#define DEADZONE_2_5US_T		37
#define DEADZONE_2US_T			30
#define DEADZONE_1_5US_T		22
#define DEADZONE_1US_T			15
#define DEADZONE_0_5US_T		7
#define SVPWM_MI_K							_IQ(1.102657791)
#define SVPWM_MI_FIRST_START				_IQ(0.9069)
#define SVPWM_MI_FIRST_LINE1_END			_IQ(0.9095)
#define SVPWM_MI_FIRST_LINE2_END            _IQ(0.9485)
#define SVPWM_MI_FIRST_LINE1_K				_IQ(-30.23)
#define SVPWM_MI_FIRST_LINE1_OFFSET			_IQ(27.94)
#define SVPWM_MI_FIRST_LINE2_K				_IQ(-8.58)
#define SVPWM_MI_FIRST_LINE2_OFFSET			_IQ(8.23)
#define SVPWM_MI_FIRST_LINE3_K				_IQ(-26.43)
#define SVPWM_MI_FIRST_LINE3_OFFSET			_IQ(25.15)

#define SVPWM_MI_FIRST_END_SECOND_START		_IQ(0.9517)

#define SVPWM_MI_SECOND_LINE1_END			_IQ(0.9529)
#define SVPWM_MI_SECOND_LINE2_END			_IQ(0.9589)
#define SVPWM_MI_SECOND_LINE3_END			_IQ(0.9911)
#define SVPWM_MI_SECOND_LINE4_END			_IQ(0.9987)
#define SVPWM_MI_SECOND_LINE1_K				_IQ(33.198)
#define SVPWM_MI_SECOND_LINE1_OFFSET		_IQ(31.577)
#define SVPWM_MI_SECOND_LINE2_K				_IQ(12.135)
#define SVPWM_MI_SECOND_LINE2_OFFSET		_IQ(11.507)
#define SVPWM_MI_SECOND_LINE3_K				_IQ(7.572)
#define SVPWM_MI_SECOND_LINE3_OFFSET		_IQ(7.130)
#define SVPWM_MI_SECOND_LINE4_K				_IQ(12.181)
#define SVPWM_MI_SECOND_LINE4_OFFSET		_IQ(11.700)
#define SVPWM_MI_SECOND_LINE5_K				_IQ(37.557)
#define SVPWM_MI_SECOND_LINE5_OFFSET		_IQ(37.041)
#define SVPWM_MI_SECOND_END					_IQ(1.0)

typedef struct
{
	// 电压环输入参数
	_iq			Vd_ref;              //Software packaging
	_iq			Vq_ref;              //Software packaging
	_iq         Vd_ref_Filter;       //Software packaging
	_iq			Vq_ref_Filter;       //Software packaging
	_iq         Vd_ref_Filter_025;       //Software packaging
	_iq			Vq_ref_Filter_025;       //Software packaging
	_iq			Vd_ref_HMI;          //Software packaging
	_iq			Vq_ref_HMI;          //Software packaging

	_iq         Vdc;
	_iq         thetaEle;
	_iq         omegaEleHz;
	_iq         pwmPeriod;
	_iq         pwmNzTime;               //
	_iq         thetaCmp_timeGap;
	_iq         thetaCmp;
	_iq         omega;

	//_iq         VPhaseMax;
	_iq         sinTheta;
	_iq         cosTheta;
	_iq         VsAlpha;
	//_iq         VsAlpha_Coeff;
	_iq         VsBeta;
	//_iq         VsBeta_Coeff;

	// 电压环输出参数
	_iq			Vdq;                 //Software packaging
	_iq         Vdq_Filter;          //Software packaging
	_iq			Vdq_HMI;             //Software packaging
	Uint32	    CMPR1;               //Software packaging
	Uint32		CMPR2;               //Software packaging
	Uint32		CMPR3;               //Software packaging
	
	_iq         Ta;
	_iq         Tb;
	_iq         Tc;

	_iq         DeadZoneComEn_Flag;
	//_iq         DeadZoneCom_Coeff_Pos;
	//_iq         DeadZoneCom_Coeff_Neg;
	_iq         DeadZoneCom_Threshold;
	_iq         DeadZoneCom_IdIn;
	_iq         DeadZoneCom_IqIn;
	//_iq         DeadZoneCom_UalphaOut1;
	//_iq         DeadZoneCom_UbetaOut1;
	_iq         DeadZoneCom_IsTheta;
	_iq         DeadZoneCom_Theta;
	_iq         DeadZoneCom_Ta;
	_iq         DeadZoneCom_Tb;
	_iq         DeadZoneCom_Tc;
	_iq         DeadZoneCom_T;
	_iq         DeadZoneCom_deta;
	_iq         DeadZoneCom_Is_limit;
	
	//_iq         Valpha_hfi;
	//_iq         Vbeta_hfi;

	Uint16      Svpwm_way;
}voltLoopVars;

#ifdef _VOLTLOOP_C_
	voltLoopVars	voltLoopPara = {0};
	void voltLoopslowInput(void);
	void voltLoopCtrl(voltLoopVars *v);
	void voltLoopInput(void);
	void voltLoopOutput(void);
	void voltLoopParaClr();
	void voltLoopIparkAngle(void);
#else
	extern voltLoopVars	voltLoopPara;
	extern void voltLoopslowInput(void);
	extern void voltLoopCtrl(voltLoopVars *v);
	extern void voltLoopInput(void);
	extern void voltLoopOutput(void);
	extern void voltLoopParaClr();
	extern void voltLoopIparkAngle(void);
#endif

#endif
