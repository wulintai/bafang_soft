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
			mtrFSMState[i].cond		= &mtrSpdLoopCond;        //sccw2.field.runLoop_mode
			mtrFSMState[i].action	= &mtrSpdLoopAct;         //sccw2.field.runLoop_mode=速度环、逆变器状态
			mtrFSMState[i].entrance	= &mtrSpdLoopEntrance;    //速度环参数设置，设置系统状态
			mtrFSMState[i].export	= &mtrSpdLoopExport;      //空
			break;
		case MTR_TRQ_LOOP:
			mtrFSMState[i].cond		= &mtrTrqLoopCond;        //sccw2.field.runLoop_mode
			mtrFSMState[i].action	= &mtrTrqLoopAct;         //sccw2.field.runLoop_mode=力矩环、逆变器状态
			mtrFSMState[i].entrance	= &mtrTrqLoopEntrance;    //速度环参数清零、设置系统状态
			mtrFSMState[i].export	= &mtrTrqLoopExport;      //空
			break;
		case MTR_CURRENT_LOOP:
			mtrFSMState[i].cond		= &mtrCurLoopCond;        //sccw2.field.runLoop_mode
			mtrFSMState[i].action	= &mtrCurLoopAct;         //sccw2.field.runLoop_mode=电流环、逆变器状态
			mtrFSMState[i].entrance	= &mtrCurLoopEntrance;    //电流环参数清零、设置系统状态
			mtrFSMState[i].export	= &mtrCurLoopExport;      //空
			break;
		case MTR_VOLT_LOOP:
			mtrFSMState[i].cond		= &mtrVoltLoopCond;       //sccw2.field.runLoop_mode
			mtrFSMState[i].action	= &mtrVoltLoopAct;        //sccw2.field.runLoop_mode=电压环、逆变器状态
			mtrFSMState[i].entrance	= &mtrVoltLoopEntrance;   //电流环参数清零、设置系统状态
			mtrFSMState[i].export	= &mtrVoltLoopExport;     //空
			break;
		default:
			mtrFSMState[i].cond		= &mtrTrqLoopCond;        //sccw2.field.runLoop_mode = 力矩环
			mtrFSMState[i].action	= &mtrTrqLoopAct;         //设为力矩环
			mtrFSMState[i].entrance	= &mtrTrqLoopEntrance;    //速度环参数清零、设置系统状态
			mtrFSMState[i].export	= &mtrTrqLoopExport;      //空
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
				(*mtrFSMState[mtrState].export)();	//退出当前系统状态
				(*mtrFSMState[i].entrance)();		//进入新系统状态
				(*mtrFSMState[i].action)();			//立即执行
				break;
			}
		}
	}
}

//电机速度环
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
	sccw2.field.run_flag = sccw2.field.run_enable;     //逆变器状态
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

//电机力矩环
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

//电机电流环
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

//电机电压环
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
	scsw2.field.runLoop_state	= MTR_VOLT_LOOP; //为了诊断状态退出时，仍能回到原环路控制中
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




