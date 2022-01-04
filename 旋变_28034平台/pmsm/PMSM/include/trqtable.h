/*
 * trqtable.h
 *
 *  Created on: 2014-3-20
 *      Author: ThinkPad
 */

#ifndef TRQTABLE_H_
#define TRQTABLE_H_

typedef struct
{
	_iq           udcmap;
	_iq           spdgrid_num;
	_iq           trqgrid_num;
	_iq           speed_min;
	_iq           speed_max;
	_iq           trq_max;
	_iq           trq_div;
	_iq           trq_div_INV;
	_iq           spd_div;
	_iq           spd_div_INV;
	_iq           area_div;
	_iq           area_div_INV;
	_iq           avgspeed_datanum;
	_iq           encoder_minspeed;
	_iq           encoder_maxspeed;
	_iq           encoder_detaspeed;
	_iq           encoder_num;
	_iq           ipark_minspeed;
	_iq           ipark_maxspeed;
	_iq           ipark_detaspeed;
	_iq           ipark_num;
}TRQ_TableVars;



#ifdef _TRQTABLE_C



const Uint16 trqlmt_eletable[];
const Uint16 trqlmt_gentable[];
const Uint16 trqidq_eletable[];
const Uint16 trqidq_gentable[];
const Uint16 trqcoeff_eletable[];
const Uint16 trqcoeff_gentable[];
const Uint16 idreflmt_eletable[];
const Uint16 idreflmt_gentable[];
const Uint16 speedKp_table[];
const Uint16 speedKi_table[];
const int16 ntc303gtemp_table[];
const int16 ntc104gtemp_table[];
const int16 encoder_offsetfwd[];
const int16 encoder_offsetrev[];
const int16 ipark_offsetfwd[];
const int16 ipark_offsetrev[];

TRQ_TableVars TRQ_Table;
#else
extern const Uint16 speedKp_table[];
extern const Uint16 speedKi_table[];
extern const Uint16 trqlmt_eletable[];
extern const Uint16 trqlmt_gentable[];
extern const Uint16 trqidq_eletable[];
extern const Uint16 trqidq_gentable[];
extern const Uint16 trqcoeff_eletable[];
extern const Uint16 trqcoeff_gentable[];
extern const Uint16 idreflmt_eletable[];
extern const Uint16 idreflmt_gentable[];
extern const int16 ntc303gtemp_table[];
extern const int16 ntc104gtemp_table[];
extern const int16 encoder_offsetfwd[];
extern const int16 encoder_offsetrev[];
extern const int16 ipark_offsetfwd[];
extern const int16 ipark_offsetrev[];

extern TRQ_TableVars TRQ_Table;

#endif

#endif /* TRQTABLE_H_ */
