/* ==============================================================================

Description:		CARMC程序
          		 
版本:1.02

=================================================================================  */
//#include "IqmathLib.h"         /* Include header for Iqmath library */
#include "include_c.h"

// Prototype statements for functions found within this file.
interrupt void cpu_timer0_isr(void);
interrupt void epwm1_timer_isr(void);

#define PWM1_INT_ENABLE			1
#define PWM2_INT_ENABLE			1
#define PWM3_INT_ENABLE			1
#define PWM4_INT_ENABLE			1
#define PWM5_INT_ENABLE			1
#define PWM6_INT_ENABLE			1

#define StartSVPWMTimer()		EPwm1Regs.ETSEL.bit.INTEN = PWM1_INT_ENABLE
//#define StopSVPWMTimer()		EPwm1Regs.ETSEL.bit.INTEN = 0

extern void 	InitMem(void);
extern void 	InitSysCtrl(void);
extern void 	Init_interrupt(void);
extern void 	InitGpio(void);
extern void 	sub_io_init(void);
extern void 	PWM_Init(void);	
extern void 	InitAdc(void);
extern void		AdcOffsetSelfCal(void);
extern void 	sub_ad_init(void);		//Configure ADC
extern void 	sub_adc_handler(void);
extern void 	sub_ad_zero(void);

void ConfigCpuTimer2(void)
{
    CpuTimer2Regs.PRD.all  = 0xFFFFFFFF;
    // Initialize pre-scale counter to divide by 1 (SYSCLKOUT):
    CpuTimer2Regs.TPR.all  = 0;
    CpuTimer2Regs.TPRH.all = 0;
    // Make sure timer is stopped:
    CpuTimer2Regs.TCR.bit.TSS = 1;
    // Reload all counter register with period value:
    CpuTimer2Regs.TCR.bit.TRB = 1;
    CpuTimer2Regs.TCR.bit.SOFT = 0;
    CpuTimer2Regs.TCR.bit.FREE = 0;
    CpuTimer2Regs.TCR.bit.TIE = 0;
}

//***********************************************
//* 子程序名:InitMem                        
//* 描述:上电ram空间清0处理                        
//* 输入:                                        
//* 输出:                                        
//***********************************************
void InitMem(void)
{
	
	int a;
	int* p1;
	p1=(int*)0x8c00;
	for(a=0;a<0x1400;a++)
	  *p1++=0;
	  
}

/*******************************************************/
//==================================
void main(void)
{
	Uint16 task_index = 0;
// Initialize System Control registers, PLL, WatchDog, Clocks to default state:
        // This function is found in the DSP2803x_SysCtrl.c file.
    EALLOW;
    EDIS;
    InitMem();
	InitSysCtrl();
    
// Disable and clear all CPU interrupts:
	DINT;
	IER = 0x0000;
	IFR = 0x0000;

// Initialize Pie Control Registers To Default State:
        // This function is found in the DSP28_PieCtrl.c file.
	InitPieCtrl();

// Initialize the PIE Vector Table To a Known State:
        // This function is found in DSP2803x_PieVect.c.
	// This function populates the PIE vector table with pointers
        // to the shell ISR functions found in DSP2803x_DefaultIsr.c.
	InitPieVectTable();	
	
	CsmUnlock();
// Section secureRamFuncs contains user defined code that runs from CSM secured RAM
	memcpy(&RamFuncsRunStart,&RamFuncsLoadStart,&RamFuncsLoadEnd - &RamFuncsLoadStart);
	//MemCopy(&RamFuncsLoadStart, &RamFuncsLoadEnd, &RamFuncsRunStart);
	
//*** Initialize the FLASH ***/
	InitFlash();				// Initialize the FLASH (FILE: SysCtrl.c)*/
	
	
// User specific functions, Reassign vectors (optional), Enable Interrupts:
// Reassign ISRs. 
        // Reassign the PIE vector for T1PINT and T3PTINT to point to a different 
        // ISR then the shell routine found in DSP28_DefaultIsr.c.
        // This is done if the user does not want to use the shell ISR routine
        // but instead wants to use their own ISR.  This step is optional:
	EALLOW;	// This is needed to write to EALLOW protected registers
	PieVectTable.TINT0 = &cpu_timer0_isr;
	//PieVectTable.ADCINT1 = &ADCINT1_ISR;
	PieVectTable.EPWM1_INT = &epwm1_timer_isr;
	//PieVectTable.EPWM1_TZINT = &EPWM1_TZINT_ISR;
	//PieVectTable.EPWM2_TZINT = &EPWM2_TZINT_ISR;
	//PieVectTable.ILLEGAL = &ILLEGAL_ISR;
	
	PieVectTable.XINT1 = &XINT1_ISR;    //H1
	PieVectTable.XINT2 = &XINT2_ISR;   	//H2
   	//PieVectTable.XINT3 = &XINT3_ISR;   	//H3

	EDIS;   // This is needed to disable write to EALLOW protected registers
	EPwm1Regs.TBCTR = 0;
	InitGpio();
	sub_io_init();
	PWM_Init();
	Init_interrupt();
	//AdcOffsetSelfCal();
	InitAdc();
	//InitAdcAio();
	sub_ad_zero();
	sub_ad_init();		//Configure ADC


	//==========================================================
	//EINT;   // Enable Global interrupt INTM
	ERTM;	// Enable Global realtime interrupt DBGM
   	KickDog();
	InitCpuTimers();
	ConfigCpuTimer(&CpuTimer0,60, 250);
	//ConfigCpuTimer2();
 	StartCpuTimer0();
 	//StartCpuTimer1();
	//StartCpuTimer2();

	System_Init();

	//Enable the watchdog
   	EALLOW;
   	SysCtrlRegs.WDCR = 0x0028;   
   	EDIS;

	EINT;   // Enable Global interrupt INTM
	while(1)
	{
		KickDog();
		for(task_index=0;task_index<TASK_NUM;task_index++)
		{
			if((TASK_ENABLE == task_struct[task_index].task_enable)
			 &&(TASK_SET_FLAG == task_struct[task_index].task_flag)
			 &&((TASK_FUNC_PTR)0 != task_struct[task_index].task_func))
			{
				(*task_struct[task_index].task_func)();
			}
		}
	}
}

#pragma CODE_SECTION(epwm1_timer_isr,"ramfuncs");
interrupt void epwm1_timer_isr(void)
{
	KickDog();

#if 1
	if (EPwm1Regs.ETSEL.bit.INTSEL == ET_CTR_PRD)
	{
		EPwm1Regs.ETSEL.bit.SOCASEL = ET_CTR_ZERO;
		EPwm1Regs.ETSEL.bit.INTSEL	= ET_CTR_ZERO;
	}
	else if (EPwm1Regs.ETSEL.bit.INTSEL == ET_CTR_ZERO)
	{
		//EPwm1Regs.ETSEL.bit.SOCASEL = ET_CTR_PRD;
		//EPwm1Regs.ETSEL.bit.INTSEL	= ET_CTR_PRD;
	}
#endif

	Resolver_read();
	ThetaCal_Resolver();   //电角度计算
	thetaConfig();
	sub_adc_handler();
	vdcConfig();
	if((scsw2.field.runLoop_state >= MTR_CURRENT_LOOP)
	 &&(1 == scsw2.field.igbt_state))
	{
		crtcmdConfigin();                   //0.2us
		crtcmdConfigctrl(&crtCmd_vars);     //0.7us

		weakcrtLoopin();                    //0.2us
		weakcrtLoopctrl(&weakcrtLoopvars);  //11us

		crtLoopInput();                     //0.3us
		crtLoopCtrl(&crtLoopPara);          //6.1us

		mtrfwdin();                         //0.2us
		mtrfwdctrl(&mtrfwdvars);            //3.2us

		vdqlmtctrl();
	}
	
	if(1==scsw2.field.igbt_state)
	{
		voltLoopInput();
		voltLoopCtrl(&voltLoopPara);
		voltLoopOutput();
	}
	SpeedCal_Resolver();    //速度计算
	FaultExpress();
	IGBT_Clock();
	//oscview();

	EALLOW;
	AdcRegs.SOCPRICTL.bit.RRPOINTER = 0x20;  //reset the sequence
	EDIS;

	EPwm1Regs.ETCLR.bit.INT = 1;
	PieCtrlRegs.PIEACK.bit.ACK3	= 1;
	
	IER |= M_INT3;										// Enable CPU INT3 connected to EPWM1-6 INT
	PieCtrlRegs.PIEIER3.bit.INTx1 = PWM1_INT_ENABLE;	// Enable EPWM1 INT in the PIE: Group 3
}

interrupt void cpu_timer0_isr(void)
{
	//KickDog();
	asm( " CLRC AMODE");
	asm("  NOP");
	asm("  NOP"); 
	asm("  NOP"); 
	asm("  NOP"); 
	asm("  NOP");
	asm("  NOP");
	asm("  NOP");
	asm("  NOP");
	PieCtrlRegs.PIEACK.all = PIEACK_GROUP1;
// Enable CPU INT1 which is connected to CPU-Timer 0:
	IER |= M_INT1;
	EINT;

	taskSchedule();
}





//===========================================================================	
// No more.                                                                  
//===========================================================================
