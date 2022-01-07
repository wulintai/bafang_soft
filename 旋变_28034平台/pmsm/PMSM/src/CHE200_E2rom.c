#ifndef _CHE200_E2ROM_C_
#define _CHE200_E2ROM_C_

#include "DSP2803x_Device.h"
#include "DSP2803x_Examples.h"
#include "CHE200_E2rom.h"
#include "DSP2803x_GlobalPrototypes.h"

#define scl_low		GpioDataRegs.GPBCLEAR.bit.GPIO33=1		
#define scl_high	GpioDataRegs.GPBSET.bit.GPIO33=1		
#define	sda_low		GpioDataRegs.GPBCLEAR.bit.GPIO32=1	
#define	sda_high	GpioDataRegs.GPBSET.bit.GPIO32=1

//;--EEPROM E2PROM Write/Read timing constence define-----
#define		TSU_STA		8	//47	//;4.7uS  30	
#define		THD_STA		8	//40	//;4.0uS  24	
#define		TSU_STO		8	//40	//;4.0uS  24	
#define  	TLOW		2//10	//47	//;4.7uS  30	
#define  	THIGH		2//8	//40	//;4.0uS  24	
#define  	TRISE		1//2	//10	//;1.0uS  10	
#define  	TFALL		1//2	//10	//;0.3uS  4
	
//#define		TSU_STA		5	//47	//;4.7uS  30	
//#define		THD_STA		4	//40	//;4.0uS  24	
//#define		TSU_STO		4	//40	//;4.0uS  24	
//#define  	TLOW		10	//47	//;4.7uS  30	
//#define  	THIGH		5	//40	//;4.0uS  24	
//#define  	TRISE		2	//10	//;1.0uS  10	
//#define  	TFALL		1	//10	//;0.3uS  4

void	STARTE2PROM(void)
{
	EALLOW;
	GpioCtrlRegs.GPBDIR.bit.GPIO32 = 1;  	// GPIO32 = output 
	GpioCtrlRegs.GPBMUX1.bit.GPIO32 = 0;   	// IO PIN (Configure GPIO32 for SDAA operation)
	EDIS;
//--->>---SCL=0-------->>----------------
	scl_low;				//SCL=0
	DELAY_US(TFALL);
//	asm(" rpt #250||nop ");
//--->>---SDA=1 -------->>----------------
	sda_high;
	DELAY_US(TLOW);
//	asm(" rpt #250||nop ");
//--->>---SCL=1 -------->>----------------	
	scl_high;				//SCL=1
	DELAY_US(TSU_STA);
//	asm(" rpt #250||nop ");
//--->>---SDA=0 -------->>----------------
	sda_low;
	DELAY_US(THD_STA);
//	asm(" rpt #250||nop ");
//--->>---SCL=0 -------->>----------------
	scl_low;				//SCL=0
	DELAY_US(TLOW);	
//	asm(" rpt #250||nop ");
}
//;¡§a?¨º?1EEPROM?¨º??¡§a¡§a?¨º?1??3?:
void	ENDE2PROM(void)
{
	EALLOW;
	GpioCtrlRegs.GPBDIR.bit.GPIO32 = 1;  // GPIO32 = output
	GpioCtrlRegs.GPBMUX1.bit.GPIO32 = 0;   	// IO PIN (Configure GPIO32 for SDAA operation)
	EDIS;

//--->>---SCL=0 -------->>----------------
	scl_low;				//SCL=0
	DELAY_US(TFALL);
//	asm(" rpt #250||nop ");
//--->>---SDA=0 -------->>----------------
	sda_low;
	DELAY_US(TLOW);	
//	asm(" rpt #250||nop ");
//--->>----SCL=1 -------->>----------------
	scl_high;				//SCL=1
	DELAY_US(TRISE+TSU_STO);
//	asm(" rpt #250||nop ");
//--->>----SDA=1 -------->>----------------
	sda_high;
	DELAY_US(THIGH);
//	asm(" rpt #250||nop ");	
//--->>----SCL=0 -------->>----------------
	scl_low;
}

//;========================================
//;E2PROM SAVE PROGRAM
//;========================================

void	EEPROM_WRITE_WORD(Uint16 address,Uint16 data)
{
	Uint16	para_temp0,para_temp1,eeprom_waitcount;
	//Uint16  spi_temp0,spi_temp1;
	//spi_temp0=address;
	//spi_temp1=data;
	eeprom_waitcount=0;

	while(eeprom_waitcount<20)
	{
		KickDog();
		eeprom_waitcount++;
		
		STARTE2PROM();
		para_temp0=0x0a0;				//;GET read ctrl byte 0xa0 
		para_temp1=EEPROM_WRITE_BYTE(para_temp0);
		if(para_temp1==0)
		{
			//;--->>---D??|¨¬???¡è?¨¢??¡§2
			para_temp0=address>>7;
			EEPROM_WRITE_BYTE(para_temp0&0xff);	//;Write Address to EEPROM
			
			para_temp0=((address<<1)&0xfe);	
			EEPROM_WRITE_BYTE(para_temp0&0xff);	//;Write Address to EEPROM
			
			WRITEE2PROMAWORD(data);	////;Write DATA to EEPROM
			ENDE2PROM();
			return;
			
		}
	}
	error_code=21;
	//;--->>---D??|¨¬???¡è?¨¢??¡§2
	para_temp0=address>>7;	
	EEPROM_WRITE_BYTE(para_temp0&0xff);	//;Write Address to EEPROM
	
	para_temp0=((address<<1)&0xfe);	
	EEPROM_WRITE_BYTE(para_temp0&0xff);	//;Write Address to EEPROM
	WRITEE2PROMAWORD(data);	//;Write DATA to EEPROM
	ENDE2PROM();
	return;
}



Uint16	EEPROM_WRITE_BYTE(Uint16 data)
{
	Uint16 para_temp0,para_temp1,para_temp2,para_temp3;
	para_temp0=(data<<8)&0xff00;

	for(para_temp2=0;para_temp2<1;para_temp2++)		//;OUTER LOOP COUNTER
	{
		for(para_temp1=0;para_temp1<8;para_temp1++)	//;INNER LOOP COUNTER
//--->>---SCL=0 -------->>----------------
		{	
			scl_low;				//SCL=0
			DELAY_US(TLOW);	
//--->>----??¡§oy?Y???¡è?SDA---->>----------------
			if(para_temp0&0x8000)
			{						//;--->>---SDA=1 -------->>
				sda_high;
				EALLOW;
				GpioCtrlRegs.GPBDIR.bit.GPIO32 = 1;  	// GPIO32 = output 
				EDIS;
				sda_high;
			}
			else
			{						//;--->>---SDA=0 -------->>
				sda_low;	
				EALLOW;
				GpioCtrlRegs.GPBDIR.bit.GPIO32 = 1;  	// GPIO32 = output 
				EDIS;
				sda_low;
			}
			
			DELAY_US(2);	
//--->>------SCL=1 -------->>----------------		
			scl_high;				//SCL=1
			DELAY_US(THIGH);
//--->>-----shift temp------>>-----------------
			para_temp0=para_temp0<<1;
		}

//--->>---SCL=0 ,SDA=High_sistance-------->>----------------  	
		scl_low;				//SCL=0
		DELAY_US(TFALL);	
//--->>---SDA=i/p mode -------->>
		EALLOW;
		GpioCtrlRegs.GPBDIR.bit.GPIO32 = 0;  // GPIO32 = input
		EDIS;
		DELAY_US(TLOW);
//--->>---SCL=1 let 24LC08B send ACK----->>----------------
		scl_high;				//SCL=1
		DELAY_US(THIGH);	
//;--->>-----READ ACK FROM E2PROM---->>---------
		para_temp3=GpioDataRegs.GPBDAT.bit.GPIO32;	//;ACK BACKUP
//--->>---SCL=0;-------->>----------------	
		scl_low;				//SCL=0
		DELAY_US(TFALL);	
//--->>---SDA=1 o/p mode-------->>
		sda_high;
		EALLOW;
		GpioCtrlRegs.GPBDIR.bit.GPIO32 = 1;  	// GPIO32 = output 
		EDIS;
		sda_high;
		DELAY_US(TLOW); 
	}	
	
	return(para_temp3);

	
}


Uint16	WRITEE2PROMAWORD(Uint16 data)
{
	Uint16 para_temp0,para_temp1,para_temp2,para_temp3;
	para_temp0=data;

	for(para_temp2=0;para_temp2<2;para_temp2++)
	{
		for(para_temp1=0;para_temp1<8;para_temp1++)
//--->>---SCL=0 -------->>----------------
		{	
			scl_low;				//SCL=0
			DELAY_US(TLOW);	
//--->>----??¡§oy?Y???¡è?SDA---->>----------------
			if(para_temp0&0x8000)
			{			//;--->>---SDA=1 -------->>
				sda_high;
				EALLOW;
				GpioCtrlRegs.GPBDIR.bit.GPIO32 = 1;  	// GPIO32 = output 
				EDIS;
				sda_high;
			
			}
			else
			{			//;--->>---SDA=0 -------->>
				sda_low;	
				EALLOW;
				GpioCtrlRegs.GPBDIR.bit.GPIO32 = 1;  	// GPIO32 = output 
				EDIS; 
				sda_low;
			}
			
			DELAY_US(2);	
//--->>------SCL=1 -------->>----------------		
			scl_high;				//SCL=1
			DELAY_US(THIGH);
//--->>-----shift temp------>>-----------------
			para_temp0=para_temp0<<1;
		}
//--->>---SCL=0 ,SDA=High_sistance-------->>----------------  	
		scl_low;				//SCL=0
		DELAY_US(TFALL);	
//--->>---SDA=i/p mode -------->>
		EALLOW;
		GpioCtrlRegs.GPBDIR.bit.GPIO32 = 0;  // GPIO32 = input
		EDIS;
		DELAY_US(TLOW);
//--->>---SCL=1 let 24LC08B send ACK----->>----------------
		scl_high;				//SCL=1
		DELAY_US(THIGH);	
//;--->>-----READ ACK FROM E2PROM---->>---------
		para_temp3=GpioDataRegs.GPBDAT.bit.GPIO32;	//;ACK BACKUP
//--->>---SCL=0;-------->>----------------	
		scl_low;				//SCL=0
		DELAY_US(TFALL);	
//--->>---SDA=1 o/p mode-------->>
		sda_high;
		EALLOW;
		GpioCtrlRegs.GPBDIR.bit.GPIO32 = 1;  	// GPIO32 = output 
		EDIS;
		sda_high;
		DELAY_US(TLOW); 
	}	
	return(para_temp3);
}


Uint16	READEEAWORD(void)
{
	Uint16 para_temp0,para_temp1,para_temp2,para_temp3;
	para_temp0=0x8000;
	para_temp3=0;
	para_temp2=0;						//;READ DATA BUFFER

//	for(para_temp2=0;para_temp2<2;para_temp2++)		//;OUTER LOOP COUNTER
	while(para_temp2<2)
	{
		for(para_temp1=0;para_temp1<8;para_temp1++)	//;INNER LOOP COUNTER
		{
	
//;--->>---SCL=0 -------->>----------------
			scl_low;				//SCL=0
			DELAY_US(TFALL);	
//;--->>----??¡§oy?YSDA¡§|¡§¡§?a¡§o?¡§¡§???¨º¡§o?---->>----
			EALLOW;
			GpioCtrlRegs.GPBDIR.bit.GPIO32 = 0;  // GPIO32 = input
			EDIS;
			DELAY_US(TLOW);
//;--->>---SCL=1 -------->>----------------	
			scl_high;				//SCL=1
			DELAY_US(THIGH);	
//;--->>----READ SDA LEVEL & RECORD---->>--
			if(GpioDataRegs.GPBDAT.bit.GPIO32==1) para_temp3=para_temp3|para_temp0;	//;BUFF
	
//;--->>-----shift bit mask------>>-----------------
			para_temp0=para_temp0>>1;
		}
		para_temp2++;
		if(para_temp2>=2) break;

//;--->>---SCL=0 ,SDA=0,SAY ACKNOWLEDGE->>-------
		scl_low;				//SCL=0
		DELAY_US(TFALL);
//;--->>---SDA=O/P MODE -------->>
		sda_low;		
		EALLOW;
		GpioCtrlRegs.GPBDIR.bit.GPIO32 = 1;  	// GPIO32 = output 
		EDIS;
		sda_low;
		DELAY_US(TLOW); 
//;--->>---SCL=1  SEND ACK----->>----------------		
		scl_high;				//SCL=1
		DELAY_US(THIGH);	
//;--->>---SCL=0;SDA= I/P MODE AGAIN-------->>----------------
		scl_low;
	//	EALLOW;
	//	GpioCtrlRegs.GPBDIR.bit.GPIO32 = 0;  // GPIO32 = input
	//	EDIS;
	}
//;--->>---SCL=0 -------->>----------------	
	scl_low;
	DELAY_US(TLOW);	
	return(para_temp3);
}
//;************************************************/
//;*o?£¤¡§oy??3?:EEPROM_READ_WORD     		*/
//;************************************************/

Uint16	EEPROM_READ_WORD(Uint16 address)
{
	Uint16	para_temp0,para_temp1,para_temp2,eeprom_waitcount;
	//Uint16  spi_temp0;
	//spi_temp0=address;
	eeprom_waitcount=0;

	while(eeprom_waitcount<20)
	{
		KickDog();
		eeprom_waitcount++;
		
		STARTE2PROM();
		para_temp0=0x0a0;		//;GET ctrl byte
		para_temp1=EEPROM_WRITE_BYTE(para_temp0);
		if(para_temp1==0)
		{
			//;--->>---D??|¨¬???¡è?¨¢??¡§2
			para_temp0=address>>7;	
			EEPROM_WRITE_BYTE(para_temp0);	//;Write hi Address to EEPROM
			
			para_temp0=((address<<1)&0xfe);	
			EEPROM_WRITE_BYTE(para_temp0);	//;Write lo Address to EEPROM
			
			//;--->>---?D?1"D??2¡§¡ä?¨¢??"
			STARTE2PROM();			//;RE-SATART AGAIN
			//;--->>---D??"?¡§¡é2¡§¡ä?¨¢??"?????¨¢??¡§2
			para_temp0=0x0a1;		//;GET ctrl byte
			EEPROM_WRITE_BYTE(para_temp0);			//;Write Ctrl byte to EEPROM
			//;--->>---?¡§¡é¡§¡ã????¨¢?|¨¬?¡§oy?Y
			para_temp2=READEEAWORD();
			ENDE2PROM();
			return(para_temp2);		//;RETURN(BACKUP)
			
		}
	}
	error_code=21;
	//;--->>---D??|¨¬???¡è?¨¢??¡§2
	//para_temp0=((address<<1)&0xfe);
	
	para_temp0=address>>7;
	EEPROM_WRITE_BYTE(para_temp0);	//;Write Address to EEPROM
	
	para_temp0=((address<<1)&0xfe);
	EEPROM_WRITE_BYTE(para_temp0);	//;Write Address to EEPROM
	
	//;--->>---?D?1"D??2¡§¡ä?¨¢??"
	STARTE2PROM();			//;RE-SATART AGAIN
	//;--->>---D??"?¡§¡é2¡§¡ä?¨¢??"?????¨¢??¡§2
	para_temp0=0x0a0;		//;GET ctrl byte
	EEPROM_WRITE_BYTE(para_temp0);			//;Write Ctrl byte to EEPROM
	//;--->>---?¡§¡é¡§¡ã????¨¢?|¨¬?¡§oy?Y
	para_temp2=READEEAWORD();
	ENDE2PROM();
	return(para_temp2);		
}
#endif
