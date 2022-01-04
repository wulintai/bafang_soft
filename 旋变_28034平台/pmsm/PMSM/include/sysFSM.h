/*
 * sysFSM.h
 *
 *  Created on: 2014-3-19
 *      Author: ThinkPad
 */

#ifndef SYSFSM_H_
#define SYSFSM_H_

//========================================================================================
// 常量定义
//========================================================================================
// 系统状态个数
#define SYSFSM_STATE_NUM	9

//系统状态定义
#define SYS_INIT            0       //初始化完成
#define SYS_READY			1 		//上强电结束，没有开管信号
#define SYS_RUN				2       //运行
#define SYS_SPDLMT			3       //速度限制速度环
#define SYS_ZEROSTOP		4       //零位置力矩环
#define SYS_POWERDOWN		5       //下强电
#define SYS_DIAGNOSIS       6       //电机学习
#define SYS_FAULT			7       //故障
#define SYS_UDCLOOP         8       //电压闭环


typedef int (*FUNCPTR_COND)();
typedef void (*FUNCPTR_ACTION)();
typedef void (*FUNCPTR_ENTRANCE)();
typedef void (*FUNCPTR_EXPORT)();

//========================================================================================
// 数据结构定义
//========================================================================================
typedef struct
{
	FUNCPTR_COND		cond;		//状态的满足条件
	FUNCPTR_ACTION		action;		//状态的行为
	FUNCPTR_ENTRANCE	entrance;	//进状态时的准备行为
	FUNCPTR_EXPORT		export;		//出状态时的准备行为
}tag_FSMState;

#ifdef	_SYSFSM_C
int sysState = SYS_POWERDOWN;
tag_FSMState sysFSMState[SYSFSM_STATE_NUM] = {0};
void sysFSMInit();
void sysFSMCtrl();
//系统初始化 弱电上电自检完成    0
int sysInitCond();
void sysInitAct();
void sysInitEntrance();
void sysInitExport();

//系统准备好 强电上电完成      1
int sysReadyCond();
void sysReadyAct();
void sysReadyEntrance();
void sysReadyExport();


//系统运行               2
int sysRunCond();
void sysRunAct();
void sysRunEntrance();
void sysRunExport();



//超速速度环限制     3
int sysSpdLmtCond();
void sysSpdLmtAct();
void sysSpdLmtEntrance();
void sysSpdLmtExport();
//驻坡              4
int sysZeroStopCond();
void sysZeroStopAct();
void sysZeroStopEntrance();
void sysZeroStopExport();



//系统下电      5
int sysPowerdownCond();
void sysPowerdownAct();
void sysPowerdownEntrance();
void sysPowerdownExport();



//自学习		 6
int sysDiagnosisCond(void);
void sysDiagnosisAct(void);
void sysDiagnosisEntrance(void);
void sysDiagnosisExport(void);


//系统故障         7
int sysFaultCond();
void sysFaultAct();
void sysFaultEntrance();
void sysFaultExport();

//电压闭环        8
int sysUdcloopCond();
void sysUdcloopAct();
void sysUdcloopEntrance();
void sysUdcloopExport();
#else
//========================================================================================
// 外部引用接口声明
//========================================================================================
extern int sysState;

extern void sysFSMInit();
extern void sysFSMCtrl();

#endif	// end of #ifdef _SYSFSM_C

#endif /* SYSFSM_H_ */
