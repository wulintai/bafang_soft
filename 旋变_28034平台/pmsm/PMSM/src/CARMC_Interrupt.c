/* ==============================================================================

File Name:	CARMC_INTERRUPT.C

=====================================================================================
 History:
-------------------------------------------------------------------------------------
 11-25-2010	Release	Rev 1.0
=================================================================================  */

//#include "IQmathLib.h"         /* Include header for IQmath library */
#include "DSP2803x_Device.h"


void Init_interrupt(void)
{
	EALLOW;	// This is needed to write to EALLOW protected registers

// Configure CPU-Timer 0 to interrupt every second:
//===========TINT0_ISR  ==================	
// 	Enable TINT0 in the PIE: Group 1 interrupt 7
	PieCtrlRegs.PIEIER1.bit.INTx7 = 1;
// 	Enable INT1 which is connected to CPU-Timer 0:
	IER |= M_INT1;

	//===========EPWM1INT_ISR  ==================	
	PieCtrlRegs.PIEIER3.bit.INTx1 = 1;
	IER |= M_INT3;
    EPwm1Regs.ETSEL.bit.INTEN = 1;   // Enable EPWM1INT generation 
    EPwm1Regs.ETSEL.bit.INTSEL = 1;  // Enable interrupt CNT_zero event
    EPwm1Regs.ETPS.bit.INTPRD = 1;   // Generate interrupt on the 1st event
    EPwm1Regs.ETCLR.bit.INT = 1;     // Enable more interrupts
	
//===========ADCINT_ISR  ==================
	//PieCtrlRegs.PIEIER1.bit.INTx1 = 1;
// Enable CPU Interrupt 1
	//IER |= M_INT1;	   // Enable Global INT1
	
	//=======EPWM1_TZINT_ISR EPWM2_TZINT_ISR==================
	// Enable EPWM INTn in the PIE: Group 2 interrupt 1
	//PieCtrlRegs.PIEIER2.bit.INTx1 = 1;		// PWM1 TZ INT
	//IER |= M_INT2;

//=======XINT3_ISR ==================	
//	PieCtrlRegs.PIECTRL.bit.ENPIE = 1;          // Enable the PIE block
//   	PieCtrlRegs.PIEIER12.bit.INTx1 = 1;          // Enable PIE Gropu 12 INT12   
//   	IER |= M_INT12;                              // Enable CPU int12
//=======XINT1_ISR==================	
	PieCtrlRegs.PIECTRL.bit.ENPIE = 1;			// Enable the PIE block
	PieCtrlRegs.PIEIER1.bit.INTx4 = 1;			// Enable PIE Gropu 1 INT4
	IER |= M_INT1;

	//=======XINT2_ISR ==================
	PieCtrlRegs.PIECTRL.bit.ENPIE = 1;			// Enable the PIE block
	PieCtrlRegs.PIEIER1.bit.INTx5 = 1;			// Enable PIE Gropu 1 INT4
	IER |= M_INT1;
	
#if 0
	//=======XINT3_ISR ==================	
	PieCtrlRegs.PIECTRL.bit.ENPIE = 1;			// Enable the PIE block
	PieCtrlRegs.PIEIER12.bit.INTx1 = 1; 		 // Enable PIE Gropu 12 INT12	
	IER |= M_INT12;
#endif


	//=======EPWM1_TZINT_ISR EPWM2_TZINT_ISR==================
	// Enable EPWM INTn in the PIE: Group 2 interrupt 1,2
   	//PieCtrlRegs.PIEIER2.bit.INTx1 = 1;		// PWM1 TZ INT-2.1
   	//PieCtrlRegs.PIEIER2.bit.INTx2 = 1;		// PWM2 TZ INT-2.2
   	//IER |= M_INT2;

	EDIS;   // This is needed to disable write to EALLOW protected registers
}

