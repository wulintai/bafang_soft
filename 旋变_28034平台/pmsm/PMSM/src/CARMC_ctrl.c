//================================================================================
// 控制程序
//================================================================================
#ifndef _CARMC_CTRL_C_
#define _CARMC_CTRL_C_

#include "include_c.h"

#pragma CODE_SECTION(sub_adc_handler,"ramfuncs");
//=============================================
//AD采样子程序：完成8kHz的采样计算
//=============================================
void sub_adc_handler(void)
{
	_iq deta                    = 0;
	_iq time                    = 0;
	_iq frequency               = 0;
	_iq id_temp                 = 0;
	_iq iq_temp                 = 0;
	_iq Temp                    = 0;
	_iq	theta		            = sysCfgPara.Theta_ele;
	static Uint32 Iu_fbk_accum  = 0;
	static Uint32 Iv_fbk_accum  = 0;
	static Uint32 Iw_fbk_accum  = 0;
	static int	 delayCnt	    = 0;
	static int	 accumCnt	    = 0;

	while(AdcRegs.ADCCTL1.bit.ADCBSY == 1)
	{
	}
	sysFbkPara.Iu_fbk_ad = AdcResult.ADCRESULT0;
	sysFbkPara.Iv_fbk_ad = AdcResult.ADCRESULT2;
	sysFbkPara.Iw_fbk_ad = AdcResult.ADCRESULT1;
	sysFbkPara.Vdc_fbk_ad = AdcResult.ADCRESULT5;
	
	Iu_fbk_raw = (_iq)sysFbkPara.Iu_fbk_ad-EV_MCU_Para.field.Iu_offset;
	Iv_fbk_raw = (_iq)sysFbkPara.Iv_fbk_ad-EV_MCU_Para.field.Iv_offset;
	Iw_fbk_raw = (_iq)sysFbkPara.Iw_fbk_ad-EV_MCU_Para.field.Iw_offset;

	if(scsw2.field.igbt_state == 0 && _IQ15abs(sysFbkPara.Speed_fbk_HMI)<_IQ15div(100,SysBase.speed)) //546:100RPM
	{
		if(scsw2.field.adCalib_state == 1 && scsw2.field.adCalib_state_finsh == 1 && (sysFbkPara.Is_fbk_HMI_fiter > _IQ15div(3,SysBase.iphase) )) //109:3A
		{
			scsw2.field.adCalib_state = 0;
			scsw2.field.adCalib_state_finsh = 0;
			delayCnt = CALIB_DELAYNUM;
			accumCnt = 0;
			Iu_fbk_accum  = 0;
			Iv_fbk_accum  = 0;
			Iw_fbk_accum  = 0;
		}
	}

	if(scsw2.field.adCalib_state_finsh == 0)
	{
		if (delayCnt < CALIB_DELAYNUM)
		{
			delayCnt++;
		}
		else
		{
			if((1 == scsw2.field.adCalib_state)
			 &&(1 == sccw2.field.run_flag))
			{
				scsw2.field.adCalib_state_finsh=1;
				first_adCalib_state_finsh = 1;
			}
			else if(scsw2.field.igbt_state == 0)
			{
				if (accumCnt < CALIB_ACCUMNUM)
				{
					Iu_fbk_accum  += sysFbkPara.Iu_fbk_ad;
					Iv_fbk_accum  += sysFbkPara.Iv_fbk_ad;
					Iw_fbk_accum  += sysFbkPara.Iw_fbk_ad;
					accumCnt++;
				}
				else
				{
					if(_IQ15abs(sysFbkPara.Speed_fbk_HMI)<_IQ15div(20,SysBase.speed))
					{
						Iu_fbk_accum   = Iu_fbk_accum-(Iu_fbk_accum>>12) +sysFbkPara.Iu_fbk_ad;
						Iv_fbk_accum   = Iv_fbk_accum-(Iv_fbk_accum>>12) +sysFbkPara.Iv_fbk_ad;
						Iw_fbk_accum   = Iw_fbk_accum-(Iw_fbk_accum>>12) +sysFbkPara.Iw_fbk_ad;
					}
					else
					{
						Iu_fbk_accum = Iu_fbk_accum;
						Iv_fbk_accum = Iv_fbk_accum;
						Iw_fbk_accum = Iw_fbk_accum;
					}
					epdw.field.Iu_offset = EV_MCU_Para.field.Iu_offset  = Iu_fbk_accum>>12;
					epdw.field.Iv_offset = EV_MCU_Para.field.Iv_offset  = Iv_fbk_accum>>12;
					epdw.field.Iw_offset = EV_MCU_Para.field.Iw_offset  = Iw_fbk_accum>>12;
					scsw2.field.adCalib_state = 1;
				} 
			}
		} 
	}

	Iu_fbk_raw	= (Iu_fbk_raw > MAX_CURRENT_POS_AD) ? MAX_CURRENT_POS_AD : Iu_fbk_raw;
	Iu_fbk_raw	= (Iu_fbk_raw < MAX_CURRENT_NEG_AD) ? MAX_CURRENT_NEG_AD : Iu_fbk_raw;

	Iv_fbk_raw	= (Iv_fbk_raw > MAX_CURRENT_POS_AD) ? MAX_CURRENT_POS_AD : Iv_fbk_raw;
	Iv_fbk_raw	= (Iv_fbk_raw < MAX_CURRENT_NEG_AD) ? MAX_CURRENT_NEG_AD : Iv_fbk_raw;

	Iw_fbk_raw	= (Iw_fbk_raw > MAX_CURRENT_POS_AD) ? MAX_CURRENT_POS_AD : Iw_fbk_raw;
	Iw_fbk_raw	= (Iw_fbk_raw < MAX_CURRENT_NEG_AD) ? MAX_CURRENT_NEG_AD : Iw_fbk_raw;

	sysFbkPara.Iv_fbk_real = _IQmpyI32(EV_MCU_Para.field.Iac_adjustCoeff,(long)Iv_fbk_raw);
	sysFbkPara.Iw_fbk_real = _IQmpyI32(EV_MCU_Para.field.Iac_adjustCoeff,(long)Iw_fbk_raw);
	sysFbkPara.Iu_fbk_real = 0-(sysFbkPara.Iv_fbk_real+sysFbkPara.Iw_fbk_real);

	sysFbkPara.Iv_fbk = sysFbkPara.Iv_fbk_real;
	sysFbkPara.Iw_fbk = sysFbkPara.Iw_fbk_real;
	sysFbkPara.Iu_fbk = 0-(sysFbkPara.Iv_fbk+sysFbkPara.Iw_fbk);

	Is_alpha = sysFbkPara.Iu_fbk;
	Is_beta  = _IQmpy((sysFbkPara.Iu_fbk + (sysFbkPara.Iv_fbk<<1)),INVSQRT3);

#if 1
	//以下是角度补偿-补偿时间为24.6us
	if(SYS_DIAGNOSIS != scsw1.field.system_state)
	{
		time = _IQ(0.0246);
		frequency = _IQmpy(sysCfgPara.Omega_ele,SysBase.omega);
		if(MOTOR_CLOCKWISE == EV_MCU_Para.field.Motor_Direction)
		{
			frequency = 0-frequency;
		}
		deta = _IQmpyI32(frequency,time)/1000;
		theta = sysCfgPara.Theta_ele+deta;
		while((theta<0)||(theta>=_IQ(1)))
		{
			if(theta<0)
			{
				theta = theta+_IQ(1);
			}
			else if(theta>=_IQ(1))
			{
				theta = theta-_IQ(1);
			}
		}
	}
#endif
	
	sin_theta = _IQsinPU(theta);
	cos_theta = _IQcosPU(theta);

#if 0
	sysFbkPara.Id_fbk = _IQmpy(Is_alpha,cos_theta) + _IQmpy(Is_beta,sin_theta);
    sysFbkPara.Iq_fbk = _IQmpy(Is_beta,cos_theta) - _IQmpy(Is_alpha,sin_theta);
	LPFilter(sysFbkPara.Id_fbk,&sysFbkPara.Id_fbk_filter,_IQ(0.1),_IQ(0.9));
	LPFilter(sysFbkPara.Iq_fbk,&sysFbkPara.Iq_fbk_filter,_IQ(0.1),_IQ(0.9));
	LPFilter(sysFbkPara.Id_fbk,&sysFbkPara.Id_fbk_filter_0025,_IQ(0.0025),_IQ(0.9975));
	LPFilter(sysFbkPara.Iq_fbk,&sysFbkPara.Iq_fbk_filter_0025,_IQ(0.0025),_IQ(0.9975));
#else
    
    id_temp = _IQmpy(Is_alpha,cos_theta) + _IQmpy(Is_beta,sin_theta);
    iq_temp = _IQmpy(Is_beta,cos_theta) - _IQmpy(Is_alpha,sin_theta);
	LPFilter(id_temp,&sysFbkPara.Id_fbk,_IQ(0.9),_IQ(0.1));
	LPFilter(iq_temp,&sysFbkPara.Iq_fbk,_IQ(0.9),_IQ(0.1));
	LPFilter(id_temp,&sysFbkPara.Id_fbk_filter,_IQ(0.1),_IQ(0.9));
	LPFilter(iq_temp,&sysFbkPara.Iq_fbk_filter,_IQ(0.1),_IQ(0.9));
	LPFilter(id_temp,&sysFbkPara.Id_fbk_filter_0025,_IQ(0.0025),_IQ(0.9975));
	LPFilter(iq_temp,&sysFbkPara.Iq_fbk_filter_0025,_IQ(0.0025),_IQ(0.9975));
#endif
#if 0
	LPFilter(sysFbkPara.Id_fbk,&sysFbkPara.Id_fbk_filter,_IQ(0.1),_IQ(0.9));
	LPFilter(sysFbkPara.Iq_fbk,&sysFbkPara.Iq_fbk_filter,_IQ(0.1),_IQ(0.9));
	
	LPFilter(sysFbkPara.Id_fbk,&sysFbkPara.Id_fbk_filter_0025,_IQ(0.0025),_IQ(0.9975));
	LPFilter(sysFbkPara.Iq_fbk,&sysFbkPara.Iq_fbk_filter_0025,_IQ(0.0025),_IQ(0.9975));
#endif

	if(LimitI2CUpdate_en)   //写入eeprom的数据
	{
		Temp  = abs(_IQmpy(sysFbkPara.Iu_fbk_real,SysBase.iphase));
		if(I2CValueLimit_Record.field.iu < Temp)
		{
			I2CValueLimit_Record.field.iu = Temp;
			eeprom_valuelimit_index[0]	 = 1;//U phase current
			LimitI2CRecord_en			 = 1;
		}
		Temp  = abs(_IQmpy(sysFbkPara.Iv_fbk_real,SysBase.iphase));
		if(I2CValueLimit_Record.field.iv < Temp)
		{
			I2CValueLimit_Record.field.iv = Temp;
			eeprom_valuelimit_index[1]	 = 1;//V phase current
			LimitI2CRecord_en			 = 1;
		}
		Temp  = abs(_IQmpy(sysFbkPara.Iw_fbk_real,SysBase.iphase));
		if(I2CValueLimit_Record.field.iw < Temp)
		{
			I2CValueLimit_Record.field.iw = Temp;
			eeprom_valuelimit_index[2]	 = 1;//W phase current
			LimitI2CRecord_en			 = 1;
		}
	}

	Temp = sysFbkPara.Vdc_fbk_ad;//-EV_MCU_Para.field.Vdc_offset+2048;
	Temp = (Temp>MAX_VDC_AD)?MAX_VDC_AD: Temp;
	Temp = (Temp<0)?0: Temp;
	sysFbkPara.Vdc_fbk_ad_filter = (Temp*64+sysFbkPara.Vdc_fbk_ad_filter*64)>>7;
	sysFbkPara.Vdc_unfilter = _IQmpyI32(EV_MCU_Para.field.Vdc_adjustCoeff,sysFbkPara.Vdc_fbk_ad_filter);//6990
	LPFilter(sysFbkPara.Vdc_unfilter,&sysFbkPara.Vdc_filter_protect,_IQ(0.1),_IQ(0.9));
	LPFilter(sysFbkPara.Vdc_unfilter,&sysFbkPara.Vdc_filter,_IQ(0.1),_IQ(0.9));
	if(0 == scsw2.field.igbt_state)
	{
		sysFbkPara.Vdc_fbk = sysFbkPara.Vdc_filter;
		sysFbkPara.Vdc_count = 0;
		sysFbkPara.Vdc_min = sysFbkPara.Vdc_filter;
	}
	else
	{
		if(sysFbkPara.Vdc_count==0)
		{
			sysFbkPara.Vdc_min = sysFbkPara.Vdc_filter;
		}

		if(sysFbkPara.Vdc_count>EV_MCU_Para.field.Vdc_sample_time)
		{
			sysFbkPara.Vdc_count = 0;
			sysFbkPara.Vdc_fbk = sysFbkPara.Vdc_min;
		}
		else
		{
			if(sysFbkPara.Vdc_filter<sysFbkPara.Vdc_min)
			{
				sysFbkPara.Vdc_min = sysFbkPara.Vdc_filter;
			}
			else
			{
				sysFbkPara.Vdc_min = sysFbkPara.Vdc_min;
			}
			sysFbkPara.Vdc_count++;
		}
    }    
	
	if(LimitI2CUpdate_en)
	{
		Temp = _IQmpy(sysFbkPara.Vdc_fbk,SysBase.udc);
		if(I2CValueLimit_Record.field.udc_up < Temp)
		{
			I2CValueLimit_Record.field.udc_up = Temp;
			eeprom_valuelimit_index[4]    = 1;
			LimitI2CRecord_en             = 1;
		}
	}
	if(1 == EV_MCU_Para.field.OpenPhaseCheck_En)
	{
		if(1 == scsw2.field.igbt_state)
		{
			if(sysFbkPara.Speed_fbk_HMI_abs>Spd_OpenPhaseMin  && sysFbkPara.Is_fbk_HMI_fiter>Crt_OpenPhaseMin)
			{
				if(Cnt_OpenPhase <= 300000)
				{
					Ia_checksum += abs(sysFbkPara.Iu_fbk_ad-2055);
		            Ib_checksum += abs(sysFbkPara.Iv_fbk_ad-2055);
		            Ic_checksum += abs(sysFbkPara.Iw_fbk_ad-2055);
		            Cnt_OpenPhase++;
				}
				else
				{
					Cnt_OpenPhase = 350000;
				}
			}
		}
		else
		{
			Ia_checksum   = 0;
            Ib_checksum   = 0;
            Ic_checksum   = 0;
            Cnt_OpenPhase = 0;
		}
	}
} 

//========================================
//功能:上电电流零漂检测
//========================================
void sub_ad_zero()
{
	
	EALLOW;

	AdcRegs.ADCCTL1.bit.INTPULSEPOS	= 1;	//ADCINT1 trips after AdcResults latch
	AdcRegs.INTSEL1N2.bit.INT2E     = 1;	//Enabled ADCINT1
	AdcRegs.INTSEL1N2.bit.INT2CONT  = 1;	//Disable ADCINT1 Continuous mode
	AdcRegs.INTSEL1N2.bit.INT2SEL	= 2;	//setup EOC2 to trigger ADCINT1 to fire

	AdcRegs.ADCSOC0CTL.bit.CHSEL 	= 5;	//set SOC0 channel select to ADCINA0 -- FRQ
	AdcRegs.ADCSOC1CTL.bit.CHSEL 	= 8;	//set SOC1 channel select to ADCINA1 -- IU
	AdcRegs.ADCSOC2CTL.bit.CHSEL 	= 9;	//set SOC2 channel select to ADCINA3 -- IW


	AdcRegs.ADCSOC0CTL.bit.TRIGSEL 	= 5;	//set SOC0 start trigger on software, due to round-robin SOC0 converts first then SOC1
	AdcRegs.ADCSOC1CTL.bit.TRIGSEL 	= 5;	//set SOC1 start trigger on software, due to round-robin SOC0 converts first then SOC1
	AdcRegs.ADCSOC2CTL.bit.TRIGSEL 	= 5;	//set SOC2 start trigger on software, due to round-robin SOC0 converts first then SOC1
	
	AdcRegs.ADCSOC0CTL.bit.ACQPS 	= 6;	//set SOC0 S/H Window to 7 ADC Clock Cycles, (6 ACQPS plus 1)
	AdcRegs.ADCSOC1CTL.bit.ACQPS 	= 6;	//set SOC1 S/H Window to 7 ADC Clock Cycles, (6 ACQPS plus 1)
	AdcRegs.ADCSOC2CTL.bit.ACQPS 	= 6;	//set SOC2 S/H Window to 7 ADC Clock Cycles, (6 ACQPS plus 1)
	
	EPwm1Regs.ETSEL.bit.SOCAEN	= 1;		// Enable SOC on A group
   	EPwm1Regs.ETSEL.bit.SOCASEL	= 1;		// Select SOC from ZERO on upcount
   	EPwm1Regs.ETPS.bit.SOCAPRD 	= 1;		// Generate pulse on 1st event	
	EDIS;
}

void sub_spi_init(void)
{
#if 1
	//EALLOW;
	//SpiaRegs.SPICCR.all =0x001F;	             // Reset on, rising edge, 16-bit char bits  
	//SpiaRegs.SPICTL.all =0x000E;    		     // Enable master mode, normal phase,
                                                 // enable talk, and SPI int disabled.
	//SpiaRegs.SPIBRR =0x0004;								//sysclock/4/(B+1);	
    //SpiaRegs.SPICCR.all =0x009F;		         // Relinquish SPI from Reset   
    //SpiaRegs.SPIPRI.bit.FREE = 1;                // Set so breakpoints don't disturb xmission

	SpiaRegs.SPICCR.all =0x001F;	             // Reset on, rising edge, 16-bit char bits  ,rise edge in,trip edge out
	SpiaRegs.SPICTL.all =0x000E;    		     // Enable master mode, normal phase,disable intr,SPICLK延时半个周期
                                                 // enable talk, and SPI int disabled.
	SpiaRegs.SPIBRR = 0x0009;								//LSCPLK(25M)/(B+1);
    SpiaRegs.SPICCR.all =0x008F;		         // Relinquish SPI from Reset  
    SpiaRegs.SPIPRI.bit.FREE = 1;                // Set so breakpoints don't disturb xmission

    
    //Initialize SPI FIFO registers
    //SpiaRegs.SPIFFTX.all=0xE040;
    //SpiaRegs.SPIFFRX.all=0x204f;
    //SpiaRegs.SPIFFCT.all=0x01;
    //EDIS;
#else
	SpiaRegs.SPICCR.bit.SPISWRESET=0; //Reset SPI
	SpiaRegs.SPICCR.all=0x004F;       // 16-bit character, Loopback mode disabled
	SpiaRegs.SPICTL.all=0x0004;       // SPI Interrupt disabled, Master mode, Master/slave transmit enabled
	SpiaRegs.SPIBRR=0x0003;           // 0.9615M, SPI Baud rate=LSPCLK/(SPIBRR+1), here, LSPCLK=37.5M, SPIBRR=38
	SpiaRegs.SPIPRI.all=0x0010;
	SpiaRegs.SPICCR.bit.SPISWRESET=1; //Enable SPI
#endif
}

//===================================
// Configure ADC
//===================================
void	sub_ad_init(void)
{
	EALLOW;
	//AdcRegs.ADCCTL1.bit.INTPULSEPOS	= 1;	//ADCINT1 trips after AdcResults latch
	//AdcRegs.INTSEL1N2.bit.INT1E     = 1;	//Enabled ADCINT1
	//AdcRegs.INTSEL1N2.bit.INT1CONT  = 0;	//Disable ADCINT1 Continuous mode
	//AdcRegs.INTSEL1N2.bit.INT1SEL	= 6;	//setup EOC9 to trigger ADCINT1 to fire

	///EOC6转换完成产生ADCINT11中断
	AdcRegs.ADCSOC0CTL.bit.CHSEL 	= 8;	//set SOC0 channel select to  -- IU
	AdcRegs.ADCSOC1CTL.bit.CHSEL 	= 9;	//set SOC1 channel select to  -- IW
	AdcRegs.ADCSOC2CTL.bit.CHSEL 	= 10;	//set SOC6 channel select to  -- IV
	AdcRegs.ADCSOC3CTL.bit.CHSEL 	= 1;	//set SOC7 channel select to  -- TEMP_AD
	AdcRegs.ADCSOC4CTL.bit.CHSEL 	= 11;	//set SOC7 channel select to  -- T_MOS_AD
	AdcRegs.ADCSOC5CTL.bit.CHSEL 	= 7;	//set SOC6 channel select to  -- VDC
	AdcRegs.ADCSOC6CTL.bit.CHSEL 	= 5;	//set SOC5 channel select to  -- AI1_AD
	AdcRegs.ADCSOC7CTL.bit.CHSEL 	= 3;	//set SOC7 channel select to  -- AI3_AD
	AdcRegs.ADCSOC8CTL.bit.CHSEL 	= 13;	//set SOC7 channel select to  -- V12_AD
	//AdcRegs.ADCSOC8CTL.bit.CHSEL 	= 2;	//set SOC5 channel select to  -- AI2_AD
	//AdcRegs.ADCSOC9CTL.bit.CHSEL 	= 4;	//set SOC5 channel select to  -- AI4_AD
	//AdcRegs.ADCSOC10CTL.bit.CHSEL 	= 6;//set SOC5 channel select to  -- TA_AD

	AdcRegs.ADCSOC0CTL.bit.TRIGSEL 	= 5;	//set SOC0 start trigger on software, due to round-robin SOC0 converts first then SOC1
	AdcRegs.ADCSOC1CTL.bit.TRIGSEL 	= 5;	//set SOC1 start trigger on software, due to round-robin SOC0 converts first then SOC1
	AdcRegs.ADCSOC2CTL.bit.TRIGSEL 	= 5;	//set SOC2 start trigger on software, due to round-robin SOC0 converts first then SOC1
	AdcRegs.ADCSOC3CTL.bit.TRIGSEL 	= 5;	//set SOC3 start trigger on software, due to round-robin SOC0 converts first then SOC1
	AdcRegs.ADCSOC4CTL.bit.TRIGSEL 	= 5;	//set SOC4 start trigger on software, due to round-robin SOC0 converts first then SOC1
	AdcRegs.ADCSOC5CTL.bit.TRIGSEL 	= 5;	//set SOC5 start trigger on software, due to round-robin SOC0 converts first then SOC1
	AdcRegs.ADCSOC6CTL.bit.TRIGSEL 	= 5;	//set SOC6 start trigger on software, due to round-robin SOC0 converts first then SOC1
	AdcRegs.ADCSOC7CTL.bit.TRIGSEL 	= 5;	//set SOC6 start trigger on software, due to round-robin SOC0 converts first then SOC1
	AdcRegs.ADCSOC8CTL.bit.TRIGSEL 	= 5;	//set SOC6 start trigger on software, due to round-robin SOC0 converts first then SOC1
	//AdcRegs.ADCSOC9CTL.bit.TRIGSEL 	= 5;	//set SOC6 start trigger on software, due to round-robin SOC0 converts first then SOC1
	//AdcRegs.ADCSOC10CTL.bit.TRIGSEL = 5;	//set SOC6 start trigger on software, due to round-robin SOC0 converts first then SOC1
	//AdcRegs.ADCSOC10CTL.bit.TRIGSEL = 5;
	AdcRegs.ADCSOC0CTL.bit.ACQPS 	= 6;	//set SOC0 S/H Window to 7 ADC Clock Cycles, (6 ACQPS plus 1)
	AdcRegs.ADCSOC1CTL.bit.ACQPS 	= 6;	//set SOC1 S/H Window to 7 ADC Clock Cycles, (6 ACQPS plus 1)
	AdcRegs.ADCSOC2CTL.bit.ACQPS 	= 6;	//set SOC2 S/H Window to 7 ADC Clock Cycles, (6 ACQPS plus 1)
	AdcRegs.ADCSOC3CTL.bit.ACQPS 	= 6;	//set SOC3 S/H Window to 7 ADC Clock Cycles, (6 ACQPS plus 1)
	AdcRegs.ADCSOC4CTL.bit.ACQPS 	= 6;	//set SOC4 S/H Window to 7 ADC Clock Cycles, (6 ACQPS plus 1)
	AdcRegs.ADCSOC5CTL.bit.ACQPS 	= 6;	//set SOC5 S/H Window to 7 ADC Clock Cycles, (6 ACQPS plus 1)
	AdcRegs.ADCSOC6CTL.bit.ACQPS 	= 6;	//set SOC6 S/H Window to 7 ADC Clock Cycles, (6 ACQPS plus 1)
	AdcRegs.ADCSOC7CTL.bit.ACQPS 	= 6;	//set SOC6 S/H Window to 7 ADC Clock Cycles, (6 ACQPS plus 1)
	AdcRegs.ADCSOC8CTL.bit.ACQPS 	= 6;	//set SOC6 S/H Window to 7 ADC Clock Cycles, (6 ACQPS plus 1)
	//AdcRegs.ADCSOC9CTL.bit.ACQPS 	= 6;	//set SOC6 S/H Window to 7 ADC Clock Cycles, (6 ACQPS plus 1)
	//AdcRegs.ADCSOC10CTL.bit.ACQPS = 6;	//set SOC6 S/H Window to 7 ADC Clock Cycles, (6 ACQPS plus 1)
	//AdcRegs.ADCSOC10CTL.bit.ACQPS = 6;
	AdcRegs.ADCCTL2.bit.ADCNONOVERLAP = 1;  // 
	AdcRegs.ADCCTL2.bit.CLKDIV2EN = 1;  	//
	
	//AdcRegs.ADCCTL1.bit.ADCREFSEL = 0;		// Select external BG
	
	//EPwm1Regs.ETSEL.bit.SOCAEN	= 1;		// Enable SOC on A group
   	//EPwm1Regs.ETSEL.bit.SOCASEL	= 1;		// Select SOC from  PRD
   	//EPwm1Regs.ETPS.bit.SOCAPRD 	= 1;		// Generate pulse on 1st event

#if 1
   	EPwm1Regs.ETSEL.bit.SOCAEN = 1;        // Enable SOC on A group
    EPwm1Regs.ETSEL.bit.SOCBEN = 1;        // Enable SOC on A group
	EPwm1Regs.ETSEL.bit.SOCASEL = 1;       // Select SOC from ZERO on upcount
	EPwm1Regs.ETSEL.bit.SOCBSEL = 2;       // Select SOC from ZERO on upcount
	EPwm1Regs.ETPS.bit.SOCAPRD = 1;        // Generate pulse on 1st event   
	EPwm1Regs.ETPS.bit.SOCBPRD = 1;        // Generate pulse on 1st event 
#endif
   
	
	EDIS;
}


interrupt void  ADCINT1_ISR(void)     // ADC                                     
{    
	PieCtrlRegs.PIEACK.all |= PIEACK_GROUP1; 
} 

#endif

//===========================================================================	
// No more.                                                                  
//===========================================================================



