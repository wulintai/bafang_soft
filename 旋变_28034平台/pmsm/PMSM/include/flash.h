#ifndef FLASH_H_
#define FLASH_H_

#define STATUS_SUCCESS1  0

//MCU_ID,0x3f3ff8,0x11=48-60-72V platform,0x22=144V platform
#define MCU_ID  0x11

#ifdef _FLASH_C

FLASH_ST      FlashStatus = {0};

Uint16  Flash_FlagProgram(int type,unsigned char  baud_filetype);
void  App_FlashAPIInit(void);
void  Example_MemCopy(Uint16 *SourceAddr, Uint16* SourceEndAddr, Uint16* DestAddr);
void ProgFlashProc(unsigned int *data,int type);
unsigned int GetCrc8(unsigned int *data,unsigned int length);
void MyCallBackFunc(void);

#else

extern FLASH_ST      FlashStatus;

extern Uint16  Flash_FlagProgram(int type,unsigned char  baud_filetype);
extern void  App_FlashAPIInit(void);
extern void  Example_MemCopy(Uint16 *SourceAddr, Uint16* SourceEndAddr, Uint16* DestAddr);
extern void  ProgFlashProc(unsigned int *data,int type);
extern void MyCallBackFunc(void);

#endif /* FLASH_H_ */

#endif

