;Funciones:
;LCD_INIT:	Inicializa el LCD
;_linea2:	Escribe en la segunda linea del LCD
;limpiar:	Borra los datos impresos del LCD
;CMNDWRT:	Escribe una instrucción al LCD
;DATAWRT:	Escribe un dato al LCD (sin perder el dato anterior)
;DELAY_2ms:	Retardo de 2 milisegundos
;SDELAY:	Retardo de 6 ciclos maquina

;Constantes:
;MSG:		Hola Mundo, 0 

;//////////////////////////////////////////////////////////
;                Configuracion de Puertos
;//////////////////////////////////////////////////////////
	.equ	LCD_PRT	=	PORTA
	.equ	LCD_DDR =	DDRA
	.equ	LCD_PIN =	PINA
	.equ	LCD_RS  =	0
	.equ	LCD_RW  =	1
	.equ	LCD_EN	=	2

	MSG:	.DB	"Hola Mundo!", 0	;Constante que conteiene Hola Mundo!
	;			 1   2   3   4   5   6   7   8   9   10  11  12  13  14  15  16  17  18  19  20
	VARS:	.DB 'f','F','#','*','1','2','3','^','4','5','6','v','7','8','9','E','<','0','>','I'
	VARS2:	.DB 'f','F','#','*','A','B','C','D','E','F','G','H','I','J','K','@','L','M','N','@'
	VARS3:	.DB 'f','F','#','*','O','P','Q','R','S','T','U','V','W','X','Y','@','Z',' ','@','@'

	;> 19 0x13 derecha
	;< 17 0x11 izquierda
	
;//////////////////////////////////////////////////////////
;                    Inicia el LCD
;//////////////////////////////////////////////////////////
LCD_INIT:
	ldi		r22, 0xFF
	out		LCD_DDR, R22	;LCD dato de salida
	ldi		r16, 0x33		;
	call	CMNDWRT
	call	DELAY_2ms
	ldi		r16, 0x32		
	call	CMNDWRT
	call	DELAY_2ms
	ldi		r16, 0x28		;inicializar las matrices 5x7
	call	CMNDWRT
	call	DELAY_2ms
	ldi		r16, 0x0E		;Encender el cursor del display
	call	CMNDWRT
	call	DELAY_2ms
	ldi		r16, 0x01		;limpiar el LCD
	call	CMNDWRT
	call	DELAY_2ms
	ldi		r16, 0x06		;cambiar cursor a la derecha
	call	CMNDWRT
	call	DELAY_2ms
	ret

;//////////////////////////////////////////////////////////
;                    Escribe Segudna Linea
;//////////////////////////////////////////////////////////
_linea2:
	ldi		r16, 0xC0		;segunda linea
	call	CMNDWRT
	call	DELAY_2ms
	ret

;//////////////////////////////////////////////////////////
;                    Cambiar Cursor
;//////////////////////////////////////////////////////////
	
CambiaCusorD:

	ldi		r16, 0x16		;cambiar cursor a la derecha
	call	CMNDWRT
	call	tiempo1
		
	call	tiempo1
	call	tiempo1
	call	tiempo1
	call	tiempo1
	
	call puntero
	
	ret
	
CambiaCusorI:

	ldi		r16, 0x10		;cambiar cursor a la derecha
	call	CMNDWRT

	call	tiempo1
	call	tiempo1
	call	tiempo1
	call	tiempo1
	
	call puntero
	
	ret
;//////////////////////////////////////////////////////////
;                    Limpia Pantalla
;//////////////////////////////////////////////////////////
limpiar:
	ldi		r16, 0x01		;limpiar el LCD
	call	CMNDWRT
	call	DELAY_2ms
	ret

;//////////////////////////////////////////////////////////
;            Escribe Instrucciones en el LCD
;//////////////////////////////////////////////////////////
CMNDWRT:
	mov		r27, r16
	andi	r27, 0xF0
	in		r26, LCD_PRT
	andi	r26, 0x0F
	or		r26, r27
	
	out		LCD_PRT, r26
	cbi		LCD_PRT, LCD_RS		;0 en el registro
	cbi		LCD_PRT, LCD_RW		;0 en el registro
	sbi		LCD_PRT, LCD_EN		;1 en el registro
	call	SDELAY
	cbi		LCD_PRT, LCD_EN
	call	DELAY_2ms

	mov		r27, r16
	swap	r27
	andi	r27, 0xF0
	in		r26, LCD_PRT
	andi	r26, 0x0F
	or		r26, r27
	out		LCD_PRT, r26
	sbi		LCD_PRT, LCD_EN
	call	SDELAY
	cbi		LCD_PRT, LCD_EN
	call	DELAY_2ms
	ret

;//////////////////////////////////////////////////////////
;              Escribe un Dato en el LCD
;//////////////////////////////////////////////////////////

DATAWRT:

	call puntero
	
	mov		r27, r16
	andi	r27, 0xF0
	in		r26, LCD_PRT
	andi	r26, 0x0F
	or		r26, r27

	out		LCD_PRT, r26
	sbi		LCD_PRT, LCD_RS
	cbi		LCD_PRT, LCD_RW
	sbi		LCD_PRT, LCD_EN
	call	SDELAY
	cbi		LCD_PRT, LCD_EN
	call	DELAY_2ms

	mov		r27, r16
	swap	r27
	andi	r27, 0xF0
	in		r26, LCD_PRT
	andi	r26, 0x0F
	or		r26, r27
	out		LCD_PRT, r26
	sbi		LCD_PRT, LCD_EN
	call	SDELAY
	cbi		LCD_PRT, LCD_EN
	call	DELAY_2ms
	ret

;///////////////////////////////////////////////////////
;   Puntero
;///////////////////////////////////////////////////////

Puntero:
	
	mov r8,r4
    clr r4
    
    push r21
    
        ldi r21,0x11
        mov r9,r21
    
    pop r21

    push r16
    
	   call _regreso
	   call _ubicarPuntero
	
    pop r16

	ret
	
;///////////////////////////////////////////////////////
;   Retardo
;///////////////////////////////////////////////////////
    
tiempo1:
	call DELAY_2ms
	call DELAY_2ms
	call DELAY_2ms
	call DELAY_2ms
	call DELAY_2ms	
	call DELAY_2ms
	call DELAY_2ms
	call DELAY_2ms
	call DELAY_2ms
	call DELAY_2ms
	call DELAY_2ms
	call DELAY_2ms
	call DELAY_2ms
	call DELAY_2ms
	call DELAY_2ms
	ret

;///////////////////////////////////////////////////////
;   Posicionar a puntero
;///////////////////////////////////////////////////////

_regreso:
	cp r8,r9
	brlo _terminoR
	
	dec r8
	ldi r16,0x1C
	call CMNDWRT	
	rjmp _regreso
	
_terminoR:
	ret
	
	
_ubicarPuntero:

	push r21

		ldi r21,0x20
		mov r9,r21

	pop r21

	cp contador,r9
	brlo _punteroL1
	
_punteroL2:

	mov r7,contador
	sub r7,r9
    
	push r21

		ldi r21,0x11
		mov r9,r21

	pop r21

_puntL2:
	cp r4,r7
	brge _salirP2
	
	inc r4
	
	cp r4,r9
	brlo _puntL2
	
	ldi r16,0x18
	call CMNDWRT
	
	jmp _puntL2
	
_salirP2:	
	ret

	
_punteroL1:

	push r21

		ldi r21,0x11
		mov r9,r21

	pop r21

_puntL1:
	cp r4,contador
	brge _salirP1
	
	inc r4
	
	cp r4,r9
	brlo _puntL1
	
	ldi r16,0x18
	call CMNDWRT
	
	jmp _puntL1
	
_salirP1:
	ret	
	
;//////////////////////////////////////////////////////////
;                    MoverTexto
;//////////////////////////////////////////////////////////

_Desplaza:

    push r21
    
	   ldi r21,0x01
        mov r6,r21
    
    pop r21
    
	cp r5,r6
	brlo _DesplazateDer
	brge _DesplazateIz
	ret
	
_DesplazateDer:
	
	cp r4,contador
	brge _Izq
	
    push r21
    
	   ldi r21,0x1F
	   mov r6,r21
    
    pop r21
    
    cp r4,r6
	brge _Izq

    push r21
    
        ldi r21,0x10
        mov r6,r21
    
    pop r21
    
	cp r4,r6
	brge _MovD
	inc r4
	ret
	
_Izq:
	inc r5
	ret

_MovD:
	inc	r4
	ldi	r16, 0x18
	call CMNDWRT
	ret
	
_DesplazateIz:

    push r21
    
	   ldi r21,0x11
       mov r6,r21
       
    pop r21
	cp r4,r6
	brge _MovI
	clr r5
	ret
	
_MovI:
	dec r4
	ldi r16,0x1C
	call CMNDWRT
	ret

;//////////////////////////////////////////////////////////
;                    Retardo de 2ms
;////////////////////////////////////////////////////////// 
DELAY_2ms:
	push	r16
	push	r17
	ldi		r16, 100
loop1:
	ldi		r17, 160
loop0:
	dec		r17
	brne	loop0
	dec		r16
	brne	loop1
	pop		r17
	pop		r16
	ret

;//////////////////////////////////////////////////////////
;                   Retardo de 6 ciclos maquina
;//////////////////////////////////////////////////////////
SDELAY:
	nop
	nop
	nop
	nop
	nop
	nop
	ret