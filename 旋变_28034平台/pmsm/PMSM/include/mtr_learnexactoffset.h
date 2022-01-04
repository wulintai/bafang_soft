/*
 * mtr_learn.h
 *
 *  Created on: 2017-4-17
 *      Author: ZGJ
 */

#ifndef MTR_LEARNEXACTOFFSET_H_
#define MTR_LEARNEXACTOFFSET_H_

typedef struct
{
	Uint16         finsh_flag;
	Uint16         speed_keep_cnt;
	Uint16         speed_cnt;
	Uint16         write_exactune_flag;

	_iq            positive_offset;
	_iq            negative_offset;
	_iq            deta_offset;
	_iq            speed_theta;
	_iq            Ismax;
    _iq            Isstop;
    _iq            volmax;
    _iq            volstep;

    _iq            Speed_fbk_HMI;
	_iq            IsFbkFilter;
	_iq            Theta_fbk_Ele;
    Uint16         IGBT_run_enable;
    Uint16         DIAG_ENCODER_error;
    Uint16         runLoop_mode;
	Uint16         run_step;
	Uint16         theta_mode;
	Uint16    	   Theta_man;
    Uint16         keepflag;
	Uint16         delay_period;
	Uint16         delay_cnt;
    Uint16         quitcyle_period;
    Uint16         Encoder_direction;
    _iq            VdCmd_man;
    _iq            VqCmd_man;
	_iq            SpeedCmd_man;
	_iq            IsCmd_man;
	_iq            Study_EncoderOffset_is;
	_iq            Study_EncoderOffset_speed;
    _iq            SysBasespeed;
    _iq            Motor_Encoder_offset;
    _iq            Motor_Direction;
    _iq            theta_Ud;
    _iq            theta_Uq;
	_iq            thetatemp;
}tag_exactTune;


#ifdef	_MTR_LEARNEXACTOFFSET_C

tag_exactTune exactTune = {0};

void exactTuneinit(void);
void diagexactEncOffsetTuneAct_input(void);
void diagexactEncOffsetTuneAct_ctrl(tag_exactTune *v);
void diagexactEncOffsetTuneAct_output(void);

#else

extern tag_exactTune exactTune;

extern exactTuneinit(void);
extern void diagexactEncOffsetTuneAct_input(void);
extern void diagexactEncOffsetTuneAct_ctrl(tag_exactTune *v);
extern void diagexactEncOffsetTuneAct_output(void);

#endif

#endif /* MTR_LEARN_H_ */
