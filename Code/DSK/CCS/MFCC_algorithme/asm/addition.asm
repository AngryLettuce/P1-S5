;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;;;;;;; Les directives ;;;;;;;;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
	.def _AddEntierNonSigne32bits
	.def _AddEntierSigne32bits
	.def _AddFractionnaire32bits_Q7p24_Q15p16
	.data



CSRclearSAT .set 0xFFFFFDFF
CSRclearNotSAT  .set 0x00000200

	.text


_AddEntierNonSigne32bits
    .asmfunc
; Protection du CSR et de l'AMR sur la pile
	MVC	CSR, B5
	STW B5, *B15--[2]		;Push CSR sur la pile (sur ce DSP, le pointeur de pile doit toujour changer en multiples de 8 bytes)
	MVC	AMR, B5
	STW B5, *B15--[2]		;Push AMR sur la pile (sur ce DSP, le pointeur de pile doit toujour changer en multiples de 8 bytes)

	MVK 0x00000000, A6

	LDW *A4++, A1
	LDW *A4++, A2
	NOP 5

	ADDU A1, A2, A5:A4

	CMPEQ A5,A6,A1
	[!A1] MVK 0xFFFFFFFF, A4

; Restauration du CSR et de l'AMR
	LDW *++B15[2], B5		;Pop AMR de la pile (sur ce DSP, le pointeur de pile doit toujour changer en multiples de 8 bytes)
	NOP 4
	MVC B5, AMR
	LDW *++B15[2], B5		;Pop CSR de la pile (sur ce DSP, le pointeur de pile doit toujour changer en multiples de 8 bytes)
	NOP 4
	MVC B5, CSR

    B B3 ; INDISPENSABLE ; B3 contient l'adresse de retour
    NOP 5

    .endasmfunc


_AddEntierSigne32bits
    .asmfunc
; Protection du CSR et de l'AMR sur la pile
	MVC	CSR, B5
	STW B5, *B15--[2]		;Push CSR sur la pile (sur ce DSP, le pointeur de pile doit toujour changer en multiples de 8 bytes)
	MVC	AMR, B5
	STW B5, *B15--[2]		;Push AMR sur la pile (sur ce DSP, le pointeur de pile doit toujour changer en multiples de 8 bytes)

	LDW *A4++, A1
	LDW *A4++, A2
	NOP 5

	SADD A1, A2, A4

; Restauration du CSR et de l'AMR
	LDW *++B15[2], B5		;Pop AMR de la pile (sur ce DSP, le pointeur de pile doit toujour changer en multiples de 8 bytes)
	NOP 4
	MVC B5, AMR
	LDW *++B15[2], B5		;Pop CSR de la pile (sur ce DSP, le pointeur de pile doit toujour changer en multiples de 8 bytes)
	NOP 4
	MVC B5, CSR

    B B3 ; INDISPENSABLE ; B3 contient l'adresse de retour
    NOP 5

    .endasmfunc

_AddFractionnaire32bits_Q7p24_Q15p16
    .asmfunc
; Protection du CSR et de l'AMR sur la pile
	MVC	CSR, B5
	STW B5, *B15--[2]		;Push CSR sur la pile (sur ce DSP, le pointeur de pile doit toujour changer en multiples de 8 bytes)
	MVC	AMR, B5
	STW B5, *B15--[2]		;Push AMR sur la pile (sur ce DSP, le pointeur de pile doit toujour changer en multiples de 8 bytes)

	LDW *A4++, A1;Q7.24
	LDW *A4++, A6;Q15.16
	NOP 5

	MVKL 0x00000000, A7
	MVKH 0x00000000, A7

	;shift left 8 bits
	SHL A7:A6, 8, A7:A6

	;add align fractional number
	ADD A1, A7:A6, A7:A6


	CMPLT A7, 0, A1
	CMPLT A6, 0, A2

	CMPEQ A1, A2, A2

	[!A2] B formatQ15p16
	NOP 5

	[!A1]  MVK 0x00000000, A0
	[A1] MVK 0xFFFFFFFF, A0

	CMPEQ A7, A0, A1;

	[!A1] B formatQ15p16
	NOP 5

	MV A6, A4

; Restauration du CSR et de l'AMR
	LDW *++B15[2], B5		;Pop AMR de la pile (sur ce DSP, le pointeur de pile doit toujour changer en multiples de 8 bytes)
	NOP 4
	MVC B5, AMR
	LDW *++B15[2], B5		;Pop CSR de la pile (sur ce DSP, le pointeur de pile doit toujour changer en multiples de 8 bytes)
	NOP 4
	MVC B5, CSR

	;return
	B B3 ; INDISPENSABLE ; B3 contient l'adresse de retour
    NOP 5

formatQ15p16:

	SHR A7:A6, 8, A7:A6
	MV A6, A4

; Restauration du CSR et de l'AMR
	LDW *++B15[2], B5		;Pop AMR de la pile (sur ce DSP, le pointeur de pile doit toujour changer en multiples de 8 bytes)
	NOP 4
	MVC B5, AMR
	LDW *++B15[2], B5		;Pop CSR de la pile (sur ce DSP, le pointeur de pile doit toujour changer en multiples de 8 bytes)
	NOP 4
	MVC B5, CSR

	;return
    B B3 ; INDISPENSABLE ; B3 contient l'adresse de retour
    NOP 5

    .endasmfunc

