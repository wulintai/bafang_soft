//########################################################################################
//
//
// File Name:		variableInit.c
//
// Description:
//
//########################################################################################
#ifndef _VARIABLEINIT_C
#define _VARIABLEINIT_C

#include "include_c.h"

void GlobalVar_Init(void)
{
	Uint16 i = 0;

    SysCtrlVar_Init();
	SysCtrlBase_Init();
	SysCtrltrqtable_Init();
	for(i=0;i<5000;i++)
	{
		asm(" nop");
	}
	eepromPara_Recall(EEPROM_START_ADDR);
	if((EEPROM_PASSWORD == epdw.field.Customer_ID)
	 &&(EEPROM_PASSWORD1 == epdw.field.EEPROM_version)
	 &&(EEPROM_PASSWORD2 == epdw.field.EEPROM_version_date))
	{
		Eeprom_ParaInt();
	    Default_ExParaInt();
	}
	else
	{
		emsw.field.eeprom_fault	= 1;
		epdw.field.Customer_ID 			= EEPROM_PASSWORD;
		epdw.field.EEPROM_version 		= EEPROM_PASSWORD1;
		epdw.field.EEPROM_version_date  = EEPROM_PASSWORD2;
		Default_ParaInt();
		Default_ExParaInt();
		eepromPara_Wricall();
		eepromPara_Recall(EEPROM_START_ADDR);
		I2C_FaultDateClear();
		I2C_LimitDateClear();
	}
#if 0
	do
	{
        while (I2cReadData(EEPROM_PASSWORD_ADDR,&epdw.value[EEPROM_PASSWORD_ADDR])) { }
        for(i=0;i<1000;i++) {}
        while (I2cReadData(EEPROM_PASSWORD_ADDR1,&epdw.value[EEPROM_PASSWORD_ADDR1])) { }
        for(i=0;i<1000;i++) {}
        while (I2cReadData(EEPROM_PASSWORD_ADDR2,&epdw.value[EEPROM_PASSWORD_ADDR2])) { }
        for(i=0;i<1000;i++) {}
	    eeprom_default_cnt++;
	    if((epdw.field.Customer_ID != EEPROM_PASSWORD) || (epdw.field.EEPROM_version != EEPROM_PASSWORD1) ||(epdw.field.EEPROM_version_date != EEPROM_PASSWORD2) )
	    eeprom_default++;
    }
	while(eeprom_default_cnt<3);

    if(eeprom_default>=2)
    {
    	emsw.field.eeprom_fault       	= 1;
    	epdw.field.Customer_ID 			= EEPROM_PASSWORD;
    	epdw.field.EEPROM_version 		= EEPROM_PASSWORD1;
    	epdw.field.EEPROM_version_date  = EEPROM_PASSWORD2;

     	while (I2cWriteData(EEPROM_PASSWORD_ADDR,&epdw.value[EEPROM_PASSWORD_ADDR])){}
     	for(i=0;i<40000;i++) {}
     	while (I2cWriteData(EEPROM_PASSWORD_ADDR1,&epdw.value[EEPROM_PASSWORD_ADDR1])){}
    	for(i=0;i<40000;i++) {}
     	while (I2cWriteData(EEPROM_PASSWORD_ADDR2,&epdw.value[EEPROM_PASSWORD_ADDR2])){}
    	for(i=0;i<40000;i++) {}

    	Default_ParaInt();
    	Default_ExParaInt();
    	eepromPara_Wricall();
    	eepromPara_Recall();
		I2C_FaultDateClear();
		I2C_LimitDateClear();
    }
    else
    {
	    eepromPara_Recall();
	    Eeprom_ParaInt();
	    Default_ExParaInt();
    }
#endif

    //故障信息回调
    I2C_FaultRecordInt();
    //极限信息回调
    I2C_ValueLimit_Int();

    //注意：一定要EEPROM值回调后面
    //控制器过载报警初始化
    InvOverLoadProtectInt();
    //电机过载初始化
    MtrOverLoadProtectInt();
    //缺相保护初始化
    //MtrOpenPhaseProtectInt();
    //三相不平衡初始化
    //PhaseUnbalanceProtectint();
    //速度检测故障初始化
    SpeedDetectProtectint();
    //电流检测故障初始化
    CrtDetectProtectint();

    //控制器环路初始
    ClearCtrlLoopPara();
    ClearManualCtrlCmd();

    SysCfgParaInt();
}
//========================================================================================
// EEPROM~{2NJ}6AH!~}
//========================================================================================
void eepromPara_Recall(unsigned int sector_addr)
{
	Uint16 i = 0;
	Uint16 j = 0;

	for(i=sector_addr;i<sector_addr+EEPROM_SUM_LEN;i++)
	{
		while(I2cReadData(i,&epdw.value[i-sector_addr])) {}
		for(j=0;j<EEPROM_READ_DELAY_CNT;j++) {}
	}
	
#if 0
	for(i=EEPROM_START_ADDR;i<=EEPROM_END_ADDR;i++)
	{
		while(I2cReadData(i,&epdw.value[i])) {}
		for(j=0;j<EEPROM_READ_DELAY_CNT;j++) {}
	}
	for(i=Motor_Basic_Para_ADDR_START;i<=Motor_Basic_Para_ADDR_END;i++)      //1
	{
		while(I2cReadData(i,&epdw.value[i])) {}
		for(j=0;j<1000;j++) {}
	}
	for(i=Motor_Charact_Para_ADDR_START;i<=Motor_Charact_Para_ADDR_END;i++)           //2
	{
		while(I2cReadData(i,&epdw.value[i])) {}
		for(j=0;j<1000;j++) {}
	}
	for(i=Motor_Calibrat_Para_ADDR_START;i<=Motor_Calibrat_Para_ADDR_END;i++)             //3
	{
	    while(I2cReadData(i,&epdw.value[i])) {}
		for(j=0;j<1000;j++) {}
	}
	for(i=VoltFeedfwd_Para_ADDR_START;i<=VoltFeedfwd_Para_ADDR_END;i++)             //4
	{
		while(I2cReadData(i,&epdw.value[i])) {}
		for(j=0;j<1000;j++) {}
	}
	for(i=AD_Para_ADDR_START;i<=AD_Para_ADDR_END;i++)           //5
	{
		while(I2cReadData(i,&epdw.value[i])) {}
		for(j=0;j<1000;j++) {}
	}
	for(i=PI_Para_ADDR_START;i<=PI_Para_ADDR_END;i++)            //6
	{
		while(I2cReadData(i,&epdw.value[i])) {}
		for(j=0;j<1000;j++) {}
	}

	for(i=UDCLOOP_Para_ADDR_START;i<=UDCLOOP_Para_ADDR_END;i++)         //6
	{
		while(I2cReadData(i,&epdw.value[i])){}
		for(j=0;j<1000;j++) {}
	}

	for(i=CMD_Para_ADDR_START;i<=CMD_Para_ADDR_END;i++)            //7
	{
		while(I2cReadData(i,&epdw.value[i])) {}
		for(j=0;j<1000;j++) {}
	}
	for(i=Weak_Para_ADDR_START;i<=Weak_Para_ADDR_END;i++)      //8
	{
		while(I2cReadData(i,&epdw.value[i])) {}
		for(j=0;j<1000;j++) {}
	}
	for(i=EliJitter_Para_ADDR_START;i<=EliJitter_Para_ADDR_END;i++)        //9
	{
		while(I2cReadData(i,&epdw.value[i])) {}
		for(j=0;j<1000;j++) {}
	}
	for(i=TBDVALUE_Para_ADDR_START;i<=TBDVALUE_Para_ADDR_END;i++)        //9
	{
		while(I2cReadData(i,&epdw.value[i])) {}
		for(j=0;j<1000;j++) {}
	}
	for(i=Zero_Sation_Para_ADDR_START;i<=Zero_Sation_Para_ADDR_END;i++)             //10
	{
		while(I2cReadData(i,&epdw.value[i])) {}
		for(j=0;j<1000;j++) {}
	}
	for(i=Spd_Over_Limt_Para_ADDR_START;i<=Spd_Over_Limt_Para_ADDR_END;i++)           //11
	{
		while(I2cReadData(i,&epdw.value[i])) {}
		for(j=0;j<1000;j++) {}
	}
	for(i=Motor_study_Para_ADDR_START;i<=Motor_study_Para_ADDR_END;i++)            //12
	{
		while(I2cReadData(i,&epdw.value[i])) {}
		for(j=0;j<1000;j++) {}
	}
	for(i=Motor_Protect_Alarm_Para_ADDR_START;i<=Motor_Protect_Alarm_Para_ADDR_END;i++)            //13
	{
		while(I2cReadData(i,&epdw.value[i])) {}
		for(j=0;j<1000;j++) {}
	}
	for(i=Motor_Protect_Para_ADDR_START;i<=Motor_Protect_Para_ADDR_END;i++)      //14
	{
		while(I2cReadData(i,&epdw.value[i])) {}
		for(j=0;j<1000;j++) {}
	}
	for(i=Ctr_Para_ADDR_START;i<=Ctr_Para_ADDR_END;i++)        //15
	{
		while(I2cReadData(i,&epdw.value[i])) {}
		for(j=0;j<1000;j++) {}
	}
	for(i=IGBT_Short_Para_ADDR_START;i<=IGBT_Short_Para_ADDR_END;i++)        //15
	{
		while(I2cReadData(i,&epdw.value[i])) {}
		for(j=0;j<1000;j++) {}
	}
	for(i=Tooth_TRQ_Para_ADDR_START;i<=Tooth_TRQ_Para_ADDR_END;i++)        //15
	{
		while(I2cReadData(i,&epdw.value[i])) {}
		for(j=0;j<1000;j++) {}
	}
#endif
}

void I2Cfault_Recall(void)
{
	Uint16 i = 0;
	Uint16 j = 0;
	
	for(i=I2C_FaultStart;i<=I2C_FaultDataEnd;i++)   //~{9JUO5DH+2?6AH!~}
	{
		while(I2cReadData(i,&I2CErr_Record.value[i-I2C_FaultStart])) {}
		for(j=0;j<EEPROM_READ_DELAY_CNT;j++) {}
	}
}

void eepromPara_WricallAddr(unsigned int addr)
{
	Uint16  i = 0;
	Uint16  j = 0;

	for(i=addr;i<addr+EEPROM_SUM_LEN;i++)  //1
	{
		while(I2cWriteData(addr,&epdw.value[i-addr])){}
		for(j=0;j<EEPROM_WRITE_DELAY_CNT;j++) {}
	}
}

//========================================================================================
// ~{;V84D,HO2NJ}~}
//========================================================================================
void eepromPara_Wricall(void)
{
	Uint16  i = 0;
	Uint16  j = 0;

	for(i=EEPROM_START_ADDR;i<=EEPROM_END_ADDR;i++)  //1
	{
		while(I2cWriteData(i,&epdw.value[i])){}
		for(j=0;j<EEPROM_WRITE_DELAY_CNT;j++) {}
	}
#if 0
	for(i=Motor_Basic_Para_ADDR_START;i<=Motor_Basic_Para_ADDR_END;i++)  //1
	{
		while(I2cWriteData(i,&epdw.value[i])){}
		for(j=0;j<40000;j++) {}
	}

	for(i=Motor_Charact_Para_ADDR_START;i<=Motor_Charact_Para_ADDR_END;i++)       //2
	{
		while(I2cWriteData(i,&epdw.value[i])){}
		for(j=0;j<40000;j++) {}
	}
	for(i=Motor_Calibrat_Para_ADDR_START;i<=Motor_Calibrat_Para_ADDR_END;i++)          //3
	{
		while(I2cWriteData(i,&epdw.value[i])) {}
		for(j=0;j<40000;j++) {}
	}
	for(i=VoltFeedfwd_Para_ADDR_START;i<=VoltFeedfwd_Para_ADDR_END;i++)           //4
	{
		while(I2cWriteData(i,&epdw.value[i])) {}
		for(j=0;j<40000;j++) {}
	}
	for(i=AD_Para_ADDR_START;i<=AD_Para_ADDR_END;i++)         //5
	{
		while(I2cWriteData(i,&epdw.value[i])){}
		for(j=0;j<40000;j++) {}
	}
	for(i=PI_Para_ADDR_START;i<=PI_Para_ADDR_END;i++)         //6
	{
		while(I2cWriteData(i,&epdw.value[i])){}
		for(j=0;j<40000;j++) {}
	}

	for(i=UDCLOOP_Para_ADDR_START;i<=UDCLOOP_Para_ADDR_END;i++)         //6
	{
		while(I2cWriteData(i,&epdw.value[i])){}
		for(j=0;j<40000;j++) {}
	}

	for(i=CMD_Para_ADDR_START;i<=CMD_Para_ADDR_END;i++)         //7
	{
		while(I2cWriteData(i,&epdw.value[i])){}
		for(j=0;j<40000;j++) {}
	}
	for(i=Weak_Para_ADDR_START;i<=Weak_Para_ADDR_END;i++)    //8
	{
		while(I2cWriteData(i,&epdw.value[i])){}
		for(j=0;j<40000;j++) {}
	}
	for(i=EliJitter_Para_ADDR_START;i<=EliJitter_Para_ADDR_END;i++)       //9
	{
		while(I2cWriteData(i,&epdw.value[i])){}
		for(j=0;j<40000;j++) {}
	}
	for(i=TBDVALUE_Para_ADDR_START;i<=TBDVALUE_Para_ADDR_END;i++)       //9
	{
		while(I2cWriteData(i,&epdw.value[i])){}
		for(j=0;j<40000;j++) {}
	}
	for(i=Zero_Sation_Para_ADDR_START;i<=Zero_Sation_Para_ADDR_END;i++)         //10
	{
		while(I2cWriteData(i,&epdw.value[i])){}
		for(j=0;j<40000;j++) {}
	}
	for(i=Spd_Over_Limt_Para_ADDR_START;i<=Spd_Over_Limt_Para_ADDR_END;i++)         //11
	{
		while(I2cWriteData(i,&epdw.value[i])){}
		for(j=0;j<40000;j++) {}
	}
	for(i=Motor_study_Para_ADDR_START;i<=Motor_study_Para_ADDR_END;i++)    //12
	{
		while(I2cWriteData(i,&epdw.value[i])){}
		for(j=0;j<40000;j++) {}
	}
	for(i=Motor_Protect_Alarm_Para_ADDR_START;i<=Motor_Protect_Alarm_Para_ADDR_END;i++)       //13
	{
		while(I2cWriteData(i,&epdw.value[i])){}
		for(j=0;j<40000;j++) {}
	}
	for(i=Motor_Protect_Para_ADDR_START;i<=Motor_Protect_Para_ADDR_END;i++)    //14
	{
		while(I2cWriteData(i,&epdw.value[i])){}
		for(j=0;j<40000;j++) {}
	}
	for(i=Ctr_Para_ADDR_START;i<=Ctr_Para_ADDR_END;i++)       //15
	{
		while(I2cWriteData(i,&epdw.value[i])){}
		for(j=0;j<40000;j++) {}
	}
	for(i=IGBT_Short_Para_ADDR_START;i<=IGBT_Short_Para_ADDR_END;i++)       //16
	{
		while(I2cWriteData(i,&epdw.value[i])){}
		for(j=0;j<40000;j++) {}
	}
	for(i=IGBT_Short_Para_ADDR_START;i<=IGBT_Short_Para_ADDR_END;i++)       //16
	{
		while(I2cWriteData(i,&epdw.value[i])){}
		for(j=0;j<40000;j++) {}
	}
	for(i=Tooth_TRQ_Para_ADDR_START;i<=Tooth_TRQ_Para_ADDR_END;i++)       //16
	{
		while(I2cWriteData(i,&epdw.value[i])){}
		for(j=0;j<40000;j++) {}
	}
#endif
}

//========================================
void Eeprom_ParaInt(void)
{
	int i = 0;
	
	for(i=0;i<EEPROM_SUM_LEN;i++)
	{
		EV_MCU_Para.value[i] = epdw.value[i];
	}
}

void Default_ParaInt(void)
{
	//=====================电机基本参数（0-9）=====================
	EV_MCU_Para.field.Motor_Np			               = epdw.field.Motor_Np	                    = 3;// 电机极对数
	EV_MCU_Para.field.Encoder_Np                       = epdw.field.Encoder_Np	                    = 3;//编码器极对数
	EV_MCU_Para.field.Motor_Direction	               = epdw.field.Motor_Direction                 = 1;//电机旋转方向
	EV_MCU_Para.field.Motor_Encoder_Direction          = epdw.field.Motor_Encoder_Direction 	    = 0;//编码器旋转方向
	EV_MCU_Para.field.Motor_Encoder_offset		       = epdw.field.Motor_Encoder_offset    	    = 2042;//编码器零偏
	EV_MCU_Para.field.Motor_Encoder_offset_elecoeff    = epdw.field.Motor_Encoder_offset_elecoeff   = 2048;//编码器零偏 电动系数
	EV_MCU_Para.field.Motor_Encoder_offset_gencoeff    = epdw.field.Motor_Encoder_offset_gencoeff   = 2048;//编码器零偏 发电系数
	EV_MCU_Para.field.Motor_TempSensor_type		       = epdw.field.Motor_TempSensor_type           = 1;//电机温度传感器类型
	EV_MCU_Para.field.Motor_McuTempSensor_type         = epdw.field.Motor_McuTempSensor_type        = 0;//MCU温度传感器类型
	EV_MCU_Para.field.Motor_Encoder_rev_offset		   = epdw.field.Motor_Encoder_rev_offset    	= 2042;//编码器 反转 零偏
	//=====================电机特性参数（10-19）====================
	EV_MCU_Para.field.Motor_RatePower                  = epdw.field.Motor_RatePower                 = 15;//电机额定功率
	EV_MCU_Para.field.Motor_PeakPowerEle               = epdw.field.Motor_PeakPowerEle              = 30;//电动峰值功率
	EV_MCU_Para.field.Motor_PeakPowerGen               = epdw.field.Motor_PeakPowerGen              = 30;//发电峰值功率
	EV_MCU_Para.field.Motor_RateTorque                 = epdw.field.Motor_RateTorque                =_IQ15div(36,SysBase.torque);//额定扭矩
	EV_MCU_Para.field.Motor_PeakTorqueEle			   = epdw.field.Motor_PeakTorqueEle             =_IQ15div(150,SysBase.torque);//500(715)电动峰值扭矩
	EV_MCU_Para.field.Motor_PeakTorqueGen			   = epdw.field.Motor_PeakTorqueGen             =_IQ15div(150,SysBase.torque);//500(715)发电峰值扭矩
	EV_MCU_Para.field.Motor_RateSpeed                  = epdw.field.Motor_RateSpeed                 =_IQ15div(4000,SysBase.speed);//电机额定转速
	EV_MCU_Para.field.Motor_PeakSpeedFwd			   = epdw.field.Motor_PeakSpeedFwd              =_IQ15div(6000,SysBase.speed);//电机正转峰值转速
	EV_MCU_Para.field.Motor_PeakSpeedRev			   = epdw.field.Motor_PeakSpeedRev              =_IQ15div(6000,SysBase.speed);//电机反转峰值转速
	//=====================~{5g;z1j6(2NJ}#(~}20-29~{#)~}====================
	EV_MCU_Para.field.Motor_UDCMAP                     = epdw.field.Motor_UDCMAP                    = _IQ15div(67,SysBase.udc);//标定母线电压
	EV_MCU_Para.field.Motor_Trqidqmap_IdcoeffEle	   = epdw.field.Motor_Trqidqmap_IdcoeffEle      = 4096;//修正系数
	EV_MCU_Para.field.Motor_Trqidqmap_IqcoeffEle	   = epdw.field.Motor_Trqidqmap_IqcoeffEle      = 4096;
	EV_MCU_Para.field.Motor_Trqidqmap_IdcoeffGen	   = epdw.field.Motor_Trqidqmap_IdcoeffGen      = 4096;
	EV_MCU_Para.field.Motor_Trqidqmap_IqcoeffGen	   = epdw.field.Motor_Trqidqmap_IqcoeffGen      = 4096;
	EV_MCU_Para.field.Motor_IdLimt_enable	           = epdw.field.Motor_IdLimt_enable             = 0;//id电流限制使能
	EV_MCU_Para.field.Motor_Trqcoeff_enable            = epdw.field.Motor_Trqcoeff_enable           = 0;//力矩修正使能
	EV_MCU_Para.field.Motor_Idc_Select                 = epdw.field.Motor_Idc_Select                = 0;//母线电流选择
	//=====================电压前馈（30-39）=======================
	EV_MCU_Para.field.VoltFeedfwd_Coeff                = epdw.field.VoltFeedfwd_Coeff               = 7093; //电压前馈系数
	EV_MCU_Para.field.Rs_Stator                        = epdw.field.Rs_Stator                       = _IQ15(0.004045);   //电机定子电阻       //*50000
	EV_MCU_Para.field.Ld_Stator                        = epdw.field.Ld_Stator                       = _IQ15(0.000078);    //d轴电感         //*50000
	EV_MCU_Para.field.Flux_Rotor                       = epdw.field.Flux_Rotor                      = _IQ15(0.0431432);   //转子磁链  （磁钢强度）    //Q15
	EV_MCU_Para.field.Lq_Stator                        = epdw.field.Lq_Stator                       = _IQ15(0.00022);      //q轴电感     //*50000
	EV_MCU_Para.field.Lq_SatStator                     = epdw.field.Lq_SatStator                    = _IQ15(0.000096);     //q轴静态电感    //Q15
	EV_MCU_Para.field.iq_Lq_Stator                     = epdw.field.iq_Lq_Stator                    = _IQ15div(100,SysBase.iphase);  //没用
	EV_MCU_Para.field.iq_Lq_SatStator                  = epdw.field.iq_Lq_SatStator                 = _IQ15div(300,SysBase.iphase);  //没用  //IQ15


	//=====================AD~{2NJ}EdVC#(~}40-59~{#)~}=====================
	EV_MCU_Para.field.Iu_offset						   = epdw.field.Iu_offset                       = 2040; //u相电流0偏
	EV_MCU_Para.field.Iv_offset						   = epdw.field.Iv_offset                       = 2040; //v相电流0偏
	EV_MCU_Para.field.Iw_offset						   = epdw.field.Iw_offset                       = 2040; //w相电流0偏
	EV_MCU_Para.field.Iac_adjustCoeff            	   = epdw.field.Iac_adjustCoeff                 = 9000; //交流电流校正系数 //设计与实际的比例不一致，目的接近真实的电流值
	EV_MCU_Para.field.Idc_offset					   = epdw.field.Idc_offset                      = 2048; //母线电流的0偏
	EV_MCU_Para.field.Idc_adjustCoeff                  = epdw.field.Idc_adjustCoeff                 = 9000; //母线电流校正系数
	EV_MCU_Para.field.Vdc_offset			           = epdw.field.Vdc_offset                      = 2048; //母线电压0偏
	EV_MCU_Para.field.Vdc_adjustCoeff	               = epdw.field.Vdc_adjustCoeff                 = 1440; //母线电压校正系数  //2500;
	EV_MCU_Para.field.Mtrtemp_offset                   = epdw.field.Mtrtemp_offset                  = 2048; //电机温度0偏
	EV_MCU_Para.field.Mdltemp_offset                   = epdw.field.Mdltemp_offset                  = 2048; //（igbt）mos管温度的0偏
	//-----------------------------------------------------------
	EV_MCU_Para.field.AI1_offset_on			           = epdw.field.AI1_offset_on                   = 550;  //模拟输入（油门） 小于550，认为是0
	EV_MCU_Para.field.AI1_offset_off			       = epdw.field.AI1_offset_off                  = 2350; //大于2350，认为是100%
	EV_MCU_Para.field.AI1_adjustCoeff                  = epdw.field.AI1_adjustCoeff                 = 335;  //校正系数
	EV_MCU_Para.field.AI2_offset_on			           = epdw.field.AI2_offset_on                   = 550;
	EV_MCU_Para.field.AI2_offset_off			       = epdw.field.AI2_offset_off                  = 2350;
	EV_MCU_Para.field.AI2_adjustCoeff                  = epdw.field.AI2_adjustCoeff                 = 335;
	EV_MCU_Para.field.Mtrtemp_adjustCoeff			   = epdw.field.Mtrtemp_adjustCoeff				= 1024;  //电机温度校正系数
	EV_MCU_Para.field.V12_adjustCoeff			       = epdw.field.V12_adjustCoeff				    = 110;   //12v采样校正系数
	EV_MCU_Para.field.V5_adjustCoeff			       = epdw.field.V5_adjustCoeff				    = 3072;  //5v采样校正系数

	EV_MCU_Para.field.Vdc_sample_time                  = epdw.field.Vdc_sample_time                 = 5000;  //母线电压时间或次数

	//EV_MCU_Para.field.Idc_calc_mode                    = epdw.field.Idc_calc_mode                   = 0;
	//EV_MCU_Para.field.Idc_calc_ele_coeff               = epdw.field.Idc_calc_ele_coeff              = _IQ12(0.8696);
	//EV_MCU_Para.field.Idc_calc_gen_coeff               = epdw.field.Idc_calc_gen_coeff              = _IQ12(0.9);

	//=====================PI~{2NJ}~}(60-79)==========================
	EV_MCU_Para.field.Id_kp                            = epdw.field.Id_kp	                        = 500;   //Id比例
	EV_MCU_Para.field.Id_ki                            = epdw.field.Id_ki	                        = 20;    //Id积分
	EV_MCU_Para.field.Iq_kp                            = epdw.field.Iq_kp	                        = 500;   //Iq比例
	EV_MCU_Para.field.Iq_ki                            = epdw.field.Iq_ki	                        = 20;    //Iq积分
	EV_MCU_Para.field.Idq_low_speed                    = epdw.field.Idq_low_speed                   = _IQ15div(3000,SysBase.speed); //3000转以下，上面的4个系数  //中间平滑过度
	EV_MCU_Para.field.Idq_high_speed                   = epdw.field.Idq_high_speed                  = _IQ15div(6000,SysBase.speed); //6000转以上，下面的4个系数
	EV_MCU_Para.field.Id_high_speed_kp                 = epdw.field.Id_high_speed_kp                = 2000;
	EV_MCU_Para.field.Id_high_speed_ki                 = epdw.field.Id_high_speed_ki                = 150;
	EV_MCU_Para.field.Iq_high_speed_kp                 = epdw.field.Iq_high_speed_kp                = 2000;
	EV_MCU_Para.field.Iq_high_speed_ki                 = epdw.field.Iq_high_speed_ki                = 150;
	EV_MCU_Para.field.Idq_pi_swap_en                   = epdw.field.Idq_pi_swap_en                  = 0;     //电流环分段使能，用一个系数，还是分段（3000转以下，6000转以上）
	EV_MCU_Para.field.Speed_kp					       = epdw.field.Speed_kp                        = 5000;  //速度比例
	EV_MCU_Para.field.Speed_ki                         = epdw.field.Speed_ki                        = 20;       //速度积分
	EV_MCU_Para.field.Speed_kd                         = epdw.field.Speed_kd                        = 5;        //速度微分



	//======================电压闭环(80-89)=====================================
	EV_MCU_Para.field.Udcloop_Enable                    = epdw.field.Udcloop_Enable	                = 0;        //电压环使能
	EV_MCU_Para.field.Udcloop_kp                        = epdw.field.Udcloop_kp	                    = 7500;     //电压环比例
	EV_MCU_Para.field.Udcloop_ki                        = epdw.field.Udcloop_ki	                    = 10;       //电压环积分
	EV_MCU_Para.field.Udcloop_UdcUplmt                  = epdw.field.Udcloop_UdcUplmt	            = _IQ15div(300,SysBase.udc);    //？？?
	EV_MCU_Para.field.Udcloop_UdcDownlmt                = epdw.field.Udcloop_UdcDownlmt             = _IQ15div(250,SysBase.udc);
	EV_MCU_Para.field.Udcloop_Speedmin                  = epdw.field.Udcloop_Speedmin               = _IQ15div(300,SysBase.speed);
	EV_MCU_Para.field.Udcloop_Speedmax			        = epdw.field.Udcloop_Speedmax               = _IQ15div(1700,SysBase.speed);
	EV_MCU_Para.field.Udcloop_Udcref                    = epdw.field.Udcloop_Udcref                 = _IQ15div(275,SysBase.udc);
	EV_MCU_Para.field.Udcloop_PowerLimt                 = epdw.field.Udcloop_PowerLimt              = 2;
    //=====================~{V8An?XVF2NJ}~}(80-99)======================
	EV_MCU_Para.field.TrqCmd_SourceType                = epdw.field.TrqCmd_SourceType               = TRQCMD_SOURCE_AI;  //力矩命令来源  //TRQCMD_SOURCE_AI=0
	EV_MCU_Para.field.SpdCmd_SourceType                = epdw.field.SpdCmd_SourceType               = SPDCMD_SOURCE_AI;     //速度命令来源
	EV_MCU_Para.field.Trqloop_UpTrqRamp                = epdw.field.Trqloop_UpTrqRamp               = _IQ15div(1,SysBase.torque);   //转矩环上升的步长 1N.M  单位
	EV_MCU_Para.field.Trqloop_DownTrqRamp              = epdw.field.Trqloop_DownTrqRamp             = _IQ15div(2,SysBase.torque);   //转矩环下降的步长 1N.M  单位
	EV_MCU_Para.field.SpdLoop_Mincalspdstep            = epdw.field.SpdLoop_Mincalspdstep           = _IQ15div(2,SysBase.speed);    //？？？
	EV_MCU_Para.field.SpdLoop_StepCaltimeSlice         = epdw.field.SpdLoop_StepCaltimeSlice        = 28;                           //？？？
	EV_MCU_Para.field.SpdLoop_Filter_num               = epdw.field.SpdLoop_Filter_num              = _IQ15div(5,SysBase.torque);//~{KY6H;7A&>X2=3$~}   //计算滤波的数目
	EV_MCU_Para.field.SpdLoop_Cycle                    = epdw.field.SpdLoop_Cycle                   = 5;                            //经过5次任务计数变量，执行一次
	//=====================~{WT6/Hu4E?XVF2NJ}~}(100-119)==================
	EV_MCU_Para.field.Weak_enable                      = epdw.field.Weak_enable                     = 1;                            //弱磁使能
	EV_MCU_Para.field.Weak_Kp                          = epdw.field.Weak_Kp                         = 500;                          //弱磁比例
	EV_MCU_Para.field.Weak_Ki                          = epdw.field.Weak_Ki                         = 5;        //弱磁积分
	EV_MCU_Para.field.Weak_Kd                          = epdw.field.Weak_Kd                         = 0;        //弱磁微分
	EV_MCU_Para.field.Weak_iq_Limt                     = epdw.field.Weak_iq_Limt                    = _IQ15div(600,SysBase.iphase); //弱磁时，iq电流限制
	EV_MCU_Para.field.Weak_id_Limt                     = epdw.field.Weak_id_Limt                    = _IQ15div(600,SysBase.iphase);//弱磁时，id电流限制
    EV_MCU_Para.field.Weak_Vol_Utilization             = epdw.field.Weak_Vol_Utilization            = 90;       //弱磁时，电压利用率
	EV_MCU_Para.field.Weak_Kp_iq                       = epdw.field.Weak_Kp_iq                      = 1000;     //弱磁时，q轴比例
	EV_MCU_Para.field.Weak_Ki_iq                       = epdw.field.Weak_Ki_iq                      = 5;        //弱磁时，q轴积分
	EV_MCU_Para.field.Weak_Kd_iq                       = epdw.field.Weak_Kd_iq                      = 0;        //弱磁时，q轴微分
	EV_MCU_Para.field.Weak_iqcoff_Limt                 = epdw.field.Weak_iqcoff_Limt                = _IQ15div(100,SysBase.iphase);//没用到
	EV_MCU_Para.field.Weak_idcoff_Limt                 = epdw.field.Weak_idcoff_Limt                = _IQ15div(100,SysBase.iphase);//没用到
	EV_MCU_Para.field.Weak_is_Limit                    = epdw.field.Weak_is_Limit                   = _IQ15div(650,SysBase.iphase);//电流矢量幅值限制
	EV_MCU_Para.field.Weakunsat_mode                   = epdw.field.Weakunsat_mode                  = 0;//
	EV_MCU_Para.field.Weak_theta_Limt                  = epdw.field.Weak_theta_Limt                 = _IQ12(0.25); //1024  弱磁的角度限制
	EV_MCU_Para.field.Sub_Weak_enable                  = epdw.field.Sub_Weak_enable                 = 1;//二级弱磁使能
	EV_MCU_Para.field.Ipark_angle_func_enable          = epdw.field.Ipark_angle_func_enable         = 0;//park逆变换的角度功能使能
	EV_MCU_Para.field.Ipark_angle_kp                   = epdw.field.Ipark_angle_kp                  = 1000;
	EV_MCU_Para.field.Ipark_angle_ki                   = epdw.field.Ipark_angle_ki                  = 10;
	EV_MCU_Para.field.Ipark_angle_limit                = epdw.field.Ipark_angle_limit               = _IQ12(0.25);
    //=====================~{O{662NJ}~}(120-139)=======================
	EV_MCU_Para.field.EliJitter_enable                 = epdw.field.EliJitter_enable                = 1;//防抖使能
	EV_MCU_Para.field.EliJitter_TrqLimt                = epdw.field.EliJitter_TrqLimt               = _IQ15div(2,SysBase.torque);//防抖力矩限制
	EV_MCU_Para.field.EliJitter_Stop_Kp                = epdw.field.EliJitter_Stop_Kp               = 5000;//防抖比例
	EV_MCU_Para.field.EliJitter_Stop_Kd                = epdw.field.EliJitter_Stop_Kd               = 10;//防抖微分
	EV_MCU_Para.field.EliJitter_Kp                     = epdw.field.EliJitter_Kp                    = 25000;//防抖比例
    EV_MCU_Para.field.EliJitter_Ki                     = epdw.field.EliJitter_Ki                    = 0;//防抖积分
	EV_MCU_Para.field.EliJitter_Kd                     = epdw.field.EliJitter_Kd                    = 100;//防抖微分

	EV_MCU_Para.field.Spd_FWD_BrakeFault_Limt_Spd      = epdw.field.Spd_FWD_BrakeFault_Limt_Spd     = _IQ15div(2000,SysBase.speed);//
	EV_MCU_Para.field.Spd_FWD_TboxLifeFault_Limt_Spd   = epdw.field.Spd_FWD_TboxLifeFault_Limt_Spd  = _IQ15div(1700,SysBase.speed);
	EV_MCU_Para.field.Spd_FWD_TboxLoseFault_Limt_Spd   = epdw.field.Spd_FWD_TboxLoseFault_Limt_Spd  = _IQ15div(600,SysBase.speed);
	EV_MCU_Para.field.Spd_FWD_BMSFault_Limt_Spd		   = epdw.field.Spd_FWD_BMSFault_Limt_Spd       = _IQ15div(3480,SysBase.speed);
	//=====================~{W$FB2NJ}~}(140-149)==================
	EV_MCU_Para.field.Zero_Sation_Enable               = epdw.field.Zero_Sation_Enable              = 1;//驻坡使能
	EV_MCU_Para.field.Zero_Sation_time                 = epdw.field.Zero_Sation_time                = 12000;//驻坡时间
	EV_MCU_Para.field.Zero_Sation_Trq_Lmt              = epdw.field.Zero_Sation_Trq_Lmt             = _IQ15div(130,SysBase.torque);//驻坡力矩限制
	EV_MCU_Para.field.Zero_Sation_Trq_Ramp             = epdw.field.Zero_Sation_Trq_Ramp            = _IQ15div(10,SysBase.torque);//驻坡力矩步长
	EV_MCU_Para.field.Zero_Sation_kp                   = epdw.field.Zero_Sation_kp                  = 25000;//驻坡比例
	EV_MCU_Para.field.Zero_Sation_ki                   = epdw.field.Zero_Sation_ki                  = 200;//驻坡积分
	EV_MCU_Para.field.Zero_Sation_lengthlmt            = epdw.field.Zero_Sation_lengthlmt           = 10;//驻坡长度限制
	EV_MCU_Para.field.Zero_Sation_sliplengthlmt        = epdw.field.Zero_Sation_sliplengthlmt       = 1;//驻坡溜车长度限制
    //=====================~{O^KY2NJ}~}(150-159)==================
    EV_MCU_Para.field.Spd_Over_Limt_enable             = epdw.field.Spd_Over_Limt_enable            = 1;//超速限制使能
  	EV_MCU_Para.field.Spd_FWD_Limt_Spd                 = epdw.field.Spd_FWD_Limt_Spd                = _IQ15div(4500,SysBase.speed);//前进速度限制值
  	EV_MCU_Para.field.Spd_REV_Limt_Spd           	   = epdw.field.Spd_REV_Limt_Spd                = _IQ15div(750,SysBase.speed);//后退速度限制值
	EV_MCU_Para.field.Spd_FWD_L_Limt_Spd               = epdw.field.Spd_FWD_L_Limt_Spd              = _IQ15div(4500,SysBase.speed);//L档前进速度限制值
	EV_MCU_Para.field.Spd_FWD_E_Limt_Spd               = epdw.field.Spd_FWD_E_Limt_Spd              = _IQ15div(1600,SysBase.speed);//E档前进速度限制值
	EV_MCU_Para.field.Study_EncoderOffset_kp           = epdw.field.Study_EncoderOffset_kp          = 35000;//编码器自学习零偏比例
	EV_MCU_Para.field.Study_EncoderOffset_ki           = epdw.field.Study_EncoderOffset_ki          = 40;//编码器自学习零偏积分
	EV_MCU_Para.field.Study_EncoderOffset_is           = epdw.field.Study_EncoderOffset_is          = _IQ15div(10,SysBase.iphase);//编码器自学习零偏幅值
	EV_MCU_Para.field.Study_EncoderOffset_speed        = epdw.field.Study_EncoderOffset_speed       = _IQ15div(1000,SysBase.speed);//编码器自学习零偏时的速度

	EV_MCU_Para.field.Motor_Mras_Kp                    = epdw.field.Motor_Mras_Kp                   = 5000;//
	EV_MCU_Para.field.Motor_Mras_Ki                    = epdw.field.Motor_Mras_Ki                   = 50;
  	//=====================~{WTQ'O02NJ}~}(160-169)======================
  	EV_MCU_Para.field.Motor_study_MaxVol               = epdw.field.Motor_study_MaxVol              = _IQ15div(50,SysBase.udc);//电机自学习最大电压
  	EV_MCU_Para.field.Motor_study_MaxCrt               = epdw.field.Motor_study_MaxCrt              = _IQ15div(60,SysBase.iphase);//
  	EV_MCU_Para.field.Motor_studyFluex_is1             = epdw.field.Motor_studyFluex_is1            = _IQ15div(10,SysBase.iphase);
	EV_MCU_Para.field.Motor_studyFluex_is2             = epdw.field.Motor_studyFluex_is2            = _IQ15div(20,SysBase.iphase);
  	EV_MCU_Para.field.Motor_studyFluex_speed           = epdw.field.Motor_studyFluex_speed          = _IQ15div(1000,SysBase.speed);
	EV_MCU_Para.field.Motor_discharge_current          = epdw.field.Motor_discharge_current         = _IQ15div(10,SysBase.iphase);

  	//=====================~{O5M31(>/2NJ}~}(170-189)=====================
  	EV_MCU_Para.field.Motor_Protect_AlarmTempCtr	   = epdw.field.Motor_Protect_AlarmTempCtr		= 85;//控制器的温度报警 MOS管报警温度
  	EV_MCU_Para.field.Motor_Protect_AlarmTempMtr	   = epdw.field.Motor_Protect_AlarmTempMtr      = 160;//电机的温度报警 电机报警温度
  	EV_MCU_Para.field.Motor_Protect_AlarmVdcLv		   = epdw.field.Motor_Protect_AlarmVdcLv 	    = _IQ15div(90,SysBase.udc);//母线电压的欠压报警
  	EV_MCU_Para.field.Motor_Protect_AlarmVdcOv		   = epdw.field.Motor_Protect_AlarmVdcOv 	    = _IQ15div(125,SysBase.udc);//母线电压的过压报警
    EV_MCU_Para.field.Motor_Protect_InvCrt		       = epdw.field.Motor_Protect_InvCrt 	        = _IQ15div(150,SysBase.iphase);//控制器过流保护点
  	EV_MCU_Para.field.Motor_Protect_MtrCrt		       = epdw.field.Motor_Protect_MtrCrt 	        = _IQ15div(150,SysBase.iphase);//电机的过流保护点
  	EV_MCU_Para.field.Motor_Protect_InvCrtCoeff	       = epdw.field.Motor_Protect_InvCrtCoeff 	    = _IQ12(1.0);//控制器过流保护的修正系数
  	EV_MCU_Para.field.Motor_Protect_MtrCrtCoeff	       = epdw.field.Motor_Protect_MtrCrtCoeff 	    = _IQ12(1.0);//电机的过流保护的修正系数
  	EV_MCU_Para.field.Protect_OvLdFlag		           = epdw.field.Protect_OvLdFlag 	            = 0;//过载保护标志
  	EV_MCU_Para.field.Motor_Protect_AlarmSpdFwd	       = epdw.field.Motor_Protect_AlarmSpdFwd 	    = _IQ15div(6000,SysBase.speed);//正转的报警转速
  	EV_MCU_Para.field.Motor_Protect_AlarmSpdRev	       = epdw.field.Motor_Protect_AlarmSpdRev 	    = _IQ15div(6000,SysBase.speed);//反转的报警转速
  	EV_MCU_Para.field.OpenPhaseCheck_En	               = epdw.field.OpenPhaseCheck_En 	            = 0;//三相开路检测使能
  	EV_MCU_Para.field.OpenPhaseCheck_CrtCoeff	       = epdw.field.OpenPhaseCheck_CrtCoeff 	    = _IQ12(0.2);//三相开路的修正系数 修正阈值
  	EV_MCU_Para.field.PhaseUnbalanceCheck_En	       = epdw.field.PhaseUnbalanceCheck_En 	        = 0;//三相电流平衡检测使能
  	EV_MCU_Para.field.PhaseUnbalanceCheck_CrtCoeff	   = epdw.field.PhaseUnbalanceCheck_CrtCoeff    = _IQ12(1.0);//三相电流平衡检测修正系数
  	EV_MCU_Para.field.SpeedDetectFault_En	           = epdw.field.SpeedDetectFault_En 	        = 0;//速度检测故障使能
  	EV_MCU_Para.field.CrtDetectFault_En	               = epdw.field.CrtDetectFault_En 	            = 0;//电流检测故障使能
  	EV_MCU_Para.field.HWresolverFault_En               = epdw.field.HWresolverFault_En              = 0;//旋变硬件故障检测使能
	EV_MCU_Para.field.Motor_Protect_HWOverVolMtr	   = epdw.field.Motor_Protect_HWOverVolMtr 	    = 110;//硬件过压点
  	//=====================~{O5M39JUO2NJ}~}(190-209)=====================
  	EV_MCU_Para.field.Motor_Protect_OverTempCtr		   = epdw.field.Motor_Protect_OverTempCtr	 	= 95;//控制器过温故障点
  	EV_MCU_Para.field.Motor_Protect_OverTempMtr		   = epdw.field.Motor_Protect_OverTempMtr	    = 170;//电机过温故障点
  	EV_MCU_Para.field.Motor_Protect_VdcLv		       = epdw.field.Motor_Protect_VdcLv	 	        = _IQ15div(80,SysBase.udc);//母线欠压故障点
  	EV_MCU_Para.field.Motor_Protect_VdcOv		       = epdw.field.Motor_Protect_VdcOv	            = _IQ15div(135,SysBase.udc);//母线过压故障点
  	EV_MCU_Para.field.Motor_Protect_HWOverCrtMtr	   = epdw.field.Motor_Protect_HWOverCrtMtr 	    = 3800;//硬件过流
  	EV_MCU_Para.field.Protect_overLoadInv    		   = epdw.field.Protect_overLoadInv 	        = _IQ15div(650,SysBase.iphase);//控制器过载 一定时间超过一定额定功率运行
  	EV_MCU_Para.field.Motor_Protect_OverSpdFwd	       = epdw.field.Motor_Protect_OverSpdFwd 	    = _IQ15div(6500,SysBase.speed);//正转超速故障
  	EV_MCU_Para.field.Motor_Protect_OverSpdRev	       = epdw.field.Motor_Protect_OverSpdRev 	    = _IQ15div(6500,SysBase.speed);//反转超速故障
	EV_MCU_Para.field.Motor_Protect_AlarmV12Lv	       = epdw.field.Motor_Protect_AlarmV12Lv 	    = _IQ15div(9,SysBase.udc);//12V欠压报警
	EV_MCU_Para.field.Motor_Protect_V12Lv	           = epdw.field.Motor_Protect_V12Lv 	        = _IQ15div(8,SysBase.udc);//12V欠压故障
	EV_MCU_Para.field.Motor_Protect_AlarmV12Ov	       = epdw.field.Motor_Protect_AlarmV12Ov 	    = _IQ15div(17,SysBase.udc);//12V过压报警
	EV_MCU_Para.field.Motor_Protect_V12Ov	           = epdw.field.Motor_Protect_V12Ov 	        = _IQ15div(18,SysBase.udc);//12V过压故障
  	//=====================~{1dF5Fw;y1>2NJ}IhVC~}(210-219)===============
  	EV_MCU_Para.field.PWM_Lowspeed_Period			   = epdw.field.PWM_Lowspeed_Period             = 8000;//低速PWM的频率
	EV_MCU_Para.field.PWM_Highspeed_Period			   = epdw.field.PWM_Highspeed_Period            = 10000;//高速PWM的频率
	EV_MCU_Para.field.PWM_Period_Swap_speed            = epdw.field.PWM_Period_Swap_speed           = _IQ15div(3000,SysBase.speed);//变载频转速点
  	EV_MCU_Para.field.PWM_Period			           = epdw.field.PWM_Period                      = 8000;//PWM默认频率
  	EV_MCU_Para.field.PWM_Change_En			           = epdw.field.PWM_Change_En                   = 0;//变载频使能
  	EV_MCU_Para.field.Deadtimecomp_state               = epdw.field.Deadtimecomp_state              = 1;//死区补偿状态
	EV_MCU_Para.field.Deadtimecomp_percent             = epdw.field.Deadtimecomp_percent            = 100;//死区补偿百分比
  	//======================IGBT_Short_Para(220-229)==================================
  	EV_MCU_Para.field.Short_IGBT_en			           = epdw.field.Short_IGBT_en                   = 0;//主动短路使能
  	EV_MCU_Para.field.Short_IGBT_udc_in			       = epdw.field.Short_IGBT_udc_in               = _IQ15div(750,SysBase.udc);
  	EV_MCU_Para.field.Short_IGBT_udc_out			   = epdw.field.Short_IGBT_udc_out              = _IQ15div(600,SysBase.udc);
    //======================Tooth_trq_para(230-239)==================================
  	EV_MCU_Para.field.Tooth_trq_en			           = epdw.field.Tooth_trq_en                   = 1;//贴齿力矩使能
  	EV_MCU_Para.field.Tooth_trq			               = epdw.field.Tooth_trq                      = _IQ15div(2,SysBase.torque);//最大贴齿力矩
  	EV_MCU_Para.field.Tooth_trq_ramp			       = epdw.field.Tooth_trq_ramp                 = _IQ15div(1,SysBase.torque);//贴齿力矩的步长
  	//=======================预留参数=====================================
  	EV_MCU_Para.field.Speed_maxerr			           = epdw.field.Speed_maxerr                   	= _IQ15div(1000,SysBase.speed); //速度超差
  	EV_MCU_Para.field.Brake_gearenable                 = epdw.field.Brake_gearenable               	= 0;//

	//=======================堵转降载频参数=====================================
  	EV_MCU_Para.field.Lack_Phase_en                    = epdw.field.Lack_Phase_en                   = 0;//电机堵转降载频使能
  	EV_MCU_Para.field.Lack_Phase_Enter_Speed		   = epdw.field.Lack_Phase_Enter_Speed          = _IQ15div(200,SysBase.speed);//进入堵转速度
  	EV_MCU_Para.field.Lack_Phase_Exit_Speed			   = epdw.field.Lack_Phase_Exit_Speed           = _IQ15div(300,SysBase.speed);//退出堵转速度
	EV_MCU_Para.field.Lack_Phase_Pwm_Period            = epdw.field.Lack_Phase_Pwm_Period           = 2000;//堵转载频

	//=======================高频注入参数=====================================
	EV_MCU_Para.field.high_fre_enable                  = epdw.field.high_fre_enable                 = 0;
	EV_MCU_Para.field.high_fre_frequency               = epdw.field.high_fre_frequency              = 600;
	EV_MCU_Para.field.high_fre_voltage                 = epdw.field.high_fre_voltage                = _IQ15div(50,SysBase.udc);
	EV_MCU_Para.field.high_fre_kp                      = epdw.field.high_fre_kp                     = 5000;
	EV_MCU_Para.field.high_fre_ki                      = epdw.field.high_fre_ki                     = 250;
	EV_MCU_Para.field.high_fre_nspole_voltage          = epdw.field.high_fre_nspole_voltage         = _IQ15div(30,SysBase.udc);
	EV_MCU_Para.field.resolver_fault_theta_maxerr      = epdw.field.resolver_fault_theta_maxerr     = 50;

	EV_MCU_Para.field.Svpwm_way                        = epdw.field.Svpwm_way                       = 0;
	EV_MCU_Para.field.Svpwm_way_swap_speed             = epdw.field.Svpwm_way_swap_speed            = _IQ15div(2000,SysBase.speed);
}

void Default_ExParaInt(void)
{
	//mtrPara.Encoder_type.value			        = ENC_TYPE_RESOLVER;
	mtrPara.Encoder_rollOver.value              = RESOLVER_LINES_12BIT;
	invPara.ThetaCmp_timeGap.value		        = THETACOMP_TIMEGAP;
	//invPara.PWM_NzTime.value			        = EV_MCU_Para.field.PWM_Period-200;
	//invPara.PWM_Deadband.value					= PWM_DEADBAND_TIME;

  	EV_MCU_Para.field.SW_version			    = epdw.field.SW_version                      = SOFTWARE_VERSION;
  	EV_MCU_Para.field.SW_version_date			= epdw.field.SW_version_date                 = SOFTWARE_VERSION_DATE;
  	EV_MCU_Para.field.Error_rest			    = epdw.field.Error_rest                      = 0;
	EV_MCU_Para.field.Test_Mode                 = epdw.field.Test_Mode                       = 0;
	SysBase.mtr_ratio                           = _IQdiv(EV_MCU_Para.field.Motor_Np,EV_MCU_Para.field.Encoder_Np);
  	/*EV_MCU_Para.field.speed_base			    = epdw.field.speed_base  = SysBase.speed;
  	EV_MCU_Para.field.torque_base			    = epdw.field.torque_base = SysBase.torque;
  	EV_MCU_Para.field.iphase_base			    = epdw.field.iphase_base = SysBase.iphase;
  	EV_MCU_Para.field.udc_base			        = epdw.field.udc_base    = SysBase.udc;*/
}

void SysCtrlVar_Init(void)
{
//================================================================
	// ~{O5M3?XVFWV~}1~{3uJ<;/~}
	sccw1.wValue                	= 0;
	sccw1.field.run_enable      	= 0;
	sccw1.field.runLoop_mode    	= MTR_TRQ_LOOP;
	// ~{O5M3?XVFWV~}2~{3uJ<;/~}
	sccw2.wValue                	= 0;
	sccw2.field.theta_mode      	= THETA_MODE_FEEDBACK;      // ~{74@!=G6H~}//THETA_MODE_OMEGA

	sccw2.field.vdc_mode            = VDC_MODE_FEEDBACK;        // 反馈电压

	sccw2.field.igbtFault_ChkMode   = 1;                        //zgjtest
	sccw2.field.crtFbk_ChkMode      = 1;
	sccw2.field.mtrtempFbk_ChkMode  = 1;
	sccw2.field.crtFbkFault_ChkMode = 1;                        // 检测电流反馈通道故障

	sccw2.field.lv_check=1;                                 // ~{<l2bG7Q9~}

	// ~{O5M3W4L,WV~}1~{3uJ<;/~}
	scsw1.wValue                	= 0;
	scsw1.field.runLoop_state   	= MTR_TRQ_LOOP;         	//
	scsw1.field.system_state    	= SYS_POWERDOWN;          	// Ready~{W4L,~}

	// ~{O5M3W4L,WV~}2~{3uJ<;/~}
	scsw2.wValue                	= 0;
	scsw2.field.runLoop_state   	= MTR_TRQ_LOOP;
	scsw2.field.theta_state     	= THETA_MODE_FEEDBACK;      // ~{74@!=G6H~}
	scsw2.field.vdc_mode        	= VDC_MODE_FEEDBACK;        // ~{74@!5gQ9~}
	scsw2.field.deadtimecomp_state	= 0;						// ~{K@Gx293%2;FpWwSC~}
	scsw2.field.MTPA_mode         	= 0;

	/*emsw&emsw1*/
	emsw.wValue   					= 0;							/*~{N^9JUO~}*/
	emsw1.wValue 					= 0;							/*~{N^9JUO~}*/
	emsw2.wValue   					= 0;							/*~{N^9JUO~}*/
	emswold.wValue   				= 0;							/*~{N^9JUO~}*/
	emsw1old.wValue 				= 0;							/*~{N^9JUO~}*/
	emsw2old.wValue   				= 0;							/*~{N^9JUO~}*/
    alarm.wValue 				    = 0;							/*~{N^1(>/~}*/

    //IO_state.wValue				=0;
    Vehicle_cmd.cmdmode.Value   =0;

    //软件示波器初始化
    //osc.OSC_enable              =1;
    //osc.transmitFlag			=0;

    //旋变粗学完成标志
    //diagEncoffsetVar.finshflag = 0;
}

void SysCtrlBase_Init(void)
{
    //速度基值
  	/*EV_MCU_Para.field.speed_base			           = epdw.field.speed_base  = SysBase.speed           	=12000;
  	EV_MCU_Para.field.torque_base			           = epdw.field.torque_base = SysBase.torque          	=500;
  	EV_MCU_Para.field.iphase_base			           = epdw.field.iphase_base = SysBase.iphase          	=900;
  	EV_MCU_Para.field.udc_base			               = epdw.field.udc_base    = SysBase.udc             	=800;*/

	SysBase.speed           	= 12000;
  	SysBase.torque          	= 500;
  	SysBase.iphase          	= 900;
  	SysBase.udc             	= 800;
	//SysBase.freq                = 900;

	SysBase.invspeed        	=_IQdiv(1,SysBase.speed);
	SysBase.omega           	= SysBase.speed/60;
	SysBase.invomega        	=_IQdiv(1,SysBase.omega);
	SysBase.invtorque       	=_IQdiv(1,SysBase.torque);
	SysBase.inviphase      	 	=_IQdiv(1,SysBase.iphase);
	SysBase.invudc          	=_IQdiv(1,SysBase.udc);
	//SysBase.ele_power           = SysBase.iphase*SysBase.udc;

	//SysBase.uphase          	=_IQdiv(SysBase.udc,SQRT3);
	//SysBase.iphase_uphase_coeff =_IQdiv(SysBase.iphase,SysBase.uphase);
	//SysBase.omega_uphase_coeff  =_IQdiv(SysBase.omega,SysBase.uphase);
}
void SysCtrltrqtable_Init(void)
{
	TRQ_Table.udcmap        	= _IQmpyI32(60,SysBase.invudc);
	TRQ_Table.trqgrid_num   	= 75;
	TRQ_Table.speed_min     	=_IQmpyI32(1600,SysBase.invspeed);
	TRQ_Table.speed_max     	=_IQmpyI32(5600,SysBase.invspeed);
	TRQ_Table.trq_max       	=_IQmpyI32(150,SysBase.invtorque);
	TRQ_Table.trq_div       	=TRQ_Table.trq_max/TRQ_Table.trqgrid_num+1;
	/*
	 * 力矩基值2500   力矩最小步长 20
	 * 力矩基值500     力矩最小步长 4
	 * 力矩肌值4000  力矩最小步长  32
	 */
    if(TRQ_Table.trq_div<_IQ(0.001))  emsw2.field.data_overflow = 1;//判断溢出

	//TRQ_Table.trq_div_INV       =(_IQdiv(_IQ(1.0),TRQ_Table.trq_div))>>9;//Q15格式
	TRQ_Table.trq_div_INV       =_IQ15div(_IQ15(1.0),_IQtoIQ15(TRQ_Table.trq_div));//Q15格式
	TRQ_Table.spd_div       	=_IQmpyI32(200,SysBase.invspeed);
	/*
	 * 速度基值6000       速度最小步长 48
	 * 力矩基值12000     速度最小步长 96
	 */
	if(TRQ_Table.spd_div<_IQ(0.001))  emsw2.field.data_overflow = 1;//判断溢出

	//TRQ_Table.spd_div_INV       =(_IQdiv(_IQ(1.0),TRQ_Table.spd_div))>>9;//Q15格式
	TRQ_Table.spd_div_INV       =_IQ15div(_IQ15(1.0),_IQtoIQ15(TRQ_Table.spd_div));//Q15格式
	TRQ_Table.area_div_INV      = (_IQmpy(TRQ_Table.trq_div_INV,TRQ_Table.spd_div_INV))<<6;//Q12格式

	TRQ_Table.avgspeed_datanum  = 152;

	//TRQ_Table.idcoeff_ele   	=_IQ(1.0);
	//TRQ_Table.iqcoeff_ele   	=_IQ(1.0);
	//TRQ_Table.idcoeff_gen   	=_IQ(1.0);
	//TRQ_Table.iqcoeff_gen   	=_IQ(1.0);

	//TRQ_Table.idlmtspd_div  	=_IQ24div(200,SysBase.speed);
	//TRQ_Table.idlmtspd_div_INV  =_IQ(1.0)/TRQ_Table.idlmtspd_div;

	//TRQ_Table.trqcoeffspd_div   =_IQ24div(400,SysBase.speed);
	//TRQ_Table.trqcoeffspd_div_INV = _IQ(1.0)/TRQ_Table.trqcoeffspd_div;

	//TRQ_Table.trqcoefftrq_div   =_IQ24div(250,SysBase.torque);
	//TRQ_Table.trqcoefftrq_div_INV = _IQ(1.0)/TRQ_Table.trqcoefftrq_div;
	//TRQ_Table.trqcoeff_num      =9;
	//TRQ_Table.trqcoeffarea_div  =_IQmpy(TRQ_Table.trqcoeffspd_div,TRQ_Table.trqcoefftrq_div);
	//TRQ_Table.trqcoeffarea_div_INV = _IQ(1.0)/TRQ_Table.trqcoeffarea_div;
	//TRQ_Table.trqcoeff_blockspd =_IQdiv(20,SysBase.speed);
	TRQ_Table.encoder_minspeed  = _IQmpyI32(500,SysBase.invspeed);
	TRQ_Table.encoder_maxspeed  = _IQmpyI32(5500,SysBase.invspeed);
	TRQ_Table.encoder_detaspeed = _IQmpyI32(500,SysBase.invspeed);
	TRQ_Table.encoder_num       = 11;
	TRQ_Table.ipark_minspeed  = _IQmpyI32(1000,SysBase.invspeed);
	TRQ_Table.ipark_maxspeed  = _IQmpyI32(5600,SysBase.invspeed);
	TRQ_Table.ipark_detaspeed = _IQmpyI32(200,SysBase.invspeed);
	TRQ_Table.ipark_num       = 24;
}



void SysCfgParaInt()
{
	sysCfgPara.SpdCmd         = 0;
	sysCfgPara.TrqCmd         = 0;

	sysCfgPara.Theta_ele				= 0.0;
	sysCfgPara.Theta_man.value			= 0.0;

	//PWM_Period_time = _IQdiv((_iq)EPwm1Regs.TBPRD,(PWM_SYS_FRE/1000));
	sysCfgPara.Omega_man.value			= MANUAL_OMEGA_ELE;
	sysCfgPara.Omega_period.value		= OMEGA_CAL_PERIOD;

	sysCfgPara.Vdc_man.value			= MANUAL_VDC;
	sysCfgPara.Vdc						= 0;
	sysCfgPara.VdCmd_man				= 0;
	sysCfgPara.VqCmd_man				= 0;

	sysCfgPara.IdCmd_man				= 0;
	sysCfgPara.IqCmd_man				= 0;

    crtLoopPara.IdPid_Kp				= _IQ12toIQ(EV_MCU_Para.field.Id_kp);    //IQ12
    crtLoopPara.IdPid_Ki			    = _IQ12toIQ(EV_MCU_Para.field.Id_ki);
	crtLoopPara.IqPid_Kp				= _IQ12toIQ(EV_MCU_Para.field.Iq_kp);    //IQ12
    crtLoopPara.IqPid_Ki			    = _IQ12toIQ(EV_MCU_Para.field.Iq_ki);

	trqLmt_eletrqmax             = _IQ15toIQ(EV_MCU_Para.field.Motor_PeakTorqueEle);
	trqLmt_gentrqmax             = _IQ15toIQ(EV_MCU_Para.field.Motor_PeakTorqueGen);
    trqLoopPara.Max_TorqueEle   = trqLoop_trqMaxelecoeff = trqLmt_eletrqmax;
    trqLoopPara.Max_TorqueGen   = trqLoop_trqMaxgencoeff = trqLmt_gentrqmax;

	trqLoopPara.Torque_ref_HMI  		= 0;

    spdLoopInput();
	ctrcmdConfigslowin();
	weakcrtLoopslowin();
	mtrfwdslowin();
	crtLoopslowInput();
	voltLoopslowInput();

	vehicle_io_state_old.wValue = 0x0000;
	vehicle_io_state_new.wValue = 0x0000;
	vehicle_io_state.wValue = 0x0000;

#if 0
	PWM_Period_lowspeed = EV_MCU_Para.field.PWM_Period_Swap_speed;
	PWM_Period_highspeed = EV_MCU_Para.field.PWM_Period_Swap_speed+_IQ15div(500,SysBase.speed);
	if(EV_MCU_Para.field.PWM_Lowspeed_Period<SVPWM_2KHZ_VALUE)
	{
		PWM_Period_low = SVPWM_2KHZ;
	}
	else if(EV_MCU_Para.field.PWM_Lowspeed_Period>SVPWM_16KHZ_VALUE)
	{
		PWM_Period_low = SVPWM_16KHZ;
	}
	else
	{
		PWM_Period_low  = (Uint16)((PWM_SYS_FRE/EV_MCU_Para.field.PWM_Lowspeed_Period)>>1);
	}
	if(EV_MCU_Para.field.PWM_Highspeed_Period>SVPWM_16KHZ_VALUE)
	{
		PWM_Period_high = SVPWM_16KHZ;
	}
	else if(EV_MCU_Para.field.PWM_Highspeed_Period<SVPWM_2KHZ_VALUE)
	{
		PWM_Period_high = SVPWM_2KHZ;
	}
	else
	{
		PWM_Period_high = (Uint16)((PWM_SYS_FRE/EV_MCU_Para.field.PWM_Highspeed_Period)>>1);
	}
	if(EV_MCU_Para.field.PWM_Period<SVPWM_2KHZ_VALUE)
	{
		PWM_Period = SVPWM_2KHZ;
	}
	else if(EV_MCU_Para.field.PWM_Period>SVPWM_16KHZ_VALUE)
	{
		PWM_Period = SVPWM_16KHZ;
	}
	else
	{
		PWM_Period      = (Uint16)((PWM_SYS_FRE/EV_MCU_Para.field.PWM_Period)>>1);
	}
#endif
}

void ClearManualCtrlCmd(void)
{
	sysCfgPara.VdCmd_man	= 0;
	sysCfgPara.VqCmd_man	= 0;
	sysCfgPara.IdCmd_man	= 0;
	sysCfgPara.IqCmd_man	= 0;
	sysCfgPara.CANA_SpdCmd_man         = 0;
	sysCfgPara.CANA_TrqCmd_man         = 0;
	sysCfgPara.TrqCmd_AI                 = 0;
	sysCfgPara.TrqCmd_AIfilter           = 0;
	sccw1.field.runLoop_mode  			 = MTR_TRQ_LOOP;
	EV_MCU_Para.field.TrqCmd_SourceType  = epdw.field.TrqCmd_SourceType;
	EV_MCU_Para.field.SpdCmd_SourceType  = epdw.field.SpdCmd_SourceType;
	sysFbkPara.Id_fbk_filter_0025  = 0;
	sysFbkPara.Iq_fbk_filter_0025  = 0;
	up_step = down_step = _IQmpy(_IQ(0.01),SysBase.invtorque);
}

void ClearCtrlLoopPara(void)
{
	trqLoopParaClr();	//初始时候要先力矩环再速度环
	spdLoopParaClr();
	crtcmdConfigclc();
	weakcrtLoopctrlclc();
	crtLoopParaClr();
	mtrfwdclc();
	vdqlmtclc();
	//harmonic_process_clear();
	voltLoopParaClr();
	//fuzzy_clear();
	//PmsmMrasClr();
	diff_deal_clear();
}

#endif	// end of #ifndef _VARIABLEINIT_C
//===========================================================================
// No more.
//===========================================================================
