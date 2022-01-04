#ifndef TRQLOOP_H_
#define TRQLOOP_H_
//========================================================================================
// 常量定义
//========================================================================================
#define ELECTRIC_MODE			1
#define GENERATE_MODE		   -1

#define DIFFSPDNUM              80
//========================================================================================
// 数据结构定义
//========================================================================================
typedef struct
{
	// 力矩环输入参数
	//_iq			Torque_cmd;
	// 力矩控制参数
	//_iq			Torque_ref;
	int16		Torque_ref_HMI;
	// 力矩环输出参数
	//_iq			Torque_fbk;
	_iq			Torque_fbk_HMI;
	_iq			IdCmd_real;
	_iq			IqCmd_real;
	_iq         IdCmd_lmt;
	_iq         Max_TorqueEle;
	_iq         Max_TorqueGen;
	Uint16		Max_TorqueEle_HMI;
	Uint16		Max_TorqueGen_HMI;
	//Uint16      weakflag;
} trqLoopVars;

typedef struct
{
	// ~{5g;z8_NBO^VF~}
	floatVar			MTR_TEMP_OV;
	floatVar		    MTR_TEMP_LMT;
	floatVar		    MTRTEMP_OV_LIMIT_P;
	longVar             TRQLMTCOEFF_MTRTEMP;
	longVar             TRQ_MTRTEMP;

	//~{D#?i8_NBO^VF~}
	floatVar			MDL_TEMP_OV;
	floatVar		    MDL_TEMP_LMT;
	floatVar		    MDLTEMP_OV_LIMIT_P;
	longVar             TRQLMTCOEFF_MDLTEMP;
	longVar             TRQ_MDLTEMP;

	//~{9}AwO^VF~}
	floatVar			Current_OV;
	floatVar		    Current_LMT;
	floatVar		    Current_OV_LIMIT_P;
	longVar             TRQLMTCOEFF_Current;
	longVar             TRQ_Current;

	//~{U}Or9}KYO^VF~}
	floatVar			SpdFWD_OV;
	floatVar		    SpdFWD_LMT;
	floatVar		    SpdFWD_OV_LIMIT_P;
	longVar             TRQLMTCOEFF_SpdFWD;
	longVar             TRQ_SpdFbk;

	//~{74Or9}KYO^VF~}
	floatVar			SpdREV_OV;
	floatVar		    SpdREV_LMT;
	floatVar		    SpdREV_OV_LIMIT_P;
	longVar             TRQLMTCOEFF_SpdREV;

	//~{9}Q9O^VF~}
	floatVar	        VoltageDC_OV;
	floatVar		    VoltageDC_OVLMT;
	floatVar		    VoltageDC_OV_LIMIT_P;
	longVar             TRQLMTCOEFF_VoltageDCOV;
	longVar             TRQ_VoltageDC;

	//~{G7Q9O^VF~}
	floatVar			VoltageDC_LACK;
	floatVar		    VoltageDC_LACKLMT;
	floatVar	        VoltageDC_LACK_LIMIT_P;
	longVar             TRQLMTCOEFF_VoltageDCLACK;

	//longVar             TRQLMTCOEFF;

} trq_Limit;

#ifdef _TRQLOOP_C
//========================================================================================
// 变量定义
//========================================================================================
// 输入参数

_iq	trqLoop_trqCmd = 0;
_iq trqLoop_trqAvl = 0;
_iq trqLoop_trqRampstep = 0;
_iq trqLoop_trqRampstep_down = 0;

_iq trqLoop_Lostrqcmd = 0;
_iq trqLoop_LostrqRef = 0;
_iq trqLoop_trqRampcmd = 0;
_iq trqLoop_trqRampref = 0;

_iq trqLoop_trqMaxgencoeff = 0;
_iq trqLoop_trqMaxelecoeff = 0;

_iq trqspdcoeff_trq11 = 0;
_iq trqspdcoeff_trq12 = 0;
_iq trqspdcoeff_trq21 = 0;
_iq trqspdcoeff_trq22 = 0;

_iq trqidqmap_reg0 = 0;
_iq trqidqmap_reg1 = 0;
_iq trqidqmap_reg2 = 0;
_iq trqidqmap_reg3 = 0;

//力矩补偿
_iq idqtrq_spdcoeff = 0;
_iq trqcoeff_spd = 0;
_iq trqLoop_absSpdFbk = 0;
_iq trqidqmap_absSpdFbk = 0;
_iq trqidqmap_SpdFbk = 0;


// 中间控制参数
_iq	trqlmtcoeff = _IQ(1.0);
_iq	trqLoop_trqMax = 0;
int16	trqLoop_EleGenMode = ELECTRIC_MODE;

_iq trqLmt_eletrqmax = 0;
_iq trqLmt_gentrqmax = 0;

// 查表参数
_iq trqidqmap_id11 = 0;
_iq trqidqmap_iq11 = 0;
_iq trqidqmap_id12 = 0;
_iq trqidqmap_iq12 = 0;

_iq trqidqmap_id21 = 0;
_iq trqidqmap_iq21 = 0;
_iq trqidqmap_id22 = 0;
_iq trqidqmap_iq22 = 0;

//查表电流补偿
_iq trqidqmap_idcoeff = 0;
_iq trqidqmap_iqcoeff = 0;

// 查表参数
_iq trqidqmap_idref = 0;
_iq trqidqmap_iqref = 0;

_iq trqidqmap_temp = 0;
_iq trqidqmap_temp1 = 0;
_iq trqidqmap_temp2 = 0;
_iq trqidqmap_reg4 = 0;
_iq trqidqmap_reg5 = 0;
_iq trqidqmap_reg6 = 0;
_iq trqidqmap_reg7 = 0;
_iq trqidqmap_absspd = 0;
_iq trqidqmap_trq = 0;

// 力矩环接口参数
trqLoopVars		trqLoopPara = {0};
trq_Limit       trqLoop_Limit = {0};

//========================================================================================
// 函数声明
//========================================================================================
void trqLoop_Init(void);
void trqLoopCtrl(void);
void trqLoopInput(void);
void trqLoopOutput(void);
void trqLoscoeff(void);
void trqLos(void);
void trqRamp(void);
void trqidqmap(void);
void trqLoopCalStep(void);

#else
//========================================================================================
// 外部引用接口声明
//========================================================================================

extern trqLoopVars		trqLoopPara;
extern _iq trqLoop_trqRampref;
extern trq_Limit       trqLoop_Limit;
extern _iq trqLmt_eletrqmax;
extern _iq trqLmt_gentrqmax;
extern int16	trqLoop_EleGenMode;
extern _iq trqLoop_trqMaxgencoeff;
extern _iq trqLoop_trqMaxelecoeff;
extern _iq trqidqmap_trq;

extern void trqLoop_Init(void);
extern void trqLoopCtrl(void);
extern void trqLoopInput(void);
extern void trqLoopOutput(void);
extern void trqLoopParaClr(void);
extern void trqLoscoeff(void);
extern void trqRamp(void);

#endif	// end of #ifdef _TRQLOOP_C

#endif /* TRQLOOP_H_ */
