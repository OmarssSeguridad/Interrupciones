;//////////////////////////////////////////////////
;	Variable Aleatoria
;//////////////////////////////////////////////////

aleatoria:
	clr r26
	clr r17
	rcall A
	clr r16
	rcall EEPROM_write
	push r17
	rcall dii
	mov r16,r26;16,26
	rcall DATAWRT
	pop r17
	rcall unidades
	clr r16
	mov r16,r17
    rcall DATAWRT
	clr r17
	clr r28
	clr r26
	clr r27
	ret

unidades:
	ldi r27,0x0A
	subi r16,0x30
	mul r16,r27
	sub r17, r0	
	ldi r27,0x30
	add r17,r27
	ret

A:
	lds r17,0x46
	rcall EEPROM_read
	add r17,r28
modulo:
	cpi r17,0x33
	brlo mod
modul:
	subi r17,0x32
	cpi r17,0x32
	brsh modul
mod:
	cpi r17,0x0A
	brsh doo
	ldi r16,0x0A
	add r17,r16
doo:
	ret

dii:
	subi r17,0x0A
	inc r26
	cpi r17, 0x0A
	brsh dii
	ldi r27,0x30
	add r26,r27
	ret

EEPROM_write:
	sbic EECR,EEPE
	rjmp EEPROM_write
	ldi r28,0
	out EEARL, r28
	out EEARH, r28
	out EEDR ,r17
	sbi EECR, EEMPE
	sbi EECR, EEPE
	ret

EEPROM_read:
	push r27
	sbic EECR,EEPE
	rjmp EEPROM_read
	ldi r27,0x00
	out EEARH, r27
	out EEARL,r27
	sbi EECR, EERE
	in r28,EEDR
	pop r27
	ret