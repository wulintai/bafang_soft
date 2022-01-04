#ifndef _GPMODULE_C_
#define _GPMODULE_C_

#include "include_c.h"

void LinearRamp(_iq cmd, _iq* ref, _iq upStep, _iq downStep, _iq upLimit, _iq downLimit)
{
//========================================================================================
// Temporary variable definition (execute time: ?? CPUCLK)
//========================================================================================
	_iq		step	= 0;
//========================================================================================
// 命令输入限幅 (execute time: ?? CPUCLK)
//========================================================================================
	cmd	= min(cmd, upLimit);
	cmd	= max(cmd, downLimit);
//========================================================================================
// Ramp方向及步长确定 (execute time: ?? CPUCLK)
//========================================================================================
	if (_IQmpy((*ref) , cmd) < 0)
	{
		step = downStep;
	}
	else
	{
		step = (_IQabs(*ref) < _IQabs(cmd)) ? upStep : downStep;
	}
//========================================================================================
// Ramp (execute time: ?? CPUCLK)
//========================================================================================
	if ((*ref) + step < cmd)
	{
		(*ref) += step;
	}
	else if ((*ref) - step > cmd)
	{
		(*ref) -= step;
	}
	else
	{
		(*ref)	= cmd;
	}
	//=====================
	(*ref)	= min((*ref), upLimit);
	(*ref)  = max((*ref), downLimit);
}

void LPFilter(_iq input,_iq* output, _iq filter_k0, _iq filter_k1)
{
		*output			= _IQmpy(filter_k0 ,input)+ _IQmpy(filter_k1 ,(*output));
}

/*void HPFilter(_iq input_n,_iq input_n_1,_iq* output, _iq filter_k)
{
	_iq input = 0;
	
	input = input_n-input_n_1+(*output);
	*output = _IQmpy(input,filter_k);
}*/

#endif

