#ifndef _TRQLMT_C
#define _TRQLMT_C

#include "include_c.h"

/*void trqLmtParaClr(void)
{
	
}*/

void trqLmt(void)
{
	trqLmtInput();
	trqLmtCtrl();
	trqLmtOutput();
}

void trqLmtInput(void)
{
	trqLmt_spdFbk    = sysFbkPara.Speed_fbk_Filter;
	trqLmt_spdFbk    =(trqLmt_spdFbk>>13)<<13;
	//trqLmt_VdcFbk    = sysCfgPara.Vdc;
	trqLmt_spdcoeff  = _IQmpy(_IQ15toIQ(EV_MCU_Para.field.Motor_UDCMAP),sysCfgPara.Vdc_INV);
}
void trqLmtCtrl(void)
{
	}
void trqLmtOutput(void)
{
	//trqLmt_eletrqmax            = _IQ15toIQ(EV_MCU_Para.field.Motor_PeakTorqueEle);
	//trqLmt_gentrqmax            = _IQ15toIQ(EV_MCU_Para.field.Motor_PeakTorqueGen);
}
#endif

