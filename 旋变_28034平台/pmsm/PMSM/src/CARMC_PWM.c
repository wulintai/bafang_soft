/* ==================================================================================
File name:       
                    
Originator:	Digital Control Systems Group
			Texas Instruments

Description:   This file contains source for the Full Compare PWM  drivers for the F28x
              
=====================================================================================
History:
-------------------------------------------------------------------------------------
  05-15-2002	Release	Rev 1.0                                      
----------------------------------------------------------------------------------*/
//#include "IQmathLib.h"         /* Include header for IQmath library */
#include "DSP2803x_Device.h"
#include "CARMC_PWM.h"
#include "f280xbmsk.h"
#include "DSP2803x_EPwm_defines.h"
#include "include_c.h"

void PWM_Init(void) 
{       
	EALLOW;                       // Enable EALLOW 
	EPwm1Regs.AQCSFRC.all=0x0005;         //强制上下桥臂封锁输出
	EPwm2Regs.AQCSFRC.all=0x0005;
	EPwm3Regs.AQCSFRC.all=0x0005;
	SysCtrlRegs.PCLKCR0.bit.TBCLKSYNC = 0;

#if 0
   	GpioCtrlRegs.GPAPUD.bit.GPIO12 = 0;    // Enable pull-up on GPIO12 (TZ1)
   	GpioCtrlRegs.GPAQSEL1.bit.GPIO12 = 3;  // Asynch input GPIO12 (TZ1)
   	GpioCtrlRegs.GPAMUX1.bit.GPIO12 = 1;  // Configure GPIO12 as TZ1

	GpioCtrlRegs.GPAPUD.bit.GPIO13 = 0;    // Enable pull-up on GPIO12 (TZ2)
	GpioCtrlRegs.GPAQSEL1.bit.GPIO13 = 3;  // Asynch input GPIO12 (TZ2)
	GpioCtrlRegs.GPAMUX1.bit.GPIO13 = 1;  // Configure GPIO12 as TZ2
#endif
   	
#if 1
    //Init Timer-Base Period Register for EPWM1-EPWM3
    EPwm1Regs.TBPRD = SVPWM_8KHZ;//1875;
    EPwm2Regs.TBPRD = SVPWM_8KHZ;
    EPwm3Regs.TBPRD = SVPWM_8KHZ;

    // Init Timer-Base Phase Register for EPWM1-EPWM3
    EPwm1Regs.TBPHS.half.TBPHS = 0;
    EPwm2Regs.TBPHS.half.TBPHS = 0;
    EPwm3Regs.TBPHS.half.TBPHS = 0;

    // Init Timer-Base Control Register for EPWM1-EPWM3
    EPwm1Regs.TBCTL.all = PWM_INIT_STATE;
    EPwm2Regs.TBCTL.all = PWM_INIT_STATE;
    EPwm3Regs.TBCTL.all = PWM_INIT_STATE;

    EPwm1Regs.TBCTL.bit.SYNCOSEL = TB_CTR_ZERO;          // Pass through
    EPwm2Regs.TBCTL.bit.SYNCOSEL = TB_SYNC_IN;          // Pass through
    EPwm3Regs.TBCTL.bit.SYNCOSEL = TB_SYNC_IN;          // Pass through

    EPwm1Regs.TBCTL.bit.PHSEN = 1;
    EPwm2Regs.TBCTL.bit.PHSEN = 1;
    EPwm3Regs.TBCTL.bit.PHSEN = 1;

    // Init Compare Control Register for EPWM1-EPWM3
    EPwm1Regs.CMPCTL.all = CMPCTL_INIT_STATE;
    EPwm2Regs.CMPCTL.all = CMPCTL_INIT_STATE;
    EPwm3Regs.CMPCTL.all = CMPCTL_INIT_STATE;

    // Init Action Qualifier Output A Register for EPWM1-EPWM3
    EPwm1Regs.AQCTLA.all = AQCTLA_INIT_STATE;
    EPwm2Regs.AQCTLA.all = AQCTLA_INIT_STATE;
    EPwm3Regs.AQCTLA.all = AQCTLA_INIT_STATE;

    // Init Dead-Band Generator Control Register for EPWM1-EPWM3
    EPwm1Regs.DBCTL.all = DBCTL_INIT_STATE;
    EPwm2Regs.DBCTL.all = DBCTL_INIT_STATE;
    EPwm3Regs.DBCTL.all = DBCTL_INIT_STATE;

    // Init Dead-Band Generator Falling/Rising Edge Delay Register for EPWM1-EPWM3
    EPwm1Regs.DBFED = DBCNT_INIT_STATE;
    EPwm1Regs.DBRED = DBCNT_INIT_STATE;
    EPwm2Regs.DBFED = DBCNT_INIT_STATE;
    EPwm2Regs.DBRED = DBCNT_INIT_STATE;
    EPwm3Regs.DBFED = DBCNT_INIT_STATE;
    EPwm3Regs.DBRED = DBCNT_INIT_STATE;

    // Init PWM Chopper Control Register for EPWM1-EPWM3
    EPwm1Regs.PCCTL.all = PCCTL_INIT_STATE;
    EPwm2Regs.PCCTL.all = PCCTL_INIT_STATE;
    EPwm3Regs.PCCTL.all = PCCTL_INIT_STATE;
 
    
	//TZ module
	EPwm1Regs.TZSEL.all 			= 0;
	//EPwm1Regs.TZSEL.bit.OSHT1		= TZ_ENABLE;			// select TZ1
	EPwm1Regs.TZCTL.bit.TZB 		= TZ_FORCE_LO;			// force pull down low
	EPwm1Regs.TZCTL.bit.TZA 		= TZ_FORCE_LO;			// force pull down low
	//EPwm1Regs.TZEINT.bit.OST		= TZ_ENABLE;			// enable TZ interrupt
	EPwm1Regs.TZCLR.all 			= 0x07; 				// clear all int flag,
	
	EPwm2Regs.TZSEL.all 			= 0;
	//EPwm2Regs.TZSEL.bit.OSHT2		= TZ_ENABLE;			// select TZ2
	EPwm2Regs.TZCTL.bit.TZB 		= TZ_FORCE_LO;			// force pull down low
	EPwm2Regs.TZCTL.bit.TZA 		= TZ_FORCE_LO;			// force pull down low
	//EPwm2Regs.TZEINT.bit.OST		= TZ_ENABLE;			// enable TZ interrupt
	EPwm2Regs.TZCLR.all 			= 0x07; 				// clear all int flag,
	
	EPwm3Regs.TZSEL.all 			= 0;
	EPwm3Regs.TZCTL.bit.TZB 		= TZ_FORCE_LO;			// force pull down low
	EPwm3Regs.TZCTL.bit.TZA 		= TZ_FORCE_LO;			// force pull down low
	EPwm3Regs.TZCLR.all 			= 0x07; 				// clear all int flag,
	
	//ET module
	EPwm1Regs.ETSEL.all 		= 0;
	EPwm1Regs.ETSEL.bit.SOCAEN	= 1;				// Enable EPWM1SOCA pulse.
	EPwm1Regs.ETSEL.bit.SOCASEL = ET_CTR_ZERO;		// Generate EPWM1SOCA pulse when TB counter to zero
	EPwm1Regs.ETSEL.bit.INTEN	= ET_1ST;			// Enable EPWM interrupt
	EPwm1Regs.ETSEL.bit.INTSEL	= ET_CTR_ZERO;		// Generate an interrupt when TB counter to zero
	EPwm1Regs.ETPS.all				= 0;
	EPwm1Regs.ETPS.bit.SOCAPRD		= ET_1ST;				// Generate EPWM1SOCA pulse on the first event
	EPwm1Regs.ETPS.bit.INTPRD		= ET_1ST;				// Generate an interrupt on the first event
	EPwm1Regs.ETCLR.bit.SOCA		= 1;
	EPwm1Regs.ETCLR.bit.INT 		= 1;					// Clear spurious interrupt
	
	EPwm2Regs.ETSEL.all 			= 0;
	EPwm2Regs.ETSEL.bit.INTEN		= ET_DISABLE;			// Disable EPWM interrupt
	EPwm2Regs.ETSEL.bit.INTSEL		= ET_CTR_ZERO;			// Generate an interrupt when TB counter to period initial
	EPwm2Regs.ETPS.bit.INTPRD		= ET_1ST;				// Generate an interrupt on the first event
	EPwm2Regs.ETCLR.bit.INT 		= 1;					// Clear spurious interrupt
	
	EPwm3Regs.ETSEL.all 			= 0;
	EPwm3Regs.ETSEL.bit.INTEN		= ET_DISABLE;			// Disable EPWM interrupt
	EPwm3Regs.ETSEL.bit.INTSEL		= ET_CTR_ZERO;			// Generate an interrupt when TB counter to period initial
	EPwm3Regs.ETPS.bit.INTPRD		= ET_1ST;				// Generate an interrupt on the first event
	EPwm3Regs.ETCLR.bit.INT 		= 1;

    //Setting six EPWM as primary output pins
    GpioCtrlRegs.GPAMUX1.bit.GPIO0 = 1;   // EPWM1A pin
    GpioCtrlRegs.GPAMUX1.bit.GPIO1 = 1;   // EPWM1B pin
    GpioCtrlRegs.GPAMUX1.bit.GPIO2 = 1;   // EPWM2A pin
    GpioCtrlRegs.GPAMUX1.bit.GPIO3 = 1;   // EPWM2B pin
    GpioCtrlRegs.GPAMUX1.bit.GPIO4 = 1;   // EPWM3A pin
    GpioCtrlRegs.GPAMUX1.bit.GPIO5 = 1;   // EPWM3B pin
    SysCtrlRegs.PCLKCR0.bit.TBCLKSYNC = 1;
#else
	// EPWM1 initialization
	// TB module
	EPwm1Regs.TBPRD					= 7500;	// SVPWM Timer Period,60MHz频率，8KHz载波
	EPwm1Regs.TBPHS.half.TBPHS      = 0;
	EPwm1Regs.TBCTR					= 0;
	EPwm1Regs.TBCTL.all				= 0;
	EPwm1Regs.TBCTL.bit.FREE_SOFT	= 0;
	EPwm1Regs.TBCTL.bit.PHSDIR		= TB_UP;    //
	EPwm1Regs.TBCTL.bit.CLKDIV		= TB_DIV1;
	EPwm1Regs.TBCTL.bit.HSPCLKDIV	= TB_DIV1;
	EPwm1Regs.TBCTL.bit.SWFSYNC		= 0;
	EPwm1Regs.TBCTL.bit.SYNCOSEL	= TB_CTR_ZERO;
	EPwm1Regs.TBCTL.bit.PRDLD		= TB_SHADOW;
    EPwm1Regs.TBCTL.bit.PHSEN       = TB_ENABLE;            //Phase loading enabled
	EPwm1Regs.TBCTL.bit.CTRMODE		= TB_COUNT_UPDOWN;

	// CC module
	EPwm1Regs.CMPA.all				= 0;
	EPwm1Regs.CMPB					= 0;
	EPwm1Regs.CMPCTL.all			= 0;
	EPwm1Regs.CMPCTL.bit.SHDWAMODE	= CC_SHADOW;
	EPwm1Regs.CMPCTL.bit.LOADAMODE	= CC_CTR_ZERO_PRD;		// load on CTR = Zero or CTR = PRD

	// AQ module
	EPwm1Regs.AQCTLA.all			= 0;
	EPwm1Regs.AQCTLA.bit.CAD		= AQ_CLEAR;
	EPwm1Regs.AQCTLA.bit.CAU		= AQ_SET;

	// DB module
	EPwm1Regs.DBCTL.all				= 0;
	EPwm1Regs.DBCTL.bit.IN_MODE		= DBA_ALL;				// EPWMxA for both falling and rising edges
	EPwm1Regs.DBCTL.bit.POLSEL		= DB_ACTV_HIC;			// active high complementary
	EPwm1Regs.DBCTL.bit.OUT_MODE	= DB_DISABLE;		// fully enable
	EPwm1Regs.DBFED                 = DBCNT_INIT_STATE;
    EPwm1Regs.DBRED                 = DBCNT_INIT_STATE;

	// PC module
	EPwm1Regs.PCCTL.all				= 0;
	EPwm1Regs.PCCTL.bit.CHPEN		= CHP_DISABLE;			// disable PWM chopping function


	// TZ module
	//EALLOW;
	EPwm1Regs.TZSEL.all				= 0;
	EPwm1Regs.TZSEL.bit.OSHT1		= TZ_ENABLE;			// select TZ5
//	EPwm1Regs.TZSEL.bit.OSHT5		= 0;			// select TZ5
	EPwm1Regs.TZCTL.bit.TZB			= TZ_FORCE_LO;
	EPwm1Regs.TZCTL.bit.TZA			= TZ_FORCE_LO;
	EPwm1Regs.TZEINT.bit.OST		= TZ_ENABLE;			// enable TZ interrupt
//	EPwm1Regs.TZFRC.bit.OST 		= 1;                    // used for force output to be low state
	EPwm1Regs.TZCLR.all				= 0x07;					// clear all int flag,
	//EDIS;

	// ET module
	EPwm1Regs.ETSEL.all				= 0;
	//EPwm1Regs.ETSEL.bit.SOCAEN		= 0;					// Enable EPWM1SOCA pulse.
	EPwm1Regs.ETSEL.bit.SOCAEN		= 1;					// Enable EPWM1SOCA pulse.
	EPwm1Regs.ETSEL.bit.SOCASEL		= ET_CTR_ZERO;			// Generate EPWM1SOCA pulse when TB counter to zero
	EPwm1Regs.ETSEL.bit.INTEN		= ET_1ST;			// Disable EPWM interrupt
	EPwm1Regs.ETSEL.bit.INTSEL		= ET_CTR_ZERO;			// Generate an interrupt when TB counter to period initial

	EPwm1Regs.ETPS.all				= 0;
	EPwm1Regs.ETPS.bit.SOCAPRD		= ET_1ST;				// Generate EPWM1SOCA pulse on the first event
	EPwm1Regs.ETPS.bit.INTPRD		= ET_1ST;				// Generate an interrupt on the first event
	EPwm1Regs.ETCLR.bit.INT			= 1;					// Clear spurious interrupt

	// EPWM2 initialization
	// TB module
	EPwm2Regs.TBPRD					= 7500;	// SVPWM Timer Period
	EPwm2Regs.TBPHS.all				= 0; 					// Set Phase register to zero
	EPwm2Regs.TBCTR					= 0; 					// clear TB counter
	EPwm2Regs.TBCTL.all				= 0;
	EPwm2Regs.TBCTL.bit.FREE_SOFT	= 0;					// stop after next inc or dec
	EPwm2Regs.TBCTL.bit.PHSDIR		= TB_UP;				// counts up after syn
	EPwm2Regs.TBCTL.bit.CLKDIV		= TB_DIV1;				// TBCLK counts 100MHz
	EPwm2Regs.TBCTL.bit.HSPCLKDIV	= TB_DIV1; 				// TBCLK = SYSCLK
	EPwm2Regs.TBCTL.bit.SWFSYNC		= 0;
	EPwm2Regs.TBCTL.bit.SYNCOSEL	= TB_SYNC_IN;			// syn out when syn in
	EPwm2Regs.TBCTL.bit.PRDLD		= TB_SHADOW;			// Period register shadowed
	EPwm2Regs.TBCTL.bit.PHSEN		= TB_ENABLE;			// Phase loading ensabled
	EPwm2Regs.TBCTL.bit.CTRMODE		= TB_COUNT_UPDOWN;		// up down counter mode

	// CC module
	EPwm2Regs.CMPA.all				= 0;
	EPwm2Regs.CMPB					= 0;
	EPwm2Regs.CMPCTL.all			= 0;
	EPwm2Regs.CMPCTL.bit.SHDWAMODE	= CC_SHADOW;
	EPwm2Regs.CMPCTL.bit.LOADAMODE	= CC_CTR_ZERO_PRD;		// load on CTR = Zero or CTR = PRD

	// AQ module
	EPwm2Regs.AQCTLA.all			= 0;
	EPwm2Regs.AQCTLA.bit.CAD		= AQ_CLEAR;
	EPwm2Regs.AQCTLA.bit.CAU		= AQ_SET;

	// DB module
	EPwm2Regs.DBCTL.all				= 0;
	EPwm2Regs.DBCTL.bit.IN_MODE		= DBA_ALL;				// EPWMxA for both falling and rising edges
	EPwm2Regs.DBCTL.bit.POLSEL		= DB_ACTV_HIC;			// active high complementary
	EPwm2Regs.DBCTL.bit.OUT_MODE	= DB_DISABLE;		// fully enable
	EPwm2Regs.DBFED					= DBCNT_INIT_STATE;
	EPwm2Regs.DBRED					= DBCNT_INIT_STATE;

	// PC module
	EPwm2Regs.PCCTL.all				= 0;
	EPwm2Regs.PCCTL.bit.CHPEN		= CHP_DISABLE;			// disable PWM chopping function

	// TZ module
	//EALLOW;
	EPwm2Regs.TZSEL.all				= 0;
	EPwm2Regs.TZSEL.bit.OSHT1		= TZ_ENABLE;			// select TZ1
	EPwm2Regs.TZCTL.bit.TZB			= TZ_FORCE_LO;
	EPwm2Regs.TZCTL.bit.TZA			= TZ_FORCE_LO;
	EPwm2Regs.TZEINT.bit.OST		= TZ_ENABLE;			// enable TZ interrupt
//	EPwm2Regs.TZFRC.bit.OST 		= 1;
	EPwm2Regs.TZCLR.all				= 0x07;					// clear all int flag
	//EDIS;

	// ET module
	EPwm2Regs.ETSEL.all				= 0;
	EPwm2Regs.ETSEL.bit.INTEN		= ET_DISABLE;			// Disable EPWM interrupt
	EPwm2Regs.ETSEL.bit.INTSEL		= ET_CTR_ZERO;			// Generate an interrupt when TB counter to period initial
	EPwm2Regs.ETPS.bit.INTPRD		= ET_1ST;				// Generate an interrupt on the first event
	EPwm2Regs.ETCLR.bit.INT			= 1;					// Clear spurious interrupt

	// EPWM3 initialization
	// TB module
	EPwm3Regs.TBPRD					= 7500;	// SVPWM Timer Period
	EPwm3Regs.TBPHS.all				= 0; 					// Set Phase register to zero
	EPwm3Regs.TBCTR					= 0; 					// clear TB counter
	EPwm3Regs.TBCTL.all				= 0;
	EPwm3Regs.TBCTL.bit.FREE_SOFT	= 0;					// stop after next inc or dec
	EPwm3Regs.TBCTL.bit.PHSDIR		= TB_UP;				// counts up after syn
	EPwm3Regs.TBCTL.bit.CLKDIV		= TB_DIV1;				// TBCLK counts 150MHz
	EPwm3Regs.TBCTL.bit.HSPCLKDIV	= TB_DIV1; 				// TBCLK = SYSCLK
	EPwm3Regs.TBCTL.bit.SWFSYNC		= 0;
	EPwm3Regs.TBCTL.bit.SYNCOSEL	= TB_SYNC_IN;			// syn out when syn in
	EPwm3Regs.TBCTL.bit.PRDLD		= TB_SHADOW;			// Period register shadowed
	EPwm3Regs.TBCTL.bit.PHSEN		= TB_ENABLE;			// Phase loading ensabled
	EPwm3Regs.TBCTL.bit.CTRMODE		= TB_COUNT_UPDOWN;		// up down counter mode

	// CC module
	EPwm3Regs.CMPA.all				= 0;
	EPwm3Regs.CMPB					= 0;
	EPwm3Regs.CMPCTL.all			= 0;
	EPwm3Regs.CMPCTL.bit.SHDWAMODE	= CC_SHADOW;
	EPwm3Regs.CMPCTL.bit.LOADAMODE	= CC_CTR_ZERO_PRD;		// load on CTR = Zero or CTR = PRD

	// AQ module
	EPwm3Regs.AQCTLA.all			= 0;
	EPwm3Regs.AQCTLA.bit.CAD		= AQ_CLEAR;
	EPwm3Regs.AQCTLA.bit.CAU		= AQ_SET;

	// DB module
	EPwm3Regs.DBCTL.all				= 0;
	EPwm3Regs.DBCTL.bit.IN_MODE		= DBA_ALL;				// EPWMxA for both falling and rising edges
	EPwm3Regs.DBCTL.bit.POLSEL		= DB_ACTV_HIC;			// active high complementary
	EPwm3Regs.DBCTL.bit.OUT_MODE	= DB_DISABLE;		// fully enable
	EPwm3Regs.DBFED					= DBCNT_INIT_STATE;
	EPwm3Regs.DBRED					= DBCNT_INIT_STATE;

	// PC module
	EPwm3Regs.PCCTL.all				= 0;
	EPwm3Regs.PCCTL.bit.CHPEN		= CHP_DISABLE;			// disable PWM chopping function

	// TZ module
	//EALLOW;
	EPwm3Regs.TZSEL.all				= 0;
	EPwm3Regs.TZSEL.bit.OSHT1		= TZ_ENABLE;			// select TZ1
	EPwm3Regs.TZCTL.bit.TZB			= TZ_FORCE_LO;
	EPwm3Regs.TZCTL.bit.TZA			= TZ_FORCE_LO;
	EPwm3Regs.TZEINT.bit.OST		= TZ_ENABLE;			// enable TZ interrupt
//	EPwm3Regs.TZFRC.bit.OST 		= 1;
	EPwm3Regs.TZCLR.all				= 0x07;					// clear all int flag
	//EDIS;

	// ET module
	EPwm3Regs.ETSEL.all				= 0;
	EPwm3Regs.ETSEL.bit.INTEN		= ET_DISABLE;			// Disable EPWM interrupt
	EPwm3Regs.ETSEL.bit.INTSEL		= ET_CTR_ZERO;			// Generate an interrupt when TB counter to period initial
	EPwm3Regs.ETPS.bit.INTPRD		= ET_1ST;				// Generate an interrupt on the first event
	EPwm3Regs.ETCLR.bit.INT			= 1;

	// Setting six EPWM as primary output pins
	EALLOW;
    GpioCtrlRegs.GPAMUX1.bit.GPIO0 = 1;   // EPWM1A pin
    GpioCtrlRegs.GPAMUX1.bit.GPIO1 = 1;   // EPWM1B pin
    GpioCtrlRegs.GPAMUX1.bit.GPIO2 = 1;   // EPWM2A pin
    GpioCtrlRegs.GPAMUX1.bit.GPIO3 = 1;   // EPWM2B pin
    GpioCtrlRegs.GPAMUX1.bit.GPIO4 = 1;   // EPWM3A pin
    GpioCtrlRegs.GPAMUX1.bit.GPIO5 = 1;   // EPWM3B pin
    SysCtrlRegs.PCLKCR0.bit.TBCLKSYNC = 1;
	EDIS;
#endif
	 EDIS;


}

