#ifndef FONCTIONS_H_
#define FONCTIONS_H_


//Includes
#include <stdio.h>


//Defines
#define TAILLE_TRAME 256

//Variables globales
extern short echant[TAILLE_TRAME];
extern long resultAutoCorrC[TAILLE_TRAME];
extern long resultAutoCorrASM[TAILLE_TRAME];



//Prototypes des fonctions de l'autocorrélation
void initAutoCorr(short entree[TAILLE_TRAME]);
void autoCorrC(void);
void autoCorrASM(short *scrTab, long *destTab);
void planDeTest(short entree[TAILLE_TRAME]);

#endif
