#ifndef TRQLMT_H_
#define TRQLMT_H_

#ifdef _TRQLMT_C

_iq trqLmt_spdFbk = 0;
//_iq trqLmt_VdcFbk = 0;
_iq trqLmt_absspdFbk = 0;
//_iq trqLmt_eletrqmax = 0;
//_iq trqLmt_gentrqmax = 0;
_iq trqLmt_spdcoeff = 0;
Uint16 max_power_15s_cnt = 0;

void trqLmtParaClr(void);
void trqLmt(void);
void trqLmtInput(void);
void trqLmtCtrl(void);
void trqLmtOutput(void);

#else

extern void trqLmtParaClr(void);
extern void trqLmt(void);
extern void trqLmtInput(void);
extern void trqLmtCtrl(void);
extern void trqLmtOutput(void);
#endif

#endif /* TRQLMT_H_ */

