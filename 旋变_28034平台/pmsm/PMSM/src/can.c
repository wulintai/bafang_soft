/*
 * can.c
 *
 *  Created on: 2015-12-11
 *      Author: ThinkPad
 */

#ifndef _CAN_C
#define _CAN_C

#include "include_c.h"

void canInit(void)
{
	InitECan();
}

void canbunpackMbx31(void)//8888
{
#if 0
	bootcandate[0]  = ECanbMBoxData[31].MDL.byte.BYTE3;
	bootcandate[1]  = ECanbMBoxData[31].MDL.byte.BYTE2;
	bootcandate[2]  = ECanbMBoxData[31].MDL.byte.BYTE1;
	bootcandate[3]  = ECanbMBoxData[31].MDL.byte.BYTE0;
	bootcandate[4]  = ECanbMBoxData[31].MDH.byte.BYTE7;
	bootcandate[5]  = ECanbMBoxData[31].MDH.byte.BYTE6;
	bootcandate[6]  = ECanbMBoxData[31].MDH.byte.BYTE5;
	bootcandate[7]  = ECanbMBoxData[31].MDH.byte.BYTE4;

	ProgFlashProc(bootcandate,2);
#endif
}

void canaunpackMbx31(void)//8888
{
	bootcandate[0]  = ECanaMBoxData[31].MDL.byte.BYTE3;
	bootcandate[1]  = ECanaMBoxData[31].MDL.byte.BYTE2;
	bootcandate[2]  = ECanaMBoxData[31].MDL.byte.BYTE1;
	bootcandate[3]  = ECanaMBoxData[31].MDL.byte.BYTE0;
	bootcandate[4]  = ECanaMBoxData[31].MDH.byte.BYTE7;
	bootcandate[5]  = ECanaMBoxData[31].MDH.byte.BYTE6;
	bootcandate[6]  = ECanaMBoxData[31].MDH.byte.BYTE5;
	bootcandate[7]  = ECanaMBoxData[31].MDH.byte.BYTE4;
	ProgFlashProc(bootcandate,1);
}

void canTransmitDriver(int canIndex, int MBoxIndex, struct MBOX* pMessage)
{

	struct ECAN_REGS	ECanaShadow;//, ECanbShadow; 影子寄存器   因为控制和状态寄存器不支持位操作
	struct MBOX*		pMbox	= NULL;     //定义指针
	unsigned long		BitMask	= 0;        //位掩码？


	if (((canIndex < 0) || (canIndex > MAX_CAN_NODE_NUM))	||
		((MBoxIndex < 0) || (MBoxIndex > MAX_CAN_MBOX_NUM))	||
		(pMessage == NULL))
	{
		return;
	}


	BitMask = (unsigned long)0x1 << MBoxIndex;      //位掩码？


	if (canIndex == CAN_A_INDEX)
	{
		ECanaShadow.CANMD.all	= ECanaRegs.CANMD.all;      //ECanaRegs不支持位操作，只能赋值给影子寄存器
		ECanaShadow.CANME.all	= ECanaRegs.CANME.all;
		ECanaShadow.CANTRS.all	= ECanaRegs.CANTRS.all;
		if ((ECanaShadow.CANMD.all	& BitMask) != 0 ||      //CANMD邮箱方向寄存器，1-接收邮箱 0-发送邮箱
			(ECanaShadow.CANME.all	& BitMask) == 0 ||      //CANME邮箱使能寄存器  用于启用/禁用32个邮箱0-禁用 1-启用
			(ECanaShadow.CANTRS.all	& BitMask) != 0)        //CANTRS发送请求置位寄存器 用于启动32个邮箱的发送1-启动发送 0-无操作
		{
			return;
		}
		else
		{
			EALLOW;     //寄存器保护，不能随意修改寄存器
			pMbox = (struct MBOX*)(&ECanaMboxes);       //获取邮箱地址指针
			pMbox = pMbox + MBoxIndex;      //指针下移？

			ECanaShadow.CANTA.all	= ECanaRegs.CANTA.all;      //CANTA发送应答寄存器 若邮箱n的信息发送成功则CANTA[n]位置
			ECanaShadow.CANTA.all	|= BitMask;     //与位掩码做按位或操作并赋值
			ECanaRegs.CANTA.all		= ECanaShadow.CANTA.all;        //将得到的值传给控制和状态寄存器

			ECanaShadow.CANME.all	= ECanaRegs.CANME.all;      //CANME邮箱使能寄存器  用于启用/禁用32个邮箱0-禁用 1-启用
			ECanaShadow.CANME.all	&= ~BitMask;        //与位掩码的反码做按位与操作并赋值
			ECanaRegs.CANME.all 	= ECanaShadow.CANME.all;        //将得到的值传给控制和状态寄存器

			pMbox->MDL.all		= pMessage->MDL.all;        //pMessage传进来的缓存区地址，把对应的数据移到邮箱里面去
			pMbox->MDH.all		= pMessage->MDH.all;

			ECanaShadow.CANME.all	= ECanaRegs.CANME.all;      //开始发送
			ECanaShadow.CANME.all	|= BitMask;
			ECanaRegs.CANME.all 	= ECanaShadow.CANME.all;

			ECanaShadow.CANTRS.all	= ECanaRegs.CANTRS.all;
			ECanaShadow.CANTRS.all	|= BitMask;
			ECanaRegs.CANTRS.all	= ECanaShadow.CANTRS.all;
			EDIS;       //结束标志
		}
	}
	else if (canIndex == CAN_B_INDEX)
	{
#if 0
		ECanbShadow.CANMD.all	= ECanbRegs.CANMD.all;
		ECanbShadow.CANME.all	= ECanbRegs.CANME.all;
		ECanbShadow.CANTRS.all	= ECanbRegs.CANTRS.all;
		if ((ECanbShadow.CANMD.all	& BitMask) != 0 ||
			(ECanbShadow.CANME.all	& BitMask) == 0 ||
			(ECanbShadow.CANTRS.all	& BitMask) != 0)
		{
			return;
		}
		else
		{
			EALLOW;

			pMbox = (struct MBOX*)(&ECanbMboxes);
			pMbox = pMbox + MBoxIndex;

			ECanbShadow.CANTA.all	= ECanbRegs.CANTA.all;
			ECanbShadow.CANTA.all	|= BitMask;
			ECanbRegs.CANTA.all		= ECanbShadow.CANTA.all;

			ECanbShadow.CANME.all	= ECanbRegs.CANME.all;
			ECanbShadow.CANME.all	&= ~BitMask;
			ECanbRegs.CANME.all 	= ECanbShadow.CANME.all;

			pMbox->MDL.all		= pMessage->MDL.all;
			pMbox->MDH.all		= pMessage->MDH.all;

			ECanbShadow.CANME.all	= ECanbRegs.CANME.all;
			ECanbShadow.CANME.all	|= BitMask;
			ECanbRegs.CANME.all 	= ECanbShadow.CANME.all;

			ECanbShadow.CANTRS.all	= ECanbRegs.CANTRS.all;
			ECanbShadow.CANTRS.all	|= BitMask;
			ECanbRegs.CANTRS.all	= ECanbShadow.CANTRS.all;
			EDIS;
		}
#endif
	}
}

//########################################################################################
// Function Name:	canReceiveDriver
// Version:			V1.0
// Input:			none
// Output:
// Description:
//########################################################################################
void canReceiveDriver(int canIndex, int MBoxIndex, struct MBOX* pMessage)
{

	struct MBOX*		pMbox	= NULL;

	if (((canIndex < 0) || (canIndex > MAX_CAN_NODE_NUM))	||//MAX_CAN_NODE_NUM=2
		((MBoxIndex < 0) || (MBoxIndex > MAX_CAN_MBOX_NUM))	||//MAX_CAN_MBOX_NUM=32
		(pMessage == NULL))
	{
		return;
	}

	if (canIndex == CAN_A_INDEX)//CAN_A_INDEX=1
	{
			pMbox = (struct MBOX*)(&ECanaMboxes);
			pMbox = pMbox + MBoxIndex;
			pMessage->MDL.all	= pMbox->MDL.all;
			pMessage->MDH.all	= pMbox->MDH.all;
	}
	else if (canIndex == CAN_B_INDEX)
	{
#if 0

			pMbox = (struct MBOX*)(&ECanbMboxes);
			pMbox = pMbox + MBoxIndex;

			pMessage->MDL.all	= pMbox->MDL.all;
			pMessage->MDH.all	= pMbox->MDH.all;
#endif
	}
}

#endif

