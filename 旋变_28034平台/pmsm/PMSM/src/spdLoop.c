#ifndef _SPDLOOP_C_
#define _SPDLOOP_C_

#include "include_c.h"

void spdLoopInput(void)
{
	_iq tooth_trq = 0;
	
	if (MTR_SPEED_LOOP == scsw2.field.runLoop_state)//速度环控制
	{
		spdLoop_spdCmd	= sysCfgPara.SpdCmd;
	}
	else//非速度环控制
	{
		spdLoop_spdCmd  = 0;
	}
	spdLoop_spdMaxPos = _IQ15toIQ(EV_MCU_Para.field.Motor_Protect_OverSpdFwd);
	spdLoop_spdMaxNeg = _IQ15toIQ(EV_MCU_Para.field.Motor_Protect_OverSpdRev);
	spdLoop_StepCaltimeSlice = EV_MCU_Para.field.SpdLoop_StepCaltimeSlice;
	spdLoop_mincalspdstep  = _IQ15toIQ(EV_MCU_Para.field.SpdLoop_Mincalspdstep);
	if(2 == EV_MCU_Para.field.Weak_enable)
	{
		spdLoop_trqupLimit			= _IQ15toIQ(EV_MCU_Para.field.Weak_iq_Limt);
		spdLoop_trqdownLimit        = -_IQ15toIQ(EV_MCU_Para.field.Weak_iq_Limt);
	}
	else
	{
		tooth_trq = _IQ15toIQ(EV_MCU_Para.field.Tooth_trq);
		if(1 == scsw2.field.SpdLmt_state)
		{
			spdLoop_trqupLimit			= trqLoopPara.Max_TorqueEle;
			spdLoop_trqdownLimit        = tooth_trq;
		}
		else if(2 == scsw2.field.SpdLmt_state)
		{
			spdLoop_trqupLimit			= -tooth_trq;
			spdLoop_trqdownLimit        = -trqLoopPara.Max_TorqueGen;
		}
		else
		{
			spdLoop_trqupLimit			= trqLoopPara.Max_TorqueEle;
			spdLoop_trqdownLimit        = -trqLoopPara.Max_TorqueGen;
		}
		//spdLoop_trqupLimit			= trqLoopPara.Max_TorqueEle;
		//spdLoop_trqdownLimit        = -trqLoopPara.Max_TorqueGen;
	}
	if(DIAG_ENCODER_EXACT_OFFSET == diagState)
	{
		spdLoopPara.Kp = _IQ12toIQ(EV_MCU_Para.field.Study_EncoderOffset_kp);
		spdLoopPara.Ki = _IQ12toIQ(EV_MCU_Para.field.Study_EncoderOffset_ki);
		spdLoopPara.Kd = 0;
		spdLoopPara.OutMax = _IQ(0.25);
		spdLoopPara.OutMin = _IQ(0);
		spdLoopPara.ErrMax = _IQ15toIQ(EV_MCU_Para.field.Speed_maxerr);
		spdLoopPara.ErrMin = 0-spdLoopPara.ErrMax;
		spdLoopPara.Study_EncoderOffset_is = _IQ15toIQ(exactTune.IsCmd_man);
	}
	else if(DIAG_MOTOR_FLUEX == diagState)
	{
		spdLoopPara.Kp = _IQ12toIQ(EV_MCU_Para.field.Study_EncoderOffset_kp);
		spdLoopPara.Ki = _IQ12toIQ(EV_MCU_Para.field.Study_EncoderOffset_ki);
		spdLoopPara.Kd = 0;
		spdLoopPara.OutMax = _IQ(0.25);
		spdLoopPara.OutMin = _IQ(0);
		spdLoopPara.ErrMax = _IQ15toIQ(EV_MCU_Para.field.Speed_maxerr);
		spdLoopPara.ErrMin = 0-spdLoopPara.ErrMax;
		spdLoopPara.Study_EncoderOffset_is = 0;
	}
	else
	{
		spdLoopPara.Kp = _IQ12toIQ(EV_MCU_Para.field.Speed_kp);
		spdLoopPara.Ki = _IQ12toIQ(EV_MCU_Para.field.Speed_ki);
		spdLoopPara.Kd = _IQ12toIQ(EV_MCU_Para.field.Speed_kd);
		spdLoopPara.OutMax = spdLoop_trqupLimit;
		spdLoopPara.OutMin = spdLoop_trqdownLimit;
		spdLoopPara.ErrMax = _IQ15toIQ(EV_MCU_Para.field.Speed_maxerr);
		spdLoopPara.ErrMin = 0-spdLoopPara.ErrMax;
		spdLoopPara.Study_EncoderOffset_is = 0;
	}

	spdLoopCalstep();
	spdRamp();

	spdLoopPara.Weak_enable = EV_MCU_Para.field.Weak_enable;
	spdLoopPara.diagState = diagState;
	spdLoopPara.spdLoop_spdRef = spdLoop_spdRef;
	spdLoopPara.Speed_fbk_Filter = sysFbkPara.Speed_fbk_Filter;
}

void spdLoopCalstep(void)
{
	_iq spdLoop_temp = 0;

	spdLoop_deltaspdCmd = _IQabs(spdLoop_spdCmd-spdLoop_spdRef);
	spdLoop_temp       = spdLoop_deltaspdCmd/spdLoop_StepCaltimeSlice;
	if(spdLoop_temp  <  spdLoop_mincalspdstep)
	{
		spdLoop_temp  =  spdLoop_mincalspdstep;
	}
	spdLoop_spdCalstep	 =	spdLoop_temp;
}

void spdRamp(void)
{
	spdLoop_spdRamp_upStep = spdLoop_spdCalstep;
	spdLoop_spdRamp_downStep = spdLoop_spdCalstep;
	LinearRamp(spdLoop_spdCmd, &spdLoop_spdRef,
			   spdLoop_spdRamp_upStep, spdLoop_spdRamp_downStep,
			   spdLoop_spdMaxPos, -spdLoop_spdMaxNeg);
}

void spdLoopCtrl(spdLoopVars *v)
{
	_iq sin_value = 0;
	_iq cos_value = 0;
	_iq mTotalMax = 0;
	_iq mTotalMin = 0;
	
	v->Ref = v->spdLoop_spdRef;
	v->Fdb = v->Speed_fbk_Filter;
	v->Err_old = v->Err;
	v->Err = v->Ref-v->Fdb;
	if((DIAG_ENCODER_EXACT_OFFSET == v->diagState)
	 ||(DIAG_MOTOR_FLUEX == v->diagState))
	{
		if(v->Ref<0)
		{
			v->Err = 0-v->Err;
		}
	}

	v->Err = min(v->Err,v->ErrMax);
	v->Err = max(v->Err,v->ErrMin);
	v->Up = _IQmpy(v->Kp,v->Err);
	v->Ud = _IQmpy(v->Kd,(v->Err-v->Err_old));
	//调整积分的上下限
	if(v->Err>0)
	{
		mTotalMax = v->OutMax - v->Up;
		if(mTotalMax < 0)
		{
			mTotalMax  = 0;
		}
		mTotalMin = v->OutMin;
	}
	else
	{
		mTotalMin = v->OutMin - v->Up;
		if(mTotalMin > 0)
		{
			mTotalMin  = 0;
		}
		mTotalMax = v->OutMax;
	}
	//计算积分作用
	v->Ui = v->Ui+_IQmpy(v->Ki,v->Err);
	v->Ui = _IQsat(v->Ui,mTotalMax,mTotalMin);
	
	/*if((PI_NO_LIMIT == v->LimitState)
	 ||((PI_UP_LIMIT == v->LimitState)&&(v->Err<0))
	 ||((PI_DOWN_LIMIT == v->LimitState)&&(v->Err>0)))
	{
		v->Ui = v->Ui+_IQmpy(v->Ki,v->Err);
		v->Ui = min(v->Ui,v->OutMax);
		v->Ui = max(v->Ui,v->OutMin);
	}*/
	v->Out = v->Up+v->Ui+v->Ud;
	if(v->Out>v->OutMax)
	{
		v->Out = v->OutMax;
		v->LimitState = PI_UP_LIMIT;
	}
	else if(v->Out<v->OutMin)
	{
		v->Out = v->OutMin;
		v->LimitState = PI_DOWN_LIMIT;
	}
	else
	{
		v->LimitState = PI_NO_LIMIT;
	}
	if((DIAG_ENCODER_EXACT_OFFSET == v->diagState)
	 ||(DIAG_MOTOR_FLUEX == v->diagState))
	{
		LPFilter(v->Out,&v->Out_Theta,_IQ(0.05),_IQ(0.95));
		sin_value = _IQsinPU(v->Out_Theta);
		cos_value = _IQcosPU(v->Out_Theta);
		v->Idcmd_real = 0-_IQmpy(_IQabs(v->Study_EncoderOffset_is),cos_value);
		v->Iqcmd_real = _IQmpy(v->Study_EncoderOffset_is,sin_value);
	}
	else
	{
		LPFilter(v->Out,&v->spdLoop_trqCmd,_IQ(0.8),_IQ(0.2));
		if(2 == v->Weak_enable)
		{
			v->Iqcmd_real = v->spdLoop_trqCmd;
		}
		else
		{
			v->TrqCmd_real	= v->spdLoop_trqCmd;
		}
	}
}

void spdLoopParaClr(void)
{
	spdLoop_spdCmd			= 0;
	spdLoop_spdRef			= sysFbkPara.Speed_fbk_Filter;
	spdLoopPara.Idcmd_real	= 0;
	spdLoopPara.Iqcmd_real	= 0;
	spdLoopPara.TrqCmd_real = 0;

	spdLoopPara.Ref=0;
	spdLoopPara.Fdb=0;
	spdLoopPara.Err=0;
	spdLoopPara.LimitState = PI_NO_LIMIT;
	spdLoopPara.Up =0;
	spdLoopPara.Ui =0;
	spdLoopPara.Ud =0;
	spdLoopPara.Out=0;
	spdLoopPara.Err_old =0;
	spdLoopPara.Out_Theta   = 0;
	spdLoopPara.Ref             = sysFbkPara.Speed_fbk_Filter;
	spdLoopPara.spdLoop_trqCmd  = spdLoopPara.Ui    = trqidqmap_trq;
	diff_trq_filter = diff_stop_trq_filter = 0;
}

#endif

