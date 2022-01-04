#ifndef VDQLMT_H_
#define VDQLMT_H_

typedef struct
{
	/*_iq IdPid_Out_fwd;
	_iq IqPid_Out_fwd;
	_iq Vdc;
    _iq Id_PI_Vd;
    _iq Iq_PI_Vq;
    _iq VdqLmt_VdMax;
    _iq VdqLmt_VqMax;*/
    _iq VdqLmt_Vd;
    _iq VdqLmt_Vq;
}tag_vdqLmtvars;

#ifdef	_VDQLMT_C

tag_vdqLmtvars vdqLmtvars = {0};

void vdqlmtinit(void);
void vdqlmtin(void);
void vdqlmtctrl(void);
void vdqlmtout(void);
void vdqlmtclc(void);

#else

extern tag_vdqLmtvars vdqLmtvars;

extern void vdqlmtinit(void);
extern void vdqlmtin(void);
extern void vdqlmtctrl(void);
extern void vdqlmtout(void);
extern void vdqlmtclc(void);

#endif

#endif /* VDQLMT_H_ */

