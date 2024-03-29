/* =================================================================================
File name:       PID_REG3.H  (IQ version)                    
                    
Originator:	Digital Control Systems Group
			Texas Instruments

Description: 
Header file containing constants, data type definitions, and 
function prototypes for the PIDREG3.
=====================================================================================
 History:
-------------------------------------------------------------------------------------
 04-15-2005	Version 3.20
------------------------------------------------------------------------------*/
#ifndef __PIDREG3_H__
#define __PIDREG3_H__

#define PI_NO_LIMIT		0
#define PI_UP_LIMIT		1
#define PI_DOWN_LIMIT	2

typedef struct {  		  _iq  Ref;   			// Input: Reference input 
				  _iq  Fdb;   			// Input: Feedback input 
				  _iq  Err;			// Variable: Error
				  _iq  Err_old;
				  _iq  Kp;			// Parameter: Proportional gain
				  _iq  Up;			// Variable: Proportional output 
				  _iq  Ui;			// Variable: Integral output 
				  _iq  Ud;			// Variable: Derivative output 	
				  //_iq  OutPreSat; 		// Variable: Pre-saturated output
				  _iq  OutMax;		    	// Parameter: Maximum output 
				  _iq  OutMin;	    		// Parameter: Minimum output
				  _iq  Out;   			// Output: PID output 
				  //_iq  SatErr;			// Variable: Saturated difference
				  _iq  Ki;			// Parameter: Integral gain
				  _iq  Kc;		     	// Parameter: Integral correction gain
				  _iq  Kd; 		        // Parameter: Derivative gain
				  //_iq  Up1;		   	// History: Previous proportional output
				  _iq LimitState;
				  _iq Out_fwd;
				  //_iq Errrate;
				  //_iq Errrate_filter;
				  _iq Temp;
				  _iq Excess;
				  //_iq ErrMax;
				  //_iq ErrMin;
				  //unsigned int pid_mode;
		 	 	  //void  (*calc)();	  	// Pointer to calculation function
				 } PIDREG3;	            

typedef PIDREG3 *PIDREG3_handle;
/*-----------------------------------------------------------------------------
Default initalizer for the PIDREG3 object.
-----------------------------------------------------------------------------*/                     
/*#define PIDREG3_DEFAULTS { 0, \
                           0, \
                           0, \
                           _IQ(1.3), \
                           0, \
                           0, \
                           0, \
                           0, \
                           _IQ(1), \
                           _IQ(0), \
                           0, \
                           0, \
                           _IQ(0.02), \
                           _IQ(0.5), \
                           _IQ(1.05), \
                           0, \
                           0,\
            		(void (*)(Uint32))pid_reg3_calc }*/

/*------------------------------------------------------------------------------
Prototypes for the functions in PIDREG3.C
------------------------------------------------------------------------------*/
#ifdef __PIDREG3_C__
	void pid_reg3_calc(PIDREG3_handle);
	void pidreg3_init(PIDREG3 *v);
	void pidreg3_calc(PIDREG3 *v);
	void pidreg2_calc(PIDREG3 *v);
#else
	extern void pidreg3_init(PIDREG3 *v);
	extern void pidreg3_calc(PIDREG3 *v);
	extern void pidreg2_calc(PIDREG3 *v);
#endif

#endif // __PIDREG3_H__
