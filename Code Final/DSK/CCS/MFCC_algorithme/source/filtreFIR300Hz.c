/*
 * filtreFIR300Hz.c
 *
 *  Created on: 2 avr. 2018
 *      Author: rolj1802
 */
#include "filtreFIR300Hz.h"
#include "data_structures.h"

#include "pitchCalculation.h"

short resultFiltre[SIGNAL_BLOCK_SIZE][7];

short filterFIR_sym_OrdN(short N, const short coeff[], short *ptrTabEntre){


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


    return out >> 15;
}


