	.def _autoCorrASM

	.data
borneMaxA1 		.set 		0x0000
initA1			.set		0x00FF
nbrEchant		.set		0x0101
	.text

_autoCorrASM
	.asmfunc

				ZERO		.L1			A3									; Initialiser la borne maximale A3 du compteur A1 à 0
	||			MVKL		.S1			initA1, 	A1						; Initialiser le compteur A1 à 255
				MVKH		.S1			initA1,		A1						; Initialiser le compteur A1 à 255
	||			ADD			.L1			B4,			0x04,		A8

DoubleCycle:

;Initialisation de compteur et d'addresse

				ZERO		.D1			A0									; Initialiser le compteur A0 à 0
	||			SHL			.S1			A1,			0x01,		A9 			; Multiplier par deux la valeur de du compteur A1 et le mettre dans A9  (Nombre d'addresse à décaler)

				MV			.S2			A4,			B5						; Initialiser le pointeur A6 à l'addresse contenue dans A4
	||			ZERO		.D2			B9 									; Initialiser le MSW du resultat de la sommation du nème élément à 0
	||			SUB			.L1			A9 ,		1,			A9

				ADD			.S1			A4,			A9 ,		A5			; Initialiser le pointeur A5 à l'addresse contenue dans A4 décalée de A9  addresse
	||			ZERO		.D2			B8 									; Initialiser le LSW du resultat de la sommation du nème élément à 0
	||			ZERO		.L1			A7
	||			ZERO		.L2			B7
	||			ZERO		.S2			B1


BoucleImpaire:
				LDW			.D1			*A5++,		A6						; Loader la valeur pointée par le pointeur A5 dans A7, incrémenter le pointeur A5
	||			LDW			.D2			*B5++,		B6						; Loader la valeur pointée par le pointeur B6 dans A8, incrémenter le pointeur A6
	||			ADD			.L1			A0,			0x01,		A0			; Incrémenter la valeur du compteur A0
	||			MVKL		.S1			nbrEchant,	A9 						; A9  = 256
	||			MV			.L2			B1,			B2

				ADD			.L2			A7,			B9:B8,		B9:B8  		; Sommation des produits A14 (word) -> B9:B8   (Long)
				ADD			.L2			B7,			B9:B8,		B9:B8  		; Sommation des produits A14 (word) -> B9:B8   (Long)

				SUB			.L1			A9 ,		A1,			A9 			; A9  = 256 - A1
				SHR			.S1			A9 ,		0x01,		A9

				MPYHL		.M1			A6,			B6,			A7
	||			MPYHL		.M2			B2,			A6,			B7
	||			CMPLT		.L1			A9,			A0,			A2			; Compteur A0 < Borne maximale A9 , resultat -> A2
	||			MV			.S2			B6,			B1

	  [!A2]								B			BoucleImpaire			; While Sommation non terminée
				NOP						5


				STW			.D2			B8 ,		*B4++[2]				; Storer B8  dans l'addresse pointée par le pointeur B4, incrémenter le pointeur A6
				STW			.D1			B9 ,		*A8++[2]				; Storer B9  dans l'addresse pointée par le pointeur B4, incrémenter le pointeur A6


;Initialisation de compteur et d'addresse

				ZERO		.D1			A0									; Initialiser le compteur A0 à 0
	||			SHL			.S1			A1,			0x01,		A9 			; Multiplier par deux la valeur de du compteur A1 et le mettre dans A9  (Nombre d'addresse à décaler)

				MV			.S2			A4,			B5						; Initialiser le pointeur A6 à l'addresse contenue dans A4
	||			ZERO		.D2			B9 									; Initialiser le MSW du resultat de la sommation du nème élément à 0
	||			SUB			.L1			A9 ,		1,			A9


				ADD			.S1			A4,			A9 ,		A5			; Initialiser le pointeur A5 à l'addresse contenue dans A4 décalée de A9  addresse
	||			ZERO		.D2			B8 									; Initialiser le LSW du resultat de la sommation du nème élément à 0
	||			ZERO		.L1			A7
	||			ZERO		.L2			B7


BouclePaire:
				LDW			.D1			*A5++,		A6						; Loader la valeur pointée par le pointeur A5 dans A7, incrémenter le pointeur A5
	||			LDW			.D2			*B5++,		B6						; Loader la valeur pointée par le pointeur B6 dans A8, incrémenter le pointeur A6
	||			ADD			.L1			A0,			0x01,		A0			; Incrémenter la valeur du compteur A0
	||			MVKL		.S1			nbrEchant,	A9 						; A9  = 256

				ADD			.L2			A7,			B9:B8  ,	B9:B8  		; Sommation des produits A14 (word) -> B9:B8   (Long)
				ADD			.L2			B7,			B9:B8  ,	B9:B8  		; Sommation des produits A14 (word) -> B9:B8   (Long)
	||			MVKH		.S1			nbrEchant,	A9 						; A9  = 256

				SUB			.L1			A9 ,		A1,			A9 			; A9  = 256 - A1
				SHR			.S1			A9 ,		0x01,		A9

				MPYH		.M1			A6,			B6,			A7
	||			MPY			.M2			A6,			B6,			B7
	||			CMPLT		.L1			A9,			A0,			A2				; Compteur A0 < Borne maximale A9 , resultat -> A2


	  [!A2]								B			BouclePaire				; While Sommation non terminée
				NOP						5

;Storer value
				STW			.D2			B8 ,		*B4++[2]				; Storer B8  dans l'addresse pointée par le pointeur B4, incrémenter le pointeur A6
	||			SUB			.S1			A1,			0x02,		A1			; Incrémenter le compteur A1
				STW			.D1			B9 ,		*A8++[2]				; Storer B9  dans l'addresse pointée par le pointeur B4, incrémenter le pointeur A6
	||			CMPEQ		.L1			A1,			-1,		A2				; Compteur A0 > A3, resultat -> A2

	  [!A2]					B			DoubleCycle							; While autocorrelation non terminée
				NOP			5


    B 			B3
    NOP 		5

    .endasmfunc
