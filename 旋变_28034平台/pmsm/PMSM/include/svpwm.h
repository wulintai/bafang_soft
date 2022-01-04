#ifndef _SVPWM_H_
#define _SVPWM_H_

typedef char s8;
typedef short s16;
typedef long s32;

typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned long u32;

// _IQ(1)
#define IQ_Z_1            16777216
// _IQ(-1)
#define IQ_F_1            -16777216

// _IQ(0.5)
#define IQ_Z_1_DIV_2      8388608
// _IQ(-0.5)
#define IQ_F_1_DIV_2      -8388608

// _IQ(0.8660254)
#define IQ_Z_SQRT3_DIV_2  14529495
// _IQ(-0.8660254)
#define IQ_F_SQRT3_DIV_2  -14529495

// _IQ(0.95)
#define IQ_Z_0_DOT_95     15938355
// _IQ(-0.95)
#define IQ_F_0_DOT_95     -15938355

//_IQ(0.01)
#define _IQ_Z_0_DOT_01    167772

//_IQ(0.1)
#define _IQ_Z_0_DOT_1     1677721


#endif
