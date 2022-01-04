/*
 * mtr_learn.h
 *
 *  Created on: 2017-4-17
 *      Author: ZGJ
 */

#ifndef MTR_LEARNOFFSET_H_
#define MTR_LEARNOFFSET_H_

typedef struct
{
	Uint16 delay_period;
	Uint16 delay_cnt;
    Uint16 quitcyle_period;
    Uint16 TuneFlag;  //角度初学结束标志
    Uint16 encTuneStep;
    Uint16 finshflag;
    Uint16 write_offset_flag;
	Uint16 write_dq_flag;
	Uint16 diag_initangle_finishflag;
    _iq    Ismax;
    _iq    Isstop;
    _iq    volmax;
    _iq    volstep;

    _iq    Speed_fbk_abs;
    Uint16    IGBT_run_enable;
    Uint16    runLoop_mode;
    Uint16    theta_mode;
    _iq       Motor_Encoder_offset;
    _iq       Theta_fbk_Ele;
    Uint16    Encoder_direction;
    Uint16    Theta_man;
    Uint16    keepflag;
    Uint16    DIAG_ENCODER_error;
    _iq       theta_Ud;
    _iq       theta_Uq;
    _iq       thetatemp;
    _iq       VdCmd_man;
    _iq       VqCmd_man;
    _iq       IsFbkFilter;

}tag_diagEncOffset;

#ifdef	_MTR_LEARNOFFSET_C

tag_diagEncOffset diagEncoffsetVar;

void diagEncOffsetTuneAct_input(void);
void diagEncOffsetTuneAct_ctrl(tag_diagEncOffset *v);
void diagEncOffsetTuneAct_output(void);

#else

extern tag_diagEncOffset diagEncoffsetVar;

extern void diagEncOffsetTuneAct_input(void);
extern void diagEncOffsetTuneAct_ctrl(tag_diagEncOffset *v);
extern void diagEncOffsetTuneAct_output(void);

#endif

#endif /* MTR_LEARN_H_ */
