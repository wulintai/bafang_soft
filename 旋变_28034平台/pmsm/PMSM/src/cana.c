#ifndef _CANA_C_
#define _CANA_C_

#include "include_c.h"

extern _iq trqidqmap_absSpdFbk;

void CANA_init(void)
{
	struct ECAN_REGS	ECanaShadow;
	struct LAM_REGS		ECanaLAMShadow;
	EALLOW;

	//========================================================================================
	// 配置邮箱发送/接收,mailbox3-0为接收，mailbox31-4为发送
	//========================================================================================
		// Disable all Mailboxes
		ECanaRegs.CANME.all = 0;
		// Clear TRS DBO
		ECanaShadow.CANTRR.all	= 0;
		ECanaShadow.CANTRR.all	= 0xFFFFFFFF;
		ECanaRegs.CANTRR.all	= ECanaShadow.CANTRR.all;
		do
		{
			ECanaShadow.CANTRS.all	= ECanaRegs.CANTRS.all;
			ECanaShadow.CANTRS.all	&= 0xFFFFFFFF;
		} while (ECanaShadow.CANTRS.all != 0);
		// Configure Mailbox0 - Mailbox3  as Receive mailboxes
		// Configure Mailbox4 - Mailbox31 as Transmit mailboxes
		ECanaRegs.CANMD.all = 0x800300FF;
	//========================================================================================
	// 配置接收邮箱的LAM，强制匹配，标准帧/扩展帧均可接收
	//========================================================================================
		ECanaMboxes.MBOX0.MSGID.all = 0x18ef0301;
		ECanaMboxes.MBOX0.MSGID.bit.IDE		 = 1;
		ECanaMboxes.MBOX0.MSGID.bit.AME		=1;
		ECanaMboxes.MBOX0.MSGCTRL.bit.DLC	= 8;
	    ECanaLAMShadow.LAM0.all         = 0;
	    ECanaLAMShadow.LAM0.bit.LAMI    = 0; //只接收扩展帧
	    ECanaLAMRegs.LAM0.all           = ECanaLAMShadow.LAM0.all;          //调试参数给定

		ECanaMboxes.MBOX1.MSGID.all = 0x182127F4;
		ECanaMboxes.MBOX1.MSGID.bit.IDE		 = 1;
		ECanaMboxes.MBOX1.MSGID.bit.AME		=1;
		ECanaMboxes.MBOX1.MSGCTRL.bit.DLC	= 8;
	    ECanaLAMShadow.LAM1.all         = 0;
	    ECanaLAMShadow.LAM1.bit.LAMI    = 0; //只接收扩展帧
	    ECanaLAMRegs.LAM1.all           = ECanaLAMShadow.LAM1.all;          //调试参数给定

		ECanaMboxes.MBOX2.MSGID.all = 0x182427F4;
		ECanaMboxes.MBOX2.MSGID.bit.IDE		 = 1;
		ECanaMboxes.MBOX2.MSGID.bit.AME		=1;
		ECanaMboxes.MBOX2.MSGCTRL.bit.DLC	= 8;
	    ECanaLAMShadow.LAM2.all         = 0;
	    ECanaLAMShadow.LAM2.bit.LAMI    = 0; //只接收扩展帧
	    ECanaLAMRegs.LAM2.all           = ECanaLAMShadow.LAM2.all;          //调试参数给定

		ECanaMboxes.MBOX3.MSGID.all = 0x18b428f4;//0x6A1;
		ECanaMboxes.MBOX3.MSGID.bit.IDE		 = 1;
		ECanaMboxes.MBOX3.MSGID.bit.AME		=1;
		ECanaMboxes.MBOX3.MSGCTRL.bit.DLC	= 8;
	    ECanaLAMShadow.LAM3.all         = 0;
	    ECanaLAMShadow.LAM3.bit.LAMI    = 0; //只接收扩展帧
	    ECanaLAMRegs.LAM3.all           = ECanaLAMShadow.LAM3.all;          //调试参数给定

		ECanaMboxes.MBOX4.MSGID.all = 0x1806e5f4;
		ECanaMboxes.MBOX4.MSGID.bit.IDE		 = 1;
		ECanaMboxes.MBOX4.MSGID.bit.AME		=1;
		ECanaMboxes.MBOX4.MSGCTRL.bit.DLC	= 8;
	    ECanaLAMShadow.LAM4.all         = 0;
	    ECanaLAMShadow.LAM4.bit.LAMI    = 0; //只接收扩展帧
	    ECanaLAMRegs.LAM4.all           = ECanaLAMShadow.LAM4.all;          //调试参数给定

		ECanaMboxes.MBOX5.MSGID.all = 0x18ff50e5;//0x3B5;
		ECanaMboxes.MBOX5.MSGID.bit.IDE		 = 1;
		ECanaMboxes.MBOX5.MSGID.bit.AME		=1;
		ECanaMboxes.MBOX5.MSGCTRL.bit.DLC	= 8;
	    ECanaLAMShadow.LAM5.all         = 0;
	    ECanaLAMShadow.LAM5.bit.LAMI    = 0; //只接收扩展帧
	    ECanaLAMRegs.LAM5.all           = ECanaLAMShadow.LAM5.all;          //调试参数给定
	    //========================================================================================
		// 配置发送邮箱
		//========================================================================================
		ECanaMboxes.MBOX8.MSGID.all   = 0x18ef0103;
		ECanaMboxes.MBOX8.MSGID.bit.IDE		   = 1;
		ECanaMboxes.MBOX8.MSGID.bit.AME		   = 1;
		ECanaMboxes.MBOX8.MSGCTRL.bit.DLC	   = 8;
		
		ECanaMboxes.MBOX9.MSGID.all   = 0x18ef0301;
		ECanaMboxes.MBOX9.MSGID.bit.IDE		   = 1;
		ECanaMboxes.MBOX9.MSGID.bit.AME		   = 1;
		ECanaMboxes.MBOX9.MSGCTRL.bit.DLC	   = 8;
		
		ECanaMboxes.MBOX11.MSGID.all         =0x4192;
		ECanaMboxes.MBOX11.MSGID.bit.IDE		= 1;
		ECanaMboxes.MBOX11.MSGID.bit.AME		=1;
		ECanaMboxes.MBOX11.MSGCTRL.bit.DLC	= 8;

		/*ECanaMboxes.MBOX12.MSGID.all         =0x4193;
		ECanaMboxes.MBOX12.MSGID.bit.IDE		= 1;
		ECanaMboxes.MBOX12.MSGID.bit.AME		=1;
		ECanaMboxes.MBOX12.MSGCTRL.bit.DLC	= 8;*/
//===================================================================================================
		//debug
		ECanaMboxes.MBOX16.MSGID.all			= 0x80004180;
		ECanaMboxes.MBOX16.MSGID.bit.AME		=1;
		ECanaMboxes.MBOX16.MSGCTRL.bit.DLC	 = 8;
	    ECanaLAMShadow.LAM16.all             = 0;
	    ECanaLAMShadow.LAM16.bit.LAMI        = 0;
	    ECanaLAMRegs.LAM16.all               = ECanaLAMShadow.LAM16.all;            //调试参数给定

		ECanaMboxes.MBOX18.MSGID.all         =0x80004185;
		ECanaMboxes.MBOX18.MSGID.bit.AME		=1;
		ECanaMboxes.MBOX18.MSGCTRL.bit.DLC	= 8;

		ECanaMboxes.MBOX19.MSGID.all         =0x80004186;
		ECanaMboxes.MBOX19.MSGID.bit.AME		=1;
		ECanaMboxes.MBOX19.MSGCTRL.bit.DLC	= 8;

		ECanaMboxes.MBOX20.MSGID.all         =0x80004187;
		ECanaMboxes.MBOX20.MSGID.bit.AME		=1;
		ECanaMboxes.MBOX20.MSGCTRL.bit.DLC	= 8;

		ECanaMboxes.MBOX21.MSGID.all         =0x80004188;
		ECanaMboxes.MBOX21.MSGID.bit.AME		=1;
		ECanaMboxes.MBOX21.MSGCTRL.bit.DLC	= 8;

		ECanaMboxes.MBOX22.MSGID.all         =0x80004189;
		ECanaMboxes.MBOX22.MSGID.bit.AME		=1;
		ECanaMboxes.MBOX22.MSGCTRL.bit.DLC	= 8;

		ECanaMboxes.MBOX23.MSGID.all         =0x8000418A;
		ECanaMboxes.MBOX23.MSGID.bit.AME		=1;
		ECanaMboxes.MBOX23.MSGCTRL.bit.DLC	= 8;

		ECanaMboxes.MBOX24.MSGID.all         =0x8000418B;
		ECanaMboxes.MBOX24.MSGID.bit.AME		=1;
		ECanaMboxes.MBOX24.MSGCTRL.bit.DLC	= 8;

		ECanaMboxes.MBOX25.MSGID.all         =0x8000418C;
		ECanaMboxes.MBOX25.MSGID.bit.AME		=1;
		ECanaMboxes.MBOX25.MSGCTRL.bit.DLC	= 8;

		ECanaMboxes.MBOX26.MSGID.all         =0x8000418D;
		ECanaMboxes.MBOX26.MSGID.bit.AME		=1;
		ECanaMboxes.MBOX26.MSGCTRL.bit.DLC	= 8;

		ECanaMboxes.MBOX27.MSGID.all         =0x8000418E;
		ECanaMboxes.MBOX27.MSGID.bit.AME		=1;
		ECanaMboxes.MBOX27.MSGCTRL.bit.DLC	= 8;

		ECanaMboxes.MBOX28.MSGID.all         =0x8000418F;
		ECanaMboxes.MBOX28.MSGID.bit.AME		=1;
		ECanaMboxes.MBOX28.MSGCTRL.bit.DLC	= 8;

		ECanaMboxes.MBOX29.MSGID.all         =0x80004190;
		ECanaMboxes.MBOX29.MSGID.bit.AME		=1;
		ECanaMboxes.MBOX29.MSGCTRL.bit.DLC	= 8;

		ECanaMboxes.MBOX30.MSGID.all         =0x4191;
		ECanaMboxes.MBOX30.MSGID.bit.IDE		= 1;
		ECanaMboxes.MBOX30.MSGID.bit.AME		=1;
		ECanaMboxes.MBOX30.MSGCTRL.bit.DLC	= 8;

		ECanaMboxes.MBOX31.MSGID.all         =0x8000077F;
		ECanaMboxes.MBOX31.MSGID.bit.AME		=1;
		ECanaMboxes.MBOX31.MSGCTRL.bit.DLC	= 8;
		ECanaLAMShadow.LAM31.all             = 0;
		ECanaLAMShadow.LAM31.bit.LAMI	    = 0;
		ECanaLAMRegs.LAM31.all	            = ECanaLAMShadow.LAM31.all;             //控制器参数给定
		// Enable all Mailboxes	 */
		ECanaRegs.CANME.all = 0xFFFFFFFF;
		EDIS;
		//canaMbxSlotCtrlint();
		canadebugMbxSlotCtrlint();
}

void canaunpackMbx16(void)//4180
{
	canb_cmd.canMBX0index  = ECanaMBoxData[16].MDL.byte.BYTE3;
	canb_cmd.canMBX0value  = (ECanaMBoxData[16].MDL.byte.BYTE2 | (ECanaMBoxData[16].MDL.byte.BYTE1<<8));
	canb_cmd.canMBX0value1 = ECanaMBoxData[16].MDL.byte.BYTE0 | (ECanaMBoxData[16].MDH.byte.BYTE7<<8);
    switch(canb_cmd.canMBX0index)
    {
    case 0:
    	sccw1.field.run_enable = canb_cmd.canMBX0value;
    	break;
    case 1:
    	sysCfgPara.IdCmd_man = canb_cmd.canMBX0value;
    	break;
    case 2:
    	sysCfgPara.IqCmd_man = canb_cmd.canMBX0value;
    	break;

    case 7:
    	sysCfgPara.VdCmd_man   =canb_cmd.canMBX0value;        //
    	break;
    case 8:
    	sysCfgPara.VqCmd_man   =canb_cmd.canMBX0value;
    	break;
    case 9:
    	sysCfgPara.CANA_TrqCmd_man      =canb_cmd.canMBX0value;        //
    	break;
    case 10:
    	sysCfgPara.CANA_SpdCmd_man     =canb_cmd.canMBX0value;        //
    	break;
    case 13:
    	sccw1.field.vdc_mode       =canb_cmd.canMBX0value;     //vdc_mode
    	sccw2.field.vdc_mode       =sccw1.field.vdc_mode;
    	break;
    case 14:
    	sysCfgPara.Vdc_man.value   =canb_cmd.canMBX0value;
    	break;
    case 15:
    	sccw1.field.theta_mode     =canb_cmd.canMBX0value;    //theta_mode
    	sccw2.field.theta_mode     =sccw1.field.theta_mode;
    	break;
    case 16:
    	sysCfgPara.Theta_man.value =canb_cmd.canMBX0value;
    	break;
    case 17:
    	sysCfgPara.Omega_man.value =canb_cmd.canMBX0value;
        break;
    case 18:
    	sccw1.field.runLoop_mode   =canb_cmd.canMBX0value;     //runLoop_mode
    	break;
	case 19:
		sccw1.field.vdqhmi_mode     =canb_cmd.canMBX0value;
		break;
    case 23:
    	sccw2.field.diag_enable		=canb_cmd.canMBX0value;
    	break;
    case 24:
    	sccw2.field.diag_mode		=canb_cmd.canMBX0value;
    	break;
    case 25:
    	scsw2.field.MTPA_mode       =canb_cmd.canMBX0value;
    	break;
    case 26:
    	sysCfgPara.IsCmd_man        =canb_cmd.canMBX0value;
    	break;
    case 27:
    	sysCfgPara.MTPAtheta_man    =canb_cmd.canMBX0value;
    	break;
    case 28:
    	sccw1.field.eeprom_mode     =canb_cmd.canMBX0value;
    	break;
    case 29:
    	eeprom_readnum              =canb_cmd.canMBX0value;
        break;
    case 30:
    	sccw1.field.eeprom_recall   =canb_cmd.canMBX0value;
    	break;
    case 31:
    	canambxCfg[0].Enable   =canb_cmd.canMBX0value;
    	break;
    case 32:
    	canambxCfg[1].Enable   =canb_cmd.canMBX0value;
    	break;
    case 33:
    	canambxCfg[2].Enable   =canb_cmd.canMBX0value;
    	break;
    case 34:
    	canambxCfg[3].Enable   =canb_cmd.canMBX0value;
    	break;
    case 35:
    	canambxCfg[4].Enable   =canb_cmd.canMBX0value;
    	break;
    case 36:
    	canambxCfg[5].Enable   =canb_cmd.canMBX0value;
    	break;
    case 37:
    	canambxCfg[6].Enable   =canb_cmd.canMBX0value;
    	break;
    case 38:
		Vehicle_cmd.cmdmode.data.gear_state   =canb_cmd.canMBX0value;
    	break;
    case 39:
    	sccw1.field.error_clr  =canb_cmd.canMBX0value;
    	break;
    case 40:
    	eeprom_readnum         =canb_cmd.canMBX0value;
    	if(canb_cmd.canMBX0value<240)
    	{
    		EV_MCU_Para.value[canb_cmd.canMBX0value]= epdw.value[canb_cmd.canMBX0value]= canb_cmd.canMBX0value1;
        	eeprom_record_enable += 1;
        	eeprom_record_index[eeprom_record_enable-1] = canb_cmd.canMBX0value;
    	}
    	break;
    case 41:
    	can_debugmode          =canb_cmd.canMBX0value;
    	break;
	case 42:
		//sysCfgPara.Spd_deta = canb_cmd.canMBX0value;
		break;
	case 43:
		EV_MCU_Para.field.Error_rest				= epdw.field.Error_rest 					 = canb_cmd.canMBX0value;
		break;
    default:
    	break;
    }
 }

#if 0
void canaunpackMbx17(void)//4181
{
	canb_cmd.canMBX1index=ECanaMBoxData[17].MDL.byte.BYTE3;
	canb_cmd.canMBX1value=ECanaMBoxData[17].MDL.byte.BYTE2 | (ECanaMBoxData[17].MDL.byte.BYTE1<<8);
	if(canb_cmd.canMBX1index<220)
	{
		EV_MCU_Para.value[canb_cmd.canMBX1index]= epdw.value[canb_cmd.canMBX1index]= canb_cmd.canMBX1value;
    	eeprom_record_enable += 1;
    	eeprom_record_index[eeprom_record_enable-1] = canb_cmd.canMBX1index;
	}
	if(canb_cmd.canMBX1index == 210 )
	{
		//invPara.PWM_NzTime.value  = EV_MCU_Para.field.PWM_Period-200;
	}
	if(canb_cmd.canMBX1index == 0 || canb_cmd.canMBX1index == 1)
	{
		//SysBase.mtr_ratio                           = EV_MCU_Para.field.Motor_Np/EV_MCU_Para.field.Encoder_Np;
	}
}
#endif

void canaunpackMbx0(void)//控制器<--仪表
{
	//车速模式
	canarev_cmd.cmdmode1.wValue = ECanaMBoxData[0].MDH.word.LOW_WORD;
}

void canaunpackMbx1(void)//BMS-->MCU 0x182127F4-200ms
{
	Uint16 tmp = 0;
	
	canarev_cmd.soc_ref = (ECanaMBoxData[1].MDL.byte.BYTE3<<1)/5;//SOC
	tmp = ECanaMBoxData[1].MDH.byte.BYTE4;
	tmp = ((tmp<<8)|ECanaMBoxData[1].MDH.byte.BYTE6);
	canarev_cmd.cmdmode2.wValue = tmp;
	
}

void canaunpackMbx2(void)//BMS-->MCU 0x182427F4-200ms
{
	if((0 != ECanaMBoxData[2].MDL.byte.BYTE3)
	 ||(0 != ECanaMBoxData[2].MDL.byte.BYTE2)
	 ||(0 != ECanaMBoxData[2].MDL.byte.BYTE1))
	{
		alarm.field.V12_ov = 1;
	}
}

void canaunpackMbx3(void)//T-BOX1 0x461
{
	//T-BOX生命信号
}

void canaunpackMbx4(void)//ABS2 0x090
{
	
}

void canaunpackMbx5(void)//BMSS 0x3B5
{
	Uint16 tmp = 0;
	
	//慢充充电线状态
	tmp = canarev_cmd.cmdmode1.wValue;
	tmp = (tmp&0x00FF);
	tmp = (tmp|(ECanaMBoxData[5].MDH.byte.BYTE4<<8));
	canarev_cmd.cmdmode1.wValue = tmp;
	
}

void canareceive(void)      //can接收
{
	if(EV_MCU_Para.field.TrqCmd_SourceType == TRQCMD_SOURCE_ECANA)//TRQCMD_SOURCE_ECANA=1,      //判断canA还是canB
	                                                              //diagFSM.c(213)diagEncOffsetexactTuneEntrance(void),
	                                                              //sysFSM.c(794)sysDiagnosisExport(),
	                                                              //variableInit.c(490)(887)Default_ParaInt(void)
	{
		if(ECanaRegs.CANRMP.all==0)     //CANRMP接收信息悬挂寄存器 若邮箱n中接收到一条信息,则CANRMP[n]位置
		{
			canaerr_cnt++;
			if(canaerr_cnt>=600)        //超过600次，can总线接收错误
			{
				//emsw2.field.can_fault = 1;
				canaerr_cnt=650;
			}
		}
		else
		{
			if((emsw2.field.can_fault == 1)&&(sccw2.field.fault_recover == 1))      //如果故障标记为1且故障恢复标记为1
			{
				emsw2.field.can_fault = 0;      //消除这个故障标记
			}
			canaerr_cnt = 0;        //can错误计数值为0
		}
	 }
	 else
	 {
		canaerr_cnt  = 0;
		emsw2.field.can_fault = 0;
	 }
	 //if(0 == can_debugmode)                   //  尝试邮箱能不能发送
	 {
		if ((ECanaRegs.CANRMP.all & 0x0001)==0x001)     //CANRMP接收信息悬挂寄存器 测试有没有接收到信息
		{
			canReceiveDriver(CAN_A_INDEX, 0, &ECanaMBoxData[0]);
			ECanaRegs.CANRMP.all=0x0001;
			canaunpackMbx0();
		}

		if ((ECanaRegs.CANRMP.all & 0x0002)==0x002)
		{
			canReceiveDriver(CAN_A_INDEX, 1, &ECanaMBoxData[1]);
			ECanaRegs.CANRMP.all=0x0002;
			canaunpackMbx1();
		}

		if ((ECanaRegs.CANRMP.all & 0x0004)==0x004)
		{
			canReceiveDriver(CAN_A_INDEX, 2, &ECanaMBoxData[2]);
			ECanaRegs.CANRMP.all=0x0004;
			canaunpackMbx2();
		}

		/*if ((ECanaRegs.CANRMP.all & 0x0008)==0x008)
		{
			canReceiveDriver(CAN_A_INDEX, 3, &ECanaMBoxData[3]);
			ECanaRegs.CANRMP.all=0x0008;
			canaunpackMbx3();
		}

		if ((ECanaRegs.CANRMP.all & 0x0010)==0x010)
		{
			canReceiveDriver(CAN_A_INDEX, 4, &ECanaMBoxData[4]);
			ECanaRegs.CANRMP.all=0x0010;
			canaunpackMbx4();
		}

		if ((ECanaRegs.CANRMP.all & 0x0020)==0x020)
		{
			canReceiveDriver(CAN_A_INDEX, 5, &ECanaMBoxData[5]);
			ECanaRegs.CANRMP.all=0x0020;
			canaunpackMbx5();
		}*/
	}

    //调试CAN  CANa邮箱16开始
    if ((ECanaRegs.CANRMP.all & 0x10000)==0x10000)
	{
    	canReceiveDriver(CAN_A_INDEX, 16, &ECanaMBoxData[16]);
		ECanaRegs.CANRMP.all|=0x10000;
		canaunpackMbx16();
	}
    if ((ECanaRegs.CANRMP.all & 0x80000000)==0x80000000)                //bootloader can烧写  不能更改
	{
	    canReceiveDriver(CAN_A_INDEX, 31, &ECanaMBoxData[31]);
	    ECanaRegs.CANRMP.all|=0x80000000;
		if(0 == scsw2.field.igbt_state)
		{
			canaunpackMbx31();
		}
	}
}

void packcanaMbx18(void)//4185
{
	ECanaMBoxData[18].MDL.word.LOW_WORD     	=   crtLoopPara.Id_ref_HMI;//crtLoop.c(271)crtLoopOutput(void),(305)crtLoopParaClr(void)
	ECanaMBoxData[18].MDL.word.HI_WORD      	=   crtLoopPara.Iq_ref_HMI;//crtLoop.c(272)crtLoopOutput(void),(306)crtLoopParaClr(void)
	ECanaMBoxData[18].MDH.word.LOW_WORD         =   sysFbkPara.Id_fbk_HMI;//task.c(640)hmi_display(void)
	ECanaMBoxData[18].MDH.word.HI_WORD      	=   sysFbkPara.Iq_fbk_HMI;//task.c(641)hmi_display(void)
}

void packcanaMbx19(void)//4186
{
	ECanaMBoxData[19].MDL.word.LOW_WORD     	=   voltLoopPara.Vd_ref_HMI>>1;/*电压环输入参数*///voltLoop.c(669)voltLoopOutput(void),(683)voltLoopParaClr()
	ECanaMBoxData[19].MDL.word.HI_WORD      	=   voltLoopPara.Vq_ref_HMI>>1;/*电压环输入参数*///voltLoop.c(670)voltLoopOutput(void),(684)voltLoopParaClr()
	ECanaMBoxData[19].MDH.word.LOW_WORD         =   voltLoopPara.Vdq_HMI>>1;/*电压环输出参数*///voltLoop.c(675)voltLoopOutput(void)
	ECanaMBoxData[19].MDH.word.HI_WORD      	=   sysCfgPara.Vdc_HMI;/**///sysConfig.c(136)vdcctrl(void)
}

void packcanaMbx20(void)//4187
{
	if(ELECTRIC_MODE == trqLoop_EleGenMode)//判断是电动还是发电
	{
		ECanaMBoxData[20].MDL.word.LOW_WORD     	=   trqLoopPara.Max_TorqueEle_HMI;/*力矩环输出参数*///trqLoop.c(10)trqLoop_Init(void)
	}
	else
	{
		ECanaMBoxData[20].MDL.word.LOW_WORD     	=   trqLoopPara.Max_TorqueGen_HMI;/*力矩环输出参数*///trqLoop.c(11)trqLoop_Init(void)
	}
    ECanaMBoxData[20].MDL.word.HI_WORD      	    =   EV_MCU_Para.field.Motor_Encoder_offset;/*电机编码器偏移量*///mtr_learnexactoffset.c (40~110),
                                                                                                                 //mtr_learnoffset.c(30~43),
                                                                                                                 //variableInit.c(397~399)
	ECanaMBoxData[20].MDH.word.LOW_WORD             =   _IQtoIQ15(sysCfgPara.SpdCmd);//mtr_learnexactoffset.c(89),
	                                                                                 //mtr_learnfluex.c(196),
	                                                                                 //sysConfig.c(91~94),
	                                                                                 //sysFSM.c(346~408),
	                                                                                 //variableInit.c(793)
	ECanaMBoxData[20].MDH.word.HI_WORD      	    =   sysFbkPara.Speed_fbk_HMI;//feedback.c(2007)
}

void packcanaMbx21(void)//4188
{
	ECanaMBoxData[21].MDL.byte.BYTE3         =   ((EV_MCU_Para.field.SpdCmd_SourceType<<6)|(EV_MCU_Para.field.TrqCmd_SourceType<<4)|(sccw1.field.runLoop_mode<<1)|(sccw1.field.run_enable));
	                                                //trqLoop_Init(10)(11)
	ECanaMBoxData[21].MDL.byte.BYTE2         =   ((scsw1.field.system_state<<4)|(scsw2.field.runLoop_state<<1)|(scsw2.field.igbt_state));
	ECanaMBoxData[21].MDL.word.HI_WORD      =   sysFbkPara.Temp_igbt;//feedback.c(1969)
	ECanaMBoxData[21].MDH.word.LOW_WORD      =   sysFbkPara.Idc_fbk_HMI;//task.c(642)
	ECanaMBoxData[21].MDH.word.HI_WORD      	=   sysFbkPara.Is_fbk_HMI_fiter;//faultdetect.c(88)
}
void packcanaMbx22(void)//4189
{
	if(eeprom_displaycnt_cana>239)
	{
		eeprom_displaycnt_cana =0;
	}
	ECanaMBoxData[22].MDL.word.LOW_WORD     	=   eeprom_displaycnt_cana;
	ECanaMBoxData[22].MDL.word.HI_WORD      	=   epdw.value[eeprom_displaycnt_cana];
	eeprom_displaycnt_cana++;

	ECanaMBoxData[22].MDH.word.LOW_WORD      	=   epdw.value[eeprom_readnum];
	ECanaMBoxData[22].MDH.word.HI_WORD      	=   (EV_MCU_Para.field.Motor_Encoder_offset<<2)|(EV_MCU_Para.field.Motor_Encoder_Direction<<1)|EV_MCU_Para.field.Motor_Direction;
}

void packcanaMbx23(void)//418A
{
	ECanaMBoxData[23].MDL.word.LOW_WORD     	=   (_IQtoIQ15(mtrfwdvars.IdPid_Out_fwd))>>1;//mtrfwd.c(59)
	ECanaMBoxData[23].MDL.word.HI_WORD      	=   (_IQtoIQ15(mtrfwdvars.IqPid_Out_fwd))>>1;//mtrfwd.c(60)
	ECanaMBoxData[23].MDH.word.LOW_WORD         =   sysFbkPara.Temp_mtr;//feedback.c(1728~1808)
	ECanaMBoxData[23].MDH.word.HI_WORD      	=   vehicle_io_state.wValue;//_IQtoIQ15(trqidqmap_absSpdFbk);task.c(469),variableInit.c(832)
}
void packcanaMbx24(void)//418B
{
	ECanaMBoxData[24].MDL.word.LOW_WORD     	=   emsw.wValue;//variableInit.c(687)SysCtrlVar_Init(void),task.c(482)task10msCtrl(void)
	ECanaMBoxData[24].MDL.word.HI_WORD      	=   emsw1.wValue;//variableInit.c(688)SysCtrlVar_Init(void),task.c(483)task10msCtrl(void)
	ECanaMBoxData[24].MDH.word.LOW_WORD         =   emsw2.wValue;//variableInit.c(689)SysCtrlVar_Init(void),task.c(484)task10msCtrl(void)
	ECanaMBoxData[24].MDH.word.HI_WORD      	=   alarm.wValue;//variableInit.c(693)SysCtrlVar_Init(void),task.c(488)task10msCtrl(void)
}
void packcanaMbx25(void)//418C
{
	ECanaMBoxData[25].MDL.word.LOW_WORD     	=   trqLoopPara.Torque_ref_HMI;//trqLoop.c(107)(171)
	
	Motor_VarPara.field.DI = vehicle_io_state.wValue;//task.c(469),variableInit.c(832)
	Motor_VarPara.field.IU_AD = sysFbkPara.Iu_fbk_ad;//CARMC_ctrl.c(31)
	Motor_VarPara.field.IV_AD = sysFbkPara.Iv_fbk_ad;//CARMC_ctrl.c(32)
	Motor_VarPara.field.IW_AD = sysFbkPara.Iw_fbk_ad;//CARMC_ctrl.c(33)
	Motor_VarPara.field.VPN_AD = sysFbkPara.Vdc_fbk_ad;//CARMC_ctrl.c(34)
	Motor_VarPara.field.OV_REF = 0;
	Motor_VarPara.field.T_IGBT_AD = sysFbkPara.Temp_mdl_ad;//feedback.c(10)
	Motor_VarPara.field.AI3_AD = 0;
	Motor_VarPara.field.TEMP_M2_AD = 0;
	Motor_VarPara.field.TEMP_M1_AD = sysFbkPara.Temp_mtr_ad;//feedback.c(11)(1690)
	Motor_VarPara.field.V12_ADC = 0;
	Motor_VarPara.field.AI1_AD = sysFbkPara.AI1_fbk_ad;//feedback.c(8)
	Motor_VarPara.field.V5_ADC = 0;
	Motor_VarPara.field.AI2_AD = sysFbkPara.AI2_fbk_ad;//feedback.c(9)
	Motor_VarPara.field.V5_fbk_HMI = 0;
	Motor_VarPara.field.V12_fbk_HMI = sysFbkPara.V12_fbk_HMI;//feedback.c(1229)(1318)
	Motor_VarPara.field.AI4_AD = 0;
	Motor_VarPara.field.TA_AD = sysFbkPara.TA_ad;//??
	Motor_VarPara.field.I_DC_AD = 0;
	Motor_VarPara.field.AI_fbk_offset = 0;
	display_var_index = display_var_index%20;
	ECanaMBoxData[25].MDL.word.HI_WORD          =   display_var_index;
	ECanaMBoxData[25].MDH.word.LOW_WORD         =   Motor_VarPara.value[display_var_index];
	display_var_index++;
	
	ECanaMBoxData[25].MDH.word.HI_WORD         	=   sysCfgPara.Theta_ele_HMI;//sysConfig.c(58)
}

void packcanaMbx26(void)//418D
{
	ECanaMBoxData[26].MDL.word.LOW_WORD         =   sysFbkPara.AI1_fbk_ad;//feedback.c(8)
	ECanaMBoxData[26].MDL.word.HI_WORD          =   sysFbkPara.AI2_fbk_ad;//feedback.c(9)
	ECanaMBoxData[26].MDH.word.LOW_WORD         =   _IQtoIQ15(spdLoopPara.Iqcmd_real);//spdLoop.c(211)(190~197)
	ECanaMBoxData[26].MDH.word.HI_WORD          =   _IQtoIQ15(spdLoopPara.Ui);//spdLoop.c(219)(157~168)
}
void packcanaMbx27(void)//418E
{
	ECanaMBoxData[27].MDL.word.LOW_WORD         =   _IQtoIQ15(crtLoopPara.Id_PI_Vd);//crtLoop.c(122)(200)
	ECanaMBoxData[27].MDL.word.HI_WORD          =   _IQtoIQ15(crtLoopPara.IdPid_Ui);//crtLoop.c(103~104)(195~196)(285)
	ECanaMBoxData[27].MDH.word.LOW_WORD         =   _IQtoIQ15(crtLoopPara.Iq_PI_Vq);//crtLoop.c(161)(243)(298)
	ECanaMBoxData[27].MDH.word.HI_WORD          =   _IQtoIQ15(crtLoopPara.IqPid_Ui);//crtLoop.c(142~143)(238~239)(295)
}

void packcanaMbx28(void)//418F
{
	ECanaMBoxData[28].MDL.word.LOW_WORD = _IQtoIQ15(weakcrtLoopvars.weakIdPid_Outfilter);//weakcrtLoop.c(57)
	ECanaMBoxData[28].MDL.word.HI_WORD  = _IQtoIQ15(weakcrtLoopvars.weakIdPid_Ui);//weakcrtLoop.c(59)
	ECanaMBoxData[28].MDH.word.LOW_WORD = diagEncoffsetVar.Motor_Encoder_offset>>12;//mtr_learnoffset.c(70)(362)
	ECanaMBoxData[28].MDH.word.HI_WORD = trqLoopPara.Torque_fbk_HMI;//feedback.c(1359)(1363),trqLoop.c(153)
#if 0
	if(eeprom_error_recall_cnt_cana>80)
	{
		eeprom_error_recall_cnt_cana =0;
	}
	ECanaMBoxData[28].MDL.word.LOW_WORD     	=   eeprom_error_recall_cnt_cana;//4
	ECanaMBoxData[28].MDL.word.HI_WORD      	=   I2CErr_Record.value[eeprom_error_recall_cnt_cana];//
	eeprom_error_recall_cnt_cana++;

	if(eeprom_ValueLimit_cnt_cana>6)
	{
		eeprom_ValueLimit_cnt_cana =0;
	}
	ECanbMBoxData[28].MDH.word.LOW_WORD         =   eeprom_ValueLimit_cnt_cana;
	ECanbMBoxData[28].MDH.word.HI_WORD      	=   I2CValueLimit_Record.value[eeprom_ValueLimit_cnt_cana];
	eeprom_ValueLimit_cnt_cana++;
#endif
}

void packcanaMbx29(void)//4190
{
#if 1
	ECanaMBoxData[29].MDL.word.LOW_WORD 		=	0;//17
	ECanaMBoxData[29].MDL.word.HI_WORD			=	0;//17
	ECanaMBoxData[29].MDH.word.LOW_WORD 		=	0;//17
	ECanaMBoxData[29].MDH.word.HI_WORD			=	0;//17
#else
	if(1 == osc.OSC_enable)
	{
		if(1 == osc.transmitFlag)
		{
			ECanaMBoxData[29].MDL.word.LOW_WORD 		=	osc.view0[osc.point];//17
			ECanaMBoxData[29].MDL.word.HI_WORD			=	osc.view1[osc.point];//17
			ECanaMBoxData[29].MDH.word.LOW_WORD 		=	osc.view2[osc.point];//17
			ECanaMBoxData[29].MDH.word.HI_WORD			=	osc.view3[osc.point];//17
			osc.point++;
			if(osc.point>=OSC_NUM)
			{
				osc.transmitFlag = 0;
				osc.point = 0;
			}
		}
		else
		{
			ECanaMBoxData[29].MDL.word.LOW_WORD 		=	0;//17
			ECanaMBoxData[29].MDL.word.HI_WORD			=	0;//17
			ECanaMBoxData[29].MDH.word.LOW_WORD 		=	0;//17
			ECanaMBoxData[29].MDH.word.HI_WORD			=	0;//17
		}
	}
	else
	{
		ECanaMBoxData[29].MDL.word.LOW_WORD 		=	0;//17
		ECanaMBoxData[29].MDL.word.HI_WORD			=	0;//17
		ECanaMBoxData[29].MDH.word.LOW_WORD 		=	0;//17
		ECanaMBoxData[29].MDH.word.HI_WORD			=	0;//17
	}
#endif
}

void packcanaMbx30(void)//4191
{
	if(eeprom_error_recall_cnt_cana>80)
	{
		eeprom_error_recall_cnt_cana =0;
	}
	ECanaMBoxData[30].MDL.word.LOW_WORD     	=   eeprom_error_recall_cnt_cana;//4
	ECanaMBoxData[30].MDL.word.HI_WORD      	=   I2CErr_Record.value[eeprom_error_recall_cnt_cana];
	eeprom_error_recall_cnt_cana++;

	if(eeprom_ValueLimit_cnt_cana>6)
	{
		eeprom_ValueLimit_cnt_cana =0;
	}
	ECanaMBoxData[30].MDH.word.LOW_WORD         =   eeprom_ValueLimit_cnt_cana;
	ECanaMBoxData[30].MDH.word.HI_WORD      	=   I2CValueLimit_Record.value[eeprom_ValueLimit_cnt_cana];
	eeprom_ValueLimit_cnt_cana++;
}

void packcanaMbx11(void)//4192
{
	
	ECanaMBoxData[11].MDL.word.LOW_WORD 		=	0;
	ECanaMBoxData[11].MDL.word.HI_WORD			=	0;
	ECanaMBoxData[11].MDH.word.LOW_WORD 		=	sysFbkPara.Temp_ta;//feedbback.c(1698)
	ECanaMBoxData[11].MDH.word.HI_WORD			=	sysFbkPara.AI1_fbk_ad;//ipark_angle_str.Ipark_angle_out_filter>>12;//feedbback.c(8)
}

void packcanaMbx8(void)//0x18ef0103 控制器-->仪表
{
	_iq rpm = 0;        //转速
	_iq speed = 0;      //速度
	_iq current = 0;        //电流
	_iq speed_rate = 0;     //速率

	if(0x00 == ECanaMBoxData[8].MDL.byte.BYTE2)
	{
		if(0xa0 == ECanaMBoxData[8].MDL.byte.BYTE3)     //发送电机转速、车速   0xa0表示汉字的开始
		{
			ECanaMBoxData[8].MDL.byte.BYTE3 = 0xa0;
			ECanaMBoxData[8].MDL.byte.BYTE2 = 0x07;
			//电机转速
			rpm = sysFbkPara.Speed+10000;//feedback.c(2009) 速度+10000  得出速度
			ECanaMBoxData[8].MDL.byte.BYTE1 = ((rpm>>8)&0xFF);      //存入高八位值
			ECanaMBoxData[8].MDL.byte.BYTE0 = (rpm&0xFF);       //存入低八位值

			ECanaMBoxData[8].MDH.word.LOW_WORD = 0x0000;        //字符流传送，两个16字节，以下都是计算打包发送环节，数据都存到ECanaMBoxData[8]中
			
			//车速
			if(1 == vehicle_io_state.data.DI6_DSP)      //低速档 (DI6_DSP为刹车)
			{
				speed_rate = _IQ15(28.7337142);//IQ15
			}
			else//高速档
			{
				speed_rate = _IQ15(12.5714285);//IQ15
			}
			speed = _IQ15div(_IQ15mpy(_IQ15(1.2064),_IQ15(sysFbkPara.Speed_abs)),speed_rate);//IQ15,sysFbkPara.Speed_abs->feedback.c(2010)
			speed = speed>>15;//IQ0
			ECanaMBoxData[8].MDH.byte.BYTE5 = ((speed>>8)&0xFF);
			ECanaMBoxData[8].MDH.byte.BYTE4 = (speed&0xFF);
		}
		else//发送开关量
		{
			ECanaMBoxData[8].MDL.byte.BYTE3 = 0xa0;
			ECanaMBoxData[8].MDL.byte.BYTE2 = 0x00;
			ECanaMBoxData[8].MDL.byte.BYTE1 = 0x00;
			//开关量
			canafbk_state.wValue = (canafbk_state.wValue&0xFF00);
			canafbk_state.data.gear_state = Vehicle_cmd.cmdmode.data.gear_state;//276,feedback.c(146~157)
			canafbk_state.data.brk_state = Vehicle_cmd.cmdmode.data.brake_state;//715,feedback.c(68~75)
			canafbk_state.data.drive_state = ((vehicle_io_state.data.DI1_WAKEUP_DSP)|(vehicle_io_state.data.DI3_DSP<<1));//task.c(439)(441)
			ECanaMBoxData[8].MDL.byte.BYTE0 = canafbk_state.wValue;
			ECanaMBoxData[8].MDH.word.LOW_WORD = 0x0000;
			ECanaMBoxData[8].MDH.word.HI_WORD = 0x0000;
		}
	}
	else if(0x07 == ECanaMBoxData[8].MDL.byte.BYTE2)//发送电控电流
	{
		ECanaMBoxData[8].MDL.byte.BYTE3 = 0xa0;
		ECanaMBoxData[8].MDL.byte.BYTE2 = 0x08;
		//电控电流
		current = _IQ15mpy(_IQ15div(sysFbkPara.Is_fbk_HMI_fiter,_IQ15(1.414214)),SysBase.iphase);//sysFbkPara.Is_fbk_HMI_fiter->faultdetect.c(88),SysBase.iphase->variableInit.c(716)
		ECanaMBoxData[8].MDL.byte.BYTE1 = ((current>>8)&0xFF);
		ECanaMBoxData[8].MDL.byte.BYTE0 = (current&0xFF);
		ECanaMBoxData[8].MDH.word.LOW_WORD = 0x0000;
		ECanaMBoxData[8].MDH.word.HI_WORD = 0x0000;
	}
	else if(0x08 == ECanaMBoxData[8].MDL.byte.BYTE2)//动力电池电量
	{
		ECanaMBoxData[8].MDL.byte.BYTE3 = 0xa0;
		ECanaMBoxData[8].MDL.byte.BYTE2 = 0x0b;
		ECanaMBoxData[8].MDL.word.HI_WORD = 0x0000;
		ECanaMBoxData[8].MDH.word.LOW_WORD = 0x0000;
		ECanaMBoxData[8].MDH.byte.BYTE5 = canarev_cmd.soc_ref;//336
		ECanaMBoxData[8].MDH.byte.BYTE4 = 0x00;
	}
	else//发送故障
	{
		ECanaMBoxData[8].MDL.byte.BYTE3 = 0xa3;
		ECanaMBoxData[8].MDL.byte.BYTE2 = 0x00;
		MCU_err_status1.wValue = 0x0000;
		if(1 == alarm.field.udc_lv)/*UDC下限报警（电机允许运行时）*/
		{
			MCU_err_status1.data.udc_lackvolt = 1;
		}
		if(1 == alarm.field.udc_ov)/*UDC上限报警*/
		{
			MCU_err_status1.data.udc_ovvolt = 1;
		}
		if(1 == emsw.field.vdc_overVolt) /*bit2:直流过压*///faultdetect.c(148,155)Vdc_faultDetect()
		{
			MCU_err_status1.data.udc_ovvolt_heavy = 1;
		}
		if(1 == emsw.field.mtr_temp)/*bit6:电机过温*///feedback.c(1780,1787,1805)tempFeedback()
		{
			MCU_err_status1.data.mtr_temp = 1;
		}
		if(1 == emsw.field.ac_current)/*bit13:12V弱电故障*///faultdetect.c(93,100,106)OverLoad_faultDetect(void)
		{
			MCU_err_status1.data.mcu_overcurrent = 1;
		}
		if((1 == emsw1.field.APHsensor_error)
		 ||(1 == emsw1.field.BPHsensor_error)
		 ||(1 == emsw1.field.CPHsensor_error)
		 ||(1 == emsw1.field.Crt_detect_fault))/*四个传感器中有错误*///faultdetect.c(359,380)CrtDetectProtect()
		{
			MCU_err_status1.data.current_sensor_fault = 1;
		}
		if(1 == emsw.field.resolver_error)/*bit9:旋变硬件故障*///feedback.c(1870)ThetaCal_Resolver()
		{
			MCU_err_status1.data.resolver_fault = 1;
		}
		if(1 == emsw1.field.open_phase)//faultdetect.c(568,577)MtrOpenPhaseProtect()(#if 0)
		{
			MCU_err_status1.data.open_phase = 1;
		}
		if(1 == alarm.field.mdl_temp)/*模块温度报警*/
		{
			MCU_err_status1.data.mdl_temp_high = 1;
		}
		if(1 == emsw.field.mdl_temp)/*bit5:模块过温*///feedback.c(1715,1722)tempFeedback()
		{
			MCU_err_status1.data.mdl_temp_high_heavy = 1;
		}
		if(1 == emsw.field.vdc_lackVolt)/*bit3:直流欠压*///faultdetect.c(163,167)Vdc_faultDetect()
		{
			MCU_err_status1.data.udc_lackvolt_heavy = 1;
		}
		ECanaMBoxData[8].MDL.word.HI_WORD = MCU_err_status1.wValue;//747,=0x0000

		MCU_err_status2.wValue = 0x0000;
		if(1 == emsw.field.eeprom_fault)/*bit11:EEPROM故障*///variableInit.c(35)GlobalVar_Init(void)
		{
			MCU_err_status2.data.eeprom_fault = 1;
		}
		if(1 == emsw2.field.can_fault)/*can发现错误*///392,400,canareceive(void)
		{
			MCU_err_status2.data.can_fault = 1;
		}
		if(1 == emsw2.field.DIAG_ENCODER_error)/*?编码器错误*///mtr_learnexactoffset.c(87)diagexactEncOffsetTuneAct_output(),mtr_learnoffset.c(38)diagEncOffsetTuneAct_output()
		{
			MCU_err_status2.data.diagstudy_error = 1;
		}
		if((1 == emsw1.field.mtrsensor_short_error)//feedback.c(1774,1788,1806)tempFeedback(void)
		 ||(1 == emsw1.field.mtrsensor_open_error))//feedback.c(1769,1789,1807)tempFeedback(void)
		{
			MCU_err_status2.data.mtr_sensor_fault = 1;
		}
		if((1 == emsw1.field.V12_lackVolt)/*欠压*///feedback.c(1342,1349)v12Feedback(void)
		 ||(1 == emsw1.field.V12_overVolt))/*过压*///feedback.c(1329,1336)v12Feedback(void)
		{
			MCU_err_status2.data.v12_fault = 1;
		}
		ECanaMBoxData[8].MDH.word.LOW_WORD = MCU_err_status2.wValue;//797,=0x0000

		MCU_err_status3.wValue = 0x0000;
		ECanaMBoxData[8].MDH.word.HI_WORD = MCU_err_status3.wValue;//822,=0x0000
	}
}

void packcanaMbx9(void)//0x18ef0301 控制器-->仪表
{
	_iq tmp = 0;

	ECanaMBoxData[9].MDL.byte.BYTE3 = 0xa0;
	ECanaMBoxData[9].MDL.byte.BYTE2 = 0x00;
	ECanaMBoxData[9].MDL.word.HI_WORD = 0x0000;

	canafbk_state.data.speed_mode = 0;
	if(1 == vehicle_io_state.data.DI6_DSP)//低速
	{
		canafbk_state.data.speed_mode = 3;
	}
	else
	{
		canafbk_state.data.speed_mode = 1;
	}
	
	ECanaMBoxData[9].MDH.byte.BYTE7 = ((canafbk_state.wValue>>8)&0x03);//714,packcanaMbx8(void)
	ECanaMBoxData[9].MDH.byte.BYTE6 = 0x00;
	ECanaMBoxData[9].MDH.word.HI_WORD = 0;
}

void canadebugMbxSlotCtrlint(void)
{
	canambxCfg[0].txEnable=1;       //所有标志位使能设置为1
	canambxCfg[1].txEnable=1;
	canambxCfg[2].txEnable=1;
	canambxCfg[3].txEnable=1;
	canambxCfg[4].txEnable=1;
	canambxCfg[5].txEnable=1;
 	canambxCfg[6].txEnable=1;
   	canambxCfg[7].txEnable=1;
   	canambxCfg[8].txEnable=1;
   	canambxCfg[9].txEnable=1;
   	canambxCfg[10].txEnable=1;
   	canambxCfg[11].txEnable=1;
 	canambxCfg[12].txEnable=1;
   	canambxCfg[13].txEnable=1;
   	canambxCfg[14].txEnable=1;
   	canambxCfg[15].txEnable=1;
   	canambxCfg[16].txEnable=1;
   	canambxCfg[17].txEnable=1;
	canambxCfg[18].txEnable=1;
	canambxCfg[19].txEnable=1;
	canambxCfg[20].txEnable=1;

	canambxCfg[0].prd=40;
	canambxCfg[1].prd=40;
	canambxCfg[2].prd=100;
	canambxCfg[3].prd=2;
	canambxCfg[4].prd=2;
	canambxCfg[5].prd=100;
 	canambxCfg[6].prd=100;
   	canambxCfg[7].prd=20;
   	canambxCfg[8].prd=10;
   	canambxCfg[9].prd=10;
   	canambxCfg[10].prd=20;
   	canambxCfg[11].prd=20;
 	canambxCfg[12].prd=20;
   	canambxCfg[13].prd=20;
   	canambxCfg[14].prd=20;
   	canambxCfg[15].prd=20;
   	canambxCfg[16].prd=20;
   	canambxCfg[17].prd=20;
	canambxCfg[18].prd=20;
	canambxCfg[19].prd=100;
	canambxCfg[20].prd=20;

	canambxCfg[0].cnt=0;
	canambxCfg[1].cnt=0;
	canambxCfg[2].cnt=0;
	canambxCfg[3].cnt=0;
	canambxCfg[4].cnt=0;
	canambxCfg[5].cnt=0;
 	canambxCfg[6].cnt=0;
   	canambxCfg[7].cnt=0;
   	canambxCfg[8].cnt=0;
   	canambxCfg[9].cnt=0;
   	canambxCfg[10].cnt=0;
   	canambxCfg[11].cnt=0;
 	canambxCfg[12].cnt=0;
   	canambxCfg[13].cnt=0;
   	canambxCfg[14].cnt=0;
   	canambxCfg[15].cnt=0;
   	canambxCfg[16].cnt=0;
   	canambxCfg[17].cnt=0;
	canambxCfg[18].cnt=0;
	canambxCfg[19].cnt=0;
	canambxCfg[20].cnt=0;

   	eeprom_displaycnt_cana=0;
   	eeprom_error_recall_cnt_cana=0;
   	eeprom_ValueLimit_cnt_cana=0;
	display_var_index = 0;
}

void canaMbxSlotCtrl(void)      //can邮箱位置控制
{
    int cana_index = 0;     //创建can位置索引，赋初始位置为0
	
	for(cana_index=0;cana_index<21;cana_index++)        //索引小于21进行循环
	{
		if(1 == canambxCfg[cana_index].txEnable)//849,canadebugMbxSlotCtrlint  can调试邮箱位置lint  标志位
		{
			canambxCfg[cana_index].cnt++;       //计数自增
			if(canambxCfg[cana_index].cnt>=canambxCfg[cana_index].prd)
			{
				canambxCfg[cana_index].flag = 1;        //标记赋值1
				canambxCfg[cana_index].cnt = 0;        // 计数赋值0
			}
		}
		else
		{
			canambxCfg[cana_index].flag = 0;        //标记赋值0
			canambxCfg[cana_index].cnt = 0;         // 计数赋值0
		}
	}
}

void canatransmit(void)     //can发送
{
	canaMbxSlotCtrl();//933,canaMbxSlotCtrl     //can邮箱位置控制
	if((1 == canambxCfg[0].txEnable) && (1 == canambxCfg[0].flag))//如果发送使能和标志都是1  //canadebugMbxSlotCtrlint,851//canaMbxSlotCtrl,923
	{
		packcanaMbx8();     //打包can邮箱，把信息放到缓存里
		canTransmitDriver(CAN_A_INDEX, 8, &ECanaMBoxData[8]);       //can发送驱动，存进去几号邮箱，缓冲区的地址
		canambxCfg[0].flag=0;       //canambxCfg[0]标记赋值0
	}
	if((1 == canambxCfg[1].txEnable) && (1 == canambxCfg[1].flag))
	{
		packcanaMbx9();
		canTransmitDriver(CAN_A_INDEX, 9, &ECanaMBoxData[9]);
		canambxCfg[1].flag=0;
	}
	
	if(1 == can_debugmode)
	{
		if((1 == canambxCfg[7].txEnable) && (1 == canambxCfg[7].flag))//4185
		{
			packcanaMbx11();
			canTransmitDriver(CAN_A_INDEX, 11, &ECanaMBoxData[11]);
			canambxCfg[7].flag=0;
		}
		
		if((1 == canambxCfg[8].txEnable) && (1 == canambxCfg[8].flag))//4185
		{
			packcanaMbx18();
			canTransmitDriver(CAN_A_INDEX, 18, &ECanaMBoxData[18]);
			canambxCfg[8].flag=0;
		}

		if((1 == canambxCfg[9].txEnable) && (1 == canambxCfg[9].flag))//4186
		{
			packcanaMbx19();
			canTransmitDriver(CAN_A_INDEX, 19, &ECanaMBoxData[19]);
			canambxCfg[9].flag=0;
		}
		if((1 == canambxCfg[10].txEnable) && (1 == canambxCfg[10].flag))//4187
		{
			packcanaMbx20();
			canTransmitDriver(CAN_A_INDEX, 20, &ECanaMBoxData[20]);
			canambxCfg[10].flag=0;
		}

		if((1 == canambxCfg[11].txEnable) && (1 == canambxCfg[11].flag))//4188
		{
			packcanaMbx21();
			canTransmitDriver(CAN_A_INDEX, 21, &ECanaMBoxData[21]);
			canambxCfg[11].flag=0;
		}
		if((1 == canambxCfg[12].txEnable) && (1 == canambxCfg[12].flag))//4189
		{
			packcanaMbx22();
			canTransmitDriver(CAN_A_INDEX, 22, &ECanaMBoxData[22]);
			canambxCfg[12].flag=0;
		}

		if((1 == canambxCfg[13].txEnable) && (1 == canambxCfg[13].flag))//418A
		{
			packcanaMbx23();
			canTransmitDriver(CAN_A_INDEX, 23, &ECanaMBoxData[23]);
			canambxCfg[13].flag=0;
		}
		if((1 == canambxCfg[14].txEnable) && (1 == canambxCfg[14].flag))//418B
		{
			packcanaMbx24();
			canTransmitDriver(CAN_A_INDEX, 24, &ECanaMBoxData[24]);
			canambxCfg[14].flag=0;
		}
		if((1 == canambxCfg[15].txEnable) && (1 == canambxCfg[15].flag))//418C
		{
			packcanaMbx25();
			canTransmitDriver(CAN_A_INDEX, 25, &ECanaMBoxData[25]);
			canambxCfg[15].flag=0;
		}
		if((1 == canambxCfg[16].txEnable) && (1 == canambxCfg[16].flag))//418D
		{
			packcanaMbx26();
			canTransmitDriver(CAN_A_INDEX, 26, &ECanaMBoxData[26]);
			canambxCfg[16].flag=0;
		}
		if((1 == canambxCfg[17].txEnable) && (1 == canambxCfg[17].flag))//418E
		{
			packcanaMbx27();
			canTransmitDriver(CAN_A_INDEX, 27, &ECanaMBoxData[27]);
			canambxCfg[17].flag=0;
		}
		if((1 == canambxCfg[18].txEnable) && (1 == canambxCfg[18].flag))//418F
		{
			packcanaMbx28();
			canTransmitDriver(CAN_A_INDEX, 28, &ECanaMBoxData[28]);
			canambxCfg[18].flag=0;
		}
		if((1 == canambxCfg[19].txEnable) && (1 == canambxCfg[19].flag))//4190
		{
			packcanaMbx29();
			canTransmitDriver(CAN_A_INDEX, 29, &ECanaMBoxData[29]);
			canambxCfg[19].flag=0;
		}
		if((1 == canambxCfg[20].txEnable) && (1 == canambxCfg[20].flag))//4191
		{
			packcanaMbx30();
			canTransmitDriver(CAN_A_INDEX, 30, &ECanaMBoxData[30]);
			canambxCfg[20].flag=0;
		}
	}
}

#endif
