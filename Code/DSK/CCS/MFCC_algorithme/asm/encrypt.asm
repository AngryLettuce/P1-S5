;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;;;;;;; Les directives ;;;;;;;;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
	.def _EncrypterDonnees
	.data

AMRClear .set 0xFFE0FF3F
AMRSet32 .set 0x00040040
;         0000 0111 1111 1|000
nbTours .set 0x000007F8;255 tour
encryptClef .set 0xFFFFFFFF

	.text

_EncrypterDonnees
    .asmfunc
; Protection du CSR et de l'AMR sur la pile
	MVC	CSR, B5
	STW B5, *B15--[2]		;Push CSR sur la pile (sur ce DSP, le pointeur de pile doit toujour changer en multiples de 8 bytes)
	MVC	AMR, B5
	STW B5, *B15--[2]		;Push AMR sur la pile (sur ce DSP, le pointeur de pile doit toujour changer en multiples de 8 bytes)

;retrive inputed table adress
	MV A4, A7
	NOP 5

;Reset champs à modifier AMR
	MVKL AMRClear,B4
	MVKH AMRClear,B4
	AND B5, B4, B5

	;Set des champs à modifier AMR
	MVKL AMRSet32, B4
	MVKH AMRSet32, B4
	OR B5, B4, B5

	MVC B5, AMR

	MVK 0, A1
	MVKL nbTours, A3
	MVKH nbTours, A3
encryptLoop:
	LDW *A7, A0
	NOP 5

	XOR A0, encryptClef, A0
	STW A0, *A7++
	NOP 5

	ADD A1,1,A1
	CMPEQ A1, A3, A2
	[!A2] B encryptLoop
	NOP 5


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


