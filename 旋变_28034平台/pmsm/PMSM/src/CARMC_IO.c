#ifndef _CARMC_IO_C_
#define _CARMC_IO_C_

#include "include_c.h"

void sub_io_init(void)
{
	EALLOW;
	GpioCtrlRegs.GPAMUX1.bit.GPIO0 = 1;   	// EPWM1A      
	GpioCtrlRegs.GPAMUX1.bit.GPIO1 = 1;   	// EPWM1B      
	GpioCtrlRegs.GPAMUX1.bit.GPIO2 = 1;   	// EPWM2A       
	GpioCtrlRegs.GPAMUX1.bit.GPIO3 = 1;   	// EPWM2B      
	GpioCtrlRegs.GPAMUX1.bit.GPIO4 = 1;   	// EPWM3A       
	GpioCtrlRegs.GPAMUX1.bit.GPIO5 = 1;   	// EPWM3B  

	EPwm1Regs.AQCSFRC.all = 0x0005;         //????¨¦?????¡À?¡¤a??¨º?3?¦Ì¨ª¦Ì???
	EPwm2Regs.AQCSFRC.all = 0x0005;
	EPwm3Regs.AQCSFRC.all = 0x0005;

	EPwm1Regs.DBCTL.bit.OUT_MODE = 0;
	EPwm2Regs.DBCTL.bit.OUT_MODE = 0;
	EPwm3Regs.DBCTL.bit.OUT_MODE = 0;

    GpioCtrlRegs.GPAPUD.bit.GPIO0 = 1;
	GpioCtrlRegs.GPAPUD.bit.GPIO1 = 1;
	GpioCtrlRegs.GPAPUD.bit.GPIO2 = 1;
	GpioCtrlRegs.GPAPUD.bit.GPIO3 = 1;
	GpioCtrlRegs.GPAPUD.bit.GPIO4 = 1;
	GpioCtrlRegs.GPAPUD.bit.GPIO5 = 1;
	
	//AD2S1205_FS1
	GpioDataRegs.GPASET.bit.GPIO6 = 1;
 	GpioCtrlRegs.GPAPUD.bit.GPIO6 = 0;     //Enable pu
 	GpioCtrlRegs.GPAMUX1.bit.GPIO6 = 0;   	// IO PIN ->DRIVE
	GpioCtrlRegs.GPADIR.bit.GPIO6 = 1;  	// GPIO15 = Output
	
	//AD2S1205_FS2
	GpioDataRegs.GPASET.bit.GPIO7 = 1;
 	GpioCtrlRegs.GPAPUD.bit.GPIO7 = 0;     //Enable pu
 	GpioCtrlRegs.GPAMUX1.bit.GPIO7 = 0;   	// IO PIN ->DRIVE
	GpioCtrlRegs.GPADIR.bit.GPIO7 = 1;  	// GPIO15 = Output

	//AD2S1205_RST
	GpioDataRegs.GPASET.bit.GPIO8 = 1;
 	GpioCtrlRegs.GPAPUD.bit.GPIO8 = 0;     //Enable pu
 	GpioCtrlRegs.GPAMUX1.bit.GPIO8 = 0;   	// IO PIN ->DRIVE
	GpioCtrlRegs.GPADIR.bit.GPIO8 = 1;  	// GPIO15 = Output

#if 0
	//GPIO9 is XINT1-OCH
	GpioCtrlRegs.GPAMUX1.bit.GPIO9 = 0;         // GPIO
   	GpioCtrlRegs.GPADIR.bit.GPIO9 = 0;          // input
   	GpioCtrlRegs.GPAQSEL1.bit.GPIO9 = 0;        // XINT1 Synch to SYSCLKOUT only
   	GpioIntRegs.GPIOXINT1SEL.bit.GPIOSEL = 9;   // Xint1 is GPIO9
   	XIntruptRegs.XINT1CR.bit.POLARITY = 0;      // Falling edge interrupt  
   	XIntruptRegs.XINT1CR.bit.ENABLE = 1;        // Enable Xint1

	//DI3_DSP
	GpioCtrlRegs.GPAMUX1.bit.GPIO10 = 0;
	GpioCtrlRegs.GPADIR.bit.GPIO10 = 0;
	GpioCtrlRegs.GPAPUD.bit.GPIO10 = 1; 
#else
	//DO4_DSP
	GpioCtrlRegs.GPAMUX1.bit.GPIO9 = 0; // IO PIN
	GpioCtrlRegs.GPADIR.bit.GPIO9 = 1;	// GPIO11 = OUTPUT	
	GpioCtrlRegs.GPAPUD.bit.GPIO9 = 0;	// Enable pullup on GPIO11	 
	GpioDataRegs.GPASET.bit.GPIO9 = 1;	// Load output latch

	//DO3(YB)_DSP
	GpioCtrlRegs.GPAMUX1.bit.GPIO10 = 0; // IO PIN
	GpioCtrlRegs.GPADIR.bit.GPIO10 = 1;	// GPIO11 = OUTPUT	
	GpioCtrlRegs.GPAPUD.bit.GPIO10 = 0;	// Enable pullup on GPIO11	 
	GpioDataRegs.GPACLEAR.bit.GPIO10 = 1;	// Load output latch
#endif

    //BUZZER
	GpioCtrlRegs.GPAMUX1.bit.GPIO11 = 0; // IO PIN
	GpioCtrlRegs.GPADIR.bit.GPIO11 = 1;	// GPIO11 = OUTPUT	
	GpioCtrlRegs.GPAPUD.bit.GPIO11 = 0;	// Enable pullup on GPIO11	 
	GpioDataRegs.GPACLEAR.bit.GPIO11 = 1;	// Load output latch

#if 0
	//OCH-TZ1
	GpioCtrlRegs.GPADIR.bit.GPIO12 = 0;    // input
	GpioCtrlRegs.GPAPUD.bit.GPIO12 = 0;    // Enable pull-up on GPIO12 (TZ1)
   	GpioCtrlRegs.GPAQSEL1.bit.GPIO12 = 3;  // Asynch input GPIO12(TZ1)
   	GpioCtrlRegs.GPAMUX1.bit.GPIO12 = 1;  // Configure GPIO12 as TZ1
#else
	//GPIO12 is XINT1-OCH
	GpioCtrlRegs.GPAMUX1.bit.GPIO12 = 0;         // GPIO
   	GpioCtrlRegs.GPADIR.bit.GPIO12 = 0;          // input
   	GpioCtrlRegs.GPAQSEL1.bit.GPIO12 = 0;        // XINT1 Synch to SYSCLKOUT only
   	GpioIntRegs.GPIOXINT1SEL.bit.GPIOSEL = 12;   // Xint1 is GPIO12
   	XIntruptRegs.XINT1CR.bit.POLARITY = 0;      // Falling edge interrupt  
   	XIntruptRegs.XINT1CR.bit.ENABLE = 1;        // Enable Xint1
#endif

#if 0
	//FO-TZ2
	GpioCtrlRegs.GPADIR.bit.GPIO13 = 0;    // input
	GpioCtrlRegs.GPAPUD.bit.GPIO13 = 0;    // Enable pull-up on GPIO13 (TZ2)
   	GpioCtrlRegs.GPAQSEL1.bit.GPIO13 = 3;  // Asynch input GPIO13 (TZ2)
   	GpioCtrlRegs.GPAMUX1.bit.GPIO13 = 1;  // Configure GPIO13 as TZ2
#else
	//GPIO13 is XINT2-FO
	GpioCtrlRegs.GPAMUX1.bit.GPIO13 = 0;         // GPIO
   	GpioCtrlRegs.GPADIR.bit.GPIO13 = 0;          // input
   	GpioCtrlRegs.GPAQSEL1.bit.GPIO13 = 0;        // XINT1 Synch to SYSCLKOUT only
   	GpioIntRegs.GPIOXINT2SEL.bit.GPIOSEL = 13;   // Xint1 is GPIO12
   	XIntruptRegs.XINT2CR.bit.POLARITY = 0;      // Falling edge interrupt  
   	XIntruptRegs.XINT2CR.bit.ENABLE = 1;        // Enable Xint1
#endif

	//DI3_DSP
	GpioCtrlRegs.GPAMUX1.bit.GPIO14 = 0;   	// IO PIN-DI3-DSP
	GpioCtrlRegs.GPADIR.bit.GPIO14 = 0;   	// GPIO14 = INPUT
	GpioCtrlRegs.GPAPUD.bit.GPIO14 = 1;	    //Disable pull-up for GPIO14 (brake)

	//DRIVE
	GpioDataRegs.GPACLEAR.bit.GPIO15 = 1;
 	GpioCtrlRegs.GPAPUD.bit.GPIO15 = 0;     //Enable pu
 	GpioCtrlRegs.GPAMUX1.bit.GPIO15 = 0;   	// IO PIN ->DRIVE
	GpioCtrlRegs.GPADIR.bit.GPIO15 = 1;  	// GPIO15 = Output

	//SPIA
	GpioCtrlRegs.GPAPUD.bit.GPIO17 = 0;   // Enable pull-up on GPIO55 (SPISOMIA)
    GpioCtrlRegs.GPAPUD.bit.GPIO18 = 0;   // Enable pull-up on GPIO56 (SPICLKA)
    GpioCtrlRegs.GPAPUD.bit.GPIO19 = 0;   // Enable pull-up on GPIO57 (SPISTEA)

    GpioCtrlRegs.GPAQSEL2.bit.GPIO17 = 3; // Asynch input GPIO17 (SPISOMIA)
    GpioCtrlRegs.GPAQSEL2.bit.GPIO18 = 3; // Asynch input GPIO18 (SPICLKA)
    GpioCtrlRegs.GPAQSEL2.bit.GPIO19 = 3; // Asynch input GPIO19 (SPISTEA)

    GpioCtrlRegs.GPAMUX2.bit.GPIO17 = 1; // Configure GPIO55 as SPISOMIA
    GpioCtrlRegs.GPAMUX2.bit.GPIO18 = 1; // Configure GPIO56 as SPICLKA
    GpioCtrlRegs.GPAMUX2.bit.GPIO19 = 1; // Configure GPIO57 as SPISTEA

    //???
	GpioCtrlRegs.GPAMUX2.bit.GPIO20=1;//QEPA
	GpioCtrlRegs.GPAMUX2.bit.GPIO21=1;//QEPB
	GpioCtrlRegs.GPAMUX2.bit.GPIO23=1;//QEPZ
	GpioCtrlRegs.GPAMUX2.bit.GPIO24=1;//ECAP	 
	GpioCtrlRegs.GPAPUD.bit.GPIO20 = 1;   // Enable pull-up on GPIO20 (EQEP1A)
    GpioCtrlRegs.GPAPUD.bit.GPIO21 = 1;   // Enable pull-up on GPIO21 (EQEP1B)
    GpioCtrlRegs.GPAPUD.bit.GPIO23 = 1;   // Enable pull-up on GPIO21 (EQEP1B)
    GpioCtrlRegs.GPAPUD.bit.GPIO24 = 1;   // Enable pull-up on GPIO21 (ECAP1)

	//DI6_DSP
	GpioCtrlRegs.GPAMUX2.bit.GPIO22 = 0;
	GpioCtrlRegs.GPAPUD.bit.GPIO22 = 1; 
	GpioCtrlRegs.GPADIR.bit.GPIO22 = 0;

	//DI1(WAKEUP)_DSP
	GpioCtrlRegs.GPAMUX2.bit.GPIO25 = 0;
	GpioCtrlRegs.GPAPUD.bit.GPIO25 = 1; 
	GpioCtrlRegs.GPADIR.bit.GPIO25 = 0;

	//DI2(BRAKE)_DSP
	GpioCtrlRegs.GPAMUX2.bit.GPIO26 = 0;
	GpioCtrlRegs.GPAPUD.bit.GPIO26 = 1; 
	GpioCtrlRegs.GPADIR.bit.GPIO26 = 0;
	
	//DI8_DSP
	GpioCtrlRegs.GPAMUX2.bit.GPIO27 = 0;
	GpioCtrlRegs.GPAPUD.bit.GPIO27 = 1; 
	GpioCtrlRegs.GPADIR.bit.GPIO27 = 0;

#if 0
	//SCI
	GpioCtrlRegs.GPAMUX2.bit.GPIO28 = 1;
	GpioCtrlRegs.GPAMUX2.bit.GPIO29 = 1;	
#else
	//DI7_DSP
	GpioCtrlRegs.GPAMUX2.bit.GPIO28 = 0;
	GpioCtrlRegs.GPAPUD.bit.GPIO28 = 1; 
	GpioCtrlRegs.GPADIR.bit.GPIO28 = 0;

	//WAKEUP_DSP
	GpioCtrlRegs.GPAMUX2.bit.GPIO29 = 0; // IO PIN
	GpioCtrlRegs.GPADIR.bit.GPIO29  = 1;	// GPIO29 = OUTPUT	
	GpioCtrlRegs.GPAPUD.bit.GPIO29  = 0;	// Enable pullup on GPIO11	 
	GpioDataRegs.GPACLEAR.bit.GPIO29 = 1;	// Load output latch
#endif

	//CANA
	GpioCtrlRegs.GPAMUX2.bit.GPIO30 = 1;
	GpioCtrlRegs.GPAMUX2.bit.GPIO31 = 1;

	//SDA
	GpioCtrlRegs.GPBMUX1.bit.GPIO32 = 0;
	GpioCtrlRegs.GPBDIR.bit.GPIO32 = 1;
	GpioCtrlRegs.GPBPUD.bit.GPIO32 = 0;
	//SCL
	GpioCtrlRegs.GPBMUX1.bit.GPIO33 = 0;
	GpioCtrlRegs.GPBDIR.bit.GPIO33 = 1;
	GpioCtrlRegs.GPBPUD.bit.GPIO33 = 0;

	//LED2
	GpioCtrlRegs.GPBMUX1.bit.GPIO34 = 0; // IO PIN
	GpioCtrlRegs.GPBDIR.bit.GPIO34 = 1;	// GPIO34 = OUTPUT	
	GpioCtrlRegs.GPBPUD.bit.GPIO34 = 0;	// Enable pullup on GPIO34	 
	GpioDataRegs.GPBCLEAR.bit.GPIO34 = 1;	// Load output latch
	
	GpioCtrlRegs.GPBMUX1.bit.GPIO35 = 1;   // Configure GPIO35 for TDI
	GpioCtrlRegs.GPBMUX1.bit.GPIO36 = 1;   // Configure GPIO36 for TMS	
	GpioCtrlRegs.GPBMUX1.bit.GPIO37 = 1;   // Configure GPIO37 for TDO
	GpioCtrlRegs.GPBMUX1.bit.GPIO38 = 1;   // Configure GPIO38 for TCK	

	//RE_CS
	GpioDataRegs.GPBCLEAR.bit.GPIO39 = 1;   //RE_CS   
	GpioCtrlRegs.GPBPUD.bit.GPIO39 = 0;	    //Enable pull-up for GPIO39 (brake)
	GpioCtrlRegs.GPBMUX1.bit.GPIO39 = 0;   	//IO PIN (Configure GPIO39 for enable resolver operation)
	GpioCtrlRegs.GPBDIR.bit.GPIO39 = 1;  	//GPIO39 = OUTPUT   

	//RLY
	GpioCtrlRegs.GPBMUX1.bit.GPIO40 = 0; // IO PIN
	GpioCtrlRegs.GPBDIR.bit.GPIO40 = 1;	// GPIO16 = OUTPUT	
	GpioCtrlRegs.GPBPUD.bit.GPIO40 = 0;	// Enable pullup on GPIO27	 
	GpioDataRegs.GPBSET.bit.GPIO40 = 1;	// Load output latch

	//DO1_DSP-Fan control
	GpioCtrlRegs.GPBMUX1.bit.GPIO41 = 0; // IO PIN
	GpioCtrlRegs.GPBDIR.bit.GPIO41 = 1;	// GPIO11 = OUTPUT	
	GpioCtrlRegs.GPBPUD.bit.GPIO41 = 0;	// Enable pullup on GPIO11	 
	GpioDataRegs.GPBSET.bit.GPIO41 = 1;	// Load output latch

	//LOT
	GpioCtrlRegs.GPBMUX1.bit.GPIO42 = 0;
	GpioCtrlRegs.GPBPUD.bit.GPIO42 = 1; 
	GpioCtrlRegs.GPBDIR.bit.GPIO42 = 0;

	//DOS
	GpioCtrlRegs.GPBMUX1.bit.GPIO43 = 0;
	GpioCtrlRegs.GPBPUD.bit.GPIO43 = 1; 
	GpioCtrlRegs.GPBDIR.bit.GPIO43 = 0;
	
	//DO2(CONVER)_DSP
	GpioCtrlRegs.GPBMUX1.bit.GPIO44 = 0; // IO PIN
	GpioCtrlRegs.GPBDIR.bit.GPIO44 = 1; // GPIO11 = OUTPUT	
	GpioCtrlRegs.GPBPUD.bit.GPIO44 = 0; // Enable pullup on GPIO11	 
	GpioDataRegs.GPBCLEAR.bit.GPIO44 = 1;	// Load output latch
	
	//SP_SW_DSP
	//GpioCtrlRegs.AIODIR.bit.AIO2= 0;	   // AIO2  INTPUT
	//GpioCtrlRegs.AIOMUX1.bit.AIO2 = 0;   // Configure AIO2 for IO Input
	GpioCtrlRegs.AIOMUX1.bit.AIO2 = 2;  //disable AIO, 
	GpioCtrlRegs.AIOMUX1.bit.AIO4 = 2;  //disable AIO, 
	GpioCtrlRegs.AIOMUX1.bit.AIO6 = 2;  //disable AIO, 
	GpioCtrlRegs.AIOMUX1.bit.AIO10 = 2; //disable AIO, 
	//DI5_DSP
	GpioCtrlRegs.AIODIR.bit.AIO12= 0;	   // AIO12  INTPUT
	GpioCtrlRegs.AIOMUX1.bit.AIO12 = 0;   // Configure AIO12 for IO Input

    //DI4_DSP
	GpioCtrlRegs.AIODIR.bit.AIO14= 0;	   // AIO14  INTPUT
	GpioCtrlRegs.AIOMUX1.bit.AIO14 = 0;   // Configure AIO14 for IO Input

 	EDIS;
}

#endif

//===============================================================




