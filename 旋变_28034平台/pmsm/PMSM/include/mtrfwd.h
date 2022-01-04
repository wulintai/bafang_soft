/*
 * mtrfwd.h
 *
 *  Created on: 2017-4-19
 *      Author: ZGJ
 */

#ifndef MTRFWD_H_
#define MTRFWD_H_

typedef struct
{
	_iq motor_Np;
	_iq motor_Ld;
	_iq motor_Lq;
	_iq motor_Fm;
	_iq fwd_coeff;

	//_iq speedbase;
	//_iq iphasebase;
	_iq udcbase;
	_iq speedbase_div_udcbase;
	_iq iphasebase_div_udcbase;
	_iq id_cmd;
	_iq iq_cmd;
	//_iq id_fbk;
	//_iq iq_fbk;
	_iq speedfbk;
	_iq Motor_Direction;

	//_iq motor_Lq_Kp;
	_iq motor_wr;


	_iq IdPid_Out_fwd;
	_iq IqPid_Out_fwd;
	//_iq vdc;
}tag_mtrfwdvars;

#ifdef	_MTRFWD_C

tag_mtrfwdvars mtrfwdvars = {0};

void mtrfwdin(void);
void mtrfwdslowin(void);
void mtrfwdctrl(tag_mtrfwdvars *v);
void mtrfwdout(void);
void mtrfwdclc(void);

#else

extern tag_mtrfwdvars mtrfwdvars;

extern void mtrfwdin(void);
extern void mtrfwdslowin(void);
extern void mtrfwdctrl(tag_mtrfwdvars *v);
extern void mtrfwdout(void);
extern void mtrfwdclc(void);

#endif

#endif /* MTRFWD_H_ */

