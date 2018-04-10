/*
 * filtreFIR300Hz.c
 *
 *  Created on: 2 avr. 2018
 *      Author: rolj1802
 */
#include "filtreFIR300Hz.h"

#include "bancDeTestFiltreResultat.dat"
#include "fonctions.h"
#include "pitchCalculation.h"

short resultFiltre[TAILLE_TRAME][7];

void filterFIR_sym_OrdN(short N, const short coeff[], short *ptrTabEntre, short *ptrTabSortie){


    short *ptrEchantOldest;
    short *ptrEchantNewest;

    ptrEchantNewest = ptrTabEntre;
    ptrEchantOldest = ptrTabEntre - N;


    short i;
    int out = 0;
    for (i = 0; i < N/2; i++)
    {
        out = out + coeff[i] * (*ptrEchantOldest + *ptrEchantNewest);
        ptrEchantOldest++;
        ptrEchantNewest--;
    }
    out = out + coeff[10] * *ptrEchantOldest;


    *ptrTabSortie = out >> 15;
}


