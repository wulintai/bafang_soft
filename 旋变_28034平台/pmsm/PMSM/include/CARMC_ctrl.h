#ifndef DSP28034_CARMCCTRL_H
#define DSP28034_CARMCCTRL_H

#define MAX_CURRENT_POS_AD		2047					// 12bit ADC
#define MAX_CURRENT_NEG_AD		-2047
#define MAX_VDC_AD			 	4095
#define MAX_TEMPERATION_AD		4095

// 传感器初始零位标定常数
#define CALIB_DELAYNUM			5000					// 延时20000采样周期
#define CALIB_ACCUMNUM			4096					// 累加10000次采样结果求取平均
#define CALIB_OFFSET_UPLIMIT	2200					//1.87*4096/3	// 1.87V
#define CALIB_OFFSET_DOWNLIMIT	1900					//1.47*4096/3	// 1.47V

/*******************************************************************************************/
#ifdef	_CARMC_CTRL_C_ 
	Uint16 Iu_zero = 0;
	Uint16 Iv_zero = 0;
	Uint16 Iw_zero = 0;
	Uint16 Acor_zero = 0;
	Uint16 first_adCalib_state_finsh = 0;
	_iq	sin_theta = 0;
	_iq	cos_theta = 0;
	_iq	Is_alpha = 0;
	_iq	Is_beta = 0;
	_iq	Iu_fbk_raw	= 0;
	_iq	Iv_fbk_raw	= 0;
	_iq	Iw_fbk_raw	= 0;
#else
	extern Uint16 Iu_zero;
	extern Uint16 Iv_zero;
	extern Uint16 Iw_zero;
	extern Uint16 Acor_zero;
	extern Uint16 first_adCalib_state_finsh;
	extern _iq	sin_theta;
	extern _iq	cos_theta;
	extern _iq	Is_alpha;
	extern _iq	Is_beta;
#endif

void 	sub_adc_handler(void);
void 	sub_ad_zero(void);

#endif

