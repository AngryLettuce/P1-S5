//Includes
#include <stdio.h>

#include "testBench.dat"
#include "autoCorrTest.dat"
#include "bancDeTestTotal.dat"
#include "CoeffFIR300HzN20.dat"
#include "fonctions.h"
#include "filtreFIR300Hz.h"
#include "pitchCalculation.h"


void main(void) {



    static short trameFiltre[TAILLE_TRAME] = {0};
    short *ptrTrameFiltre = &trameFiltre[0];
    short *ptrTableauEntree = &signalBancDeTest[20];

    short i;
    for (i = 0; i < TAILLE_TRAME; i++){
        filterFIR_sym_OrdN(20, coeffFIR300HzN20, ptrTableauEntree, ptrTrameFiltre);
        ptrTrameFiltre++;
        ptrTableauEntree++;}

    long autoCorrRestult[256] = {0};
    autoCorrASM(trameFiltre, autoCorrRestult);
    float pitch = autoCorr_getpitch(autoCorrRestult, 8000, 256);


}

