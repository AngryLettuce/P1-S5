;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;;;;;;; Les directives ;;;;;;;;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
	.def _SubEntierNonSigne32bits
	.def _SubEntierSigne32bits
	.def _SubFlottant64bits
	.data


	.text




_SubEntierNonSigne32bits
    .asmfunc
; Protection du CSR et de l'AMR sur la pile
	MVC	CSR, B5
	STW B5, *B15--[2]		;Push CSR sur la pile (sur ce DSP, le pointeur de pile doit toujour changer en multiples de 8 bytes)
	MVC	AMR, B5
	STW B5, *B15--[2]		;Push AMR sur la pile (sur ce DSP, le pointeur de pile doit toujour changer en multiples de 8 bytes)


	LDW *A4++, A1
	LDW *A4++, A2
	NOP 5

	SUBU A1, A2, A5:A4

	CMPGT A5, 0, A1
	[A1] MVK 0x00000000, A4

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


_SubEntierSigne32bits
    .asmfunc
; Protection du CSR et de l'AMR sur la pile
	MVC	CSR, B5
	STW B5, *B15--[2]		;Push CSR sur la pile (sur ce DSP, le pointeur de pile doit toujour changer en multiples de 8 bytes)
	MVC	AMR, B5
	STW B5, *B15--[2]		;Push AMR sur la pile (sur ce DSP, le pointeur de pile doit toujour changer en multiples de 8 bytes)


	LDW *A4++, A1
	LDW *A4++, A2
	NOP 5

	SSUB A1, A2, A4

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

_SubFlottant64bits
	.asmfunc
; Protection du CSR et de l'AMR sur la pile
	MVC	CSR, B5
	STW B5, *B15--[2]		;Push CSR sur la pile (sur ce DSP, le pointeur de pile doit toujour changer en multiples de 8 bytes)
	MVC	AMR, B5
	STW B5, *B15--[2]		;Push AMR sur la pile (sur ce DSP, le pointeur de pile doit toujour changer en multiples de 8 bytes)


	LDDW *A4++, A1:A0
	LDDW *A4++, A3:A2
	NOP 5

	SUBDP A1:A0, A3:A2, A5:A4
	NOP 6

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
