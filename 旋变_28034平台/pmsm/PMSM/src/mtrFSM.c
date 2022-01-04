/*
 * mtrFSM.c
 *
 *  Created on: 2014-3-19
 *      Author: ThinkPad
 */
#ifndef _MTRFSM_C
#define _MTRFSM_C

#include "include_c.h"

void mtrFSMInit()
{
	int i = 0;
	
	mtrState = MTR_TRQ_LOOP;
	for (i = 0; i < MTRFSM_STATE_NUM; i++)
	{
		switch (i)
		{
		case MTR_SPEED_LOOP:
			mtrFSMState[i].cond		= &mtrSpdLoopCond;
			mtrFSMState[i].action	= &mtrSpdLoopAct;
			mtrFSMState[i].entrance	= &mtrSpdLoopEntrance;
			mtrFSMState[i].export	= &mtrSpdLoopExport;
			break;
		case MTR_TRQ_LOOP:
			mtrFSMState[i].cond		= &mtrTrqLoopCond;
			mtrFSMState[i].action	= &mtrTrqLoopAct;
			mtrFSMState[i].entrance	= &mtrTrqLoopEntrance;
			mtrFSMState[i].export	= &mtrTrqLoopExport;
			break;
		case MTR_CURRENT_LOOP:
			mtrFSMState[i].cond		= &mtrCurLoopCond;
			mtrFSMState[i].action	= &mtrCurLoopAct;
			mtrFSMState[i].entrance	= &mtrCurLoopEntrance;
			mtrFSMState[i].export	= &mtrCurLoopExport;
			break;
		case MTR_VOLT_LOOP:
			mtrFSMState[i].cond		= &mtrVoltLoopCond;
			mtrFSMState[i].action	= &mtrVoltLoopAct;
			mtrFSMState[i].entrance	= &mtrVoltLoopEntrance;
			mtrFSMState[i].export	= &mtrVoltLoopExport;
			break;
		default:
			mtrFSMState[i].cond		= &mtrTrqLoopCond;
			mtrFSMState[i].action	= &mtrTrqLoopAct;
			mtrFSMState[i].entrance	= &mtrTrqLoopEntrance;
			mtrFSMState[i].export	= &mtrTrqLoopExport;
			break;
		}
	}
}


void mtrFSMCtrl()
{
	int i = 0;

	if ((*mtrFSMState[mtrState].cond)())
	{
		(*mtrFSMState[mtrState].action)();
	}
	else
	{
		for (i = 0; i < MTRFSM_STATE_NUM; i++)
		{
			if ((*mtrFSMState[i].cond)())
			{
				(*mtrFSMState[mtrState].export)();	//�˳���ǰϵͳ״̬
				(*mtrFSMState[i].entrance)();		//������ϵͳ״̬
				(*mtrFSMState[i].action)();			//����ִ��
				break;
			}
		}
	}
}

//����ٶȻ�
int mtrSpdLoopCond()
{
	if (MTR_SPEED_LOOP == sccw2.field.runLoop_mode)
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

void mtrSpdLoopAct()
{
	scsw2.field.runLoop_state	= MTR_SPEED_LOOP;
	sccw2.field.run_flag = sccw2.field.run_enable;
}

void mtrSpdLoopEntrance()
{
	spdLoopParaClr();
	scsw2.field.runLoop_state	= MTR_SPEED_LOOP;
	mtrState					= MTR_SPEED_LOOP;
}

void mtrSpdLoopExport()
{
}

//������ػ�
int mtrTrqLoopCond()
{
	if (MTR_TRQ_LOOP == sccw2.field.runLoop_mode)
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

void mtrTrqLoopAct()
{
	scsw2.field.runLoop_state	= MTR_TRQ_LOOP;
	sccw2.field.run_flag = sccw2.field.run_enable;
}

void mtrTrqLoopEntrance()
{
	spdLoopParaClr();
	scsw2.field.runLoop_state	= MTR_TRQ_LOOP;
	mtrState					= MTR_TRQ_LOOP;
}

void mtrTrqLoopExport()
{
}

//���������
int mtrCurLoopCond()
{
	if (MTR_CURRENT_LOOP == sccw2.field.runLoop_mode)
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

void mtrCurLoopAct()
{
	scsw2.field.runLoop_state	= MTR_CURRENT_LOOP;
	sccw2.field.run_flag = sccw2.field.run_enable;
}

void mtrCurLoopEntrance()
{
	ClearCtrlLoopPara();
	scsw2.field.runLoop_state	= MTR_CURRENT_LOOP;
	mtrState					= MTR_CURRENT_LOOP;
}

void mtrCurLoopExport()
{
}

//�����ѹ��
int mtrVoltLoopCond()
{
	if (MTR_VOLT_LOOP == sccw2.field.runLoop_mode)
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

void mtrVoltLoopAct()
{
	scsw2.field.runLoop_state	= MTR_VOLT_LOOP; //Ϊ�����״̬�˳�ʱ�����ܻص�ԭ��·������
	sccw2.field.run_flag = sccw2.field.run_enable;
}

void mtrVoltLoopEntrance()
{
	ClearCtrlLoopPara();
	scsw2.field.runLoop_state	= MTR_VOLT_LOOP;
	mtrState					= MTR_VOLT_LOOP;
}

void mtrVoltLoopExport()
{
}

#endif	// end of #ifndef _MTRFSM_C




