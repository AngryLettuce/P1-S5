/*
 * pitchCalculation.c
 *
 *  Created on: 3 avr. 2018
 *      Author: rolj1802
 */

short getpitch_findMinimum(long *corrTab, int size)
{
    short i;
    short minIndex = -1 ;
    long min = 0;

    for (i = 0; i < size; i++)
    {
        if(*corrTab < min){
            minIndex = i;
            min = *corrTab;
        }

        corrTab++;
    }
    return minIndex;
}


short getpitch_find2ndMaximum(long *corrTab, short minIndex)
{
    short i;
    short maxIndex = -1 ;
    long max = 0;

    corrTab = corrTab + minIndex;
    for (i = minIndex; i >= 0; i--)
    {
        if(*corrTab > max){
            maxIndex = i;
            max = *corrTab;
        }
        corrTab--;
    }
    return maxIndex;
}


float autoCorr_getpitch(long *corrTab, int Fe, int size)
{
    short min,max;
    min = getpitch_findMinimum(corrTab, size);
    max = getpitch_find2ndMaximum(corrTab, min);

    float pitch = ((size - 1) - (float) max) / Fe;
    pitch = 1/pitch;
    return pitch;
}
