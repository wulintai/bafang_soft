/* ==================================================================================
File name:        GD200A_PWM.H                     
                    
Originator:	Digital Control Systems Group
			Texas Instruments
Description:  
Header file containing data type and object definitions and 
initializers. Also contains prototypes for the functions in GD200A_PWM.C.
=====================================================================================
 History:
-------------------------------------------------------------------------------------
 05-15-2002	Release	Rev 1.0                                                  
---------------------------------------------------------------------------------- */

#ifndef __F2803X_PWM_H__
#define __F2803X_PWM_H__

#include "f280xbmsk.h"
#include "IQmathLib.h"

/*----------------------------------------------------------------------------
Initialization constant for the F280X Time-Base Control Registers for PWM Generation. 
Sets up the timer to run free upon emulation suspend, count up-down mode
prescaler 1.
----------------------------------------------------------------------------*/
//#define PWM_INIT_STATE ( FREE_RUN_FLAG +         \
//			 TB_SHADOW  +       \
//                         TIMER_CNT_UPDN +         \
//                         HSPCLKDIV_PRESCALE_X_1 + \
//                         CLKDIV_PRESCALE_X_2  +   \
//                         PHSDIR_CNT_UP    +       \
//                         CNTLD_DISABLE )

#define PWM_INIT_STATE ( 0 +         \
			 TB_SHADOW  +       \
                         TIMER_CNT_UPDN +         \
                         HSPCLKDIV_PRESCALE_X_1 + \
                         CLKDIV_PRESCALE_X_2  +   \
                         PHSDIR_CNT_UP    +       \
                         CNTLD_DISABLE )

/*----------------------------------------------------------------------------
Initialization constant for the F280X Compare Control Register. 
----------------------------------------------------------------------------*/
#define CMPCTL_INIT_STATE ( LOADAMODE_ZRO_PRD + \
                            LOADBMODE_ZRO_PRD + \
                            SHDWAMODE_SHADOW + \
                            SHDWBMODE_SHADOW )

/*----------------------------------------------------------------------------
Initialization constant for the F280X Action Qualifier Output A Register. 
----------------------------------------------------------------------------*/
#define AQCTLA_INIT_STATE ( CAU_SET + CAD_CLEAR )

/*----------------------------------------------------------------------------
Initialization constant for the F280X Dead-Band Generator registers for PWM Generation. 
Sets up the dead band for PWM and sets up dead band values.
----------------------------------------------------------------------------*/
#define DBCTL_INIT_STATE  (BP_DISABLE + POLSEL_ACTIVE_HI_CMP)

#define DBCNT_INIT_STATE   90   // 100 counts = 1 usec (delay) * 100 count/usec (for TBCLK = SYSCLK/1)

/*----------------------------------------------------------------------------
Initialization constant for the F280X PWM Chopper Control register for PWM Generation. 
----------------------------------------------------------------------------*/
#define  PCCTL_INIT_STATE  CHPEN_DISABLE

/*----------------------------------------------------------------------------
Initialization constant for the F280X Trip Zone Select Register 
----------------------------------------------------------------------------*/
#define  TZSEL_INIT_STATE  ENABLE_TZ1_CBC
//#define  TZSEL_INIT_STATE  (ENABLE_TZ1_OST+ENABLE_TZ2_OST+ENABLE_TZ3_OST)              
/*----------------------------------------------------------------------------
Initialization constant for the F280X Trip Zone Control Register 
----------------------------------------------------------------------------*/
#define  TZCTL_INIT_STATE ( TZA_HI_Z + TZB_HI_Z + \
                            DCAEVT1_HI_Z + DCAEVT2_HI_Z + \
                            DCBEVT1_HI_Z + DCBEVT2_HI_Z )
                                                                 

/*-----------------------------------------------------------------------------
Define the structure of the PWM Driver Object 
-----------------------------------------------------------------------------*/
typedef struct {   
        int n_period;       /* Input: PWM Half-Period in CPU clock cycles (Q0)  */
        int Mfunc_p;           /* Input: Period scaler (Q15)                */
        int Mfunc_c1;          /* Input: PWM 1&2 Duty cycle ratio (Q15)       */
        int Mfunc_c2;          /* Input: PWM 3&4 Duty cycle ratio (Q15)       */
        int Mfunc_c3;          /* Input: PWM 5&6 Duty cycle ratio (Q15)       */
        _iq	IA;		/* Input: Phase A current for deadband compensation */
        _iq	IB;		/* Input: Phase B current for deadband compensation */
        _iq	IC;		/* Input: Phase C current for deadband compensation */
        int CMPR1_TEMP;		/* Parameter: Buffer of CMPR1 for deadband compensation */
        int CMPR2_TEMP;		/* Parameter: Buffer of CMPR2 for deadband compensation */
        int CMPR3_TEMP;		/* Parameter: Buffer of CMPR3 for deadband compensation */
        int DB_COM;		/* Input: Deadband compensation */
/*		Uint32 tmp;*/
        void (*init)();        /* Pointer to the init function               */
        void (*update)();      /* Pointer to the update function             */
        } PWMGEN ;    

/*-----------------------------------------------------------------------------
Define a PWMGEN_handle
-----------------------------------------------------------------------------*/
typedef PWMGEN *PWMGEN_handle;

/*------------------------------------------------------------------------------
Default Initializers for the F28X PWMGEN Object 
------------------------------------------------------------------------------*/
#define F28X_EV1_FC_PWM_GEN {1000,   \
                              0x7FFF, \
                              0x4000, \
                              0x4000, \
                              0x4000, \
                              0,0,0,\
                              0x00,\
                              0x00,\
                              0x00,\
                              0x00,\
                             (void (*)(long))PWM_Init,  \
                             (void (*)(long))PWM_Update \
                             }

#define F28X_EV2_FC_PWM_GEN {1000,   \
                              0x7FFF, \
                              0x4000, \
                              0x4000, \
                              0x4000, \
                              0x00,\
                              0x00,\
                             (void (*)(long))F28X_EV2_PWM_Init,  \
                             (void (*)(long))F28X_EV2_PWM_Update \
                             }

#define PWMGEN_DEFAULTS 	F28X_EV1_FC_PWM_GEN
/*------------------------------------------------------------------------------
 Prototypes for the functions in F28XPWM.C
------------------------------------------------------------------------------*/
void PWM_Init(void);
void PWM_Update(PWMGEN_handle);

#endif  /*__F28X_PWM_H__*/

