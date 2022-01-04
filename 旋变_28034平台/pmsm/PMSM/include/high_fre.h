#ifndef _HIGH_FRE_H_
#define _HIGH_FRE_H_

#if 0

#define HIGH_FRE_SUCCESS 1
#define HIGH_FRE_FAILURE 0

#define HIGH_FRE_SAMPLE_ID_NUM	3

typedef struct
{
	_iq high_fre_init_abz_count_value;
	_iq high_fre_theta;
	_iq high_fre_voltage;
	_iq high_fre_iq_n_1;
	_iq high_fre_iq_n;
	_iq high_fre_iq_bp_filter;
	_iq high_fre_iq_lp_filter;
	_iq high_fre_id_positive;
	_iq high_fre_id_negative;
	_iq high_fre_result_theta;
	_iq high_fre_result_we;
	Uint16 high_fre_first_zpulse_arrive_flag;
	Uint16 high_fre_get_nspole_flag;
}HIGH_FRE_STRUCT,* HIGH_FRE_STRUCT_PTR;

typedef struct {  		 
				  _iq  Err;			// Variable: Error
				  _iq  Kp;			// Parameter: Proportional gain
				  _iq  Up;			// Variable: Proportional output 
				  _iq  Ui;			// Variable: Integral output 
				  //_iq  OutMax;		    	// Parameter: Maximum output 
				  //_iq  OutMin;	    		// Parameter: Minimum output
				  _iq  Out;   			// Output: PID output 
				  _iq  Ki;			// Parameter: Integral gain
				  //_iq  Up1;		   	// History: Previous proportional output
				  //_iq LimitState;
				  //_iq ErrMax;
				  //_iq ErrMin;
				 } PIDREG_HIGH_FRE;	


#ifdef _HIGH_FRE_C_
	HIGH_FRE_STRUCT high_fre_str = {0};
	PIDREG_HIGH_FRE pid_high_fre = {0};
	int16 diagInitPositionState = -1;
	_iq diagInitPosition_new = 0;
	_iq diagInitPosition_old = 0;
	_iq diagInitPosition_count = 0;
	void highFrequency_control(void);
	void highFrequency_init(void);
	void highFrequency_learnNSPole(void);
#else
	extern HIGH_FRE_STRUCT high_fre_str;
	extern PIDREG_HIGH_FRE pid_high_fre;
	extern void highFrequency_control(void);
	extern void highFrequency_init(void);
#endif

#endif

#endif
