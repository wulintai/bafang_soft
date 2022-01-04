#ifndef _FLASH_C
#define _FLASH_C

#include "include_c.h"

void MyCallBackFunc(void)
{
	asm(" NOP");
}
void App_FlashAPIInit(void)
{
	EALLOW;
	Flash_CPUScaleFactor = SCALE_FACTOR;
	Flash_CallbackPtr = 0;//NULL;
	EDIS;
}

Uint16  Flash_FlagProgram(int type,unsigned char  baud_filetype)
{
	Uint16* p_Flash = (Uint16 *)0x3f3FFF;
	Uint16 FlashBuffer1[1] = {0x55BA};
	Uint16 u16_Status = STATUS_SUCCESS1;
    unsigned char baudtmp = 0;
	
	App_FlashAPIInit();
	u16_Status = Flash_Program(p_Flash, FlashBuffer1, 1, &FlashStatus);
	if(STATUS_SUCCESS1 != u16_Status)
	{
		return u16_Status;
	}
	p_Flash = (Uint16 *)0x3f3FFa;
	FlashBuffer1[0] = 0xff00|type;
	u16_Status = Flash_Program(p_Flash, FlashBuffer1, 1, &FlashStatus);
	if (STATUS_SUCCESS1 != u16_Status)
	{
		return u16_Status;	
	}

	//写波特率
	if(type!=3)	//baud,bit7-bit5
	{
		p_Flash=(Uint16 *)0x3f3FFb;
		baudtmp = (baud_filetype&0x0e0);
		if(0x20 == baudtmp)//125k
		{
			FlashBuffer1[0]=0xfff1;
		}
		else if(0x80 == baudtmp)//500k
		{
			FlashBuffer1[0]=0xfff4;
		}
		else//250k
		{
			FlashBuffer1[0]=0xffff;
		}
		u16_Status = Flash_Program(p_Flash, FlashBuffer1, 1, &FlashStatus);
		if(STATUS_SUCCESS1 != u16_Status)
		{
			return u16_Status;	
		}
	}
	//写文件类型,bit4
	p_Flash=(Uint16 *)0x3f3FFc;
	baudtmp = (baud_filetype&0x10);
	if(0x10 == baudtmp)	//加密
	{
		FlashBuffer1[0]=0xffff;
	}
	else
	{
		FlashBuffer1[0]=0xfffe;
	}
	u16_Status = Flash_Program(p_Flash, FlashBuffer1, 1, &FlashStatus);
	if(STATUS_SUCCESS1 != u16_Status)
	{
		return u16_Status;
	}
	return u16_Status;
}

unsigned int GetCrc8(unsigned int *data,unsigned int length)
{
    int j = 0;
    unsigned int crc = 0x0;
	
    for(j=0;j<length;j++)
    {
        crc += data[j];
    }
    return crc;
}

void ProgFlashProc(unsigned int *data,int type)
{
	unsigned int crc = 0;
	unsigned int crc2 = 0;
	unsigned int ver = 0;
	
	ver = (*((Uint16 *)0x3f3ff9))&0xff;//Bootloader version
	if((MCU_ID == data[0])&&(data[1]==ver)&&((0x8 == data[2]))&&(0x11 == data[3])&&(0xff == data[7]))
	{
		crc2 = data[5];
		crc2 = crc2<<8;
		crc2 = crc2|(data[6]&0xFF);
		crc = GetCrc8(data,5);
		if(crc == crc2)//Crc is right
		{
		#if 0
			EALLOW;
	   		SysCtrlRegs.WDCR = 0x0068;	//关闭看门狗
	    	EDIS;

			DINT;
			Flash_FlagProgram(type,data[4]);
			
			//握手信号延时，看门狗启动
			EALLOW;
	   		SysCtrlRegs.WDCR = 0x00af;
	    	EDIS;
		#else
			DINT;
		#endif
			
			while(1)
			{
				asm(" NOP");
			}
		}
	}
}

#endif

