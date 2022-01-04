/*
 * eeprom.h
 *
 *  Created on: 2014-4-15
 *      Author: ThinkPad
 */

#ifndef _EEPROM_H_
#define _EEPROM_H_

#define ControlCode                   0x0A

#define Motor_Basic_Para_ADDR_START        0
#define Motor_Basic_Para_ADDR_END          7

#define Motor_Charact_Para_ADDR_START      10
#define Motor_Charact_Para_ADDR_END        18

#define Motor_Calibrat_Para_ADDR_START     20
#define Motor_Calibrat_Para_ADDR_END       26

#define VoltFeedfwd_Para_ADDR_START        30
#define VoltFeedfwd_Para_ADDR_END          37

#define AD_Para_ADDR_START                 40
#define AD_Para_ADDR_END                   59

#define PI_Para_ADDR_START                 60
#define PI_Para_ADDR_END                   65

#define UDCLOOP_Para_ADDR_START             70
#define UDCLOOP_Para_ADDR_END               78

#define CMD_Para_ADDR_START                80
#define CMD_Para_ADDR_END                  91

#define Weak_Para_ADDR_START               100
#define Weak_Para_ADDR_END                 119

#define EliJitter_Para_ADDR_START          120
#define EliJitter_Para_ADDR_END		       126

#define TBDVALUE_Para_ADDR_START           130
#define TBDVALUE_Para_ADDR_END		       135

#define Zero_Sation_Para_ADDR_START        140
#define Zero_Sation_Para_ADDR_END		   146

#define Spd_Over_Limt_Para_ADDR_START      150
#define Spd_Over_Limt_Para_ADDR_END		   154

#define Motor_study_Para_ADDR_START        160
#define Motor_study_Para_ADDR_END		   169

#define Motor_Protect_Alarm_Para_ADDR_START  170
#define Motor_Protect_Alarm_Para_ADDR_END    187

#define Motor_Protect_Para_ADDR_START      190
#define Motor_Protect_Para_ADDR_END		   200

#define Ctr_Para_ADDR_START                210
#define Ctr_Para_ADDR_END		           218

#define IGBT_Short_Para_ADDR_START         220
#define IGBT_Short_Para_ADDR_END		   229

#define Tooth_TRQ_Para_ADDR_START         230
#define Tooth_TRQ_Para_ADDR_END		      239

#define EEPROM_PASSWORD_ADDR               216
#define EEPROM_PASSWORD_ADDR1              217
#define EEPROM_PASSWORD_ADDR2              218

#define	SOFTWARE_VERSION			       0x0201         //软件版本
#define SOFTWARE_VERSION_DATE			   0xA91B         //软件版本最后修改日期
#define EEPROM_PASSWORD                    0x0737         //客户代码:4D120G 大运 4.5T
#define EEPROM_PASSWORD1                   0x0201         //EEPROM最后固化程序值
#define EEPROM_PASSWORD2                   0xA91B         //EEPROM最后修改日期      //EEPROM最后修改日期

#define I2C_FaultNum                       16
#define I2C_FaultStart                     300
#define I2C_FaultDataEnd                   380

#define I2C_ValueLimitStart                390
#define I2C_ValueLimitEnd                  396
//========================================================================================
// ~{J}>]=a996(Re~}
//========================================================================================

#define EEPROM_CRC16_ADDR					400
#define EEPROM_CRC16_ADDR1                  401
#define EEPROM_CRC16_ADDR2                  402

#define EEPROM_READ_DELAY_CNT				1000
#define EEPROM_WRITE_DELAY_CNT				10000

typedef union
{
	struct
	{
	//=====================~{5g;z;y1>2NJ}#(~}0-9~{#)~}=====================
		unsigned int Motor_Np;                                //0
		unsigned int Encoder_Np;                              //1
		unsigned int Motor_Direction;                         //2
		unsigned int Motor_Encoder_Direction;                 //3
		unsigned int Motor_Encoder_offset;                    //4
		unsigned int Motor_Encoder_offset_elecoeff;           //5
		unsigned int Motor_Encoder_offset_gencoeff;           //6
		unsigned int Motor_TempSensor_type;                   //7
		unsigned int Motor_McuTempSensor_type;                //8
		unsigned int Motor_Encoder_rev_offset;                //9
	//=====================~{5g;zLXPT2NJ}#(~}10-19~{#)~}====================
		unsigned int Motor_RatePower;                         //10
		unsigned int Motor_PeakPowerEle;                      //11
		unsigned int Motor_PeakPowerGen;                      //12
		unsigned int Motor_RateTorque;                        //13
		unsigned int Motor_PeakTorqueEle;                     //14
		unsigned int Motor_PeakTorqueGen;                     //15
		unsigned int Motor_RateSpeed;                         //16
		unsigned int Motor_PeakSpeedFwd;                      //17
		unsigned int Motor_PeakSpeedRev;                      //18
		unsigned int Motor_Charact_reserved;                  //19
	//=====================~{5g;z1j6(2NJ}#(~}20-29~{#)~}====================
		unsigned int Motor_UDCMAP;                            //20
        unsigned int Motor_Trqidqmap_IdcoeffEle;              //21
        unsigned int Motor_Trqidqmap_IqcoeffEle;              //22
        unsigned int Motor_Trqidqmap_IdcoeffGen;              //23
        unsigned int Motor_Trqidqmap_IqcoeffGen;              //24
        unsigned int Motor_IdLimt_enable;                     //25
        unsigned int Motor_Trqcoeff_enable;                   //26
        unsigned int Motor_Idc_Select;                        //27
        unsigned int Motor_Calibration_reserved[2];           //28-29
    //=====================~{5gQ9G0@!#(~}30-39~{#)~}=======================
        unsigned int VoltFeedfwd_Coeff;                       //30
        unsigned int Rs_Stator;                               //31
        unsigned int Ld_Stator;                               //32
        unsigned int Lq_Stator;                               //33
        unsigned int Lq_SatStator;                         //34
        unsigned int Flux_Rotor;                              //35
        unsigned int iq_Lq_Stator;                          //36
        unsigned int iq_Lq_SatStator;                    //37
        unsigned int VoltFeedfwd_reserved[2];                 //38-39
    //=====================AD~{2NJ}EdVC#(~}40-59~{#)~}=====================
		unsigned int Iu_offset;                               //40
		unsigned int Iv_offset;                               //41
		unsigned int Iw_offset;                               //42
		unsigned int Iac_adjustCoeff;                         //43
		unsigned int Idc_offset;                              //44
		unsigned int Idc_adjustCoeff;                         //45
		unsigned int Vdc_offset;                              //46
		unsigned int Vdc_adjustCoeff;                         //47
		unsigned int Mtrtemp_offset;                          //48
		unsigned int Mdltemp_offset;                          //49
	//-----------------------------------------------------------
		unsigned int AI1_offset_on;                           //50
		unsigned int AI1_offset_off;                          //51
		unsigned int AI1_adjustCoeff;                         //52
		unsigned int AI2_offset_on;                           //53
		unsigned int AI2_offset_off;                          //54
		unsigned int AI2_adjustCoeff;                         //55
		unsigned int Vdc_sample_time;                         //56
		unsigned int Mtrtemp_adjustCoeff;					  //57
		unsigned int V12_adjustCoeff;                         //58
		unsigned int V5_adjustCoeff; 		   				  //59
    //=====================PI~{2NJ}~}(60-79)==========================
		unsigned int Id_kp;                                   //60
		unsigned int Id_ki;                                   //61
		unsigned int Iq_kp;                                   //62
		unsigned int Iq_ki;	                                  //63
		unsigned int Speed_kp;                                //64
		unsigned int Speed_ki;                                //65
		unsigned int Speed_kd;                                //66
		unsigned int Encoder_kp;                              //67
		unsigned int Encoder_ki;                              //68
		unsigned int V12_offset;                              //69
    //=====================TrqGen~{2NJ}~}(70-79)==========================
		unsigned int  Udcloop_Enable;                           //70
		unsigned int  Udcloop_kp;                               //71
		unsigned int  Udcloop_ki;                               //72
		unsigned int  Udcloop_UdcUplmt;                        //73
		unsigned int  Udcloop_UdcDownlmt;                      //74
		unsigned int  Udcloop_Speedmin;                         //75
		unsigned int  Udcloop_Speedmax;                         //76
		unsigned int  Udcloop_Udcref;                           //77
		unsigned int  Udcloop_PowerLimt;                        //78
		unsigned int  Udcloop_reserved;                         //79
    //=====================~{V8An?XVF2NJ}~}(80-99)======================
		unsigned int TrqCmd_SourceType;                       //80
		unsigned int SpdCmd_SourceType;                       //81
		unsigned int Trqloop_UpTrqRamp;                       //82
		unsigned int Trqloop_DownTrqRamp;                     //83
		unsigned int Trqloop_UpTimeSlice;                     //84
		unsigned int Trqloop_DownTimeSlice;                   //85
		unsigned int SpdLoop_Mincalspdstep;                   //86
		unsigned int SpdLoop_StepCaltimeSlice;                //87
		unsigned int SpdLoop_Filter_num;                      //88
		unsigned int SpdLoop_Cycle;                           //89
		unsigned int spdLoop_mincaltrqstep;                   //90
		unsigned int spdtrqref_StepCaltimeSlice;              //91
		unsigned int high_fre_enable;                          //92
		unsigned int high_fre_frequency;                       //93
		unsigned int high_fre_voltage;                         //94
		unsigned int high_fre_kp;                              //95
		unsigned int high_fre_ki;                              //96
		unsigned int high_fre_nspole_voltage;                  //97
		unsigned int resolver_fault_theta_maxerr;              //98
		unsigned int CMD_reserved;                             //99
	//=====================~{WT6/Hu4E?XVF2NJ}~}(100-119)==================
		unsigned int Weak_enable;                             //100 0:禁止 1：查表自动弱磁 （速度力矩环）2：不带查表速度（速度电流环）3：安标定电压自动弱磁
		unsigned int Weak_Kp;                                 //101
		unsigned int Weak_Ki;                                 //102
		unsigned int Weak_iq_Limt;                            //103
		unsigned int Weak_id_Limt;                            //104
		unsigned int Weak_Vol_Utilization;                    //105
		unsigned int Weak_Kd;                                 //106
		unsigned int Weak_Kp_iq;                              //107
		unsigned int Weak_Ki_iq;                              //108
		unsigned int Weak_iqcoff_Limt;                        //109
		unsigned int Weak_Kd_iq;                              //110
		unsigned int Weak_idcoff_Limt;                        //111
		unsigned int Weak_theta_Limt;                         //112
		unsigned int Ipark_angle_func_enable;                 //113
		unsigned int Ipark_angle_kp;                          //114
		unsigned int Ipark_angle_ki;                          //115
		unsigned int Ipark_angle_limit;                       //116
		unsigned int Weak_is_Limit;                           //117
		unsigned int Weakunsat_mode;                          //118
		unsigned int Sub_Weak_enable;                         //119
    //=====================~{O{662NJ}~}(120-139)=======================
		unsigned int EliJitter_enable;                        //120
		unsigned int EliJitter_TrqLimt;                       //121
		unsigned int EliJitter_Stop_Kp;                       //122
		unsigned int EliJitter_Stop_Kd;                      //123
		unsigned int EliJitter_Kp;                            //124
		unsigned int EliJitter_Ki;                            //125
		unsigned int EliJitter_Kd;                            //126
		unsigned int Spd_FWD_BrakeFault_Limt_Spd;             //127
		unsigned int Spd_FWD_TboxLifeFault_Limt_Spd;          //128
		unsigned int Spd_FWD_TboxLoseFault_Limt_Spd;          //129
	//==================================================================
		unsigned int Spd_FWD_BMSFault_Limt_Spd;               //130
		unsigned int Speed_maxerr;                  			 //131
		unsigned int Brake_gearenable;                  		//132
		unsigned int Lack_Phase_en;                  		//133
		unsigned int Lack_Phase_Enter_Speed;                 //134
		unsigned int Lack_Phase_Exit_Speed;                  //135
		unsigned int Lack_Phase_Pwm_Period;                  //136
		unsigned int Lack_Phase_reserved;                  	//137
		unsigned int Svpwm_way;                  			 //138
		unsigned int Svpwm_way_swap_speed;                  			 //139

	//=====================~{W$FB2NJ}~}(140-149)========================
		unsigned int Zero_Sation_Enable;                      //140
		unsigned int Zero_Sation_time;                         //141
		unsigned int Zero_Sation_Trq_Lmt;                     //142
		unsigned int Zero_Sation_Trq_Ramp;                    //143
		unsigned int Zero_Sation_kp;                          //144
		unsigned int Zero_Sation_ki;                          //145
		unsigned int Zero_Sation_lengthlmt;                   //146
		unsigned int Zero_Sation_sliplengthlmt;               //147
		unsigned int Zero_Sation_reserved[2];                 //148-149
    //=====================~{O^KY2NJ}~}(150-159)========================
		unsigned int Spd_Over_Limt_enable;                    //150
		unsigned int Spd_FWD_Limt_Spd;                       //151
		unsigned int Spd_REV_Limt_Spd;                       //152
		unsigned int Spd_FWD_L_Limt_Spd;                     //153
		unsigned int Spd_FWD_E_Limt_Spd;                     //154
		unsigned int Study_EncoderOffset_kp;                  //155
		unsigned int Study_EncoderOffset_ki;                  //156
		unsigned int Study_EncoderOffset_is;                  //157
		unsigned int Study_EncoderOffset_speed;               //158
		unsigned int Spd_Over_Limt_reserved;                  //159
	//=====================~{WTQ'O02NJ}~}(160-169)======================
		unsigned int Motor_study_MaxVol;                      //160
		unsigned int Motor_study_MaxCrt;                      //161
		unsigned int  Motor_study_FM;                          //162
		unsigned int  Motor_study_Rs;							//163
		unsigned int  Motor_study_Ld;							//164
		unsigned int  Motor_study_Lq;							//165
		unsigned int  Motor_studyFluex_is1;                     //166
		unsigned int  Motor_studyFluex_speed;                  //167
		unsigned int Motor_studyFluex_is2;                      //168
		unsigned int  Motor_discharge_current;                  //169
    //=====================~{O5M31(>/2NJ}~}(170-189)=====================
        unsigned int Motor_Protect_AlarmTempCtr;             //170
		unsigned int Motor_Protect_AlarmTempMtr;             //171
		unsigned int Motor_Protect_AlarmVdcLv;               //172
		unsigned int Motor_Protect_AlarmVdcOv;               //173		unsigned int Motor_Protect_InvCrt;                   //174
		unsigned int Motor_Protect_InvCrt;                   //174
		unsigned int Motor_Protect_MtrCrt;                   //175
		unsigned int Motor_Protect_InvCrtCoeff;              //176
		unsigned int Motor_Protect_MtrCrtCoeff;              //177
		unsigned int Protect_OvLdFlag;                       //178
		unsigned int Motor_Protect_AlarmSpdFwd;              //179
		unsigned int Motor_Protect_AlarmSpdRev;              //180
		unsigned int OpenPhaseCheck_En;                      //181
		unsigned int OpenPhaseCheck_CrtCoeff;                //182
		unsigned int PhaseUnbalanceCheck_En;                 //183
		unsigned int PhaseUnbalanceCheck_CrtCoeff;           //184
		unsigned int SpeedDetectFault_En;                    //185
		unsigned int CrtDetectFault_En;                      //186
		unsigned int HWresolverFault_En;                     //187
		unsigned int Motor_Protect_HWOverVolMtr;             //188
		unsigned int Motor_Protect_Alarm_reserved;           //189
	//=====================~{O5M39JUO2NJ}~}(190-209)=====================
        unsigned int Motor_Protect_OverTempCtr;              //190
		unsigned int Motor_Protect_OverTempMtr;              //191
		unsigned int Motor_Protect_VdcLv;                    //192
		unsigned int Motor_Protect_VdcOv;                    //193
		unsigned int Motor_Protect_OverIdcIn;                //194
		unsigned int Motor_Protect_OverIdcOut;               //195
		unsigned int Motor_Protect_SWOverCrtMtr;             //196
		unsigned int Motor_Protect_HWOverCrtMtr;             //197
		unsigned int Protect_overLoadInv;                    //198
		unsigned int Motor_Protect_OverSpdFwd;              //199
		unsigned int Motor_Protect_OverSpdRev;              //200
		unsigned int Motor_Protect_AlarmV12Lv;               //201
		unsigned int Motor_Protect_V12Lv;                    //202
		unsigned int Motor_Protect_AlarmV12Ov;               //203
		unsigned int Motor_Protect_V12Ov;                    //204
		unsigned int Motor_Mras_Kp;                          //205
		unsigned int Motor_Mras_Ki;                          //206
		unsigned int PWM_Lowspeed_Period;                    //207
		unsigned int PWM_Highspeed_Period;                   //208
		unsigned int PWM_Period_Swap_speed;                  //209
	//=====================~{1dF5Fw;y1>2NJ}IhVC~}(210-219)===============
		unsigned int PWM_Period;                              //210
		unsigned int PWM_Change_En;                           //211
		unsigned int Deadtimecomp_state;//~{T$3d5gJ1<d~}      //212
		unsigned int Deadtimecomp_percent;                    //213
		unsigned int SW_version;                              //214
		unsigned int SW_version_date;                         //215
		unsigned int Customer_ID;                             //216
		unsigned int EEPROM_version;                          //217
		unsigned int EEPROM_version_date;                     //218
		unsigned int Error_rest;                              //219;
	//=========================IGBT_Short_Para(220-229)==================
		unsigned int Short_IGBT_en;                           //220
		unsigned int Short_IGBT_udc_in;                       //221
		unsigned int Short_IGBT_udc_out;//~{T$3d5gJ1<d~}      //222
		unsigned int Iac_adjustCoeff_current1;                //223
		unsigned int Iac_adjustCoeff_percent1;                //224           
		unsigned int Iac_adjustCoeff_current2;                //225
		unsigned int Iac_adjustCoeff_percent2;                //226
		unsigned int Vdc_adjustCoeff_percent;                 //227
		unsigned int Harmonic_process_reserved;               //228
		unsigned int Test_Mode;                               //229
    //=========================咬齿力矩（230-239）=========================
		unsigned int Tooth_trq_en;                             //230
		unsigned int Tooth_trq;                                //231
		unsigned int Tooth_trq_ramp;                           //232
		unsigned int Idq_low_speed;                            //233
		unsigned int Idq_high_speed;                           //234
		unsigned int Id_high_speed_kp;                         //235
		unsigned int Id_high_speed_ki;                         //236
		unsigned int Iq_high_speed_kp;                         //237
		unsigned int Iq_high_speed_ki;                         //238
		unsigned int Idq_pi_swap_en;                           //239
	//=================================================================
	}field;
 	struct
    {
	    unsigned int Motor_Basic_Para[10];
	    unsigned int Motor_Charact_Para[10];
	    unsigned int Motor_Calibrat_Para[10];
	    unsigned int VoltFeedfwd_Para[10];
	    unsigned int AD_Para[20];
	    unsigned int PI_Para[20];
	    unsigned int CMD_Para[20];
	    unsigned int Weak_Para[20];
	    unsigned int EliJitter_Para[20];
	    unsigned int Zero_Sation_Para[10];
	    unsigned int Spd_Over_Limt_Para[10];
	    unsigned int Motor_study_Para[10];
	    unsigned int Motor_Protect_Alarm_Para[20];
	    unsigned int Motor_Protect_Para[20];
	    unsigned int Ctr_Para[10];
	    unsigned int IGBT_Short_Para[10];
	    unsigned int Tooth_trq_Para[10];
    }group;
	unsigned int value[240];
}tag_epdw;


typedef union
{
	struct
	{
	//=====================~{5g;z;y1>2NJ}#(~}0-9~{#)~}=====================
		_iq Motor_Np;                                //0
		_iq Encoder_Np;                              //1
		_iq Motor_Direction;                         //2
		_iq Motor_Encoder_Direction;                 //3
		_iq Motor_Encoder_offset;                    //4
		_iq Motor_Encoder_offset_elecoeff;           //5
		_iq Motor_Encoder_offset_gencoeff;           //6
		_iq Motor_TempSensor_type;                   //7
		_iq Motor_McuTempSensor_type;                //8
		_iq Motor_Encoder_rev_offset;                //9
	//=====================~{5g;zLXPT2NJ}#(~}10-19~{#)~}====================
		_iq Motor_RatePower;                         //10
		_iq Motor_PeakPowerEle;                      //11
		_iq Motor_PeakPowerGen;                      //12
		_iq Motor_RateTorque;                        //13
		_iq Motor_PeakTorqueEle;                     //14
		_iq Motor_PeakTorqueGen;                     //15
		_iq Motor_RateSpeed;                         //16
		_iq Motor_PeakSpeedFwd;                      //17
		_iq Motor_PeakSpeedRev;                      //18
		_iq Motor_Charact_reserved;                  //19
	//=====================~{5g;z1j6(2NJ}#(~}20-29~{#)~}====================
		_iq Motor_UDCMAP;                            //20
        _iq Motor_Trqidqmap_IdcoeffEle;              //21
        _iq Motor_Trqidqmap_IqcoeffEle;              //22
        _iq Motor_Trqidqmap_IdcoeffGen;              //23
        _iq Motor_Trqidqmap_IqcoeffGen;              //24
        _iq Motor_IdLimt_enable;                     //25
        _iq Motor_Trqcoeff_enable;                    //26
        _iq Motor_Idc_Select;                         //27
        _iq Motor_Calibration_reserved[2];           //28-29
    //=====================~{5gQ9G0@!#(~}30-39~{#)~}=======================
        _iq VoltFeedfwd_Coeff;                       //30
        _iq Rs_Stator;                               //31
        _iq Ld_Stator;                               //32
        _iq Lq_Stator;                               //33
        _iq Lq_SatStator;                         //34
        _iq Flux_Rotor;                              //35
        _iq iq_Lq_Stator;                          //36
        _iq iq_Lq_SatStator;                    //37
        _iq VoltFeedfwd_reserved[2];                 //38-39
    //=====================AD~{2NJ}EdVC#(~}40-59~{#)~}=====================
		_iq Iu_offset;                               //40
		_iq Iv_offset;                               //41
		_iq Iw_offset;                               //42
		_iq Iac_adjustCoeff;                         //43
		_iq Idc_offset;                              //44
		_iq Idc_adjustCoeff;                         //45
		_iq Vdc_offset;                              //46
		_iq Vdc_adjustCoeff;                         //47
		_iq Mtrtemp_offset;                          //48
		_iq Mdltemp_offset;                          //49
	//-----------------------------------------------------------
		_iq AI1_offset_on;                           //50
		_iq AI1_offset_off;                          //51
		_iq AI1_adjustCoeff;                         //52
		_iq AI2_offset_on;                           //53
		_iq AI2_offset_off;                          //54
		_iq AI2_adjustCoeff;                         //55
		_iq Vdc_sample_time;                         //56
		_iq Mtrtemp_adjustCoeff;					 //57
		_iq V12_adjustCoeff;					     //58
		_iq V5_adjustCoeff; 						 //59
    //=====================PI~{2NJ}~}(60-79)==========================
		_iq Id_kp;                                   //60
		_iq Id_ki;                                   //61
		_iq Iq_kp;                                   //62
		_iq Iq_ki;	                                  //63
		_iq Speed_kp;                                //64
		_iq Speed_ki;                                //65
		_iq Speed_kd;                                //66
		_iq Encoder_kp;                              //67
		_iq Encoder_ki;                              //68
		_iq V12_offset;                              //69
	//=====================TrqGen======================================
		_iq Udcloop_Enable;                           //70
		_iq Udcloop_kp;                               //71
		_iq Udcloop_ki;                               //72
		_iq Udcloop_UdcUplmt;                         //73
		_iq Udcloop_UdcDownlmt;                       //74
		_iq Udcloop_Speedmin;                         //75
		_iq Udcloop_Speedmax;                         //76
		_iq Udcloop_Udcref;                           //77
		_iq Udcloop_PowerLimt;                        //78
		_iq Udcloop_reserved;                         //79
    //=====================~{V8An?XVF2NJ}~}(80-99)======================
		_iq TrqCmd_SourceType;                       //80
		_iq SpdCmd_SourceType;                       //81
		_iq Trqloop_UpTrqRamp;                       //82
		_iq Trqloop_DownTrqRamp;                     //83
		_iq Trqloop_UpTimeSlice;                     //84
		_iq Trqloop_DownTimeSlice;                   //85
		_iq SpdLoop_Mincalspdstep;                   //86
		_iq SpdLoop_StepCaltimeSlice;                //87
		_iq SpdLoop_Filter_num;                      //88
		_iq SpdLoop_Cycle;                           //89
		_iq spdLoop_mincaltrqstep;                    //90
		_iq spdtrqref_StepCaltimeSlice;               //91
		_iq high_fre_enable;                          //92
		_iq high_fre_frequency;                       //93
		_iq high_fre_voltage;                         //94
		_iq high_fre_kp;                              //95
		_iq high_fre_ki;                              //96
		_iq high_fre_nspole_voltage;                  //97
		_iq resolver_fault_theta_maxerr;              //98
		_iq CMD_reserved;                             //99
	//=====================~{WT6/Hu4E?XVF2NJ}~}(100-119)==================
		_iq Weak_enable;                             //100
		_iq Weak_Kp;                                 //101
		_iq Weak_Ki;                                 //102
		_iq Weak_iq_Limt;                            //103
		_iq Weak_id_Limt;                            //104
		_iq Weak_Vol_Utilization;                    //105
		_iq Weak_Kd;                                 //106
		_iq Weak_Kp_iq;                              //107
		_iq Weak_Ki_iq;                              //108
		_iq Weak_iqcoff_Limt;                        //109
		_iq Weak_Kd_iq;                              //110
		_iq Weak_idcoff_Limt;                        //111
		_iq Weak_theta_Limt;                         //112
		_iq Ipark_angle_func_enable;                 //113
		_iq Ipark_angle_kp;                          //114
		_iq Ipark_angle_ki;                          //115
		_iq Ipark_angle_limit;                       //116
		_iq Weak_is_Limit;                           //117
		_iq Weakunsat_mode;                          //118
		_iq Sub_Weak_enable;                         //119
    //=====================~{O{662NJ}~}(120-139)=======================
		_iq EliJitter_enable;                        //120
		_iq EliJitter_TrqLimt;                       //121
		_iq EliJitter_Stop_Kp;                       //122
		_iq EliJitter_Stop_Kd;                      //123
		_iq EliJitter_Kp;                            //124
		_iq EliJitter_Ki;                            //125
		_iq EliJitter_Kd;                            //126
		_iq Spd_FWD_BrakeFault_Limt_Spd;             //127
		_iq Spd_FWD_TboxLifeFault_Limt_Spd;          //128
		_iq Spd_FWD_TboxLoseFault_Limt_Spd;          //129
		_iq Spd_FWD_BMSFault_Limt_Spd;               //130
		_iq Speed_maxerr;                  			 //131
		_iq Brake_gearenable;                  		//132
		_iq Lack_Phase_en;                  		//133
		_iq Lack_Phase_Enter_Speed;                 //134
		_iq Lack_Phase_Exit_Speed;                  //135
		_iq Lack_Phase_Pwm_Period;                  //136
		_iq Lack_Phase_reserved;                  	//137
		_iq Svpwm_way;                  			 //138
		_iq Svpwm_way_swap_speed;                  			 //139
	//=====================~{W$FB2NJ}~}(140-149)========================
		_iq Zero_Sation_Enable;                      //140
		_iq Zero_Sation_time;                         //141
		_iq Zero_Sation_Trq_Lmt;                     //142
		_iq Zero_Sation_Trq_Ramp;                    //143
		_iq Zero_Sation_kp;                          //144
		_iq Zero_Sation_ki;                          //145
		_iq Zero_Sation_lengthlmt;                   //146
		_iq Zero_Sation_sliplengthlmt;               //147
		_iq Zero_Sation_reserved[2];                 //148-149
    //=====================~{O^KY2NJ}~}(150-159)========================
		_iq Spd_Over_Limt_enable;                    //150
		_iq Spd_FWD_Limt_Spd;                       //151
		_iq Spd_REV_Limt_Spd;                       //152
		_iq Spd_FWD_L_Limt_Spd;                     //153
		_iq Spd_FWD_E_Limt_Spd;                     //154
		_iq Study_EncoderOffset_kp;                  //155
		_iq Study_EncoderOffset_ki;                  //156
		_iq Study_EncoderOffset_is;                  //157
		_iq Study_EncoderOffset_speed;               //158
		_iq Spd_Over_Limt_reserved;                  //159
	//=====================~{WTQ'O02NJ}~}(160-169)======================
		_iq Motor_study_MaxVol;                      //160
		_iq Motor_study_MaxCrt;                      //161
		_iq Motor_study_FM;                          //162
		_iq Motor_study_Rs;							  //163
		_iq Motor_study_Ld;							  //164
		_iq Motor_study_Lq;							  //165
		_iq Motor_studyFluex_is1;                     //166
		_iq Motor_studyFluex_speed;                  //167
		_iq Motor_studyFluex_is2;                     //168
		_iq Motor_discharge_current;                  //169
    //=====================~{O5M31(>/2NJ}~}(170-189)=====================
        _iq Motor_Protect_AlarmTempCtr;             //170
		_iq Motor_Protect_AlarmTempMtr;             //171
		_iq Motor_Protect_AlarmVdcLv;               //172
		_iq Motor_Protect_AlarmVdcOv;               //173
		_iq Motor_Protect_InvCrt;                   //174
		_iq Motor_Protect_MtrCrt;                   //175
		_iq Motor_Protect_InvCrtCoeff;              //176
		_iq Motor_Protect_MtrCrtCoeff;              //177
		//_iq Protect_AlarmInvLoadOv;               //178
		_iq Protect_OvLdFlag;                       //178
		_iq Motor_Protect_AlarmSpdFwd;              //179
		_iq Motor_Protect_AlarmSpdRev;              //180
		_iq OpenPhaseCheck_En;                      //181
		_iq OpenPhaseCheck_CrtCoeff;                //182
		_iq PhaseUnbalanceCheck_En;                 //183
		_iq PhaseUnbalanceCheck_CrtCoeff;           //184
		_iq SpeedDetectFault_En;                    //185
		_iq CrtDetectFault_En;                      //186
		_iq HWresolverFault_En;                     //187
		_iq Motor_Protect_HWOverVolMtr;             //188
		_iq Motor_Protect_Alarm_reserved;           //189
	//=====================~{O5M39JUO2NJ}~}(190-209)=====================
        _iq Motor_Protect_OverTempCtr;              //190
		_iq Motor_Protect_OverTempMtr;              //191
		_iq Motor_Protect_VdcLv;                    //192
		_iq Motor_Protect_VdcOv;                    //193
		_iq Motor_Protect_OverIdcIn;                //194
		_iq Motor_Protect_OverIdcOut;               //195
		_iq Motor_Protect_SWOverCrtMtr;             //196
		_iq Motor_Protect_HWOverCrtMtr;             //197
		_iq Protect_overLoadInv;                    //198
		_iq Motor_Protect_OverSpdFwd;              //199
		_iq Motor_Protect_OverSpdRev;              //200
		_iq Motor_Protect_AlarmV12Lv;               //201
		_iq Motor_Protect_V12Lv;                    //202
		_iq Motor_Protect_AlarmV12Ov;               //203
		_iq Motor_Protect_V12Ov;                    //204
		_iq Motor_Mras_Kp;                          //205
		_iq Motor_Mras_Ki;                          //206
		_iq PWM_Lowspeed_Period;                    //207
		_iq PWM_Highspeed_Period;                   //208
		_iq PWM_Period_Swap_speed;                  //209
	//=====================~{1dF5Fw;y1>2NJ}IhVC~}(210-219)===============
		_iq PWM_Period;                              //210
		_iq PWM_Change_En;                           //211
		_iq Deadtimecomp_state;//~{T$3d5gJ1<d~}      //212
		_iq Deadtimecomp_percent;                    //213
		_iq SW_version;                              //214
		_iq SW_version_date;                         //215
		_iq Customer_ID;                             //216
		_iq EEPROM_version;                          //217
		_iq EEPROM_version_date;                     //218
		_iq Error_rest;                              //219
	//=========================IGBT_Short_Para(220-229)==================
		_iq Short_IGBT_en;                           //220
		_iq Short_IGBT_udc_in;//~{T$3d5gJ1<d~}       //221
		_iq Short_IGBT_udc_out;                      //222
		_iq Iac_adjustCoeff_current1;                //223
		_iq Iac_adjustCoeff_percent1;                //224           
		_iq Iac_adjustCoeff_current2;                //225
		_iq Iac_adjustCoeff_percent2;                //226
		_iq Vdc_adjustCoeff_percent;                 //227
		_iq Harmonic_process_reserved;               //228
		_iq Test_Mode;                               //229
	//=========================咬齿力矩（230-239）=========================
		_iq Tooth_trq_en;                             //230
		_iq Tooth_trq;                                //231
		_iq Tooth_trq_ramp;                           //232
		_iq Idq_low_speed;                            //233
		_iq Idq_high_speed;                           //234
		_iq Id_high_speed_kp;                         //235
		_iq Id_high_speed_ki;                         //236
		_iq Iq_high_speed_kp;                         //237
		_iq Iq_high_speed_ki;                         //238
		_iq Idq_pi_swap_en;                           //239
    //=================================================================
	}field;
 	struct
    {
	    _iq Motor_Basic_Para[10];
	    _iq Motor_Charact_Para[10];
	    _iq Motor_Calibrat_Para[10];
	    _iq VoltFeedfwd_Para[10];
	    _iq AD_Para[20];
	    _iq PI_Para[20];
	    _iq CMD_Para[20];
	    _iq Weak_Para[20];
	    _iq EliJitter_Para[20];
	    _iq Zero_Sation_Para[10];
	    _iq Spd_Over_Limt_Para[10];
	    _iq Motor_study_Para[10];
	    _iq Motor_Protect_Alarm_Para[20];
	    _iq Motor_Protect_Para[20];
	    _iq Ctr_Para[10];
	    _iq IGBT_Short_Para[10];
	    _iq Tooth_trq_Para[10];
    }group;
	_iq value[240];
}tag_EV_MCU_Para;

typedef struct
{
	unsigned int  emsw;
	unsigned int  emsw1;
	unsigned int  emsw2;
	unsigned int  alarm;
	unsigned int  speed;
	unsigned int  torque;
	unsigned int  mdl_temp;
	unsigned int  mtr_temp;
	unsigned int  udc;
	unsigned int  udq;
	unsigned int  id;
	unsigned int  iq;
	unsigned int  ia;
	unsigned int  ib;
	unsigned int  ic;
	unsigned int  reseverd;
}tag_I2CErr_Data;

typedef union
{
	struct
	{
		unsigned int      I2C_fault_num;
		tag_I2CErr_Data   I2CErr_Data[5];
	}field;
	unsigned int value[81];
}tag_I2CErr_Record;

typedef union
{
	struct
	{
		unsigned int iu;
		unsigned int iv;
		unsigned int iw;
		unsigned int speed;
		unsigned int udc_up;
		unsigned int mdl_temp;
		unsigned int mtr_temp;
	}field;
	unsigned int value[7];
}tag_I2CValueLimit_Record;


#ifdef _EEPROM_C_

unsigned int eeprom_record_enable = 0;
unsigned int eeprom_record_index[10] = {0};
unsigned int eeprom_valuelimit_index[11] = {0};
unsigned int eeprom_errclear_enable = 0;
unsigned int eeprom_readnum = 0;
unsigned int I2C_fault_writenum = 0;
unsigned int I2C_fault_writeaddr = 0;
tag_I2CErr_Record           I2CErr_Record = {0};
tag_I2CValueLimit_Record    I2CValueLimit_Record = {0};
tag_epdw epdw = {0};

void eeprom_Init(void);
void I2C_variableInit(void);
void eeprom_Process(void);

void I2C_FaultDateClear(void);
void I2C_LimitDateClear(void);
void I2C_FaultRecordInt(void);
void I2C_ValueLimit_Int(void);
void eeprom_test(void);
unsigned int eeprom_crc16_ccitt_false(unsigned int *data,unsigned int len);

unsigned int I2cWriteData(unsigned int I2CWrite_Index,unsigned int *I2CWrite_Add);
unsigned int I2cReadData(unsigned int I2CRead_Index,unsigned int *I2CRead_Add);
#else

extern unsigned int eeprom_readnum;
extern unsigned int eeprom_record_enable;
extern unsigned int eeprom_record_index[10];
extern unsigned int eeprom_valuelimit_index[11];

extern unsigned int I2C_fault_writenum;
extern unsigned int I2C_fault_writeaddr;


extern tag_I2CErr_Record           I2CErr_Record;
extern tag_I2CValueLimit_Record    I2CValueLimit_Record;
extern tag_epdw epdw;


extern void eeprom_Init(void);
extern void I2C_variableInit(void);
extern void eeprom_Process(void);

extern void I2C_FaultDateClear(void);
extern void I2C_LimitDateClear(void);
extern void I2C_FaultRecordInt(void);
extern void I2C_ValueLimit_Int(void);
extern void eeprom_test(void);
extern unsigned int I2cWriteData(unsigned int I2CWrite_Index,unsigned int *I2CWrite_Add);
extern unsigned int I2cReadData(unsigned int I2CRead_Index,unsigned int *I2CRead_Add);
extern unsigned int eeprom_crc16_ccitt_false(unsigned int *data,unsigned int len);

#endif

#endif /* EEPROM_H_ */

