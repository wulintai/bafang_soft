#ifndef _EEPROM_C_
#define _EEPROM_C_

#include "include_c.h"

extern void InitI2C(void);

void eeprom_Init(void)
{
	eeprom_record_enable = 0;
	eeprom_readnum = 0;
#if 0
	InitI2CGpio();
	InitI2C();
#else
	EALLOW;
	GpioCtrlRegs.GPBPUD.bit.GPIO32 = 0;  	// Enable pullup on GPIO35   
	GpioDataRegs.GPBSET.bit.GPIO32 = 1;  	// Load output latch   
	GpioCtrlRegs.GPBMUX1.bit.GPIO32 = 0;   	// IO PIN ->DSP_RST
	GpioCtrlRegs.GPBDIR.bit.GPIO32 = 1;  	// GPIO35 = OUTPUT 

	GpioCtrlRegs.GPBPUD.bit.GPIO33 = 0;  	// Enable pullup on GPIO35   
	GpioDataRegs.GPBSET.bit.GPIO33 = 1;  	// Load output latch   
	GpioCtrlRegs.GPBMUX1.bit.GPIO33 = 0;   	// IO PIN ->DSP_RST
	GpioCtrlRegs.GPBDIR.bit.GPIO33 = 1;  	// GPIO35 = OUTPUT 
	EDIS;
#endif
}

/*void eeprom_test(void)
{
	Uint16 i = 0;

	unsigned int write_value = 0;
	unsigned int read_value = 0;

	write_value = 0x3456;
	KickDog();
	I2cWriteData(80,&write_value);
	for(i=0;i<60000;i++)
	{
		asm(" NOP ");
		KickDog();
	}
	write_value = 0x6543;
	I2cWriteData(81,&write_value);
	for(i=0;i<60000;i++)
	{
		asm(" NOP ");
		KickDog();
	}
	read_value = 0;
	I2cReadData(0,&read_value);
	KickDog();
}*/

unsigned int I2cWriteData(unsigned int I2CWrite_Index,unsigned int *I2CWrite_Add)
{
#if 0
	unsigned int write_high = 0;
	unsigned int write_low = 0;

	write_high = (*I2CWrite_Add&0xFF00)>>8;
	write_low = *I2CWrite_Add&0x00FF;
	if(I2caRegs.I2CMDR.bit.STP == 1)
	{
		return I2C_STP_NOT_READY_ERROR;
	}
	//Setup slave address
	I2caRegs.I2CSAR = (ControlCode << 3) + ((I2CWrite_Index*2)>>13);//8192 change page
	//Check if bus is busy
	if(I2caRegs.I2CSTR.bit.BB == 1)
	{
		return I2C_BUS_BUSY_ERROR;
	}
	//Setup number of bytes to send: 2 byte WordAddress + 2 byte MsgBuffer ( not include Slave Address)
	I2caRegs.I2CCNT = 2+2;

	I2caRegs.I2CDXR = ((I2CWrite_Index*2)&0x1F00)>>8;//WordAddress high byte
	I2caRegs.I2CDXR = (I2CWrite_Index*2)&0x00FF;//WordAddress low byte
	
	I2caRegs.I2CDXR = write_low;//MsgBuffer low byte
	I2caRegs.I2CDXR = write_high;//MsgBuffer high byte
	//Send start as master transmitter
	I2caRegs.I2CMDR.all = 0x6E20;

	asm( " NOP " );
	asm( " NOP " );

#else
	Uint16 address = 0;
	Uint16 data = 0;

	address = I2CWrite_Index*2;
	data = *I2CWrite_Add;
	EEPROM_WRITE_WORD(address,data);
#endif
	
	return I2C_SUCCESS;
}

unsigned int I2cReadData(unsigned int I2CRead_Index,unsigned int *I2CRead_Add)
{
#if 0
	unsigned int read_high = 0;
	unsigned int read_low = 0;

	if(I2caRegs.I2CMDR.bit.STP == 1)
	{
	return I2C_STP_NOT_READY_ERROR;
	}
	//Setup slave address
	I2caRegs.I2CSAR = (ControlCode << 3) + ((I2CRead_Index*2)>>13);
	//Check if bus is busy
	if(I2caRegs.I2CSTR.bit.BB == 1)
	{
		return I2C_BUS_BUSY_ERROR;
	}
	I2caRegs.I2CCNT = 2;
	I2caRegs.I2CDXR = ((I2CRead_Index*2)&0x1F00)>>8;//WordAddress high byte
	I2caRegs.I2CDXR = (I2CRead_Index*2)&0x00FF;//WordAddress low byte
	
	I2caRegs.I2CMDR.all = 0x6620;//Send data to setup EEPROM address
	asm( " NOP " );
	asm( " NOP " );
	I2caRegs.I2CSTR.bit.SCD = 1; // 清除该位，否则连续读时会在while SCD时等待
	asm( " NOP " );
	asm( " NOP " );
	while(I2caRegs.I2CSTR.bit.ARDY != 1)
	{
		asm( " NOP " );          //Register Access Ready ,
                             //the EEPROM address setup portion of
                             //the read data communication is complete
	}
	
	I2caRegs.I2CCNT = 2;       //设置要接收的数据个数
	I2caRegs.I2CMDR.all = 0x6C20;//0x2C20;// Send restart as master receiver

	while(I2caRegs.I2CSTR.bit.SCD != 1)
	{
		asm( " NOP " );           //未全部完成等待
	}

	read_low = I2caRegs.I2CDRR;
	read_high = I2caRegs.I2CDRR;
	*I2CRead_Add = (read_low | (read_high << 8));

#else
	Uint16 address = I2CRead_Index*2;
	
	*I2CRead_Add = EEPROM_READ_WORD(address);
#endif
	return I2C_SUCCESS;
}

/*unsigned int I2cWriteDataByModule(unsigned int I2CWrite_Index,unsigned int *I2CWrite_Add)
{
#if 1
	unsigned int write_high = 0;
	unsigned int write_low = 0;

	write_high = (*I2CWrite_Add&0xFF00)>>8;
	write_low = *I2CWrite_Add&0x00FF;
	if(I2caRegs.I2CMDR.bit.STP == 1)
	{
		return I2C_STP_NOT_READY_ERROR;
	}
	//Setup slave address
	I2caRegs.I2CSAR = (ControlCode << 3) + ((I2CWrite_Index*2)>>13);//8192 change page
	//Check if bus is busy
	if(I2caRegs.I2CSTR.bit.BB == 1)
	{
		return I2C_BUS_BUSY_ERROR;
	}
	//Setup number of bytes to send: 2 byte WordAddress + 2 byte MsgBuffer ( not include Slave Address)
	I2caRegs.I2CCNT = 2+2;

	I2caRegs.I2CDXR = ((I2CWrite_Index*2)&0x1F00)>>8;//WordAddress high byte
	I2caRegs.I2CDXR = (I2CWrite_Index*2)&0x00FF;//WordAddress low byte
	
	I2caRegs.I2CDXR = write_low;//MsgBuffer low byte
	I2caRegs.I2CDXR = write_high;//MsgBuffer high byte
	//Send start as master transmitter
	I2caRegs.I2CMDR.all = 0x6E20;

	asm( " NOP " );
	asm( " NOP " );

#else
	Uint16 address = 0;
	Uint16 data = 0;

	address = I2CWrite_Index*2;
	data = *I2CWrite_Add;
	EEPROM_WRITE_WORD(address,data);
#endif
	
	return I2C_SUCCESS;
}*/

void eeprom_Process(void)
{
	unsigned int i = 0;
	unsigned int j = 0;
	unsigned int k = 0;
	unsigned int recall_value = 0;
	
	if(sccw1.field.eeprom_mode == 1)
	{
		if(0 == scsw2.field.igbt_state)
		{
			if(eeprom_record_enable != 0)
			{
				EALLOW;
				SysCtrlRegs.WDCR = 0x0068;	//关闭看门狗
				EDIS;
				DINT;
				
				while(I2cWriteData(eeprom_record_index[eeprom_record_enable-1],&epdw.value[eeprom_record_index[eeprom_record_enable-1]])){}
				for(j=0;j<EEPROM_WRITE_DELAY_CNT;j++){}
				while(I2cReadData(eeprom_record_index[eeprom_record_enable-1],&recall_value)){}
				for(j=0;j<EEPROM_READ_DELAY_CNT;j++){}
				if(recall_value==epdw.value[eeprom_record_index[eeprom_record_enable-1]])
				{
					alarm.field.I2C_alarm = 0;
				}
				else
				{
					alarm.field.I2C_alarm = 1;
				}
				eeprom_record_enable--;
				
				EINT;
				EALLOW;
        		SysCtrlRegs.WDCR = 0x0028;
        		EDIS;
			}
		}
		else
		{
			eeprom_record_enable = 0;

		}
	}
	else
	{
		alarm.field.I2C_alarm = 0;
		eeprom_record_enable = 0;
	}
	if((1 == sccw1.field.eeprom_recall)
	 &&(0 == scsw2.field.igbt_state))
	{
		EALLOW;
		SysCtrlRegs.WDCR = 0x0068;	//关闭看门狗
		EDIS;
		DINT;
    	Default_ParaInt();
    	Default_ExParaInt();
    	eepromPara_Wricall();
		I2C_FaultDateClear();
		I2C_LimitDateClear();
	    EALLOW;
        SysCtrlRegs.WDCR = 0x0028;
        EDIS;
	    while (1)
	    {
		   asm(" nop");
		}
	}

	if((1 == sccw1.field.error_clr)
	 &&(0 == scsw2.field.igbt_state))
	{
		//DINT;
		I2C_FaultDateClear();
		I2C_LimitDateClear();
		//EINT;
		sccw1.field.error_clr  = 0;
		emswold.wValue         = 0;
		emsw1old.wValue        = 0;
		emsw2old.wValue        = 0;
	}

	//故障记录至I2C的条件：故障数据已经更新，IGBT状态为关管，系统故障等级大于3，IGBT曾经开过管
	if((1 == FaultI2CRecord_en)
	 &&(0 == scsw2.field.igbt_state)
	 &&(!sccw1.field.eeprom_mode))
	{
		//DINT;
		//更新并写入组号
		I2CErr_Record.field.I2C_fault_num = FaultDataRecord_num;
		while(I2cWriteData(I2C_FaultStart,&I2CErr_Record.field.I2C_fault_num)){}
		for(j=0;j<EEPROM_WRITE_DELAY_CNT;j++){}

        //记录更新的故障数据
        for(i=1;i<=I2C_FaultNum;i++)
        {
        	I2C_fault_writeaddr = I2C_FaultStart+I2CErr_Record.field.I2C_fault_num*I2C_FaultNum+i;
        	I2C_fault_writenum  = I2CErr_Record.field.I2C_fault_num*I2C_FaultNum+i;
            while(I2cWriteData(I2C_fault_writeaddr,&I2CErr_Record.value[I2C_fault_writenum])){}
		    for(j=0;j<EEPROM_WRITE_DELAY_CNT;j++) {}
        }
    	FaultDataRecord_en      = 1;    //允许更新故障数据
    	FaultI2CRecord_en       = 0;    //I2C写入操作空闲
    	//EINT;
	}

	if(LimitI2CRecord_en==1 && scsw2.field.igbt_state==0 && !sccw1.field.eeprom_mode && !sccw1.field.error_clr)
	{
		for(k=I2C_ValueLimitStart;k<=I2C_ValueLimitEnd;k++)
		{
			if(eeprom_valuelimit_index[k-I2C_ValueLimitStart])
			{
				eeprom_valuelimit_index[k-I2C_ValueLimitStart] = 0;
				//DINT;
	            while(I2cWriteData(k,&I2CValueLimit_Record.value[k-I2C_ValueLimitStart])){}
			    for(j=0;j<EEPROM_WRITE_DELAY_CNT;j++) {}
				//EINT;
			}
		}
		LimitI2CRecord_en = 0;
	}

	if((0 == scsw2.field.igbt_state)
	 &&((1 == diagEncoffsetVar.write_offset_flag)||(1 == exactTune.write_exactune_flag)))
	{
		while(I2cWriteData(4,&epdw.value[4])){}
		for(j=0;j<EEPROM_WRITE_DELAY_CNT;j++) {}
	    while(I2cReadData(4,&recall_value)){}
		for(j=0;j<EEPROM_READ_DELAY_CNT;j++) {}
		if(recall_value==epdw.value[4])
		{
			alarm.field.I2C_alarm = 0;
		}
		else
		{
			alarm.field.I2C_alarm = 1;
		}

		while(I2cWriteData(3,&epdw.value[3])){}
		for(j=0;j<EEPROM_WRITE_DELAY_CNT;j++) {}
	    while(I2cReadData(3,&recall_value)){}
		for(j=0;j<EEPROM_READ_DELAY_CNT;j++) {}
		if(recall_value==epdw.value[3])
		{
			alarm.field.I2C_alarm = 0;
		}
		else
		{
			alarm.field.I2C_alarm = 1;
		}

		diagEncoffsetVar.write_offset_flag = 0;
		exactTune.write_exactune_flag = 0;
	}

	/*if((0 == scsw2.field.igbt_state)//存储磁链
	 &&(1 == diagFluexStr.diagFluex_writeflag))
	{
		while(I2cWriteData(162,&epdw.value[162])){}
		for(j=0;j<EEPROM_WRITE_DELAY_CNT;j++) {}
	    while(I2cReadData(162,&recall_value)){}
		for(j=0;j<EEPROM_READ_DELAY_CNT;j++) {}
		if(recall_value==epdw.value[162])
		{
			alarm.field.I2C_alarm = 0;
		}
		else
		{
			alarm.field.I2C_alarm = 1;
		}
		while(I2cWriteData(162+EEPROM_START_ADDR1,&epdw.value[162])){}
		for(j=0;j<EEPROM_WRITE_DELAY_CNT;j++) {}
	    while(I2cReadData(162+EEPROM_START_ADDR1,&recall_value)){}
		for(j=0;j<EEPROM_READ_DELAY_CNT;j++) {}
		if(recall_value==epdw.value[162])
		{
			alarm.field.I2C_alarm = 0;
		}
		else
		{
			alarm.field.I2C_alarm = 1;
		}
		while(I2cWriteData(162+EEPROM_START_ADDR2,&epdw.value[162])){}
		for(j=0;j<EEPROM_WRITE_DELAY_CNT;j++) {}
	    while(I2cReadData(162+EEPROM_START_ADDR2,&recall_value)){}
		for(j=0;j<EEPROM_READ_DELAY_CNT;j++) {}
		if(recall_value==epdw.value[162])
		{
			alarm.field.I2C_alarm = 0;
		}
		else
		{
			alarm.field.I2C_alarm = 1;
		}

		crc16_value = eeprom_crc16_ccitt_false(epdw.value,EEPROM_SUM_LEN);
		while(I2cWriteData(EEPROM_CRC16_ADDR,&crc16_value)){}
		for(j=0;j<EEPROM_WRITE_DELAY_CNT;j++) {}
	    while(I2cReadData(EEPROM_CRC16_ADDR,&recall_value)){}
		for(j=0;j<EEPROM_READ_DELAY_CNT;j++) {}
		if(recall_value==crc16_value)
		{
			alarm.field.I2C_alarm = 0;
		}
		else
		{
			alarm.field.I2C_alarm = 1;
		}
		while(I2cWriteData(EEPROM_CRC16_ADDR1,&crc16_value)){}
		for(j=0;j<EEPROM_WRITE_DELAY_CNT;j++) {}
	    while(I2cReadData(EEPROM_CRC16_ADDR1,&recall_value)){}
		for(j=0;j<EEPROM_READ_DELAY_CNT;j++) {}
		if(recall_value==crc16_value)
		{
			alarm.field.I2C_alarm = 0;
		}
		else
		{
			alarm.field.I2C_alarm = 1;
		}
		while(I2cWriteData(EEPROM_CRC16_ADDR2,&crc16_value)){}
		for(j=0;j<EEPROM_WRITE_DELAY_CNT;j++) {}
	    while(I2cReadData(EEPROM_CRC16_ADDR2,&recall_value)){}
		for(j=0;j<EEPROM_READ_DELAY_CNT;j++) {}
		if(recall_value==crc16_value)
		{
			alarm.field.I2C_alarm = 0;
		}
		else
		{
			alarm.field.I2C_alarm = 1;
		}
		diagFluexStr.diagFluex_writeflag = 0;
	}*/

	/*if((0 == scsw2.field.igbt_state)//存储电阻值
	 &&(1 == diagRsStr.RsLearning_write_flag))
	{
		while(I2cWriteData(163,&epdw.value[163])){}
		for(j=0;j<EEPROM_WRITE_DELAY_CNT;j++) {}
	    while(I2cReadData(163,&recall_value)){}
		for(j=0;j<EEPROM_READ_DELAY_CNT;j++) {}
		if(recall_value==epdw.value[163])
		{
			alarm.field.I2C_alarm = 0;
		}
		else
		{
			alarm.field.I2C_alarm = 1;
		}
		while(I2cWriteData(163+EEPROM_START_ADDR1,&epdw.value[163])){}
		for(j=0;j<EEPROM_WRITE_DELAY_CNT;j++) {}
	    while(I2cReadData(163+EEPROM_START_ADDR1,&recall_value)){}
		for(j=0;j<EEPROM_READ_DELAY_CNT;j++) {}
		if(recall_value==epdw.value[163])
		{
			alarm.field.I2C_alarm = 0;
		}
		else
		{
			alarm.field.I2C_alarm = 1;
		}
		while(I2cWriteData(163+EEPROM_START_ADDR2,&epdw.value[163])){}
		for(j=0;j<EEPROM_WRITE_DELAY_CNT;j++) {}
	    while(I2cReadData(163+EEPROM_START_ADDR2,&recall_value)){}
		for(j=0;j<EEPROM_READ_DELAY_CNT;j++) {}
		if(recall_value==epdw.value[163])
		{
			alarm.field.I2C_alarm = 0;
		}
		else
		{
			alarm.field.I2C_alarm = 1;
		}

		crc16_value = eeprom_crc16_ccitt_false(epdw.value,EEPROM_SUM_LEN);
		while(I2cWriteData(EEPROM_CRC16_ADDR,&crc16_value)){}
		for(j=0;j<EEPROM_WRITE_DELAY_CNT;j++) {}
	    while(I2cReadData(EEPROM_CRC16_ADDR,&recall_value)){}
		for(j=0;j<EEPROM_READ_DELAY_CNT;j++) {}
		if(recall_value==crc16_value)
		{
			alarm.field.I2C_alarm = 0;
		}
		else
		{
			alarm.field.I2C_alarm = 1;
		}
		while(I2cWriteData(EEPROM_CRC16_ADDR1,&crc16_value)){}
		for(j=0;j<EEPROM_WRITE_DELAY_CNT;j++) {}
	    while(I2cReadData(EEPROM_CRC16_ADDR1,&recall_value)){}
		for(j=0;j<EEPROM_READ_DELAY_CNT;j++) {}
		if(recall_value==crc16_value)
		{
			alarm.field.I2C_alarm = 0;
		}
		else
		{
			alarm.field.I2C_alarm = 1;
		}
		while(I2cWriteData(EEPROM_CRC16_ADDR2,&crc16_value)){}
		for(j=0;j<EEPROM_WRITE_DELAY_CNT;j++) {}
	    while(I2cReadData(EEPROM_CRC16_ADDR2,&recall_value)){}
		for(j=0;j<EEPROM_READ_DELAY_CNT;j++) {}
		if(recall_value==crc16_value)
		{
			alarm.field.I2C_alarm = 0;
		}
		else
		{
			alarm.field.I2C_alarm = 1;
		}
		diagRsStr.RsLearning_write_flag = 0;
	}*/

	if((0 == scsw2.field.igbt_state)//存储dq电感
	 &&(1 == diagEncoffsetVar.write_dq_flag))
	{
		while(I2cWriteData(164,&epdw.value[164])){}
		for(j=0;j<EEPROM_WRITE_DELAY_CNT;j++) {}
	    while(I2cReadData(164,&recall_value)){}
		for(j=0;j<EEPROM_READ_DELAY_CNT;j++) {}
		if(recall_value==epdw.value[164])
		{
			alarm.field.I2C_alarm = 0;
		}
		else
		{
			alarm.field.I2C_alarm = 1;
		}

		while(I2cWriteData(165,&epdw.value[165])){}
		for(j=0;j<EEPROM_WRITE_DELAY_CNT;j++) {}
	    while(I2cReadData(165,&recall_value)){}
		for(j=0;j<EEPROM_READ_DELAY_CNT;j++) {}
		if(recall_value==epdw.value[165])
		{
			alarm.field.I2C_alarm = 0;
		}
		else
		{
			alarm.field.I2C_alarm = 1;
		}

		diagEncoffsetVar.write_dq_flag = 0;
	}
}

void I2C_FaultDateClear(void)
{
	unsigned int i = 0;
	unsigned int j = 0;
	
	for(i=I2C_FaultStart;i<=I2C_FaultDataEnd;i++)
	{
		I2CErr_Record.value[i-I2C_FaultStart] = 0;
		while(I2cWriteData(i,&I2CErr_Record.value[i-I2C_FaultStart])){}
		for(j=0;j<EEPROM_WRITE_DELAY_CNT;j++) {}
	}
}


void I2C_FaultRecordInt(void)
{
	I2Cfault_Recall();   //读取故障记录数据

	if(I2CErr_Record.field.I2C_fault_num>4) I2CErr_Record.field.I2C_fault_num = 0;
	FaultDataRecord_num     = I2CErr_Record.field.I2C_fault_num;

	FaultDataRecord_en      = 1;    //允许更新故障数据
	FaultI2CRecord_en       = 0;    //I2C写入操作空闲
}

void I2C_LimitDateClear(void)
{
	unsigned int i = 0;
	unsigned int j = 0;
	
	for(i=I2C_ValueLimitStart;i<=I2C_ValueLimitEnd;i++)
	{
		I2CValueLimit_Record.value[i-I2C_ValueLimitStart] = 0;
		while(I2cWriteData(i,&I2CValueLimit_Record.value[i-I2C_ValueLimitStart])){}
		for(j=0;j<EEPROM_WRITE_DELAY_CNT;j++) {}
	}
}

void I2C_ValueLimit_Int(void)
{
	int i = 0;
	int j = 0;
	
	for(i=I2C_ValueLimitStart;i<=I2C_ValueLimitEnd;i++)
	{
		eeprom_valuelimit_index[i-I2C_ValueLimitStart] = 0;                                //清除标志值
		while(I2cReadData(i,&I2CValueLimit_Record.value[i-I2C_ValueLimitStart])) {}        //读取上一次记录的峰值数据
		for(j=0;j<EEPROM_READ_DELAY_CNT;j++){}
	}
	LimitI2CRecord_en             = 0;
	LimitI2CUpdate_en             = 0;
	FaultI2CIGBT_flag             = 0;
}

#endif
