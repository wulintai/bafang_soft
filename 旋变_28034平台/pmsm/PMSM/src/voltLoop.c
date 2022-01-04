#ifndef _VOLTLOOP_C_
#define _VOLTLOOP_C_

#include "include_c.h"

//#pragma	CODE_SECTION(voltLoopInput,		"ramfuncs");
//#pragma	CODE_SECTION(voltLoopOutput,	"ramfuncs");
//#pragma	CODE_SECTION(voltLoopCtrl,		"ramfuncs");

void voltLoopslowInput(void)
{
	//角度补偿时间和频率基值
	invPara.ThetaCmp_timeGap.value = THETACOMP_TIMEGAP;//_IQ24mpy((_iq)PWM_Period_time,THETACOMP_TIMEGAP_PERCENT);
	if(MOTOR_CLOCKWISE == EV_MCU_Para.field.Motor_Direction)
	{
		voltLoopPara.thetaCmp_timeGap	= 0-invPara.ThetaCmp_timeGap.value;//电机反向,运行时间取反
	}
	else
	{
		voltLoopPara.thetaCmp_timeGap	= invPara.ThetaCmp_timeGap.value;
	}
	voltLoopPara.omega              = SysBase.omega;
	voltLoopPara.DeadZoneCom_T = _IQmpy(DEADZONE_3US_T,_IQdiv(EV_MCU_Para.field.Deadtimecomp_percent,100));
}

void voltLoopInput(void)
{
	_iq frequency = 0;
	_iq deta = 0;
	
	if(MTR_VOLT_LOOP == scsw2.field.runLoop_state)
	{
		voltLoopPara.Vd_ref = _IQ15toIQ(sysCfgPara.VdCmd_man);
		voltLoopPara.Vq_ref = _IQ15toIQ(sysCfgPara.VqCmd_man);
	}
	else
	{
		voltLoopPara.Vd_ref = vdqLmtvars.VdqLmt_Vd;
		voltLoopPara.Vq_ref = vdqLmtvars.VdqLmt_Vq;
	}
	voltLoopPara.pwmPeriod			= EPwm1Regs.TBPRD;               //EV_MCU_Para.field.PWM_Period;  //10k   ,10000
	voltLoopPara.pwmNzTime			= EPwm1Regs.TBPRD-150;           // invPara.PWM_NzTime.value;  // 10000-200
	voltLoopPara.thetaEle			= sysCfgPara.Theta_ele;         //_iq24()
	voltLoopPara.omegaEleHz			= sysCfgPara.Omega_ele;          // BASE_Q IQ24
	voltLoopPara.Vdc				= sysCfgPara.Vdc;                //_BASE_Q
	if(2 == EV_MCU_Para.field.Svpwm_way)
	{
		if(sysFbkPara.Speed_fbk_HMI_abs<EV_MCU_Para.field.Svpwm_way_swap_speed)
		{
			voltLoopPara.Svpwm_way = 0;//七段式发波
		}
		else if(sysFbkPara.Speed_fbk_HMI_abs>EV_MCU_Para.field.Svpwm_way_swap_speed+_IQtoIQ15(_IQmpyI32(500,SysBase.invspeed)))
		{
			voltLoopPara.Svpwm_way = 1;//五段式发波
		}
	}
	else
	{
		voltLoopPara.Svpwm_way      = EV_MCU_Para.field.Svpwm_way;
	}
	if((SYS_DIAGNOSIS != scsw1.field.system_state)
	 &&(MTR_VOLT_LOOP != scsw2.field.runLoop_state))
	{
		frequency = _IQmpy(voltLoopPara.omegaEleHz,voltLoopPara.omega);
		deta = frequency*(voltLoopPara.thetaCmp_timeGap/1000);
		voltLoopPara.thetaEle = voltLoopPara.thetaEle+deta;
		while((voltLoopPara.thetaEle>_IQ(1.0))||(voltLoopPara.thetaEle<0))
		{
			if(voltLoopPara.thetaEle>_IQ(1.0))
			{
				voltLoopPara.thetaEle = voltLoopPara.thetaEle-_IQ(1.0);
			}
			if(voltLoopPara.thetaEle<0)
			{
				voltLoopPara.thetaEle = voltLoopPara.thetaEle+_IQ(1.0);
			}
		}
	}

	if((1 == EV_MCU_Para.field.Deadtimecomp_state)
	 &&(SYS_DIAGNOSIS != scsw1.field.system_state)
	 &&(MTR_VOLT_LOOP != scsw2.field.runLoop_state))//Enable,not diagnosis and not voltage loop
	{
		voltLoopPara.DeadZoneComEn_Flag    = 1;//Dead time is effect

		//voltLoopPara.DeadZoneCom_Coeff_Pos = _IQmpy(voltLoopPara.Vdc,SVPWM_DEAD_TIME_3_5US/*_IQ(0.016)*/);
		//voltLoopPara.DeadZoneCom_Coeff_Neg = _IQmpy(voltLoopPara.Vdc,SVPWM_DEAD_TIME_3_5US/*_IQ(0.016)*/);

		//voltLoopPara.DeadZoneCom_T = DEADZONE_3_5US_T;
		voltLoopPara.DeadZoneCom_deta = DEADZONE_DETA;

		voltLoopPara.DeadZoneCom_Threshold = _IQ(0.0055556);//_IQ(0.0022);//2A _IQ(0.0055556);//5A

#if 0
		voltLoopPara.DeadZoneCom_IdIn      = sysFbkPara.Id_fbk_filter;
		voltLoopPara.DeadZoneCom_IqIn      = sysFbkPara.Iq_fbk_filter;
#else
		voltLoopPara.DeadZoneCom_IdIn      = crtLoopPara.IdPid_Ref;
		voltLoopPara.DeadZoneCom_IqIn      = crtLoopPara.IqPid_Ref;
#endif
		voltLoopPara.DeadZoneCom_Is_limit  = _IQsqrt(_IQmpy(voltLoopPara.DeadZoneCom_IdIn,voltLoopPara.DeadZoneCom_IdIn)+_IQmpy(voltLoopPara.DeadZoneCom_IqIn,voltLoopPara.DeadZoneCom_IqIn));
	}
	else
	{
		voltLoopPara.DeadZoneComEn_Flag = 0;//Dead time is not effect
	}
}

void voltLoopCtrl(voltLoopVars *v)
{
	_iq Ud = 0;
	_iq Uq = 0;
	//_iq Ualpha_hfi = 0;
	//_iq Ubeta_hfi = 0;
	//_iq v_pos_com = 0;
	//_iq v_neg_com = 0;
	
	 _iq Va = 0;
	 _iq Vb = 0;
	 _iq Vc = 0;
	 _iq t1 = 0;
	 _iq t2 = 0;
	 _iq tmp = 0;
	 _iq CMPR1 = 0;
	 _iq CMPR2 = 0;
	 _iq CMPR3 = 0;
	 _iq deta_theta = 0;
     
     u32 Sector = 0; 
	
	v->sinTheta = _IQsinPU(v->thetaEle);
	v->cosTheta = _IQcosPU(v->thetaEle);

	if(1 == v->DeadZoneComEn_Flag)
	{
#if 0
		//Get the u,v,w com voltage
		v_pos_com = _IQdiv(v->DeadZoneCom_Coeff_Pos,v->Vdc);
		v_neg_com = _IQdiv(v->DeadZoneCom_Coeff_Neg,v->Vdc);
		
		v->DeadZoneCom_IsTheta = _IQatan2PU(v->DeadZoneCom_IqIn,v->DeadZoneCom_IdIn);
		v->DeadZoneCom_Theta   = v->DeadZoneCom_IsTheta+v->thetaEle;
		while((v->DeadZoneCom_Theta<0)||(v->DeadZoneCom_Theta>=_IQ(1.0)))
		{
			if(v->DeadZoneCom_Theta<0)
			{
				v->DeadZoneCom_Theta = v->DeadZoneCom_Theta+_IQ(1.0);
			}
			else if(v->DeadZoneCom_Theta>=_IQ(1.0))
			{
				v->DeadZoneCom_Theta = v->DeadZoneCom_Theta-_IQ(1.0);
			}
		}
		//补偿A相死区补偿
		if((v->DeadZoneCom_Theta<(DEADZONE_90_DEGREE-v->DeadZoneCom_deta))
		 ||(v->DeadZoneCom_Theta>(DEADZONE_270_DEGREE+v->DeadZoneCom_deta)))
		{
			v->DeadZoneCom_UaOut = v_pos_com;
		}
		else if((v->DeadZoneCom_Theta>(DEADZONE_90_DEGREE+v->DeadZoneCom_deta))
		 &&(v->DeadZoneCom_Theta<(DEADZONE_270_DEGREE-v->DeadZoneCom_deta)))
		{
			v->DeadZoneCom_UaOut = 0-v_neg_com;
		}
		else if((v->DeadZoneCom_Theta>=(DEADZONE_90_DEGREE-v->DeadZoneCom_deta))
		 &&(v->DeadZoneCom_Theta<=(DEADZONE_90_DEGREE+v->DeadZoneCom_deta)))
		{
			deta_theta = DEADZONE_90_DEGREE-v->DeadZoneCom_Theta;
			v->DeadZoneCom_UaOut = _IQmpy(_IQdiv(deta_theta,v->DeadZoneCom_deta),v_pos_com);
		}
		else
		{
			deta_theta = v->DeadZoneCom_Theta-DEADZONE_270_DEGREE;
			v->DeadZoneCom_UaOut = _IQmpy(_IQdiv(deta_theta,v->DeadZoneCom_deta),v_pos_com);
		}
		//补偿B相死区补偿
		if((v->DeadZoneCom_Theta<(DEADZONE_30_DEGREE-v->DeadZoneCom_deta))
		 ||(v->DeadZoneCom_Theta>(DEADZONE_210_DEGREE+v->DeadZoneCom_deta)))
		{
			v->DeadZoneCom_UbOut = 0-v_neg_com;
		}
		else if((v->DeadZoneCom_Theta>(DEADZONE_30_DEGREE+v->DeadZoneCom_deta))
		 &&(v->DeadZoneCom_Theta<(DEADZONE_210_DEGREE-v->DeadZoneCom_deta)))
		{
			v->DeadZoneCom_UbOut = v_pos_com;
		}
		else if((v->DeadZoneCom_Theta>=(DEADZONE_30_DEGREE-v->DeadZoneCom_deta))
		 &&(v->DeadZoneCom_Theta<=(DEADZONE_30_DEGREE+v->DeadZoneCom_deta)))
		{
			deta_theta = v->DeadZoneCom_Theta-DEADZONE_30_DEGREE;
			v->DeadZoneCom_UbOut = _IQmpy(_IQdiv(deta_theta,v->DeadZoneCom_deta),v_pos_com);
		}
		else
		{
			deta_theta = DEADZONE_210_DEGREE-v->DeadZoneCom_Theta;
			v->DeadZoneCom_UbOut = _IQmpy(_IQdiv(deta_theta,v->DeadZoneCom_deta),v_pos_com);
		}
		//补偿C相死区补偿
		if((v->DeadZoneCom_Theta<(DEADZONE_150_DEGREE-v->DeadZoneCom_deta))
		 ||(v->DeadZoneCom_Theta>(DEADZONE_330_DEGREE+v->DeadZoneCom_deta)))
		{
			v->DeadZoneCom_UcOut = 0-v_neg_com;
		}
		else if((v->DeadZoneCom_Theta>(DEADZONE_150_DEGREE+v->DeadZoneCom_deta))
		 &&(v->DeadZoneCom_Theta<(DEADZONE_330_DEGREE-v->DeadZoneCom_deta)))
		{
			v->DeadZoneCom_UcOut = v_pos_com;
		}
		else if((v->DeadZoneCom_Theta>=(DEADZONE_150_DEGREE-v->DeadZoneCom_deta))
		 &&(v->DeadZoneCom_Theta<=(DEADZONE_150_DEGREE+v->DeadZoneCom_deta)))
		{
			deta_theta = v->DeadZoneCom_Theta-DEADZONE_150_DEGREE;
			v->DeadZoneCom_UcOut = _IQmpy(_IQdiv(deta_theta,v->DeadZoneCom_deta),v_pos_com);
		}
		else
		{
			deta_theta = DEADZONE_330_DEGREE-v->DeadZoneCom_Theta;
			v->DeadZoneCom_UcOut = _IQmpy(_IQdiv(deta_theta,v->DeadZoneCom_deta),v_pos_com);
		}
		if(v->DeadZoneCom_Is_limit<v->DeadZoneCom_Threshold)
		{
			v->DeadZoneCom_UaOut = 0;
			v->DeadZoneCom_UbOut = 0;
			v->DeadZoneCom_UcOut = 0;
		}

		/*
		//Get the alpha,beta current
		v->DeadZoneCom_IalphaOut = _IQmpy(v->DeadZoneCom_IdIn,v->cosTheta)-_IQmpy(v->DeadZoneCom_IqIn,v->sinTheta);
		v->DeadZoneCom_IbetaOut  = _IQmpy(v->DeadZoneCom_IqIn,v->cosTheta)+_IQmpy(v->DeadZoneCom_IdIn,v->sinTheta);

		//Get the u,v,w current
		v->DeadZoneCom_IaOut = v->DeadZoneCom_IalphaOut;
		v->DeadZoneCom_IbOut = _IQmpy(IQ_F_1_DIV_2,v->DeadZoneCom_IalphaOut)+_IQmpy(IQ_Z_SQRT3_DIV_2,v->DeadZoneCom_IbetaOut);
		v->DeadZoneCom_IcOut = _IQmpy(IQ_F_1_DIV_2,v->DeadZoneCom_IalphaOut)+_IQmpy(IQ_F_SQRT3_DIV_2,v->DeadZoneCom_IbetaOut);

		//Get the u,v,w com voltage
		v_pos_com = _IQdiv(v->DeadZoneCom_Coeff_Pos,v->Vdc);
		v_neg_com = _IQdiv(v->DeadZoneCom_Coeff_Neg,v->Vdc);
		if(v->DeadZoneCom_IaOut>v->DeadZoneCom_Threshold)
		{
			v->DeadZoneCom_UaOut = v_pos_com;
		}
		else if(v->DeadZoneCom_IaOut<-v->DeadZoneCom_Threshold)
		{
			v->DeadZoneCom_UaOut = 0-v_neg_com;
		}
		else
		{
			if(0 != v->DeadZoneCom_Threshold)
			{
				v->DeadZoneCom_UaOut = _IQmpy(_IQdiv(v->DeadZoneCom_IaOut,v->DeadZoneCom_Threshold),v_pos_com);
			}
			else
			{
				v->DeadZoneCom_UaOut = 0;
			}
		}

		if(v->DeadZoneCom_IbOut>v->DeadZoneCom_Threshold)
		{
			v->DeadZoneCom_UbOut = v_pos_com;
		}
		else if(v->DeadZoneCom_IbOut<-v->DeadZoneCom_Threshold)
		{
			v->DeadZoneCom_UbOut = 0-v_neg_com;
		}
		else
		{
			if(0 != v->DeadZoneCom_Threshold)
			{
				v->DeadZoneCom_UbOut = _IQmpy(_IQdiv(v->DeadZoneCom_IbOut,v->DeadZoneCom_Threshold),v_pos_com);
			}
			else
			{
				v->DeadZoneCom_UbOut = 0;
			}
		}

		if(v->DeadZoneCom_IcOut>v->DeadZoneCom_Threshold)
		{
			v->DeadZoneCom_UcOut = v_pos_com;
		}
		else if(v->DeadZoneCom_IcOut<-v->DeadZoneCom_Threshold)
		{
			v->DeadZoneCom_UcOut = 0-v_neg_com;
		}
		else
		{
			if(0 != v->DeadZoneCom_Threshold)
			{
				v->DeadZoneCom_UcOut = _IQmpy(_IQdiv(v->DeadZoneCom_IcOut,v->DeadZoneCom_Threshold),v_pos_com);
			}
			else
			{
				v->DeadZoneCom_UcOut = 0;
			}
		}*/

		//Get alpha,beta com voltage
		v->DeadZoneCom_Ta = 0;
		v->DeadZoneCom_Tb = 0;
		v->DeadZoneCom_Tc = 0;
		v->DeadZoneCom_UalphaOut1 = v->DeadZoneCom_UaOut;
		v->DeadZoneCom_UbetaOut1  = _IQmpy((v->DeadZoneCom_UaOut + (v->DeadZoneCom_UbOut<<1)),INVSQRT3);
#else
		v->DeadZoneCom_IsTheta = _IQatan2PU(v->DeadZoneCom_IqIn,v->DeadZoneCom_IdIn);
		v->DeadZoneCom_Theta   = v->DeadZoneCom_IsTheta+v->thetaEle;
		while((v->DeadZoneCom_Theta<0)||(v->DeadZoneCom_Theta>=_IQ(1.0)))
		{
			if(v->DeadZoneCom_Theta<0)
			{
				v->DeadZoneCom_Theta = v->DeadZoneCom_Theta+_IQ(1.0);
			}
			else if(v->DeadZoneCom_Theta>=_IQ(1.0))
			{
				v->DeadZoneCom_Theta = v->DeadZoneCom_Theta-_IQ(1.0);
			}
		}
		//补偿A相死区补偿
		if((v->DeadZoneCom_Theta<(DEADZONE_90_DEGREE-v->DeadZoneCom_deta))
		 ||(v->DeadZoneCom_Theta>(DEADZONE_270_DEGREE+v->DeadZoneCom_deta)))
		{
			v->DeadZoneCom_Ta = v->DeadZoneCom_T;
		}
		else if((v->DeadZoneCom_Theta>(DEADZONE_90_DEGREE+v->DeadZoneCom_deta))
		 &&(v->DeadZoneCom_Theta<(DEADZONE_270_DEGREE-v->DeadZoneCom_deta)))
		{
			v->DeadZoneCom_Ta = -v->DeadZoneCom_T;
		}
		else if((v->DeadZoneCom_Theta>=(DEADZONE_90_DEGREE-v->DeadZoneCom_deta))
		 &&(v->DeadZoneCom_Theta<=(DEADZONE_90_DEGREE+v->DeadZoneCom_deta)))
		{
			deta_theta = DEADZONE_90_DEGREE-v->DeadZoneCom_Theta;
			v->DeadZoneCom_Ta = _IQmpy(_IQdiv(deta_theta,v->DeadZoneCom_deta),v->DeadZoneCom_T);
		}
		else
		{
			deta_theta = v->DeadZoneCom_Theta-DEADZONE_270_DEGREE;
			v->DeadZoneCom_Ta = _IQmpy(_IQdiv(deta_theta,v->DeadZoneCom_deta),v->DeadZoneCom_T);
		}
		//补偿B相死区补偿
		if((v->DeadZoneCom_Theta<(DEADZONE_30_DEGREE-v->DeadZoneCom_deta))
		 ||(v->DeadZoneCom_Theta>(DEADZONE_210_DEGREE+v->DeadZoneCom_deta)))
		{
			v->DeadZoneCom_Tb = -v->DeadZoneCom_T;
		}
		else if((v->DeadZoneCom_Theta>(DEADZONE_30_DEGREE+v->DeadZoneCom_deta))
		 &&(v->DeadZoneCom_Theta<(DEADZONE_210_DEGREE-v->DeadZoneCom_deta)))
		{
			v->DeadZoneCom_Tb = v->DeadZoneCom_T;
		}
		else if((v->DeadZoneCom_Theta>=(DEADZONE_30_DEGREE-v->DeadZoneCom_deta))
		 &&(v->DeadZoneCom_Theta<=(DEADZONE_30_DEGREE+v->DeadZoneCom_deta)))
		{
			deta_theta = v->DeadZoneCom_Theta-DEADZONE_30_DEGREE;
			v->DeadZoneCom_Tb = _IQmpy(_IQdiv(deta_theta,v->DeadZoneCom_deta),v->DeadZoneCom_T);
		}
		else
		{
			deta_theta = DEADZONE_210_DEGREE-v->DeadZoneCom_Theta;
			v->DeadZoneCom_Tb = _IQmpy(_IQdiv(deta_theta,v->DeadZoneCom_deta),v->DeadZoneCom_T);
		}
		//补偿C相死区补偿
		if((v->DeadZoneCom_Theta<(DEADZONE_150_DEGREE-v->DeadZoneCom_deta))
		 ||(v->DeadZoneCom_Theta>(DEADZONE_330_DEGREE+v->DeadZoneCom_deta)))
		{
			v->DeadZoneCom_Tc = -v->DeadZoneCom_T;
		}
		else if((v->DeadZoneCom_Theta>(DEADZONE_150_DEGREE+v->DeadZoneCom_deta))
		 &&(v->DeadZoneCom_Theta<(DEADZONE_330_DEGREE-v->DeadZoneCom_deta)))
		{
			v->DeadZoneCom_Tc = v->DeadZoneCom_T;
		}
		else if((v->DeadZoneCom_Theta>=(DEADZONE_150_DEGREE-v->DeadZoneCom_deta))
		 &&(v->DeadZoneCom_Theta<=(DEADZONE_150_DEGREE+v->DeadZoneCom_deta)))
		{
			deta_theta = v->DeadZoneCom_Theta-DEADZONE_150_DEGREE;
			v->DeadZoneCom_Tc = _IQmpy(_IQdiv(deta_theta,v->DeadZoneCom_deta),v->DeadZoneCom_T);
		}
		else
		{
			deta_theta = DEADZONE_330_DEGREE-v->DeadZoneCom_Theta;
			v->DeadZoneCom_Tc = _IQmpy(_IQdiv(deta_theta,v->DeadZoneCom_deta),v->DeadZoneCom_T);
		}
		if(v->DeadZoneCom_Is_limit<v->DeadZoneCom_Threshold)
		{
			v->DeadZoneCom_Ta = 0;
			v->DeadZoneCom_Tb = 0;
			v->DeadZoneCom_Tc = 0;
		}
		//v->DeadZoneCom_UalphaOut1 = 0;
		//v->DeadZoneCom_UbetaOut1  = 0;
#endif
	}
	else
	{
		v->DeadZoneCom_Ta = 0;
		v->DeadZoneCom_Tb = 0;
		v->DeadZoneCom_Tc = 0;
		//v->DeadZoneCom_UalphaOut1 = 0;
		//v->DeadZoneCom_UbetaOut1  = 0;
	}
	
	Ud = _IQdiv(v->Vd_ref,v->Vdc);
	Uq = _IQdiv(v->Vq_ref,v->Vdc);
	//Ualpha_hfi = _IQdiv(v->Valpha_hfi,v->Vdc);
	//Ubeta_hfi  = _IQdiv(v->Vbeta_hfi ,v->Vdc);
	
	v->VsAlpha = _IQmpy(Ud,v->cosTheta)-_IQmpy(Uq,v->sinTheta);//+v->DeadZoneCom_UalphaOut1+Ualpha_hfi;
	v->VsBeta  = _IQmpy(Uq,v->cosTheta)+_IQmpy(Ud,v->sinTheta);//+v->DeadZoneCom_UbetaOut1 +Ubeta_hfi;
	
	Va = v->VsBeta;
	Vb = _IQmpy(IQ_F_1_DIV_2,v->VsBeta) + _IQmpy(IQ_Z_SQRT3_DIV_2,v->VsAlpha);
	Vc = _IQmpy(IQ_F_1_DIV_2,v->VsBeta) + _IQmpy(IQ_F_SQRT3_DIV_2,v->VsAlpha);
     
     
     if (Va>_IQ(0))
     {
          Sector = 1;
     }
     
     if (Vb>_IQ(0))
     {
          Sector = Sector + 2;
     }
     
     if (Vc>_IQ(0))
     {
          Sector = Sector + 4;
     }
     
     
     Va = v->VsBeta;                                                            
     Vb = _IQmpy(IQ_Z_1_DIV_2,v->VsBeta) + _IQmpy(IQ_Z_SQRT3_DIV_2,v->VsAlpha);   
     Vc = _IQmpy(IQ_Z_1_DIV_2,v->VsBeta) - _IQmpy(IQ_Z_SQRT3_DIV_2,v->VsAlpha);  
     
     if (Sector==0)  
     {
          
          v->Ta = IQ_Z_1_DIV_2;
          v->Tb = IQ_Z_1_DIV_2;
          v->Tc = IQ_Z_1_DIV_2;
     }
     if (Sector==1)  
     {
          
          t1 = Vc;
          t2 = Vb;
          tmp = t1 + t2;
          if( tmp > IQ_Z_1 )
          {
               t1 = _IQdiv( t1, tmp );
               t2 = _IQdiv( t2, tmp );
          }
		  if(1 == v->Svpwm_way)
		  {
		  	  v->Tb = IQ_Z_1-t1-t2;      
	          v->Ta = v->Tb+t1;                             
	          v->Tc = v->Ta+t2;//W相恒低 
		  }
		  else
		  {
	          v->Tb = _IQmpy(IQ_Z_1_DIV_2,(IQ_Z_1-t1-t2));      
	          v->Ta = v->Tb+t1;                             
	          v->Tc = v->Ta+t2;       
		  }
     }
     else if (Sector==2)  
     {
          
          t1 = Vb;
          t2 = -Va;
          tmp = t1 + t2;
          if( tmp > IQ_Z_1 )
          {
               t1 = _IQdiv( t1, tmp );
               t2 = _IQdiv( t2, tmp );
          }
		  if(1 == v->Svpwm_way)
		  {
		  	  v->Ta = IQ_Z_1-t1-t2;      
	          v->Tc = v->Ta+t1;                             
	          v->Tb = v->Tc+t2;//V相恒低 
		  }
		  else
		  {
	          v->Ta = _IQmpy(IQ_Z_1_DIV_2,(IQ_Z_1-t1-t2));      
	          v->Tc = v->Ta+t1;                             
	          v->Tb = v->Tc+t2; 
		  }
     }  
     else if (Sector==3)  
     {
          
          
          
          t1 = -Vc;
          t2 = Va;
          tmp = t1 + t2;
          if( tmp > IQ_Z_1 )
          {
               t1 = _IQdiv( t1, tmp );
               t2 = _IQdiv( t2, tmp );
          }
		  if(1 == v->Svpwm_way)
		  {
		  	v->Ta = 0;//U相恒高      
          	v->Tb = v->Ta+t1;                             
          	v->Tc = v->Tb+t2;  
		  }
		  else
		  {
          	v->Ta = _IQmpy(IQ_Z_1_DIV_2,(IQ_Z_1-t1-t2));      
          	v->Tb = v->Ta+t1;                             
          	v->Tc = v->Tb+t2;  
		  }
     }  
     else if (Sector==4)  
     {
          
          t1 = -Va;
          t2 = Vc;
          tmp = t1 + t2;
          if( tmp > IQ_Z_1 )
          {
               t1 = _IQdiv( t1, tmp );
               t2 = _IQdiv( t2, tmp );
          }
		   if(1 == v->Svpwm_way)
		   {
		   	  v->Tc = IQ_Z_1-t1-t2;      
	          v->Tb = v->Tc+t1;                             
	          v->Ta = v->Tb+t2;//U相恒低 
		   }
		   else
		   {
	          v->Tc = _IQmpy(IQ_Z_1_DIV_2,(IQ_Z_1-t1-t2));      
	          v->Tb = v->Tc+t1;                             
	          v->Ta = v->Tb+t2; 
		   }
     }
     else if (Sector==5)  
     {
          
          t1 = Va;
          t2 = -Vb;
          tmp = t1 + t2;
          if( tmp > IQ_Z_1 )
          {
               t1 = _IQdiv( t1, tmp );
               t2 = _IQdiv( t2, tmp );
          }
		  if(1 == v->Svpwm_way)
		  {
		  	  v->Tb = 0;//V相恒高      
	          v->Tc = v->Tb+t1;                             
	          v->Ta = v->Tc+t2;  
		  }
		  else
		  {
	          v->Tb = _IQmpy(IQ_Z_1_DIV_2,(IQ_Z_1-t1-t2));      
	          v->Tc = v->Tb+t1;                             
	          v->Ta = v->Tc+t2;  
		  }
     }   
     else if (Sector==6)  
     {
          
          t1 = -Vb;
          t2 = -Vc;
          tmp = t1 + t2;
          if( tmp > IQ_Z_1 )
          {
               t1 = _IQdiv( t1, tmp );
               t2 = _IQdiv( t2, tmp );
          }
		  if(1 == v->Svpwm_way)
		  {
		  	  v->Tc = 0;//W相恒高    
	          v->Ta = v->Tc+t1;                             
	          v->Tb = v->Ta+t2;  
		  }
		  else
		  {
	          v->Tc = _IQmpy(IQ_Z_1_DIV_2,(IQ_Z_1-t1-t2));     
	          v->Ta = v->Tc+t1;                             
	          v->Tb = v->Ta+t2;     
		  }
     }
     
     if(v->Ta<0)
     {
          v->Ta = 0;
     }
     else if(v->Ta>IQ_Z_1)
     {
          v->Ta = IQ_Z_1;
     }
     
     if(v->Tb<0)
     {
          v->Tb = 0;
     }
     else if(v->Tb>IQ_Z_1)
     {
          v->Tb = IQ_Z_1;
     }
     
     if(v->Tc<0)
     {
          v->Tc = 0;
     }
     else if(v->Tc>IQ_Z_1)
     {
          v->Tc = IQ_Z_1;
     }     
	 
	CMPR1 = _IQmpy(v->Ta,v->pwmPeriod)-v->DeadZoneCom_Ta;
	v->CMPR1 = _IQsat(CMPR1,v->pwmPeriod,0);
	CMPR2 = _IQmpy(v->Tb,v->pwmPeriod)-v->DeadZoneCom_Tb;
	v->CMPR2 = _IQsat(CMPR2,v->pwmPeriod,0);
	CMPR3 = _IQmpy(v->Tc,v->pwmPeriod)-v->DeadZoneCom_Tc;
	v->CMPR3 = _IQsat(CMPR3,v->pwmPeriod,0);
}

void voltLoopOutput(void)
{
	_iq tmp = 0;

#if 1
	if(voltLoopPara.CMPR1<SVPWM_SMALL_PULSE)
	{
		voltLoopPara.CMPR1 = 0;
	}
	else if((voltLoopPara.pwmPeriod-voltLoopPara.CMPR1)<SVPWM_SMALL_PULSE)
	{
		voltLoopPara.CMPR1 = voltLoopPara.pwmPeriod;
	}

	if(voltLoopPara.CMPR2<SVPWM_SMALL_PULSE)
	{
		voltLoopPara.CMPR2 = 0;
	}
	else if((voltLoopPara.pwmPeriod-voltLoopPara.CMPR2)<SVPWM_SMALL_PULSE)
	{
		voltLoopPara.CMPR2 = voltLoopPara.pwmPeriod;
	}

	if(voltLoopPara.CMPR3<SVPWM_SMALL_PULSE)
	{
		voltLoopPara.CMPR3 = 0;
	}
	else if((voltLoopPara.pwmPeriod-voltLoopPara.CMPR3)<SVPWM_SMALL_PULSE)
	{
		voltLoopPara.CMPR3 = voltLoopPara.pwmPeriod;
	}
#endif
	EPwm1Regs.CMPA.half.CMPA	= voltLoopPara.CMPR1;
	EPwm2Regs.CMPA.half.CMPA	= voltLoopPara.CMPR2;
	EPwm3Regs.CMPA.half.CMPA	= voltLoopPara.CMPR3;

	LPFilter(voltLoopPara.Vd_ref,&voltLoopPara.Vd_ref_Filter,_IQ(0.0025),_IQ(0.9975));
	LPFilter(voltLoopPara.Vq_ref,&voltLoopPara.Vq_ref_Filter,_IQ(0.0025),_IQ(0.9975));

	LPFilter(voltLoopPara.Vd_ref,&voltLoopPara.Vd_ref_Filter_025,_IQ(0.0025),_IQ(0.9975));
	LPFilter(voltLoopPara.Vq_ref,&voltLoopPara.Vq_ref_Filter_025,_IQ(0.0025),_IQ(0.9975));

	voltLoopPara.Vd_ref_HMI		= _IQtoIQ15(voltLoopPara.Vd_ref_Filter);
	voltLoopPara.Vq_ref_HMI		= _IQtoIQ15(voltLoopPara.Vq_ref_Filter);

	tmp = _IQmpy(voltLoopPara.Vd_ref,voltLoopPara.Vd_ref)+_IQmpy(voltLoopPara.Vq_ref,voltLoopPara.Vq_ref);
	voltLoopPara.Vdq = _IQsqrt(tmp);
	LPFilter(voltLoopPara.Vdq,&voltLoopPara.Vdq_Filter,_IQ(0.0025),_IQ(0.9975));
	voltLoopPara.Vdq_HMI = _IQtoIQ15(voltLoopPara.Vdq_Filter);
}

void voltLoopParaClr()
{
	// 电压环输入参数
	voltLoopPara.Vd_ref				= 0;
	voltLoopPara.Vq_ref				= 0;
	voltLoopPara.Vd_ref_HMI         = 0;
	voltLoopPara.Vq_ref_HMI         = 0;
	voltLoopPara.Vd_ref_Filter      = 0;
	voltLoopPara.Vq_ref_Filter      = 0;
	voltLoopPara.Vd_ref_Filter_025  = 0;
	voltLoopPara.Vq_ref_Filter_025  = 0;
	voltLoopPara.Vdq_Filter         = 0;
	//voltLoopPara.VsAlpha_Coeff 		= 0;
	//voltLoopPara.VsBeta_Coeff		= 0;

	// 电压环输出参数
	voltLoopPara.Vdq				= 0;
	voltLoopPara.CMPR1				= 0;
	voltLoopPara.CMPR2				= 0;
	voltLoopPara.CMPR3				= 0;

	voltLoopOutput();
}

#endif

