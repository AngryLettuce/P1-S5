/*
 * pitchCalculation.h
 *
 *  Created on: 3 avr. 2018
 *      Author: rolj1802
 */

#ifndef PITCHCALCULATION_H_
#define PITCHCALCULATION_H_

short getpitch_findMinimum(long *corrTab, int size);
short getpitch_find2ndMaximum(long *corrTab, short minIndex);
float autoCorr_getpitch(long *corrTab, int Fe, int size);
void autoCorrASM(short *scrTab, long *destTab);

#endif /* SOURCE_C_PITCHCALCULATION_H_ */
