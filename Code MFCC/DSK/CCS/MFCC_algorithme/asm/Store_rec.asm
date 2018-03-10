;
;  Fichier:    Store_rec.asm
;
;  Date:       25 fevrier. 2018
;
; Auteur:     Pascal Beaulieu
;
	.def _asm_storeCircularTab256

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;;;;;;; Store Données ;;;;;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

_asm_storeCircularTab256
	.asmfunc

AMRclear 	.set 0xFFE03FFF
AMRset 		.set 0x00084000

;Protection de contexte
	MVC AMR, B1
	STW B1, *B15[2]

;Configuration de AMR
	MVKL AMRclear, B2
	MVKH AMRclear, B2
	AND B2,B1, B1

;Modification des bits
	MVKL AMRset, B2
	MVKH AMRset, B2
	OR B2,B1, B1

;Modification du registre
	MVC B1, AMR

;Pointer vers le bon tableau
	MV A4, B7

;Storage de la donnée
	STH B4, *B7++
	NOP 5

;Retour de la valeur du pointeur
	MV B7, A4

;Restauration du contexte
	LDW *B15[2], B1
	NOP 4
	MVC B1, AMR

;Retour au C
	B B3
	NOP 5
	.endasmfunc

	.end


