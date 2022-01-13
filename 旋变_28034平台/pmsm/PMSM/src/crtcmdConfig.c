#ifndef _CRTCMDCONFIG_C
#define _CRTCMDCONFIG_C

#include "include_c.h"

#pragma	CODE_SECTION(crtcmdConfigin,		"ramfuncs");
#pragma	CODE_SECTION(crtcmdConfigctrl,		"ramfuncs");
void ctrcmdConfigslowin(void)
{
	crtCmd_vars.runLoop_state 	            = scsw2.field.runLoop_state; //:0:电压环 1：电流环 2：力矩环  3：速度环
	crtCmd_vars.weak_mode                   = EV_MCU_Para.field.Weak_enable;//:0:不自动弱磁 1：查表自动弱磁 2：固定电压利用率弱磁  3：固定电压电动弱磁
	crtCmd_vars.diagState                   = diagState;
	
	//Is current
	crtCmd_vars.IsMTPACmd_man               = _IQ15toIQ(sysCfgPara.IsCmd_man);
	crtCmd_vars.ThetaMTPA_man     	        = sysCfgPara.MTPAtheta_man;
	crtCmd_vars.MTPA_mode     	            = scsw2.field.MTPA_mode;

	//Mtpa current
	//crtCmd_vars.mtpa_study    				= sccw1.field.mtpa_study;
	//crtCmd_vars.Power_ele_fbk               = _IQabs(sysFbkPara.Power_ele_fbk);

	//Man current
	crtCmd_vars.IdCmd_man			        = _IQ15toIQ(sysCfgPara.IdCmd_man);
	crtCmd_vars.IqCmd_man	          		= _IQ15toIQ(sysCfgPara.IqCmd_man);

	crtCmd_vars.Current_step                = _IQmpy(_IQ(0.025),SysBase.inviphase);//0.025A
}

void crtcmdConfigin(void)
{
	//Torque loop
	crtCmd_vars.trqLoop_Idcmd_real          = trqLoopPara.IdCmd_real;
	crtCmd_vars.trqLoop_Iqcmd_real          = trqLoopPara.IqCmd_real;
	//Speed loop
	crtCmd_vars.spdLoop_Idcmd_real          = spdLoopPara.Idcmd_real;
	crtCmd_vars.spdLoop_Iqcmd_real          = spdLoopPara.Iqcmd_real;
}

void crtcmdConfigctrl(tag_crtCmd_vars *v)
{
	_iq sin_value = 0;
	_iq cos_value = 0;
	_iq theta = 0;
	_iq mtpa_id = 0;
	_iq mtpa_iq = 0;
	
	if(MTR_SPEED_LOOP == v->runLoop_state)//速度环
	{
		if((DIAG_ENCODER_EXACT_OFFSET == v->diagState)
		 ||(DIAG_MOTOR_FLUEX == v->diagState))
		{
			v->IdCmd = v->spdLoop_Idcmd_real;
			v->IqCmd = v->spdLoop_Iqcmd_real;
		}
		else
		{
			if(2 == v->weak_mode)
			{
				v->IdCmd = 0;
				v->IqCmd = v->spdLoop_Iqcmd_real;
			}
			else
			{
				v->IdCmd = v->trqLoop_Idcmd_real;
				v->IqCmd = v->trqLoop_Iqcmd_real;
			}
		}
	}
	else if(MTR_TRQ_LOOP == v->runLoop_state)//力矩环
	{
		if(3 == v->weak_mode)
		{
			v->IdCmd = 0;
			v->IqCmd = 0;
		}
		else
		{
			v->IdCmd = v->trqLoop_Idcmd_real;
			v->IqCmd = v->trqLoop_Iqcmd_real;
		}
	}
	else if(MTR_CURRENT_LOOP == v->runLoop_state)  //电流环
	{
		if(1 == v->MTPA_mode)//Enable MTPA
		{
			if(v->ThetaMTPA_man>90)
			{
				v->ThetaMTPA_man = 90;
			}
			else
			{
				v->ThetaMTPA_man = v->ThetaMTPA_man;
			}
			theta = _IQ(1.0)*v->ThetaMTPA_man/360;
			sin_value = _IQsinPU(theta);
			cos_value = _IQcosPU(theta);
#if 1
			mtpa_id = 0-_IQmpy(_IQabs(v->IsMTPACmd_man),sin_value);
			mtpa_iq = _IQmpy(v->IsMTPACmd_man,cos_value);
			//d axis current
			if(mtpa_id>v->IdCmd)
			{
				v->IdCmd = v->IdCmd+v->Current_step;
				if(v->IdCmd>mtpa_id)
				{
					v->IdCmd = mtpa_id;
				}
			}
			else if(mtpa_id<v->IdCmd)
			{
				v->IdCmd = v->IdCmd-v->Current_step;
				if(v->IdCmd<mtpa_id)
				{
					v->IdCmd = mtpa_id;
				}
			}
			else
			{
				v->IdCmd = mtpa_id;
			}

			//q axis current
			if(mtpa_iq>v->IqCmd)
			{
				v->IqCmd = v->IqCmd+v->Current_step;
				if(v->IqCmd>mtpa_iq)
				{
					v->IqCmd = mtpa_iq;
				}
			}
			else if(mtpa_iq<v->IqCmd)
			{
				v->IqCmd = v->IqCmd-v->Current_step;
				if(v->IqCmd<mtpa_iq)
				{
					v->IqCmd = mtpa_iq;
				}
			}
			else
			{
				v->IqCmd = mtpa_iq;
			}
#else
			v->IdCmd = 0-_IQmpy(_IQabs(v->IsMTPACmd_man),sin_value);
			v->IqCmd = _IQmpy(v->IsMTPACmd_man,cos_value);
#endif
		}
		else
		{
			if((3 == v->weak_mode)
			 ||(4 == v->weak_mode))//电流环标定模式
			{
				//d axis current
				if(v->IdCmd_man>v->IdCmd)
				{
					v->IdCmd = v->IdCmd+v->Current_step;
					if(v->IdCmd>v->IdCmd_man)
					{
						v->IdCmd = v->IdCmd_man;
					}
				}
				else if(v->IdCmd_man<v->IdCmd)
				{
					v->IdCmd = v->IdCmd-v->Current_step;
					if(v->IdCmd<v->IdCmd_man)
					{
						v->IdCmd = v->IdCmd_man;
					}
				}
				else
				{
					v->IdCmd = v->IdCmd_man;
				}

				//q axis current
				if(v->IqCmd_man>v->IqCmd)
				{
					v->IqCmd = v->IqCmd+v->Current_step;
					if(v->IqCmd>v->IqCmd_man)
					{
						v->IqCmd = v->IqCmd_man;
					}
				}
				else if(v->IqCmd_man<v->IqCmd)
				{
					v->IqCmd = v->IqCmd-v->Current_step;
					if(v->IqCmd<v->IqCmd_man)
					{
						v->IqCmd = v->IqCmd_man;
					}
				}
				else
				{
					v->IqCmd = v->IqCmd_man;
				}
			}
			else
			{
				v->IdCmd = v->IdCmd_man;
				v->IqCmd = v->IqCmd_man;
			}
		}
	}
	else
	{
		v->IdCmd = 0;
		v->IqCmd = 0;
	}
}

void crtcmdConfigout(void)
{
	//sccw1.field.mtpa_study   =  crtCmd_vars.mtpa_study;
}

void crtcmdConfigclc(void)
{
    sysCfgPara.IsCmd_man        = 0;
    sysCfgPara.MTPAtheta_man   			      = 15;
    //crtCmd_vars.mtpa_study 		              = 0;
    //crtCmd_vars.ThetaMTPA_study_power_ele_max = 0;

    crtCmd_vars.trqLoop_Idcmd_real =0;
    crtCmd_vars.trqLoop_Iqcmd_real =0;

}
#endif

