// TI File $Revision: /main/2 $
// Checkin $Date: April 20, 2009   17:40:49 $
//###########################################################################
//
// FILE:	DSP2803x_Lin.c
//
// TITLE:	DSP2803x LIN Initialization & Support Functions.
//
//###########################################################################
// $TI Release: DSP2803x C/C++ Header Files V1.10 $
// $Release Date: July 27, 2009 $
//###########################################################################

#include "DSP2803x_Device.h"
#include "DSP2803x_Examples.h"

//---------------------------------------------------------------------------
// InitLin:
//---------------------------------------------------------------------------
// This function initializes the LIN to a known state.
//

void InitLin(void)
{
   InitLina();
}

void InitLina(void)
{
	//Allow write to protected registers
	EALLOW;

	LinaRegs.SCIGCR0.bit.RESET = 0; //Into reset
	LinaRegs.SCIGCR0.bit.RESET = 1; //Out of reset

	LinaRegs.SCIGCR1.bit.SWnRST = 0; //Into software reset

	//SCI Configurations
	LinaRegs.SCIGCR1.bit.COMMMODE = 0;   	//Idle-Line Mode
	LinaRegs.SCIGCR1.bit.TIMINGMODE = 1; 	//Asynchronous Timing
	LinaRegs.SCIGCR1.bit.PARITYENA = 1;  	//Parity enable.
	LinaRegs.SCIGCR1.bit.PARITY = 1;	//Even Parity
	LinaRegs.SCIGCR1.bit.STOP = 0;		//One Stop Bit
	LinaRegs.SCIGCR1.bit.CLK_MASTER = 1; 	//Enable SCI Clock
	LinaRegs.SCIGCR1.bit.LINMODE = 0;	//SCI Mode
	LinaRegs.SCIGCR1.bit.SLEEP = 0;      	//Sleep mode is disabled.
	LinaRegs.SCIGCR1.bit.MBUFMODE = 0;	//No Buffers Mode
	LinaRegs.SCIGCR1.bit.LOOPBACK = 0;   	//Loop back mode is disabled.
	LinaRegs.SCIGCR1.bit.CONT = 1;		//Continue on Suspend
	LinaRegs.SCIGCR1.bit.RXENA = 1;		//Enable RX
	LinaRegs.SCIGCR1.bit.TXENA = 1;		//Enable TX

	//Ensure IODFT is disabled
    	LinaRegs.IODFTCTRL.bit.IODFTENA = 0x0;

    	//Set transmission length
    	LinaRegs.SCIFORMAT.bit.CHAR = 7;	//Eight bits
    	LinaRegs.SCIFORMAT.bit.LENGTH = 0;   	//One byte

	//Set baudrate
    	//LinaRegs.BRSR.bit.SCI_LIN_PSL = 194;   	//Baud = 9.6khz
    	//LinaRegs.BRSR.bit.M = 5;
    	
    	LinaRegs.BRSR.bit.SCI_LIN_PSL = 389;   	//Baud = 9.6khz
    	LinaRegs.BRSR.bit.M = 10;

    	LinaRegs.SCIGCR1.bit.SWnRST = 1;  	//bring out of software reset

	//Disable write to protected registers
	EDIS;
}


//---------------------------------------------------------------------------
// GenIdParity:
//---------------------------------------------------------------------------
// This function generates the ID parity bits and appends them to the ID.
// Call this function on a desired ID before header generation in master
// mode when LinaRegs.SCIGCR1.bit.PARITYENA is set.
//
Uint16 P0;
Uint16 P1;

Uint16 GenIdParity(Uint16 id)
{
	P0 =  ((id << 6) ^ (id << 5) ^ (id << 4) ^ (id << 2)) & BIT6;
	P1 = (((id << 6) ^ (id << 4) ^ (id << 3) ^ (id << 2)) & BIT7) ^ BIT7;
	return id | P0 | P1;
}

//---------------------------------------------------------------------------
// Example: InitLinGpio:
//---------------------------------------------------------------------------
// This function initializes GPIO pins to function as LIN pins
//
// Each GPIO pin can be configured as a GPIO pin or up to 3 different
// peripheral functional pins. By default all pins come up as GPIO
// inputs after reset.
//
// Caution:
// Only one GPIO pin should be enabled for LIN TX operation.
// Only one GPIO pin shoudl be enabled for LIN RX operation.
// Comment out other unwanted lines.

void InitLinGpio()
{
   InitLinaGpio();
}

void InitLinaGpio()
{
   EALLOW;

/* Enable internal pull-up for the selected pins */
// Pull-ups can be enabled or disabled by the user.
// This will enable the pullups for the specified pins.

//	GpioCtrlRegs.GPAPUD.bit.GPIO9 = 0;		// Enable pull-up for GPIO9 (LIN TX)
//	GpioCtrlRegs.GPAPUD.bit.GPIO14 = 0;		// Enable pull-up for GPIO14 (LIN TX)
//	GpioCtrlRegs.GPAPUD.bit.GPIO18 = 0;    	// Enable pull-up for GPIO18 (LIN TX)
	GpioCtrlRegs.GPAPUD.bit.GPIO22 = 0;		// Enable pull-up for GPIO22 (LIN TX)

//	GpioCtrlRegs.GPAPUD.bit.GPIO11 = 0;		// Enable pull-up for GPIO11 (LIN RX)
//	GpioCtrlRegs.GPAPUD.bit.GPIO15 = 0;		// Enable pull-up for GPIO15 (LIN RX)
//	GpioCtrlRegs.GPAPUD.bit.GPIO19 = 0;	   	// Enable pull-up for GPIO19 (LIN RX)
	GpioCtrlRegs.GPAPUD.bit.GPIO23 = 0;		// Enable pull-up for GPIO23 (LIN RX)

/* Set qualification for selected pins to asynch only */
// Inputs are synchronized to SYSCLKOUT by default.
// This will select asynch (no qualification) for the selected pins.

//	GpioCtrlRegs.GPAQSEL1.bit.GPIO11 = 3;  // Asynch input GPIO11 (LINRXA)
//	GpioCtrlRegs.GPAQSEL1.bit.GPIO15 = 3;  // Asynch input GPIO15 (LINRXA)
//	GpioCtrlRegs.GPAQSEL2.bit.GPIO19 = 3;  // Asynch input GPIO19 (LINRXA)
	GpioCtrlRegs.GPAQSEL2.bit.GPIO23 = 3;  // Asynch input GPIO23 (LINRXA)


/* Configure LIN-A pins using GPIO regs*/
// This specifies which of the possible GPIO pins will be LIN pins.
// Only one set of pins should be enabled at any time for LIN operation.

//	GpioCtrlRegs.GPAMUX1.bit.GPIO9 = 2;    // Configure GPIO9 for LIN TX operation (2-Enable,0-Disable)
//	GpioCtrlRegs.GPAMUX1.bit.GPIO14 = 2;   // Configure GPIO14 for LIN TX operation (2-Enable,0-Disable)
//	GpioCtrlRegs.GPAMUX2.bit.GPIO18 = 2;   // Configure GPIO18 for LIN TX operation (2-Enable,0-Disable)
	GpioCtrlRegs.GPAMUX2.bit.GPIO22 = 3;   // Configure GPIO19 for LIN TX operation	 (3-Enable,0-Disable)

//	GpioCtrlRegs.GPAMUX1.bit.GPIO11 = 2;   // Configure GPIO11 for LIN RX operation	 (2-Enable,0-Disable)
//	GpioCtrlRegs.GPAMUX1.bit.GPIO15 = 2;   // Configure GPIO15 for LIN RX operation (2-Enable,0-Disable)
//	GpioCtrlRegs.GPAMUX2.bit.GPIO19 = 2;   // Configure GPIO19 for LIN RX operation (2-Enable,0-Disable)
	GpioCtrlRegs.GPAMUX2.bit.GPIO23 = 3;   // Configure GPIO23 for LIN RX operation (3-Enable,0-Disable)

    EDIS;
}

//===========================================================================
// End of file.
//===========================================================================










