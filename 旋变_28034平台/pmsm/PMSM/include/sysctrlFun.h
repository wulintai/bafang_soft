/*
 * sysctrlFun.h
 *
 *  Created on: 2014-11-14
 *      Author: ThinkPad
 */

#ifndef _SYSCTRLFUN_H_
#define _SYSCTRLFUN_H_

#define ALARM_BUZZER_INIT_STATE				0
#define ALARM_BUZZER_SHORT_ALARM_STATE		1
#define ALARM_BUZZER_LONG_ALARM_STATE		2
#define ALARM_BUZZER_DELAY_STATE			3

#define SUB_IO_BUZZER_OFF	0
#define SUB_IO_BUZZER_ON	1

#define SUB_IO_LOW			0
#define SUB_IO_HIGH			1

#define SUB_IO_EN_PT1000	0
#define SUB_IO_EN_PT100		1

#define DIFF_SPD_NUM		16

#ifdef _SYSCTRLFUN_C_

_iq      diff_trq_filter = 0;

_iq      diff_stop_trq = 0;
_iq      diff_stop_trq_filter = 0;
_iq      diff_stop_spd_err_old = 0;
_iq      diff_stop_spd_err = 0;
_iq      diff_stop_trq_ui = 0;

_iq      Speed_fbk_Filter_Stop_Lowspeed = 0;

_iq      spdlmt_cmd = 0;
_iq 	 spdlmt_trq = 0;
_iq      spdlmt_trq_filter = 0;
_iq 	 spdlmt_speed = 0;

_iq      zero_station_SpeedCal_spdFbk = 0;
_iq      zero_station_deltaTheta = 0;
_iq      zero_station_deltaThetasum = 0;
_iq      zero_station_trqCmd = 0;
_iq      zero_station_trqLmt = 0;
_iq      zero_station_trqCmd_store = 0;
Uint16   zero_station_trqCmd_flag = 0;
Uint16   motor_sys_init_cnt = 0;
Uint16   motor_main_relay_cnt = 0;

_iq     Motor_Encoder_offset = 0;
Uint16  Motor_Encoder_cnt = 0;
_iq     Motor_Encoder_offset_current = 0;
_iq     Motor_Encoder_offset_coeff= 0;

/*_iq zero_station_deltaThetasumbak = 0;
_iq zero_station_err = 0;
_iq zero_station_errbak = 0;
_iq deta_zero_station_speed = 0;*/

_iq zero_speed_step = 0;
_iq zero_speed_throad = 0;
_iq speed_start_up = 0;

void zero_stationcond(void);
void udcloop_cond(void);
void speedlooplmtcond(void);
void sub_io_yb(Uint16 buzzer_io_state);
void sub_io_buzzer(Uint16 buzzer_io_state);
void sub_io_relay(Uint16 fan_io_state);
void sub_io_ad2s1205_rst(Uint16 fan_io_state);
void sub_io_ad2s1205_fs2(Uint16 fan_io_state);
void sub_io_ad2s1205_fs1(Uint16 fan_io_state);
void errordisplay(void);
void diff_deal(void);
void diff_deal_clear(void);

#else

extern _iq   	zero_station_SpeedCal_spdFbk;
extern _iq   	zero_station_deltaTheta;
extern _iq   	zero_station_deltaThetasum;
extern _iq      zero_station_trqCmd;
extern Uint16 	zero_station_trqCmd_flag;
extern _iq      spdlmt_cmd;
extern _iq      diff_trq_filter;
extern _iq      diff_stop_trq_filter;
extern _iq      diff_stop_spd_ref;

extern _iq 		spdlmt_trq;
extern _iq      spdlmt_trq_filter;
extern _iq      Speed_fbk_Filter_Stop_Lowspeed;

extern Uint16   motor_sys_init_cnt;
extern _iq      zero_station_trqLmt;
extern _iq     Motor_Encoder_offset;
extern _iq     Motor_Encoder_offset_current;
extern _iq     Motor_Encoder_offset_coeff;
extern _iq zero_speed_step;
extern _iq zero_speed_throad;
extern Uint16  Motor_Encoder_cnt;
extern _iq      zero_station_trqCmd_store;
extern _iq speed_start_up;
extern Uint16   motor_main_relay_cnt;

extern void zero_stationcond(void);
extern void zero_stationinit(void);
extern void udcloop_cond(void);
extern void speedlooplmtcond(void);
extern void sub_io_yb(Uint16 buzzer_io_state);
extern void sub_io_buzzer(Uint16 buzzer_io_state);
extern void sub_io_relay(Uint16 fan_io_state);
extern void sub_io_ad2s1205_rst(Uint16 fan_io_state);
extern void sub_io_ad2s1205_fs2(Uint16 fan_io_state);
extern void sub_io_ad2s1205_fs1(Uint16 fan_io_state);
extern void errordisplay(void);
extern void diff_deal(void);
extern void diff_deal_clear(void);

#endif

#endif /* SYSCTRLFUN_H_ */

