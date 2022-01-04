#ifndef _GPMODULE_H_
#define _GPMODULE_H_

#define LOBYTE(w)				((Uint16)(w)			& 0x0ff)
#define HIBYTE(w)				(((Uint16)(w) >> 8)	& 0x0ff)
#define max(a,b)				(((_iq)(a) > (_iq)(b)) ? (_iq)(a) : (_iq)(b))
#define min(a,b)				(((_iq)(a) < (_iq)(b)) ? (_iq)(a) : (_iq)(b))
#define MAKEWORD(low,high)		((Uint16)(low) | (((Uint16)(high)<<8)))

#ifdef _GPMODULE_C_
	void LPFilter(_iq input,_iq* output, _iq filter_k0, _iq filter_k1);
	void HPFilter(_iq input_n,_iq input_n_1,_iq* output, _iq filter_k);
	void LinearRamp(_iq cmd, _iq* ref, _iq upStep, _iq downStep, _iq upLimit, _iq downLimit);
#else
	extern void LPFilter(_iq input,_iq* output, _iq filter_k0, _iq filter_k1);
	extern void HPFilter(_iq input_n,_iq input_n_1,_iq* output, _iq filter_k);
	extern void LinearRamp(_iq cmd, _iq* ref, _iq upStep, _iq downStep, _iq upLimit, _iq downLimit);
#endif

#endif

