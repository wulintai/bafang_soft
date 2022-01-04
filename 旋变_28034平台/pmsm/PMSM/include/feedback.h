	/*
 * feedback.h
 *
 *  Created on: 2014-4-15
 *      Author: ThinkPad
 */

#ifndef _FEEDBACK_H_
#define _FEEDBACK_H_

#define AI_BUFFER_LENGTH		8
#define	RESOLVER_LINES_12BIT	4096		// 12?????

// AD????????
/*
#define MAX_CURRENT_POS_AD		2047					// 12bit ADC
#define MAX_CURRENT_NEG_AD		-2047
#define MAX_VDC_AD			 	4095
#define MAX_TEMPERATION_AD		4095



// ???????????
//#define CALIB_DELAYNUM			25000					// ??20000????
#define CALIB_ACCUMNUM			4096					// ??10000?????????
#define CALIB_OFFSET_UPLIMIT	2200					//1.87*4096/3	// 1.87V
#define CALIB_OFFSET_DOWNLIMIT	1900					//1.47*4096/3	// 1.47V*/

//?????

#define Ref_OV_PI_MAX           3750      //10KHz  3750   300V
#define Ref_OV_PI_MIN           1125      // 90V
#define	Ref_OV_PI_KP            _IQ15(1.25)
#define	Ref_OV_PI_KI            _IQ15(0.015)
#define REF_OV_K				_IQ15(8.19)
//========================================================================================
// ??????
//========================================================================================
typedef struct
{

	// ????
	Uint32		Iu_fbk_ad;
	_iq			Iu_fbk;
	_iq         Iu_fbk_real;
	_iq			Iu_fbk_HMI;

	Uint32		Iv_fbk_ad;
	_iq			Iv_fbk;
	_iq         Iv_fbk_real;
	_iq			Iv_fbk_HMI;

	Uint32		Iw_fbk_ad;
	_iq			Iw_fbk;
	_iq         Iw_fbk_real;
	_iq			Iw_fbk_HMI;

	_iq			Idc_fbk_HMI;
	_iq         Idc_fbk_filter;

	_iq			Id_fbk;
	_iq         Id_fbk_filter;
	_iq         Id_fbk_filter_0025;
	_iq			Id_fbk_HMI;
	_iq			Iq_fbk;
	_iq         Iq_fbk_filter;
	_iq         Iq_fbk_filter_0025;
	_iq			Iq_fbk_HMI;
	_iq			Is_fbk;
	_iq         Is_square_fbk;
	_iq			Is_fbk_HMI;
	_iq         Is_fbk_HMI_fiter;
	_iq         Power_ele_fbk;

	// ????
	_iq		    Vdc_fbk_ad;
	_iq		    Vdc_fbk_ad_filter;
	_iq			Vdc_fbk;
	_iq			Vdc_filter;
	_iq         Vdc_filter_protect;
	_iq			Vdc_unfilter;
	_iq         Vdc_count;
	_iq         Vdc_min;

	// ????
	Uint32		Temp_mtr_ad;
	_iq			Temp_mtr;

	_iq			Temp_mdl_ad_filter;
	_iq			Temp_mtr_ad_filter;

	Uint32		Temp_mdl_ad;
	_iq         Temp_mdl;
	_iq			Temp_igbt;
	_iq         Temp_ta;

	_iq		    AI1_fbk_ad;
	_iq			AI1_fbk;
	_iq			AI1_fbk_offset;
	
	_iq		    AI2_fbk_ad;
	_iq			AI2_fbk;
	_iq			AI2_fbk_offset;

	_iq		    V12_fbk_ad;
	_iq		    V12_fbk;
	_iq			V12_fbk_HMI;
	_iq         V12_fbk_filter;


	_iq		    TA_ad;
	_iq         TA_ad_filter;

	// ????
	int32		Theta_fbk_raw;
	_iq			Theta_fbk_Ele;
	_iq			Theta_fbk_Ele_HMI;

	// ????
	_iq			Speed_fbk;
	_iq			Speed_fbk_Filter;
	_iq         Speed_fbk_Filter_Lowspeed;
	_iq			Speed_fbk_HMI;
	_iq			Speed_fbk_HMI_abs;
	_iq         Speed;
	_iq         Speed_abs;
	_iq			Omega_fbk_Mech;
	_iq			Omega_fbk_Ele;
} sysFbkVars;

typedef union
{
	struct
	{
		Uint16 In_fwdrev_new 		:3;
		Uint16 In_fwdrev 			:3;
		Uint16 zero_disable_flag 	:1;
		Uint16 limit_speed_flag     :1;
		
		Uint16 key_state 	        :1;
		Uint16 zerotrq_flag		    :1;
		Uint16 zerotrq_flag2		:1;
		Uint16 zerotrq_flag3		:1;
		Uint16 exit_zero_add_trq_flag :2;
		Uint16 zero_or_start_flag   :2;
	}field;
	Uint16 wValue;
}tag_VCU_IOstate;

#ifdef _FEEDBACK_C_

// ?????????
//tag_VCU_IOstate VCU_IOstate = {0};

//?????????
_iq	SpeedCal_ThetaEle = 0;
_iq	SpeedCal_ThetaEleOld = 0;
_iq	SpeedCal_spdFbk = 0;
_iq	SpeedCal_deltaTheta = 0;
_iq SpeedCal_deltaTime = 0;
Uint16 SpeedCal_Count = 0;
Uint16 SpeedCal_First_Flag = 0;

_iq up_step = 0;
_iq down_step = 0;
Uint16 motor_turn_off_cnt = 0;
/*Uint16 motor_first_flag = 0;
Uint16 motor_gear_n_cnt = 0;
Uint16 motor_swap_gear_n_cnt = 0;*/

Uint16 resolver_fault_type = 0;
// ??????
sysFbkVars	sysFbkPara = {0};
tag_VCU_IOstate VCU_IOstate = {0};
Uint16  AD_AISelf_Flag = 0;
Uint16  AD_AISelf_Cnt = 0;
Uint32  AD_AI1_Sum = 0;

_iq     Theta_fbk_raw_n = 0;
_iq     Theta_fbk_raw_n_1 = 0;
_iq     Theta_fbk_raw_deta_n = 0;
_iq     Theta_fbk_raw_deta_n_1 = 0;
_iq     Theta_fbk_raw_deta_err = 0;
Uint16  Theta_fbk_fault_cnt = 0;
Uint16  Theta_fbk_sample_cnt = 0;
#if 1
int16	AI1_Ad_Buffer[AI_BUFFER_LENGTH] = {0};
int16	AI2_Ad_Buffer[AI_BUFFER_LENGTH] = {0};
#else
_iq	AI1_Ad_Buffer = 0;
_iq	AI2_Ad_Buffer = 0;
#endif

const int TMP_NTC_TAB[]={
	3352 ,
	3350 ,
	3348 ,
	3345 ,
	3342 ,
	3340 ,
	3337 ,
	3334 ,
	3330 ,
	3327 ,
	3323 ,
	3319 ,
	3315 ,
	3310 ,
	3306 ,
	3301 ,
	3295 ,
	3290 ,
	3284 ,
	3278 ,
	3271 ,
	3265 ,
	3257 ,
	3250 ,
	3242 ,
	3234 ,
	3225 ,
	3216 ,
	3207 ,
	3197 ,
	3187 ,
	3176 ,
	3165 ,
	3153 ,
	3141 ,
	3128 ,
	3115 ,
	3102 ,
	3088 ,
	3073 ,
	3058 ,
	3042 ,
	3026 ,
	3010 ,
	2993 ,
	2975 ,
	2957 ,
	2939 ,
	2920 ,
	2901 ,
	2881 ,
	2860 ,
	2839 ,
	2818 ,
	2796 ,
	2774 ,
	2752 ,
	2729 ,
	2706 ,
	2683 ,
	2659 ,
	2635 ,
	2610 ,
	2586 ,
	2561 ,
	2536 ,
	2511 ,
	2486 ,
	2461 ,
	2435 ,
	2410 ,
	2384 ,
	2359 ,
	2333 ,
	2308 ,
	2282 ,
	2257 ,
	2232 ,
	2207 ,
	2182 ,
	2157 ,
	2132 ,
	2108 ,
	2084 ,
	2060 ,
	2036 ,
	2013 ,
	1990 ,
	1967 ,
	1945 ,
	1923 ,
	1901 ,
	1880 ,
	1859 ,
	1838 ,
	1817 ,
	1797 ,
	1778 ,
	1759 ,
	1740 ,
	1721 ,
	1703 ,
	1686 ,
	1668 ,
	1652 ,
	1635 ,
	1619 ,
	1603 ,
	1588 ,
	1572 ,
	1558 ,
	1543 ,
	1529 ,
	1516 ,
	1503 ,
	1490 ,
	1477 ,
	1465 ,
	1453 ,
	1441 ,
	1430 ,
	1419 ,
	1408 ,
	1398 ,
	1387 ,
	1377 ,
	1368 ,
	1359 ,
	1349 ,
	1341 ,
	1332 ,
	1324 ,
	1315 ,
	1308 ,
	1300 ,
	1292 ,
	1285 ,
	1278 ,
	1271 ,
	1265 ,
	1258 ,
	1252 ,
	1246 ,
	1240 ,
	1234 ,
	1229 ,
	1223 ,
	1218 ,
	1213 ,
	1208 ,
	1203 ,
	1198 ,
	1194 ,
	1189 ,
	1185 ,
	1181 ,
	1177 ,
	1173 ,
	1169 ,
	1165 ,
	1161 ,
	1158 ,
	1154 ,
	1151 ,
	1148 ,
	1145 ,
};

const int TMP_MTR_PT100_TAB[] = 
{
	2273 ,//-50
	2235 ,//-45
	2197 ,
	2159 ,
	2121 ,
	2084 ,
	2046 ,
	2008 ,
	1970 ,
	1933 ,
	1896 ,
	1859 ,
	1822 ,
	1785 ,
	1748 ,
	1712 ,
	1675 ,
	1639 ,
	1603 ,
	1566 ,
	1530 ,
	1494 ,
	1459 ,
	1423 ,
	1387 ,
	1352 ,
	1316 ,
	1281 ,
	1246 ,
	1211 ,
	1176 ,
	1141 ,
	1106 ,
	1072 ,
	1037 ,
	1003 ,
	968 ,
	934 ,
	900 ,
	866 ,
	832 ,
	798 ,
	765 ,
	731 ,
	698 ,
	664 ,
	631 ,
	598 ,
	565 ,
	532 ,
	499 ,
	466 ,
	433 ,//210
};

const int TMP_MTR_PT1000_TAB[] =
{
	3463 ,//-50
	3396 ,//-45
	3330 ,
	3264 ,
	3200 ,
	3137 ,
	3074 ,
	3012 ,
	2951 ,
	2891 ,
	2831 ,
	2772 ,
	2715 ,
	2657 ,
	2601 ,
	2546 ,
	2491 ,
	2436 ,
	2383 ,
	2330 ,
	2277 ,
	2226 ,
	2175 ,
	2125 ,
	2075 ,
	2026 ,
	1978 ,
	1930 ,
	1882 ,
	1836 ,
	1789 ,
	1744 ,
	1698 ,
	1654 ,
	1610 ,
	1566 ,
	1523 ,
	1480 ,
	1438 ,
	1396 ,
	1355 ,
	1315 ,
	1274 ,
	1235 ,
	1195 ,
	1156 ,
	1118 ,
	1079 ,
	1042 ,
	1004 ,
	967 ,
	930 ,
	894 ,//210
};

const int TMP_MTR_NTC_10K_TAB[] =
{
	257 ,//-40
	260 ,
	263 ,
	266 ,
	269 ,
	273 ,
	276 ,
	280 ,
	284 ,
	288 ,
	293 ,
	297 ,
	302 ,
	307 ,
	312 ,
	317 ,
	323 ,
	329 ,
	335 ,
	341 ,
	348 ,
	355 ,
	362 ,
	369 ,
	377 ,
	385 ,
	393 ,
	402 ,
	411 ,
	420 ,
	430 ,
	440 ,
	450 ,
	460 ,
	471 ,
	482 ,
	494 ,
	506 ,
	518 ,
	531 ,
	544 ,//0
	557 ,
	571 ,
	586 ,
	600 ,
	615 ,
	630 ,
	646 ,
	662 ,
	679 ,
	695 ,
	713 ,
	730 ,
	748 ,
	767 ,
	785 ,
	804 ,
	824 ,
	843 ,
	864 ,
	884 ,
	905 ,
	926 ,
	947 ,
	969 ,
	991 ,
	1013 ,
	1036 ,
	1058 ,
	1081 ,
	1105 ,
	1128 ,
	1152 ,
	1176 ,
	1200 ,
	1224 ,
	1249 ,
	1273 ,
	1298 ,
	1322 ,
	1347 ,
	1372 ,
	1397 ,
	1423 ,
	1448 ,
	1473 ,
	1498 ,
	1523 ,
	1548 ,
	1573 ,
	1598 ,
	1624 ,
	1649 ,
	1673 ,
	1698 ,
	1723 ,
	1747 ,
	1772 ,
	1796 ,
	1820 ,
	1844 ,
	1868 ,
	1892 ,
	1915 ,
	1939 ,
	1962 ,
	1985 ,
	2007 ,
	2029 ,
	2052 ,
	2073 ,
	2095 ,
	2116 ,
	2138 ,
	2159 ,
	2179 ,
	2200 ,
	2220 ,
	2240 ,
	2259 ,
	2279 ,
	2298 ,
	2317 ,
	2335 ,
	2353 ,
	2371 ,
	2389 ,
	2406 ,
	2423 ,
	2440 ,
	2456 ,
	2473 ,
	2489 ,
	2505 ,
	2520 ,
	2535 ,
	2550 ,
	2565 ,
	2579 ,
	2593 ,
	2607 ,
	2621 ,
	2634 ,
	2647 ,
	2660 ,
	2673 ,
	2685 ,
	2697 ,
	2709 ,
	2721 ,
	2732 ,
	2744 ,
	2755 ,
	2766 ,
	2777 ,
	2788 ,
	2798 ,
	2808 ,
	2818 ,
	2828 ,
	2838 ,
	2847 ,
	2857 ,
	2866 ,
	2875 ,
	2883 ,
	2892 ,
	2900 ,
	2908 ,
	2916 ,
	2924 ,
	2932 ,
	2940 ,
	2947 ,
	2954 ,
	2961 ,
	2968 ,
	2975 ,
	2982 ,
	2988 ,
	2994 ,
	3001 ,
	3007 ,
	3013 ,
	3018 ,
	3024 ,
	3030 ,
	3035 ,
	3040 ,
	3045 ,
	3051 ,
	3056 ,
	3060 ,
	3065 ,
	3070 ,
	3074 ,
	3079 ,
	3083 ,
	3087 ,
	3091 ,
	3095 ,
	3099 ,
	3103 ,
	3107 ,
	3111 ,
	3114 ,
	3118 ,
	3121 ,
	3125 ,
	3128 ,
	3131 ,
	3134 ,
	3138 ,
	3141 ,
	3144 ,
	3147 ,
	3149 ,
	3152 ,
	3155 ,
	3158 ,
	3160 ,
	3163 ,
	3165 ,
	3168 ,
	3170 ,
	3173 ,
	3175 ,
	3177 ,
	3179 ,
	3181 ,
	3184 ,
	3186 ,
	3188 ,
	3190 ,
	3192 ,
	3194 ,
	3195 ,
	3197 ,
	3199 ,
	3201 ,
	3202 ,
	3204 ,
	3206 ,
	3207 ,
	3209 ,
	3210 ,
	3212 ,
	3213 ,
	3215 ,
	3216 ,
	3218 ,//210
};

const int TMP_MTR_NTC_30K_TAB[] =
{
	249 ,//-40
	254 ,//-35
	260 ,
	269 ,
	280 ,
	295 ,
	314 ,
	337 ,
	366 ,
	402 ,
	445 ,
	497 ,
	558 ,
	629 ,
	711 ,
	803 ,
	907 ,
	1021 ,
	1145 ,
	1277 ,
	1415 ,
	1558 ,
	1702 ,
	1845 ,
	1987 ,
	2124 ,
	2257 ,
	2382 ,
	2500 ,
	2611 ,
	2714 ,
	2808 ,
	2894 ,
	2973 ,
	3044 ,
	3108 ,
	3165 ,
	3217 ,
	3264 ,
	3305 ,
	3343 ,
	3377 ,
	3407 ,
	3434 ,
	3458 ,
	3480 ,
	3500 ,
	3518 ,
	3533 ,
	3548 ,
	3561 ,//210
};

const int TMP_MTR_NTC_100K_TAB[] =
{
	424 ,//-30
	424 ,//-29
	425 ,
	426 ,
	427 ,
	428 ,
	429 ,
	430 ,
	431 ,
	432 ,
	433 ,
	434 ,
	435 ,
	437 ,
	438 ,
	440 ,
	441 ,
	443 ,
	445 ,
	447 ,
	449 ,
	451 ,
	453 ,
	455 ,
	458 ,
	460 ,
	463 ,
	466 ,
	469 ,
	472 ,
	475 ,
	478 ,
	482 ,
	486 ,
	490 ,
	494 ,
	498 ,
	502 ,
	507 ,
	512 ,
	517 ,
	522 ,
	528 ,
	534 ,
	540 ,
	546 ,
	553 ,
	559 ,
	567 ,
	574 ,
	582 ,
	590 ,
	598 ,
	606 ,
	615 ,
	625 ,
	634 ,
	644 ,
	654 ,
	665 ,
	676 ,
	687 ,
	699 ,
	711 ,
	724 ,
	737 ,
	750 ,
	764 ,
	778 ,
	793 ,
	808 ,
	824 ,
	839 ,
	856 ,
	873 ,
	890 ,
	908 ,
	926 ,
	945 ,
	964 ,
	983 ,
	1003 ,
	1024 ,
	1045 ,
	1066 ,
	1088 ,
	1110 ,
	1133 ,
	1156 ,
	1180 ,
	1204 ,
	1228 ,
	1253 ,
	1278 ,
	1304 ,
	1330 ,
	1356 ,
	1383 ,
	1410 ,
	1437 ,
	1465 ,
	1493 ,
	1521 ,
	1550 ,
	1578 ,
	1607 ,
	1637 ,
	1666 ,
	1696 ,
	1726 ,
	1756 ,
	1785 ,
	1816 ,
	1846 ,
	1877 ,
	1907 ,
	1938 ,
	1968 ,
	1999 ,
	2029 ,
	2060 ,
	2091 ,
	2122 ,
	2151 ,
	2183 ,
	2213 ,
	2242 ,
	2272 ,
	2302 ,
	2331 ,
	2362 ,
	2391 ,
	2419 ,
	2449 ,
	2479 ,
	2506 ,
	2533 ,
	2562 ,
	2589 ,
	2617 ,
	2646 ,
	2670 ,
	2698 ,
	2724 ,
	2751 ,
	2775 ,
	2801 ,
	2826 ,
	2850 ,
	2874 ,
	2899 ,
	3015 ,//125
};

const int TMP_MTR_NTC_15C054_TAB[] =
{
	448 , //-40
	449 ,
	449 ,
	449 ,
	450 ,
	450 ,
	451 ,
	451 ,
	452 ,
	453 ,
	453 ,
	454 ,
	455 ,
	456 ,
	456 ,
	457 ,
	458 ,
	459 ,
	460 ,
	461 ,
	462 ,
	464 ,
	465 ,
	467 ,
	468 ,
	470 ,
	471 ,
	473 ,
	475 ,
	477 ,
	479 ,
	481 ,
	483 ,
	486 ,
	488 ,
	491 ,
	493 ,
	496 ,
	499 ,
	503 ,
	506 ,
	510 , //0
	513 ,
	517 ,
	521 ,
	525 ,
	530 ,
	535 ,
	539 ,
	545 ,
	550 ,
	555 ,
	561 ,
	567 ,
	574 ,
	580 ,
	587 ,
	594 ,
	602 ,
	609 ,
	617 ,
	626 ,
	634 ,
	643 ,
	653 ,
	662 ,
	672 ,
	683 ,
	693 ,
	705 ,
	716 ,
	728 ,
	740 ,
	753 ,
	766 ,
	780 ,
	793 ,
	808 ,
	822 ,
	837 ,
	853 ,
	869 ,
	885 ,
	902 ,
	920 ,
	937 ,
	955 ,
	974 ,
	993 ,
	1012,
	1032,
	1052,
	1073,
	1094,
	1116,
	1137,
	1160,
	1182,
	1205,
	1228,
	1252,
	1276,
	1300,
	1324,
	1350,
	1375,
	1400,
	1426,
	1451,
	1477,
	1504,
	1530,
	1557,
	1584,
	1610,
	1637,
	1664,
	1691,
	1719,
	1746,
	1772,
	1800,
	1827,
	1854,
	1881,
	1908,
	1935,
	1962,
	1989,
	2016,
	2042,
	2069,
	2095,
	2121,
	2147,
	2172,
	2197,
	2222,
	2247,
	2272,
	2296,
	2320,
	2344,
	2367,
	2390,
	2413,
	2436,
	2458,
	2480,
	2502,
	2523,
	2544,
	2565,
	2585,
	2605,
	2625,
	2644,
	2663,
	2682,
	2700,
	2718,
	2736,
	2753,
	2771,
	2787,
	2804,
	2820,
	2835,
	2851,
	2866,
	2881,
	2895,
	2910,
	2924,
	2937,
	2951,
	2964,
	2977,
	2989,
	3001,
	3013,
	3025,
	3037,
	3048,
	3059,
	3069,
	3080,
	3090,
	3100,
	3110,
	3120,
	3129,
	3138,
	3147,
	3156,
	3164,
	3173,
	3181,
	3189,
	3196,
	3204,
	3211,
	3219,
	3226,
	3233,
	3239,
	3246,
	3252,
	3259,
	3265,
	3271,
	3277,
	3282,
	3288,
	3293,
	3299,
	3304,
	3309,
	3314,
	3319,
	3323,
	3328,
	3333,
	3337,
	3341,
	3346,
	3350,
	3354,
	3358,
	3361,
	3365,
	3369,
	3372,
	3376,
	3379,
	3383,
	3386,
	3389,
	3392,
	3395,
	3398,
	3401,
	3404,
	3407,
	3409,
	3412,
	3415,
	3417,
	3420,
	3422,
	3425, //210
};

const int TMP_MTR_KTY84_150_TAB[] =
{
	3489 ,//-40
	3391 ,//-30
	3292 ,
	3187 ,
	3079 ,
	2969 ,
	2854 ,
	2796 ,
	2737 ,
	2621 ,
	2499 ,
	2378 ,
	2257 ,
	2134 ,
	2010 ,
	1887 ,
	1765 ,
	1641 ,
	1518 ,
	1398 ,
	1276 ,
	1158 ,
	1040 ,
	923 ,
	807 ,//190
};

//========================================================================================
// ????
//========================================================================================
void ThetaCal_Resolver(void);
void trqFeedback(void);
void spdFeedback_Init(void);
void ThetaCal_Resolver();
void SpeedCal_Resolver();
void SpeedFbk_FilterClear(void);
void AIFeedback(void);
void AdcSEQ2(void);
void Resolver_read(void);
_iq tempLookupTable(_iq temp,const int* table);
_iq tempLookupMtrTable(Uint16 temp_type,_iq temp_ad,const int* table);
void v12Feedback(void);
void motorParameterInit(void);
void motorLookupOffsetTable(void);

#else
//========================================================================================
// ????????
//========================================================================================
extern tag_VCU_IOstate VCU_IOstate;
extern _iq	SpeedCal_deltaTheta;
extern sysFbkVars	sysFbkPara;
extern _iq	SpeedCal_spdFbk;

extern _iq up_step;
extern _iq down_step;

extern Uint16 resolver_fault_type;

extern void ThetaCal_Resolver(void);
extern void trqFeedback(void);
extern void SpeedCal_Resolver(void);
extern void tempFeedback(void);
extern void ThetaCal_Resolver();
extern void AIFeedback(void);
extern void AdcSEQ2(void);
extern void Resolver_read(void);
extern void v12Feedback(void);
extern void motorParameterInit(void);
extern void motorLookupOffsetTable(void);

#endif	// end of #ifdef _FEEDBACK_C

#endif	// end of #ifndef _FEEDBACK_H

//===========================================================================
// No more.
//===========================================================================

