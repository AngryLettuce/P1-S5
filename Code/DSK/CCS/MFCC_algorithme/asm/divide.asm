;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;;;;;;; Les directives ;;;;;;;;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
	.def _DivIncrementationNonSigne32bit
	.def _DivSubcNonSigne32bit
	.def _DivFlottant32bits
	.data


CSRclearSAT .set 0xFFFFFDFF
CSRclearNotSAT .set 0x00000200

	.text


_DivIncrementationNonSigne32bit
    .asmfunc
; Protection du CSR et de l'AMR sur la pile
	MVC	CSR, B5
	STW B5, *B15--[2]		;Push CSR sur la pile (sur ce DSP, le pointeur de pile doit toujour changer en multiples de 8 bytes)
	MVC	AMR, B5
	STW B5, *B15--[2]		;Push AMR sur la pile (sur ce DSP, le pointeur de pile doit toujour changer en multiples de 8 bytes)

    MVK 0, A8

	LDW *A4++, A0
	LDW *A4, A1
	NOP 5

forLoop:
	SUBU A0, A1, A5:A4
	MV A4, A0
	CMPLT A5:A4, 0, A2
	[!A2] ADD 1, A8, A8
	[!A2] B forLoop
	NOP 5

	MVK 1, A7
	ABS A0, A0
	SHRU A1, 1, A1
	CMPGT A0, A1, A2
	[!A2] ADDU A7, A8, A9:A8
	MV A8, A4

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


_DivSubcNonSigne32bit
    .asmfunc
; Protection du CSR et de l'AMR sur la pile
	MVC	CSR, B5
	STW B5, *B15--[2]		;Push CSR sur la pile (sur ce DSP, le pointeur de pile doit toujour changer en multiples de 8 bytes)
	MVC	AMR, B5
	STW B5, *B15--[2]		;Push AMR sur la pile (sur ce DSP, le pointeur de pile doit toujour changer en multiples de 8 bytes)



	LDW *A4++, A2;num
	LDW *A4++, A3;den
	NOP 5

	;verify special return case
	CMPGT A3, A2, A5
	CMPEQ A2, 0, A6
	CMPEQ A3, 0, A7

	OR A5, A6, A1
	OR A1, A7, A1

	[A1] B return0
	NOP 5

	LMBD 1, A2, A5;LMBD num
	LMBD 1, A3, A6;LMBD den

	SUB A6, A5, A7;nb of shift

	SHL A3, A7, A3;den << shift

	MVK 0, A0; init counter to 0
forLoop2:

	SUBC A2, A3, A2

	ADD A0, 1, A0; counter++
	CMPGT A0, A7, A1
	[!A1] B forLoop2
	NOP 5

	MVK 31, A0

	SUB A0, A7, A7

	SHL  A2, A7, A2
	SHRU A2, A7, A4

; Restauration du CSR et de l'AMR
	LDW *++B15[2], B5		;Pop AMR de la pile (sur ce DSP, le pointeur de pile doit toujour changer en multiples de 8 bytes)
	NOP 4
	MVC B5, AMR
	LDW *++B15[2], B5		;Pop CSR de la pile (sur ce DSP, le pointeur de pile doit toujour changer en multiples de 8 bytes)
	NOP 4
	MVC B5, CSR

    B B3 ; INDISPENSABLE ; B3 contient l'adresse de retour
    NOP 5

return0:
	MVK 0, A4

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

_DivFlottant32bits
    .asmfunc
; Protection du CSR et de l'AMR sur la pile
	MVC	CSR, B5
	STW B5, *B15--[2]		;Push CSR sur la pile (sur ce DSP, le pointeur de pile doit toujour changer en multiples de 8 bytes)
	MVC	AMR, B5
	STW B5, *B15--[2]		;Push AMR sur la pile (sur ce DSP, le pointeur de pile doit toujour changer en multiples de 8 bytes)

	LDW *A4++, A2;num
	LDW *A4++, A3;den
	NOP 5

	;MV A3, A4
	RCPSP A3, A3
	MPYSP A2, A3, A4
	NOP 3

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

