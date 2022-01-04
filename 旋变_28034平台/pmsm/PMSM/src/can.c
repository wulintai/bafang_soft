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

	struct ECAN_REGS	ECanaShadow;//, ECanbShadow; Ӱ�ӼĴ���   ��Ϊ���ƺ�״̬�Ĵ�����֧��λ����
	struct MBOX*		pMbox	= NULL;     //����ָ��
	unsigned long		BitMask	= 0;        //λ���룿


	if (((canIndex < 0) || (canIndex > MAX_CAN_NODE_NUM))	||
		((MBoxIndex < 0) || (MBoxIndex > MAX_CAN_MBOX_NUM))	||
		(pMessage == NULL))
	{
		return;
	}


	BitMask = (unsigned long)0x1 << MBoxIndex;      //λ���룿


	if (canIndex == CAN_A_INDEX)
	{
		ECanaShadow.CANMD.all	= ECanaRegs.CANMD.all;      //ECanaRegs��֧��λ������ֻ�ܸ�ֵ��Ӱ�ӼĴ���
		ECanaShadow.CANME.all	= ECanaRegs.CANME.all;
		ECanaShadow.CANTRS.all	= ECanaRegs.CANTRS.all;
		if ((ECanaShadow.CANMD.all	& BitMask) != 0 ||      //CANMD���䷽��Ĵ�����1-�������� 0-��������
			(ECanaShadow.CANME.all	& BitMask) == 0 ||      //CANME����ʹ�ܼĴ���  ��������/����32������0-���� 1-����
			(ECanaShadow.CANTRS.all	& BitMask) != 0)        //CANTRS����������λ�Ĵ��� ��������32������ķ���1-�������� 0-�޲���
		{
			return;
		}
		else
		{
			EALLOW;     //�Ĵ������������������޸ļĴ���
			pMbox = (struct MBOX*)(&ECanaMboxes);       //��ȡ�����ַָ��
			pMbox = pMbox + MBoxIndex;      //ָ�����ƣ�

			ECanaShadow.CANTA.all	= ECanaRegs.CANTA.all;      //CANTA����Ӧ��Ĵ��� ������n����Ϣ���ͳɹ���CANTA[n]λ��
			ECanaShadow.CANTA.all	|= BitMask;     //��λ��������λ���������ֵ
			ECanaRegs.CANTA.all		= ECanaShadow.CANTA.all;        //���õ���ֵ�������ƺ�״̬�Ĵ���

			ECanaShadow.CANME.all	= ECanaRegs.CANME.all;      //CANME����ʹ�ܼĴ���  ��������/����32������0-���� 1-����
			ECanaShadow.CANME.all	&= ~BitMask;        //��λ����ķ�������λ���������ֵ
			ECanaRegs.CANME.all 	= ECanaShadow.CANME.all;        //���õ���ֵ�������ƺ�״̬�Ĵ���

			pMbox->MDL.all		= pMessage->MDL.all;        //pMessage�������Ļ�������ַ���Ѷ�Ӧ�������Ƶ���������ȥ
			pMbox->MDH.all		= pMessage->MDH.all;

			ECanaShadow.CANME.all	= ECanaRegs.CANME.all;      //��ʼ����
			ECanaShadow.CANME.all	|= BitMask;
			ECanaRegs.CANME.all 	= ECanaShadow.CANME.all;

			ECanaShadow.CANTRS.all	= ECanaRegs.CANTRS.all;
			ECanaShadow.CANTRS.all	|= BitMask;
			ECanaRegs.CANTRS.all	= ECanaShadow.CANTRS.all;
			EDIS;       //������־
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

