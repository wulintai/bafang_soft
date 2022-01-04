/*
 * mtrFSM.h
 *
 *  Created on: 2014-3-19
 *      Author: ThinkPad
 */

#ifndef MTRFSM_H_
#define MTRFSM_H_

#define MTRFSM_STATE_NUM	4		//״̬��
#define MTR_VOLT_LOOP		0		//��ѹ��
#define MTR_CURRENT_LOOP	1		//������
#define MTR_TRQ_LOOP		2		//���ػ�
#define MTR_SPEED_LOOP		3		//�ٶȻ�

#ifdef	_MTRFSM_C

int  mtrState = MTR_TRQ_LOOP;
tag_FSMState   mtrFSMState[MTRFSM_STATE_NUM];

//===========================================================================
// ��������
//===========================================================================
void mtrFSMInit(void);
void mtrFSMCtrl(void);

//����ٶȻ�
int mtrSpdLoopCond(void);
void mtrSpdLoopAct(void);
void mtrSpdLoopEntrance(void);
void mtrSpdLoopExport(void);

//������ػ�
int mtrTrqLoopCond(void);
void mtrTrqLoopAct(void);
void mtrTrqLoopEntrance(void);
void mtrTrqLoopExport(void);

//���������
int mtrCurLoopCond(void);
void mtrCurLoopAct(void);
void mtrCurLoopEntrance(void);
void mtrCurLoopExport(void);

//�����ѹ��
int mtrVoltLoopCond(void);
void mtrVoltLoopAct(void);
void mtrVoltLoopEntrance(void);
void mtrVoltLoopExport(void);

#else
//===========================================================================
// �ⲿ���ýӿ�����
//===========================================================================
extern int  mtrState;
extern tag_FSMState   mtrFSMState[MTRFSM_STATE_NUM];

extern void mtrFSMInit(void);
extern void mtrFSMCtrl(void);


#endif	// end of #ifdef _MTRFSM_C


#endif /* MTRFSM_H_ */
