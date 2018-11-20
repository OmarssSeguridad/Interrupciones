;
; Practica.asm
;
; Created: 4/10/2018 2:06:09 PM
; Author : Luis, Jair, Marco

	.equ columna_PRT = PORTD
	.equ columna_DDR = DDRD
	.equ columna_PIN = PIND

	.equ fila_PRT	 = PORTC
	.equ fila_DDR    = DDRC
	.equ fila_PIN    = PINC

	.def control	 = r16 ;registro de operaciones
	.def control1	 = r17 ;registro de operaciones 
	.def contador	 = r18
	.def bandera	 = r19
	.def espera		 = r20

	jmp START

	.org 0x0002
	jmp EXT_INT0
	
	.org 0x0004
	jmp EXT_INT1

	.org 0x0006
	jmp EXT_INT2

	.org 0x0008
	jmp EXT_INT3

	.include "_LCD.inc"

	.include "EEPROM.inc"

START:

	;call USART_Init

	ldi  r16, HIGH(RAMEND)		;Main program start
	out  SPH, r16				;Set Stack pointer to top of RAM
	ldi  r16, LOW(RAMEND)
	out  SPL, r16		

	ldi  r16, 0x0F		
	out  columna_PRT, r16		;Habilitamos las resistencias PULL UP	

	ldi  r16, 0xF8
	out  fila_DDR, r16			;Habilitamos las resistencias PULL UP

	ldi	 r16, (1 << INT0) | (1 << INT1) | (1 << INT2) | (1 << INT3)
	out	 0x1D, r16			;Se asigna el valor 0x03 al registro EIMSK=0x1D

	ldi	 r16, (1 << ISC01) | (1 << ISC11) | (1 << ISC21) | (1 << ISC31)
	sts	 0x69, r16			;EICRA - Se le asigna el valor 0xFF para que las
                            ;interrupciones reaccionen al flanco de bajada
		 
	sei					;Se habilitan las interrupciones globales*/

	ldi r16,(1<< CS00)
	sts 0x45,r16

	ldi  contador, 0x00		;Contador de control
	ldi  bandera , 0x00
	
	call LCD_INIT

    push r21
    
        ldi r21,0x00
        mov r5,r21
        mov r4,r21
        mov r2,r21

    pop r21

	inc contador

	;call aleatoria

;///////////////////////////////////////////////
;   loop y recorrer texto
;///////////////////////////////////////////////

loop:
    
	;call USART_Receive

    push r21
    
        ldi r21,0x0F
        mov r6, r21
    
    pop r21
    
	call tiempo1
	call tiempo1
	call tiempo1
	call tiempo1
	call tiempo1
	call tiempo1
	call tiempo1

	cp r2,r6
	brge recorrerT
    
    inc r2
    
	rjmp loop
    
    
recorrerT:
    
	call _Desplaza
	call tiempo1
	call tiempo1
	call tiempo1
	call tiempo1
	call tiempo1
	call tiempo1
	call tiempo1

	rjmp loop
    
;///////////////////////////////////////////////
;   interrupciones
;///////////////////////////////////////////////

EXT_INT0:				;Renglon 1
	call barrido
	reti

EXT_INT1:				;Renglon 2
	call barrido
	reti

EXT_INT2:				;Renglon 3
	call barrido
	reti

EXT_INT3:				;Renglon 4
	call barrido
	reti

barrido:
					;dato	high	low	
	ldi  r16, 0xF0	;0b		1111	0000
	out  fila_PRT, r16	
	call compara	;primera fila

	ldi  r16, 0xE8	;0b		1110	1000
	out  fila_PRT, r16	
	call compara	;segunda fila
	
	ldi  r16, 0xD8	;0b		1101	1000
	out  fila_PRT, r16	
	call compara	;tercera fila

	ldi  r16, 0xB8	;0b		1011	1000
	out  fila_PRT, r16	
	call compara	;cuarta fila

	ldi  r16, 0x78	;0b		0111	1000
	out  fila_PRT, r16	
	call compara	;quinta fila

	ldi  r16, 0x00	;0b		0000	0000
	out  fila_PRT, r16

	ret

					;Comparaciones para encontrar fila
compara:
	in   r16, fila_PIN	;Lectura de datos PINC
	andi r16, 0XF8	;Eliminamos la parte baja
	
	ldi  r17, 0x00
	cpi  r16, 0xF0	;0b		1111	0000
	breq buscafila

	ldi  r17, 0x04	;dato	high	low
	cpi  r16, 0xE8	;0b		1110	1000
	breq buscafila
	
	ldi  r17, 0x08	 
	cpi  r16, 0xD8	;0b		1101	1000
	breq buscafila
	
	ldi  r17, 0x0C
	cpi  r16, 0xB8	;0b		1011	1000
	breq buscafila

	ldi  r17, 0x10
	cpi  r16, 0x78	;0b		0111	1000
	breq buscafila

	ret

buscafila:

	in   r16, columna_PIN
	andi r16, 0X0F
	
	inc  r17		;dato	high	low
	cpi  r16, 0x0E	;0b		1110	1000
	breq escribe	;cuarto dato

	inc  r17
	cpi  r16, 0x0D	;0b		1101	1000
	breq escribe	;tercer dato

	inc  r17
	cpi  r16, 0x0B	;0b		1011	1000
	breq escribe	;segundo dato

	inc  r17
	cpi  r16, 0x07	;0b		0111	1000
	breq escribe	;primer dato

	ret

escribe:

	/*cpi control1,0x10
	breq BorraLCD					
	
	call imprime*/
/*	cpi control1,0x13
	breq derechaCursor

	cpi control1,0x11
	breq izquierdaCursor*/

	cpi  contador, 0x1F	;Dejamos de escribir cuando ya se han escrito 60 caracteres
	brlt imprime

	clr  r17

	ret

derechaCursor:
	
	clr r2

	cpi contador,0x1F
	brge salirCursorD

	inc contador
	
	call CambiaCusorD

salirCursorD:
	ret

izquierdaCursor:

	clr r2

	cpi contador,0x02
	brlo salirCursorI

	dec contador

	call CambiaCusorI

salirCursorI:
	ret

imprime:
    
    clr r2

	;cpi control1,0x01
	;breq VarAl
	
	cpi contador,0x1F
	brlo Continuar
		
	ret

BorraLCD:
	
	call limpiar

	push r21
    
        ldi r21,0x00
        mov r5,r21
        mov r4,r21
        mov r2,r21

		ldi contador,0x01

    pop r21

	ret
	
VarAl:
	
	cpi  contador, 0x1C	;Dejamos de escribir cuando ya se han escrito 60 caracteres
	brge salirVA 

	inc contador
	inc contador
	call aleatoria

	;call escribir_eeprom2

	call tiempo1
	call tiempo1
	call tiempo1
	call tiempo1
	call tiempo1
	call tiempo1

salirVA:

	ret

Continuar:
	
	inc  contador
	ldi r30,LOW(VARS<<1)
	ldi r31, HIGH(VARS)
	ciclovar:
		dec  r17
		lpm  r16, Z+
		cpi  r17, 0x00
		brne ciclovar
	tiempo:
		in   espera, columna_PIN
		andi espera, 0x0F
		cpi  espera, 0x0F
		brne tiempo

	call DATAWRT
	clr	 r16

	ret
