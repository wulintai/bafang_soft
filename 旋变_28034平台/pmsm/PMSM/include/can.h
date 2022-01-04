#ifndef _CAN_H_
#define _CAN_H_

#define CAN_A_INDEX				1
#define CAN_B_INDEX				2

#define MAX_CAN_NODE_NUM		2
#define MAX_CAN_MBOX_NUM		32

#ifdef _CAN_C
	void canInit(void);
	void canbunpackMbx31(void);//8888
	void canaunpackMbx31(void);
	void canTransmitDriver(int canIndex, int MBoxIndex, struct MBOX* pMessage);
	void canReceiveDriver(int canIndex, int MBoxIndex, struct MBOX* pMessage);
	struct MBOX	ECanaMBoxData[MAX_CAN_MBOX_NUM];  //can数据缓冲区
#else
	extern void canInit(void);
	extern void canbunpackMbx31(void);//8888
	extern void canaunpackMbx31(void);//8888
	extern void canTransmitDriver(int canIndex, int MBoxIndex, struct MBOX* pMessage);
	extern void canReceiveDriver(int canIndex, int MBoxIndex, struct MBOX* pMessage);
	extern struct MBOX	ECanaMBoxData[MAX_CAN_MBOX_NUM];  //can数据缓冲区
#endif

#endif
