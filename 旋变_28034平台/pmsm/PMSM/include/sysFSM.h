/*
 * sysFSM.h
 *
 *  Created on: 2014-3-19
 *      Author: ThinkPad
 */

#ifndef SYSFSM_H_
#define SYSFSM_H_

//========================================================================================
// ��������
//========================================================================================
// ϵͳ״̬����
#define SYSFSM_STATE_NUM	9

//ϵͳ״̬����
#define SYS_INIT            0       //��ʼ�����
#define SYS_READY			1 		//��ǿ�������û�п����ź�
#define SYS_RUN				2       //����
#define SYS_SPDLMT			3       //�ٶ������ٶȻ�
#define SYS_ZEROSTOP		4       //��λ�����ػ�
#define SYS_POWERDOWN		5       //��ǿ��
#define SYS_DIAGNOSIS       6       //���ѧϰ
#define SYS_FAULT			7       //����
#define SYS_UDCLOOP         8       //��ѹ�ջ�


typedef int (*FUNCPTR_COND)();
typedef void (*FUNCPTR_ACTION)();
typedef void (*FUNCPTR_ENTRANCE)();
typedef void (*FUNCPTR_EXPORT)();

//========================================================================================
// ���ݽṹ����
//========================================================================================
typedef struct
{
	FUNCPTR_COND		cond;		//״̬����������
	FUNCPTR_ACTION		action;		//״̬����Ϊ
	FUNCPTR_ENTRANCE	entrance;	//��״̬ʱ��׼����Ϊ
	FUNCPTR_EXPORT		export;		//��״̬ʱ��׼����Ϊ
}tag_FSMState;

#ifdef	_SYSFSM_C
int sysState = SYS_POWERDOWN;
tag_FSMState sysFSMState[SYSFSM_STATE_NUM] = {0};
void sysFSMInit();
void sysFSMCtrl();
//ϵͳ��ʼ�� �����ϵ��Լ����    0
int sysInitCond();
void sysInitAct();
void sysInitEntrance();
void sysInitExport();

//ϵͳ׼���� ǿ���ϵ����      1
int sysReadyCond();
void sysReadyAct();
void sysReadyEntrance();
void sysReadyExport();


//ϵͳ����               2
int sysRunCond();
void sysRunAct();
void sysRunEntrance();
void sysRunExport();



//�����ٶȻ�����     3
int sysSpdLmtCond();
void sysSpdLmtAct();
void sysSpdLmtEntrance();
void sysSpdLmtExport();
//פ��              4
int sysZeroStopCond();
void sysZeroStopAct();
void sysZeroStopEntrance();
void sysZeroStopExport();



//ϵͳ�µ�      5
int sysPowerdownCond();
void sysPowerdownAct();
void sysPowerdownEntrance();
void sysPowerdownExport();



//��ѧϰ		 6
int sysDiagnosisCond(void);
void sysDiagnosisAct(void);
void sysDiagnosisEntrance(void);
void sysDiagnosisExport(void);


//ϵͳ����         7
int sysFaultCond();
void sysFaultAct();
void sysFaultEntrance();
void sysFaultExport();

//��ѹ�ջ�        8
int sysUdcloopCond();
void sysUdcloopAct();
void sysUdcloopEntrance();
void sysUdcloopExport();
#else
//========================================================================================
// �ⲿ���ýӿ�����
//========================================================================================
extern int sysState;

extern void sysFSMInit();
extern void sysFSMCtrl();

#endif	// end of #ifdef _SYSFSM_C

#endif /* SYSFSM_H_ */
