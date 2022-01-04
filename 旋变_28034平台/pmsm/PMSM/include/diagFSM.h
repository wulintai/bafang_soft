/*
 * diagFSM.h
 *
 *  Created on: 2014-4-15
 *      Author: ThinkPad
 */

#ifndef DIAGFSM_H_
#define DIAGFSM_H_

#define DIAGFSM_STATE_NUM 			10

#define DIAG_NON_TEST 				0
#define DIAG_HALL_OFFSET			1
#define DIAG_ENCODER_OFFSET			2
#define DIAG_ABZ_OFFSET             3
#define DIAG_MOTOR_INIT_POSITION	4
#define DIAG_MOTOR_FLUEX			5
#define DIAG_MOTOR_RS_LEARNING      6
#define DIAG_MOTOR_Ld_LEARNING  	7 				//学习电机直轴电感Ld
#define DIAG_MOTOR_Lq_LEARNING  	8
#define DIAG_ENCODER_EXACT_OFFSET   9  //精确调整

//========================================================================================
// 数据结构定义
//========================================================================================

#ifdef	_DIAGFSM_C

Uint16 diagState = DIAG_NON_TEST;
Uint16 diagFSM_runLoopModeOld;
Uint16 diagFSM_thetaModeOld;

//debug..
//Rs学习相关参数

tag_FSMState	diagFSMState[DIAGFSM_STATE_NUM] = {0};

void diagFSMInit(void);
void diagFSMCtrl(void);

//无动作
int diagNonTestCond(void);
void diagNonTestAct(void);
void diagNonTestEntrance(void);
void diagNonTestExport(void);

//旋变初始角测试
int diagEncOffsetTuneCond(void);
void diagEncOffsetTuneAct(void);
void diagEncOffsetTuneEntrance(void);
void diagEncOffsetTuneExport(void);

//旋变精确调整
int diagEncOffsetexactTuneCond(void);
void diagEncOffsetexactTuneAct(void);
void diagEncOffsetexactTuneEntrance(void);
void diagEncOffsetexactTuneExport(void);

//电机Rs学习
int diagMotorRsLearningCond(void);
void diagMotorRsLearningAct(void);
void diagMotorRsLearningEntrance(void);
void diagMotorRsLearningExport(void);

//电机Ld学习
int diagMotorLdLearningCond(void);
void diagMotorLdLearningAct(void);
void diagMotorLdLearningEntrance(void);
void diagMotorLdLearningExport(void);

//电机Lq学习
int diagMotorLqLearningCond(void);
void diagMotorLqLearningAct(void);
void diagMotorLqLearningEntrance(void);
void diagMotorLqLearningExport(void);

//电机转子初始位置学习
int diagMotorPositionLearningCond(void);
void diagMotorPositionLearningEntrance(void);
void diagMotorPositionLearningAct(void);
void diagMotorPositionLearningExport(void);

int diagMotorFluexLearningCond(void);
void diagMotorFluexLearningEntrance(void);
void diagMotorFluexLearningAct(void);
void diagMotorFluexLearningExport(void);

#else

extern Uint16 diagFSM_runLoopModeOld;
extern Uint16 diagFSM_thetaModeOld;
extern Uint16 diagState;


extern void diagFSMInit(void);
extern void diagFSMCtrl(void);

extern tag_FSMState	   diagFSMState[DIAGFSM_STATE_NUM];



#endif

#endif /* DIAGFSM_H_ */
