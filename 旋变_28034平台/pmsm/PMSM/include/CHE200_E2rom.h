#ifndef CHE200_E2ROM
#define CHE200_E2ROM

#ifdef _CHE200_E2ROM_C_
	Uint16 error_code = 0;
	void	STARTE2PROM(void);
	void	ENDE2PROM(void);
	void	EEPROM_WRITE_WORD(Uint16 address,Uint16 data);
	Uint16	EEPROM_WRITE_BYTE(Uint16 data);
	Uint16	WRITEE2PROMAWORD(Uint16 data);
	Uint16	READEEAWORD(void);
	Uint16	EEPROM_READ_WORD(Uint16 address);
#else
	extern Uint16 error_code;
	extern 	void	STARTE2PROM(void);
	extern 	void	ENDE2PROM(void);
	extern 	void	EEPROM_WRITE_WORD(Uint16 address,Uint16 data);
	extern 	Uint16	EEPROM_WRITE_BYTE(Uint16 data);
	extern	Uint16	WRITEE2PROMAWORD(Uint16 data);
	extern	Uint16	READEEAWORD(void);
	extern	Uint16	EEPROM_READ_WORD(Uint16 address);
#endif

#endif
