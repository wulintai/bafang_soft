/*
 * peripheral.h
 *
 *  Created on: 2014-3-19
 *      Author: ThinkPad
 */

#ifndef PERIPHERAL_H_
#define PERIPHERAL_H_




#ifdef _PERIPHERAL_C

int32   IGBT_Clock_Cnt = 0;
int32   IGBT_Clock_Enable = 0;
int32   IGBT_Clock_Flag = 0;

void InitIOPeripheral(void);
void IGBT_Ctrl(void);

void EPWM_Enable(void);
void EPWM_Disable(void);
void IGBT_Clock(void);
void IGBT_OFF(void);
void IGBT_ON(void);

void diagIO(void);
#else

extern int32   IGBT_Clock_Cnt;
extern int32   IGBT_Clock_Enable;
extern int32   IGBT_Clock_Flag;



extern void InitIOPeripheral(void);
extern void IGBT_Ctrl(void);
extern void EPWM_Enable(void);
extern void EPWM_Disable(void);
extern void IGBT_Clock(void);
extern void IGBT_OFF(void);
extern void IGBT_ON(void);

extern void diagIO(void);

#endif

#endif /* PERIPHERAL_H_ */
