#ifndef __PIDREG3_C__
#define __PIDREG3_C__

#include "include_c.h"

void pidreg3_init(PIDREG3 *v)
{
	v->Ref=0;
	v->Fdb=0;
	v->Err=0;
	v->LimitState = PI_NO_LIMIT;
	v->Up =0;
	v->Ui =0;
	v->Ud =0;
	v->Out=0;
	v->Err_old =0;
	v->Out_fwd =0;
}

void pidreg3_calc(PIDREG3 *v)
{
	v->Err=v->Ref-v->Fdb;

	v->Up =_IQmpy(v->Kp,v->Err);
	if  ((v->LimitState == PI_NO_LIMIT)		||
		((v->LimitState == PI_UP_LIMIT)		&& (v->Err < 0)) ||
		((v->LimitState == PI_DOWN_LIMIT)	&& (v->Err > 0)))
	{
		v->Ui   +=_IQmpy(v->Ki,v->Err);
		v->Ui	= min(v->Ui,  v->OutMax);
		v->Ui	= max(v->Ui, v->OutMin);
	}
	v->Ud =_IQmpy((v->Err-v->Err_old),v->Kd);
	v->Err_old = v->Err;
	v->Out=v->Up+v->Ui+v->Ud+v->Out_fwd;
	if(v->Out > v->OutMax)
	{
		v->Out=v->OutMax;
		v->LimitState=PI_UP_LIMIT;
	}
	else if(v->Out<v->OutMin)
	{
		v->LimitState=PI_DOWN_LIMIT;
		v->Out=v->OutMin;
	}
	else
	{
		v->LimitState=PI_NO_LIMIT;
	}
}

#if 0
void pidreg2_calc(PIDREG3 *v)
{
	v->Err=v->Ref-v->Fdb;
	//u=sum+Kp*err
	v->Temp = v->Ui + _IQmpy(v->Kp,v->Err);

	//Êä³öÏÞ·ù
	if(v->Temp > v->OutMax){
		v->Out = v->OutMax;
	}else if(v->Temp < v->OutMin){
		v->Out = v->OutMin;
	}else{
		v->Out = v->Temp;
	}
	//
	v->Excess = v->Temp - v->Out;
	v->Ui = v->Ui + _IQmpy(v->Ki,v->Err) - _IQmpy(v->Kc,v->Excess);
}
#endif

#endif

