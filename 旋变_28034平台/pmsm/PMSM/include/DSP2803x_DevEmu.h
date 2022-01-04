// TI File $Revision: /main/5 $
// Checkin $Date: July 6, 2009   13:52:00 $
//###########################################################################
//
// FILE:   DSP2803x_DevEmu.h
//
// TITLE:  DSP2803x Device Emulation Register Definitions.
//
//###########################################################################
// $TI Release: DSP2803x C/C++ Header Files V1.10 $
// $Release Date: July 27, 2009 $
//###########################################################################

#ifndef DSP2803x_DEV_EMU_H
#define DSP2803x_DEV_EMU_H

#ifdef __cplusplus
extern "C" {
#endif

//---------------------------------------------------------------------------
// Device Emulation Register Bit Definitions:
//
// Device Configuration Register Bit Definitions
struct DEVICECNF_BITS  {     // bits  description
   Uint16 rsvd1:3;           // 2:0   reserved
   Uint16 VMAPS:1;           // 3     VMAP Status
   Uint16 rsvd2:1;           // 4     reserved
   Uint16 XRSn:1;            // 5     XRSn Signal Status
   Uint16 rsvd3:10;          // 15:6  reserved
   Uint16 rsvd4:3;           // 18:16 reserved
   Uint16 ENPROT:1;          // 19    Enable/Disable pipeline protection
   Uint16 rsvd5:7;           // 26:21 reserved
   Uint16 TRSTn:1;           // 27    Status of TRSTn signal
   Uint16 PINOUTSELSTS:1;	 // 28    Status of PINOUSTSEL pin
   Uint16 PINOUTSELPUD:1;	 // 29	  PINOUTSEL pin pull-up disable
   Uint16 rsvd6:2;           // 31:30 reserved
};

union DEVICECNF_REG {
   Uint32                 all;
   struct DEVICECNF_BITS  bit;
};

// CLASSID
struct CLASSID_BITS   {  // bits  description
   Uint16 CLASSNO:8;     // 7:0   Class Number
   Uint16 PARTTYPE:8;    // 15:8  Part Type
};

union CLASSID_REG {
   Uint16               all;
   struct CLASSID_BITS  bit;
};

// BORCFG
struct BORCFG_BITS   {  // bits  description
   Uint16 BORENZ:1;     // 0     BOR Enable Active Low Bit
   Uint16 rsvd1:15;     // 15:1  reserved
};

union BORCFG_REG {
   Uint16               all;
   struct BORCFG_BITS  bit;
};


struct DEV_EMU_REGS {
   union DEVICECNF_REG DEVICECNF;  // Device Configuration
   union CLASSID_REG   CLASSID;    // Class ID
   Uint16              REVID;      // Device ID
   Uint16              rsvd1[257]; // reserved
   union BORCFG_REG    BORCFG;     // BOR Configuration
};

// PARTID
struct PARTID_BITS   {  // bits  description
   Uint16 PARTNO:8;     // 7:0   Part Number
   Uint16 PARTTYPE:8;   // 15:8  Part Type
};

union PARTID_REG {
   Uint16               all;
   struct PARTID_BITS   bit;
};

struct PARTID_REGS {
   union PARTID_REG PARTID; // Part ID
};


//---------------------------------------------------------------------------
// Device Emulation Register References & Function Declarations:
//
extern volatile struct DEV_EMU_REGS DevEmuRegs;
extern volatile struct PARTID_REGS PartIdRegs;

#ifdef __cplusplus
}
#endif /* extern "C" */

#endif  // end of DSP2803x_DEV_EMU_H definition

//===========================================================================
// End of file.
//===========================================================================
