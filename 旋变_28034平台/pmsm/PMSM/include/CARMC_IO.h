#ifndef _CARMC_IO_H_
#define _CARMC_IO_H_

#ifdef _CARMC_IO_C_
	void sub_io_init(void);
#else
	extern void sub_io_init(void);
#endif

#endif
