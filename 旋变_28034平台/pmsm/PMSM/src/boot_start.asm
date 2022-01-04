;***********************************************************************
;for update entry point 0213　luoj
;***********************************************************************
WD_DISABLE	.set	1		;set to 1 to disable WD, else set to 0

    .ref    _c_int00
    .global boot_start

    .sect "progMCUidVer"
	.int	0xFf11		;MCU_ID,0x3f3ff8,0x11=同步主驱,0x22=同步ISG,03=异步ISG,04=异步油泵,05=异步汽泵,06=异步空调,07=同步空调,08=同步油泵,09=同步汽泵,252=异步主驱
	.int	0xFf02		;MCU_ver,0x3f3ff9
    .int	0xFfff		;flashtype,0x3f3ffA,0xFff1-cana,0xFff2-canb,0xFff3-coma
	.int	0xFfff		;baud,0x3f3ffB,0xFff1=125,0xFff4=500k,0xFff2或其它=250k
	.int	0xFfff		;filetype,0x3f3ffC,0xFffe=加密,0xFfff或其它=未加密
	.int	0xFfff		;0x3f3ffD
	.int	0xFfff		;0x3f3ffE
	.int	0x55BB		;flashflg,0x3f3ffF

    .sect "bootstart"
boot_start:
    .if WD_DISABLE == 1
        LB wd_disable       ;Branch to watchdog disable code
    .else
        LB _c_int00         ;Branch to start of boot.asm in RTS library 
    .endif

;end codestart section
;***********************************************************************
; Function: wd_disable
; Description: Disables the watchdog timer
;***********************************************************************
    .if WD_DISABLE == 1
    .text
wd_disable:
    SETC OBJMODE        ;Set OBJMODE for 28x object code
    EALLOW              ;Enable EALLOW protected register access
    MOVZ DP, #7029h>>6  ;Set data page for WDCR register
    MOV @7029h, #0068h  ;Set WDDIS bit in WDCR to disable WD
    EDIS                ;Disable EALLOW protected register access
    LB _c_int00         ;Branch to start of boot.asm in RTS library 
    .endif

;end wd_disable
	.end	
;***********************************************************************
; End of file.
;***********************************************************************


