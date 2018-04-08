
/*
 * fft_utility.h
 *
 *  Created on: 16 f�vr. 2018
 *      Author: pascal
 */

#ifndef INCLUDE_FFT_UTILITY_H_
#define INCLUDE_FFT_UTILITY_H_


void tw_genSPxSPfft(float * w, int n);
void bit_rev(float* x, int n); 
void bitrev_index(short *index, int n);
void tw_genr2fft(float* w, int n);
void tw_genr4fft(float *, int);
void R4DigitRevIndexTableGen(int n, int * count, unsigned short *IIndex, unsigned short *JIndex);
void digit_reverse(double *yx, unsigned short *JIndex, unsigned short *IIndex, int count);

/*======================================================================== */
/*            Copyright (c) 2003 Texas Instruments, Incorporated.          */
/*                           All Rights Reserved.                          */
/* ======================================================================= */


#endif /* INCLUDE_FFT_UTILITY_H_ */