;Calculate signed:Quot32=Num32<<16/Den16 Unm32 longtype Den16 unsigned int
;_DV32_16((long)Num,(long)Den);  acc=num *-sp[0]=den
;RETUREN VALUE STORED IN ACC
;	.SECT	".text"
	.sect "ramfuncs"  
;	.global _DV32_16
;_DV32_16:
;        ADDB      SP,#1
;  	TBIT	@AH,#15
;	SBF	$10,NTC
;	NEG	ACC
;	ADDB	ACC,#1
;$10:
;	RPT	#15
;	||SUBCU ACC,*-SP[5]
;	MOV	*-SP[0],AL
;	MOV	ACC,@AH<<16
;	RPT	#15
;	||SUBCU ACC,*-SP[5]
;	MOV	AH,*-SP[0]
;	NEGTC	ACC
;	SUBB	SP,#1
;       LRETR
 
;Calculate unsigned :Quot32=Num32/Den32 Unm32 unsigned long Den32 unsigned long
;_DV32_32((unsigned long)Num,(unsigned long)Den);  acc=num *-sp[0]=den 
;RETUREN VALUE STORED IN AL
;	.global _DV32_32
;_DV32_32:
;	MOVL	XAR4,@P
;	MOVL	P,@ACC
;	MOVB	ACC,#0
;	RPT	#31
;	||SUBCUL ACC,*-SP[3]
;	MOV	AL,@PL
;	MOVL	P,@XAR4
;	LRETR	
;Calculate signed :Quot32=Num32<<16/Den32 Unm32 long Den32 unsigned long
;_DV48_32((unsigned long)Num<<16,(unsigned long)Den);  acc=num *-sp[0]=den
;RETUREN VALUE STORED IN ACC
;	.global _DV48_32
;_DV48_32:
;	PUSH	XAR4
;	MOVL	XAR4,@P
;	CLRC	TC
;	ABSTC	ACC
;	MOV	PH,@AL
;	MOV	@PL,#0
;	MOV	ACC,@AH
;	RPT	#31
;	||SUBCUL ACC,*-SP[5]
;	MOVL	ACC,@P
;	NEGTC	ACC	
;	MOVL	P,@XAR4
;	POP	XAR4
;	LRETR		
;Calculate unsigned signed:Quot32=Num32*Num32/Den32 Num1 signed long,Num2 unsigned long Den32 unsigned long
;_DV3232_32((long)Num1,(unsigned long)Num2,(unsigned long)Den);  acc=num *-sp[0]=den
;RETUREN VALUE STORED IN ACC
	.global _DV3232_32
_DV3232_32:
	PUSH	XAR4
	CLRC	TC
	MOVL	XAR4,@P
	ABSTC	ACC
	MOVL	XT,@ACC
	MOVL	ACC,*-SP[6]
	IMPYL	P,XT,*-SP[6]
	QMPYL	ACC,XT,*-SP[6]
	RPT	#31
	||SUBCUL ACC,*-SP[8]
	MOVL	ACC,@P
	NEGTC	ACC	
	MOVL	P,@XAR4
	POP	XAR4
	LRETR		
;Calculate signed:Quot64=Num32*Num32/Den32	Num1 signed long,Num2 unsigned long Den32 unsigned long
;void _DV64_32(long * Result,long Num1,unsigned long Num2,unsigned long Den)
;RETUREN VALUE STORED IN ACC
;	.global _DV64_32                                                                    
;_DV64_32:
;;* AR4   assigned to _Result
;;* AL    assigned to _Num1
;;* AR4   assigned to _Result
;;* AL    assigned to _Num1
;;* AR7   assigned to _Num2
;;* AR6   assigned to _Den
;    	ADDB    SP,#2
;	CLRC	TC
;	ABSTC	ACC
;	MOVL	XT,@ACC
;	MOVL	ACC,*-SP[6]           
;	IMPYL	P,XT,*-SP[6] 		  ;LOWER 32 BITS
;	QMPYL	ACC,XT,*-SP[6]        ;HIGHER 32 BITS
	
;	MOVL	@XAR6,P
;	MOVL	P,@ACC
;	MOVB	ACC,#0
;	RPT		#31
;	||SUBCUL	ACC,*-SP[8]        ;DEN32
;	MOVL	@XAR7,P					;HIGH QUOTIENT IN XAR7
;	MOVL	P,@XAR6
;	RPT		#31
;	||SUBCUL ACC,*-SP[8]			;DEN32
;	MOVL	ACC,@XAR7				;HIGH QUOTIENT IN ACC
;	SB 		DV64_32_B0,NTC
;	NEG64	ACC:P
;DV64_32_B0:	
;	MOVL	*XAR4++,P
;	MOVL	*XAR4,ACC

;    SUBB      SP,#2
;    LRETR
;Calculate unsigned signed:Quot32=Num32<<16/Den32 Unm32 unsigned long Den32 unsigned long
;_UDV48_32((unsigned long)Num,(unsigned long)Den);  acc=num *-sp[0]=den
;RETUREN VALUE STORED IN ACC
;	.global _UDV48_32
;_UDV48_32:
;	PUSH	XAR4
;	MOVL	XAR4,@P
;	MOV	PH,@AL
;	MOV	@PL,#0
;	MOV	ACC,@AH
;	RPT	#31
;	||SUBCUL ACC,*-SP[5]
;	MOV	ACC,@P
;	MOVL	P,@XAR4
;	POP	XAR4
;	LRETR		;Calculate signed 32bit * unsigned 16bit then right shifted 12bit;
;;_MP32_32(long m1,long m2) return (LONG)
;	.global _MP32_32
;_MP32_32:
;	PUSH	XT
;	PUSH	P
;	PUSH	XAR4
;	CLRC	TC
;	MOVL	XT,@ACC
;	MOVL	ACC,*-SP[10]
;	IMPYL	P,XT,*-SP[10]
;	QMPYL	ACC,XT,*-SP[10]
;	SETC	SXM
;	ABSTC	ACC
;	MOVL	XAR4,#200H
;	CMPL	ACC,XAR4
;	B	MPB00,GEQ
;	NEGTC	ACC	
;	ASR64	ACC:P,#10
;	MOV	ACC,@P
;	B	MPB01,UNC
;MPB00:
;	MOV	ACC,#7FFEH
;	NEGTC	ACC
;MPB01:
;	CLRC	SXM
;	POP	XAR4
;	POP	P
;	POP	XT
;	LRETR
		
	
