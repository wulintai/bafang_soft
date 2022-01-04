/*
 * mtrFSM.h
 *
 *  Created on: 2014-3-19
 *      Author: ThinkPad
 */

#ifndef MTRFSM_H_
#define MTRFSM_H_

#define MTRFSM_STATE_NUM	4		//状态数
#define MTR_VOLT_LOOP		0		//电压环
#define MTR_CURRENT_LOOP	1		//电流环
#define MTR_TRQ_LOOP		2		//力矩环
#define MTR_SPEED_LOOP		3		//速度环

#ifdef	_MTRFSM_C

int  mtrState = MTR_TRQ_LOOP;
tag_FSMState   mtrFSMState[MTRFSM_STATE_NUM];

//===========================================================================
// 函数声明
//===========================================================================
void mtrFSMInit(void);
void mtrFSMCtrl(void);

//电机速度环
int mtrSpdLoopCond(void);
void mtrSpdLoopAct(void);
void mtrSpdLoopEntrance(void);
void mtrSpdLoopExport(void);

//电机力矩环
int mtrTrqLoopCond(void);
void mtrTrqLoopAct(void);
void mtrTrqLoopEntrance(void);
void mtrTrqLoopExport(void);

//电机电流环
int mtrCurLoopCond(void);
void mtrCurLoopAct(void);
void mtrCurLoopEntrance(void);
void mtrCurLoopExport(void);

//电机电压环
int mtrVoltLoopCond(void);
void mtrVoltLoopAct(void);
void mtrVoltLoopEntrance(void);
void mtrVoltLoopExport(void);

#else
//===========================================================================
// 外部引用接口声明
//===========================================================================
extern int  mtrState;
extern tag_FSMState   mtrFSMState[MTRFSM_STATE_NUM];

extern void mtrFSMInit(void);
extern void mtrFSMCtrl(void);


#endif	// end of #ifdef _MTRFSM_C


#endif /* MTRFSM_H_ */
