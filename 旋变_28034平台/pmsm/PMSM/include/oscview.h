#ifndef OSCVIEW_H_
#define OSCVIEW_H_

#if 0

#define OSC_NUM 1//500

typedef struct
{
	int view0[OSC_NUM];
	int view1[OSC_NUM];
	int view2[OSC_NUM];
	int view3[OSC_NUM];
	Uint16 OSC_enable;
	Uint16 point;
	Uint16 transmitFlag;
}OSC_Var;

#ifdef _OSCVIEW_C

OSC_Var osc = {0};

void oscview(void);
void oscinit(void);

#else

extern OSC_Var osc;

extern void oscview(void);
extern void oscinit(void);

#endif

#endif

#endif /* OSC_VIEW_H_ */

