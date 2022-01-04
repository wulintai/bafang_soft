#ifndef _CRTLOOP_C_
#define _CRTLOOP_C_

#include "include_c.h"

#pragma	CODE_SECTION(crtLoopInput,		"ramfuncs");
//#pragma	CODE_SECTION(crtLoopOutput,	    "ramfuncs");
#pragma	CODE_SECTION(crtLoopCtrl,		"ramfuncs");

void crtLoopslowInput()
{
	_iq pid_value = 0;
	_iq speed_deta1 = 0;
	_iq speed_deta2 = 0;
	_iq pid_deta = 0;
	_iq speed_ramp = 0;
	
	crtLoopPara.Motor_Direction         = EV_MCU_Para.field.Motor_Direction;
	if(1 == EV_MCU_Para.field.Idq_pi_swap_en)
	{
		speed_deta1 = sysFbkPara.Speed_fbk_HMI_abs-EV_MCU_Para.field.Idq_low_speed;//IQ15
		speed_deta2 = EV_MCU_Para.field.Idq_high_speed-EV_MCU_Para.field.Idq_low_speed;//IQ15
		if(speed_deta2<=0)
		{
			crtLoopPara.IdPid_Kp				= _IQ12toIQ(EV_MCU_Para.field.Id_kp);    //IQ12
			crtLoopPara.IdPid_Ki			    = _IQ12toIQ(EV_MCU_Para.field.Id_ki);
			//crtLoopPara.IdPid_Kd           	    = 0;
			crtLoopPara.IqPid_Kp				= _IQ12toIQ(EV_MCU_Para.field.Iq_kp);    //IQ12
			crtLoopPara.IqPid_Ki			    = _IQ12toIQ(EV_MCU_Para.field.Iq_ki);
			//crtLoopPara.IqPid_Kd           	    = 0;
		}
		else
		{
			if(speed_deta1<=0)
			{
				crtLoopPara.IdPid_Kp				= _IQ12toIQ(EV_MCU_Para.field.Id_kp);    //IQ12
				crtLoopPara.IdPid_Ki			    = _IQ12toIQ(EV_MCU_Para.field.Id_ki);
				//crtLoopPara.IdPid_Kd           	    = 0;
				crtLoopPara.IqPid_Kp				= _IQ12toIQ(EV_MCU_Para.field.Iq_kp);    //IQ12
				crtLoopPara.IqPid_Ki			    = _IQ12toIQ(EV_MCU_Para.field.Iq_ki);
				//crtLoopPara.IqPid_Kd           	    = 0;
			}
			else if(speed_deta1>=speed_deta2)
			{
				crtLoopPara.IdPid_Kp				= _IQ12toIQ(EV_MCU_Para.field.Id_high_speed_kp);    //IQ12
				crtLoopPara.IdPid_Ki			    = _IQ12toIQ(EV_MCU_Para.field.Id_high_speed_ki);
				//crtLoopPara.IdPid_Kd           	    = 0;
				crtLoopPara.IqPid_Kp				= _IQ12toIQ(EV_MCU_Para.field.Iq_high_speed_kp);    //IQ12
				crtLoopPara.IqPid_Ki			    = _IQ12toIQ(EV_MCU_Para.field.Iq_high_speed_ki);
				//crtLoopPara.IqPid_Kd           	    = 0;
			}
			else
			{
				speed_ramp  = _IQdiv(speed_deta1,speed_deta2);//IQ24

				//Id Kp
				pid_deta    = EV_MCU_Para.field.Id_high_speed_kp-EV_MCU_Para.field.Id_kp;//IQ12
				pid_value   = _IQmpy(_IQ12toIQ(pid_deta),speed_ramp)+_IQ12toIQ(EV_MCU_Para.field.Id_kp);//IQ24
				crtLoopPara.IdPid_Kp                = pid_value;

				//Id Ki
				pid_deta    = EV_MCU_Para.field.Id_high_speed_ki-EV_MCU_Para.field.Id_ki;//IQ12
				pid_value   = _IQmpy(_IQ12toIQ(pid_deta),speed_ramp)+_IQ12toIQ(EV_MCU_Para.field.Id_ki);//IQ24
				crtLoopPara.IdPid_Ki                = pid_value;

				//Iq Kp
				pid_deta    = EV_MCU_Para.field.Iq_high_speed_kp-EV_MCU_Para.field.Iq_kp;//IQ12
				pid_value   = _IQmpy(_IQ12toIQ(pid_deta),speed_ramp)+_IQ12toIQ(EV_MCU_Para.field.Iq_kp);//IQ24
				crtLoopPara.IqPid_Kp                = pid_value;

				//Iq Ki
				pid_deta    = EV_MCU_Para.field.Iq_high_speed_ki-EV_MCU_Para.field.Iq_ki;//IQ12
				pid_value   = _IQmpy(_IQ12toIQ(pid_deta),speed_ramp)+_IQ12toIQ(EV_MCU_Para.field.Iq_ki);//IQ24
				crtLoopPara.IqPid_Ki                = pid_value;
			}
		}
	}
	else
	{
		crtLoopPara.IdPid_Kp				= _IQ12toIQ(EV_MCU_Para.field.Id_kp);    //IQ12
		crtLoopPara.IdPid_Ki			    = _IQ12toIQ(EV_MCU_Para.field.Id_ki);
		//crtLoopPara.IdPid_Kd           	    = 0;
		crtLoopPara.IqPid_Kp				= _IQ12toIQ(EV_MCU_Para.field.Iq_kp);    //IQ12
		crtLoopPara.IqPid_Ki			    = _IQ12toIQ(EV_MCU_Para.field.Iq_ki);
		//crtLoopPara.IqPid_Kd           	    = 0;
	}
}

void crtLoopCtrl(crtLoopVars *v)
{
#if 0
	v->IdPid_Ref = v->IdCmd;
	v->IdPid_Fdk = v->IdPid_Fdk;
	v->IdPid_OutMax = v->Vdc;
	v->IdPid_OutMin = 0-v->IdPid_OutMax;
	v->IdPid_Err_old = v->IdPid_Err;
	v->IdPid_Err = v->IdPid_Ref-v->IdPid_Fdk;
	v->IdPid_Up = _IQmpy(v->IdPid_Kp,v->IdPid_Err);
	if((PI_NO_LIMIT == v->IdPid_LimitState)
	 ||((PI_UP_LIMIT == v->IdPid_LimitState)&&(v->IdPid_Err<0))
	 ||((PI_DOWN_LIMIT == v->IdPid_LimitState)&&(v->IdPid_Err>0)))
	{
		v->IdPid_Ui = v->IdPid_Ui+_IQmpy(v->IdPid_Ki,v->IdPid_Err);
		v->IdPid_Ui = _IQsat(v->IdPid_Ui,v->IdPid_OutMax,v->IdPid_OutMin);
	}
	v->IdPid_Out = v->IdPid_Up+v->IdPid_Ui;
	if(v->IdPid_Out>v->IdPid_OutMax)
	{
		v->IdPid_LimitState = PI_UP_LIMIT;
		v->IdPid_Out = v->IdPid_OutMax;
	}
	else if(v->IdPid_Out<v->IdPid_OutMin)
	{
		v->IdPid_LimitState = PI_DOWN_LIMIT;
		v->IdPid_Out = v->IdPid_OutMin;
	}
	else
	{
		v->IdPid_LimitState = PI_NO_LIMIT;
		v->IdPid_Out = v->IdPid_Out;
	}
	v->Id_PI_Vd = v->IdPid_Out;

	if(MOTOR_CLOCKWISE == v->Motor_Direction)
	{
		v->IqPid_Ref = 0-v->IqCmd;
	}
	else
	{
		v->IqPid_Ref = v->IqCmd;
	}
	v->IqPid_Fdk = v->IqPid_Fdk;
	v->IqPid_OutMax = v->Vdc;
	v->IqPid_OutMin = -v->Vdc;
	v->IqPid_Err_old = v->IqPid_Err;
	v->IqPid_Err = v->IqPid_Ref-v->IqPid_Fdk;
	v->IqPid_Up = _IQmpy(v->IqPid_Kp,v->IqPid_Err);
	if((PI_NO_LIMIT == v->IqPid_LimitState)
	 ||((PI_UP_LIMIT == v->IqPid_LimitState)&&(v->IqPid_Err<0))
	 ||((PI_DOWN_LIMIT == v->IqPid_LimitState)&&(v->IqPid_Err>0)))
	{
		v->IqPid_Ui = v->IqPid_Ui+_IQmpy(v->IqPid_Ki,v->IqPid_Err);
		v->IqPid_Ui = _IQsat(v->IqPid_Ui,v->IqPid_OutMax,v->IqPid_OutMin);
	}
	v->IqPid_Out = v->IqPid_Up+v->IqPid_Ui;
	if(v->IqPid_Out>v->IqPid_OutMax)
	{
		v->IqPid_LimitState = PI_UP_LIMIT;
		v->IqPid_Out = v->IqPid_OutMax;
	}
	else if(v->IqPid_Out<v->IqPid_OutMin)
	{
		v->IqPid_LimitState = PI_DOWN_LIMIT;
		v->IqPid_Out = v->IqPid_OutMin;
	}
	else
	{
		v->IqPid_LimitState = PI_NO_LIMIT;
		v->IqPid_Out = v->IqPid_Out;
	}
	v->Iq_PI_Vq = v->IqPid_Out;
#else
	_iq mTotalMax = 0;
	_iq mTotalMin = 0;

	//Id
	v->IdPid_Ref = v->IdCmd;
	v->IdPid_Fdk = v->IdPid_Fdk;
	v->IdPid_OutMax = v->Vdc;
	v->IdPid_OutMin = 0-v->IdPid_OutMax;
	v->IdPid_Err_old = v->IdPid_Err;
	v->IdPid_Err = v->IdPid_Ref-v->IdPid_Fdk;
	//计算比例作用，并调整积分作用
	v->IdPid_Up = _IQmpy(v->IdPid_Kp,v->IdPid_Err);
	//调整积分的上下限
	if(v->IdPid_Err>0)
	{
		mTotalMax = v->IdPid_OutMax - v->IdPid_Up;
		if(mTotalMax < 0)
		{
			mTotalMax  = 0;
		}
		mTotalMin = v->IdPid_OutMin;
	}
	else
	{
		mTotalMin = v->IdPid_OutMin - v->IdPid_Up;
		if(mTotalMin > 0)
		{
			mTotalMin  = 0;
		}
		mTotalMax = v->IdPid_OutMax;
	}
	//计算积分作用
	v->IdPid_Ui = v->IdPid_Ui+_IQmpy(v->IdPid_Ki,v->IdPid_Err);
	v->IdPid_Ui = _IQsat(v->IdPid_Ui,mTotalMax,mTotalMin);
	//计算PID的输出
	v->IdPid_Out = v->IdPid_Up+v->IdPid_Ui;
	v->IdPid_Out = _IQsat(v->IdPid_Out,v->IdPid_OutMax,v->IdPid_OutMin);
	v->Id_PI_Vd = v->IdPid_Out;

	//Iq
	if(MOTOR_CLOCKWISE == v->Motor_Direction)
	{
		v->IqPid_Ref = 0-v->IqCmd;
	}
	else
	{
		v->IqPid_Ref = v->IqCmd;
	}
	v->IqPid_Fdk = v->IqPid_Fdk;
	v->IqPid_OutMax = v->Vdc;
	v->IqPid_OutMin = -v->Vdc;
	v->IqPid_Err_old = v->IqPid_Err;
	v->IqPid_Err = v->IqPid_Ref-v->IqPid_Fdk;
	//计算比例作用，并调整积分作用
	v->IqPid_Up = _IQmpy(v->IqPid_Kp,v->IqPid_Err);
	//调整积分的上下限
	if(v->IqPid_Err>0)
	{
		mTotalMax = v->IqPid_OutMax - v->IqPid_Up;
		if(mTotalMax < 0)
		{
			mTotalMax  = 0;
		}
		mTotalMin = v->IqPid_OutMin;
	}
	else
	{
		mTotalMin = v->IqPid_OutMin - v->IqPid_Up;
		if(mTotalMin > 0)
		{
			mTotalMin  = 0;
		}
		mTotalMax = v->IqPid_OutMax;
	}
	//计算积分作用
	v->IqPid_Ui = v->IqPid_Ui+_IQmpy(v->IqPid_Ki,v->IqPid_Err);
	v->IqPid_Ui = _IQsat(v->IqPid_Ui,mTotalMax,mTotalMin);
	//计算PID的输出
	v->IqPid_Out = v->IqPid_Up+v->IqPid_Ui;
	v->IqPid_Out = _IQsat(v->IqPid_Out,v->IqPid_OutMax,v->IqPid_OutMin);
	v->Iq_PI_Vq = v->IqPid_Out;
#endif
}

void crtLoopInput()
{
	crtLoopPara.Vdc		              = sysCfgPara.Vdc;  //use for Limit
#if 1
	crtLoopPara.IdCmd                 = weakcrtLoopvars.IdCmd;
	crtLoopPara.IqCmd                 = weakcrtLoopvars.IqCmd;
#else
	if(MTR_CURRENT_LOOP == scsw2.field.runLoop_state)
	{
		crtLoopPara.IdCmd                 = _IQ15toIQ(sysCfgPara.IdCmd_man);
		crtLoopPara.IqCmd                 = _IQ15toIQ(sysCfgPara.IqCmd_man);
	}
	else
	{
		crtLoopPara.IdCmd                 = 0;
		crtLoopPara.IqCmd                 = spdLoopPara.Iqcmd_real;
	}
#endif
	crtLoopPara.IdPid_Fdk		    	= sysFbkPara.Id_fbk;//d axis current
	crtLoopPara.IqPid_Fdk		    	= sysFbkPara.Iq_fbk;//q axis current
}

void crtLoopOutput(void)
{
	crtLoopPara.Id_ref_HMI	= _IQtoIQ15(crtLoopPara.IdPid_Ref);
	crtLoopPara.Iq_ref_HMI  = _IQtoIQ15(crtLoopPara.IqPid_Ref);
}

void crtLoopParaClr(void)
{
	crtLoopPara.IdCmd           = 0;
	crtLoopPara.IqCmd           = 0;

	crtLoopPara.IdPid_Ref=0;
	crtLoopPara.IdPid_Fdk=0;
	crtLoopPara.IdPid_Err=0;
	crtLoopPara.IdPid_LimitState = PI_NO_LIMIT;
	crtLoopPara.IdPid_Up =0;
	crtLoopPara.IdPid_Ui =0;
	crtLoopPara.IdPid_Out=0;
	crtLoopPara.IdPid_Err_old =0;
	crtLoopPara.Id_PI_Vd =0;

	crtLoopPara.IqPid_Ref=0;
	crtLoopPara.IqPid_Fdk=0;
	crtLoopPara.IqPid_Err=0;
	crtLoopPara.IqPid_LimitState = PI_NO_LIMIT;
	crtLoopPara.IqPid_Up =0;
	crtLoopPara.IqPid_Ui =0;
	crtLoopPara.IqPid_Out=0;
	crtLoopPara.IqPid_Err_old =0;
	crtLoopPara.Iq_PI_Vq =0;

	crtLoopPara.IdPid_Kp				= _IQ12toIQ(EV_MCU_Para.field.Id_kp);    //IQ12
	crtLoopPara.IdPid_Ki			    = _IQ12toIQ(EV_MCU_Para.field.Id_ki);
	crtLoopPara.IqPid_Kp				= _IQ12toIQ(EV_MCU_Para.field.Iq_kp);    //IQ12
	crtLoopPara.IqPid_Ki			    = _IQ12toIQ(EV_MCU_Para.field.Iq_ki);

	crtLoopPara.Id_ref_HMI	= _IQtoIQ15(crtLoopPara.IdPid_Ref);
	crtLoopPara.Iq_ref_HMI  = _IQtoIQ15(crtLoopPara.IqPid_Ref);
}

#endif
