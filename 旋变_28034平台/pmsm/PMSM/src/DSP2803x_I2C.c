// TI File $Revision: /main/1 $
// Checkin $Date: December 5, 2008   18:01:02 $
//###########################################################################
//
// FILE:	DSP2803x_I2C.c
//
// TITLE:	DSP2803x SCI Initialization & Support Functions.
//
//###########################################################################
// $TI Release: DSP2803x C/C++ Header Files V1.10 $
// $Release Date: July 27, 2009 $
//###########################################################################

#include "DSP2803x_Device.h"     // DSP2803x Headerfile Include File
#include "DSP2803x_Examples.h"   // DSP2803x Examples Include File

//---------------------------------------------------------------------------
// InitI2C:
//---------------------------------------------------------------------------
// This function initializes the I2C to a known state.
//
void InitI2C(void)
{
	// Initialize I2C-A:
	//I2caRegs.I2CMDR.bit.IRS = 1;
	//I2caRegs.I2CSTR.bit.BB = 1;
	// Initialize I2C-A:
	// Initialize I2C
	I2caRegs.I2CIER.all = 0x0000;//关闭所有中断
	I2caRegs.I2CSTR.all = 0x0FFFF;//
	//I2caRegs.I2CSAR = 0x0050;		// Slave address - EEPROM control code
	I2caRegs.I2CPSC.all = 19;         //100/(9+1)=10Mhz  4;			// Prescaler - need 7-12 Mhz on module clk
	                                   // 7 < I2C_CLOCK_IN/(X+1) < 12
	I2caRegs.I2CCLKL = 45;//50;			// NOTE: must be non zero,I2CCLKL > I2CCLKH
	I2caRegs.I2CCLKH = 45;//40;			// NOTE: must be non zero
	//I2caRegs.I2CIER.all = 0x24;	// Enable SCD & ARDY interrupts

	I2caRegs.I2CSAR=0x0000;
	I2caRegs.I2COAR=0x0000;
	I2caRegs.I2CCNT=0x0000;

	I2caRegs.I2CMDR.all = 0x0020;	// Take I2C out of reset
	   									// Stop I2C when suspended
	I2caRegs.I2CFFTX.all = 0x6000;	// Enable FIFO mode and TXFIFO
	I2caRegs.I2CFFRX.all = 0x2040;	// Enable RXFIFO, clear RXFFINT,
	//I2caRegs.I2CSTR.bit.ARDY = 1;//

	//I2caRegs.I2CMDR.bit.IRS = 1;
	//I2caRegs.I2CSTR.bit.ARDY = 1;//
	//tbd...
}

//---------------------------------------------------------------------------
// Example: InitI2CGpio:
//---------------------------------------------------------------------------
// This function initializes GPIO pins to function as I2C pins
//
// Each GPIO pin can be configured as a GPIO pin or up to 3 different
// peripheral functional pins. By default all pins come up as GPIO
// inputs after reset.
//
// Caution:
// Only one GPIO pin should be enabled for SDAA operation.
// Only one GPIO pin shoudl be enabled for SCLA operation.
// Comment out other unwanted lines.

void InitI2CGpio()
{

   EALLOW;

/* Enable internal pull-up for the selected pins */
// Pull-ups can be enabled or disabled disabled by the user.
// This will enable the pullups for the specified pins.
// Comment out other unwanted lines.

	//GpioCtrlRegs.GPAPUD.bit.GPIO28 = 0;    // Enable pull-up for GPIO28 (SDAA)
	//GpioCtrlRegs.GPAPUD.bit.GPIO29 = 0;	   // Enable pull-up for GPIO29 (SCLA)

	GpioCtrlRegs.GPBPUD.bit.GPIO32 = 0;    // Enable pull-up for GPIO32 (SDAA)
	GpioCtrlRegs.GPBPUD.bit.GPIO33 = 0;	   // Enable pull-up for GPIO33 (SCLA)

/* Set qualification for selected pins to asynch only */
// This will select asynch (no qualification) for the selected pins.
// Comment out other unwanted lines.

    //GpioCtrlRegs.GPAQSEL2.bit.GPIO28 = 3;  // Asynch input GPIO28 (SDAA)
    //GpioCtrlRegs.GPAQSEL2.bit.GPIO29 = 3;  // Asynch input GPIO29 (SCLA)

	GpioCtrlRegs.GPBQSEL1.bit.GPIO32 = 3;  // Asynch input GPIO32 (SDAA)
    GpioCtrlRegs.GPBQSEL1.bit.GPIO33 = 3;  // Asynch input GPIO33 (SCLA)

/* Configure I2C pins using GPIO regs*/
// This specifies which of the possible GPIO pins will be I2C functional pins.
// Comment out other unwanted lines.

	//GpioCtrlRegs.GPAMUX2.bit.GPIO28 = 2;   // Configure GPIO28 for SDAA operation
	//GpioCtrlRegs.GPAMUX2.bit.GPIO29 = 2;   // Configure GPIO29 for SCLA operation

	GpioCtrlRegs.GPBMUX1.bit.GPIO32 = 1;   // Configure GPIO32 for SDAA operation
	GpioCtrlRegs.GPBMUX1.bit.GPIO33 = 1;   // Configure GPIO33 for SCLA operation

    EDIS;
}

//===========================================================================
// End of file.
//===========================================================================
