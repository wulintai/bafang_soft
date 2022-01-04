/*
 * faultdetect.h
 *
 *  Created on: 2014-3-19
 *      Author: ThinkPad
 */

#ifndef FAULTDETECT_H_
#define FAULTDETECT_H_

#define NUM_BUFFER_LENGTH_POWER 128


#define InvOvLd110P             _IQ15(1.1)
#define InvOvLd120P             _IQ15(1.2)
#define InvOvLd130P             _IQ15(1.3)
#define InvOvLd140P             _IQ15(1.4)
#define InvOvLd150P             _IQ15(1.5)
#define InvOvLd165P             _IQ15(1.65)
#define InvOvLd180P             _IQ15(1.8)
#define InvOvLd190P             _IQ15(1.9)
#define InvOvLd200P             _IQ15(2.0)
#define InvOvLd240P             _IQ15(2.4)
#define InvOvLd400P             _IQ15(4.0)

#define MtrOvLd116P             _IQ15(1.16)
#define MtrOvLd137P             _IQ15(1.37)
#define MtrOvLd158P             _IQ15(1.58)
#define MtrOvLd179P             _IQ15(1.79)
#define MtrOvLd201P             _IQ15(2.01)
#define MtrOvLd222P             _IQ15(2.22)
#define MtrOvLd243P             _IQ15(2.43)
#define MtrOvLd400P             _IQ15(4.00)

#define MtrUBl020P              _IQ15(0.2)
#define MtrUBl040P              _IQ15(0.4)
#define MtrUBl060P              _IQ15(0.6)
#define MtrUBl080P              _IQ15(0.8)
#define MtrUBl100P              _IQ15(1.0)
#define MtrUBl120P              _IQ15(1.2)

#ifdef	_FAULTDETECT_C
//========================================================================================
// ~{1dA?6(Re~}
//========================================================================================



unsigned int     FaultDataRecord_en = 0;
unsigned int     FaultDataRecord_num = 0;
unsigned int     FaultI2CRecord_en = 0;
unsigned int     LimitI2CRecord_en = 0;
unsigned int     LimitI2CUpdate_en = 0;
unsigned int     FaultI2CIGBT_flag = 0;


_iq	FaultTrqLmtCoeff = _IQ(1.0);

_iq OvLdProtectCoeff = _IQ15(1.0);
//~{1dF5Fw9}TX1#;$2NJ}~}
_iq InvOvLdRatedCrt = 0;            //~{9}TX1#;$6n6(5gAw~}
_iq InvOvLdTime = 0;                //~{9}TX1#;$J1<d@[;}~}
_iq InvOvLdTimeRamp = 0;            //~{9}TX1#;$J1<d=WT>~}
_iq IpercentInv = 0;                //~{5gAw1H~}
_iq IpercentCoeffInv = 0;           //~{5gAw1HO5J}293%~}
_iq InvProtectCoeff = _IQ15(1.0);            //~{9}TXO^E$>X1#;$O5J}~}
//~{5g;z9}TX1#;$2NJ}~}
_iq MtrOvLdRatedCrt = 0;            //~{9}TX1#;$6n6(5gAw~}
_iq MtrOvLdTime = 0;                //~{9}TX1#;$J1<d@[;}~}
_iq MtrOvLdTimeRamp = 0;            //~{9}TX1#;$J1<d=WT>~}
_iq IpercentMtr = 0;                //~{5gAw1H~}
_iq IpercentCoeffMtr = 0;           //~{5gAw1HO5J}2;3vDG8v~}
_iq MtrProtectCoeff = _IQ15(1.0);            //~{9}TXO^E$>X1#;$O5J}~}
_iq  Frq_Real = 0;
int  OvLdTimeCnt = 0;


//~{Jd3vH1O`<02;F=:b1#;$~}
_iq Ia_checksum = 0;                //A~{O`F+VC@[<F~}
_iq Ib_checksum = 0;                //B~{O`F+VC@[<F~}
_iq Ic_checksum = 0;                //C~{O`F+VC@[<F~}
_iq Cnt_OpenPhase = 0;              //~{H1O`<l2b<FJ}@[<F~}
_iq Spd_OpenPhaseMin = 0;           //~{H1O`<l2bKY6H7'V5~}
_iq Crt_OpenPhaseMin = 0;           //~{H1O`<l2b5gAw7'V5~}

//~{>xT59JUO~}
_iq    Crt_PhaseUnbalanceMin = 0;      //~{>xT59JUO<l2b5gAw7'V5~}
Uint16 Cnt_PhaseUnbalance = 0;         //~{>xT5<l2bJ}>]@[;}4NJ}~}
_iq    Crt_PhaseUnbalance = 0;         //~{>xT5<l2bJ}>]@[;}4NJ}~}
_iq    CrtPer_PhaseUnbalance = 0;         //~{>xT5<l2bJ}>]@[;}4NJ}~}

//~{KY6H<l2b9JUO~}
_iq SpdEn_SpeedDetectFault = 0;
_iq SpdOld_SpeedDetectFault = 0;
_iq SpdDiff_SpeedDetectFault = 0;
_iq Cnt_SpeedDetectFault = 0;
_iq CntTime_SpeedDetectFault = 0;

//~{5gAw<l2b9JUO~}
_iq CrtEn_CrtDetectFault = 0;
_iq Cnt_CrtDetectFault = 0;
_iq hall_fault_cnt = 0;

//========================================================================================
// ~{:/J}IyCw~}
//========================================================================================
interrupt void software_fault_isr(void);
interrupt void IGBT_fault_isr(void);

void IGBT_faultDetect(void);
void IGBT_faultProcess(void);
void IGBT_pwmProcess(void);//¡§?2?t?T¡§¡é??
void IGBT_vceProcess(void);
void OverLoad_faultDetect(void);
void Vdc_faultDetect(void);
void FaultProcess(void);
void SpeedFbk_FaultChecking(void);
void FaultExpress(void);
void I2C_FaultDataRecord(void);
void InvOverLoadProtect(void);
void InvOverLoadProtectInt(void);
void MtrOverLoadProtect(void);
void MtrOverLoadProtectInt(void);

void MtrOpenPhaseProtect(void);
void MtrOpenPhaseProtectInt(void);
void PhaseUnbalanceProtect(void);
void PhaseUnbalanceProtectint(void);
void SpeedDetectProtect(void);
void SpeedDetectProtectint(void);
void CrtDetectProtect(void);
void CrtDetectProtectint(void);
void HallDetectProtect(void);

#else
//========================================================================================
// ~{Mb2?R}SC=S?ZIyCw~}
//========================================================================================
extern Uint32	IGBTFaultIsrAddress;
extern Uint32	SWFaultIsrAddress;
extern Uint32   OverLoadIsrAddress;

extern _iq	FaultTrqLmtCoeff;

extern _iq  Frq_Real;
extern _iq  OvLdProtectCoeff;
extern int  OvLdTimeCnt;
extern _iq  InvProtectCoeff;
extern _iq  MtrProtectCoeff;
extern _iq  InvOvLdTime;
extern _iq  MtrOvLdTime;

extern unsigned int  FaultDataRecord_en;
extern unsigned int  FaultDataRecord_num;
extern unsigned int  FaultI2CRecord_en;
extern unsigned int  LimitI2CRecord_en;
extern unsigned int  LimitI2CUpdate_en;
extern unsigned int  FaultI2CIGBT_flag;

//~{Jd3vH1O`<02;F=:b1#;$~}
extern _iq Ia_checksum;                //A~{O`F+VC@[<F~}
extern _iq Ib_checksum;                //B~{O`F+VC@[<F~}
extern _iq Ic_checksum;                //C~{O`F+VC@[<F~}
extern _iq Cnt_OpenPhase;              //~{H1O`<l2b<FJ}@[<F~}
extern _iq Spd_OpenPhaseMin;           //~{H1O`<l2bKY6H7'V5~}
extern _iq Crt_OpenPhaseMin;           //~{H1O`<l2b5gAw7'V5~}

extern _iq CrtPer_PhaseUnbalance;         //~{>xT5<l2bJ}>]@[;}4NJ}~}
extern _iq Cnt_PhaseUnbalance;         //~{>xT5<l2bJ}>]@[;}4NJ}~}

extern void IGBT_faultDetect(void);
extern void IGBT_faultProcess(void);
extern void IGBT_pwmProcess(void);
extern void IGBT_vceProcess(void);
extern void OverLoad_faultDetect(void);
extern void Vdc_faultDetect(void);
extern void FaultProcess(void);
extern void FaultExpress(void);
extern void I2C_FaultDataRecord(void);
extern void InvOverLoadProtect(void);
extern void InvOverLoadProtectInt(void);
extern void MtrOverLoadProtect(void);
extern void MtrOverLoadProtectInt(void);
extern void HallDetectProtect(void);
extern void SpeedFbk_FaultChecking(void);
extern void MtrOpenPhaseProtect(void);
extern void MtrOpenPhaseProtectInt(void);
extern void PhaseUnbalanceProtect(void);
extern void PhaseUnbalanceProtectint(void);
extern void SpeedDetectProtect(void);
extern void SpeedDetectProtectint(void);
extern void CrtDetectProtect(void);
extern void CrtDetectProtectint(void);

#endif	// end of #ifndef _FAULTDETECT_C

#endif /* FAULTDETECT_H_ */

