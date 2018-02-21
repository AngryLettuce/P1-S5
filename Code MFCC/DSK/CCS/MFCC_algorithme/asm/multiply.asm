;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;;;;;;; Les directives ;;;;;;;;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
	.def _MpyEntierNonSigneOp32bitsRes64bits
	.def _MpyEntierSigneOp32bitsRes64bits
	.def _MpyfractionnaireOp32bitsRes64bits_Q7p24_Q15p16
	.def _MpyFlottant64bits

	.data


	.text


_MpyEntierNonSigneOp32bitsRes64bits
    .asmfunc
; Protection du CSR et de l'AMR sur la pile
	MVC	CSR, B5
	STW B5, *B15--[2]		;Push CSR sur la pile (sur ce DSP, le pointeur de pile doit toujour changer en multiples de 8 bytes)
	MVC	AMR, B5
	STW B5, *B15--[2]		;Push AMR sur la pile (sur ce DSP, le pointeur de pile doit toujour changer en multiples de 8 bytes)


	LDW *A4++, A1
	LDW *A4++, A2
	NOP 5

	MVK 0x0000000, A4
	MVK 0x0000000, A5

	MPYU A1, A2, A6
	MPYHLU A1, A2, A7
	MPYLHU A1, A2, A8
	MPYHU A1, A2, A9

	SHL A7, 16, A0
	ADDU A6, A0, A5:A4
	SHL A8, 16, A0
	ADDU A5:A4, A0, A5:A4

	SHRU A7, 16, A0
	ADDU A9, A0, A3:A2
	SHRU A8, 16, A0
	ADDU A3:A2, A0, A3:A2
	ADDU A3:A2, A5, A3:A2

	MV A2, A5

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


_MpyEntierSigneOp32bitsRes64bits
    .asmfunc
; Protection du CSR et de l'AMR sur la pile
	MVC	CSR, B5
	STW B5, *B15--[2]		;Push CSR sur la pile (sur ce DSP, le pointeur de pile doit toujour changer en multiples de 8 bytes)
	MVC	AMR, B5
	STW B5, *B15--[2]		;Push AMR sur la pile (sur ce DSP, le pointeur de pile doit toujour changer en multiples de 8 bytes)


	LDW *A4++, A1
	LDW *A4++, A2
	NOP 5

	MPYID A1, A2, A5:A4
	NOP 9

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

_MpyfractionnaireOp32bitsRes64bits_Q7p24_Q15p16
	.asmfunc
; Protection du CSR et de l'AMR sur la pile
	MVC	CSR, B5
	STW B5, *B15--[2]		;Push CSR sur la pile (sur ce DSP, le pointeur de pile doit toujour changer en multiples de 8 bytes)
	MVC	AMR, B5
	STW B5, *B15--[2]		;Push AMR sur la pile (sur ce DSP, le pointeur de pile doit toujour changer en multiples de 8 bytes)



	LDW *A4++, A1
	LDW *A4++, A2
	NOP 5

	MPYID A1, A2, A5:A4
	NOP 9

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

_MpyFlottant64bits
	.asmfunc
; Protection du CSR et de l'AMR sur la pile
	MVC	CSR, B5
	STW B5, *B15--[2]		;Push CSR sur la pile (sur ce DSP, le pointeur de pile doit toujour changer en multiples de 8 bytes)
	MVC	AMR, B5
	STW B5, *B15--[2]		;Push AMR sur la pile (sur ce DSP, le pointeur de pile doit toujour changer en multiples de 8 bytes)



	LDDW *A4++, A1:A0
	LDDW *A4++, A3:A2
	NOP 5

	MPYDP A1:A0, A3:A2, A5:A4
	NOP 9

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
