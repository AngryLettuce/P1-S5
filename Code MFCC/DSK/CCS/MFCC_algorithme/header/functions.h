/*
 * functions.h
 *
 *  Created on: 16 févr. 2018
 *      Author: pascal
 */

#ifndef INCLUDE_FUNCTIONS_H_
#define INCLUDE_FUNCTIONS_H_

#include "data_structures.h"



//function prefix convention
//
//
//  mfcc = mfcc pipeline function
//


//------------------------------------
//  UTILS FUNCTION
//------------------------------------


//float element wise vector multiplication
void fvec_elementWise_mult(float *a, float *b, float *dest, int size);

//float tab to complex tab numbers from DSPC6713 format
void float2complex(float *a, float *b, int size);

//calculate euclidean distance without taking the square root of the result
float euclideanDistPow2(float *a, float *b, int size);




//------------------------------------
//  MFCC FUNCTIONS
//------------------------------------

//pre-amplification 1st order FIR filter
float mfcc_preAmpFIR(float x, float x_last);


//apply hamming window to a signal
void mfcc_hamming_window_256(float *x);
void mfcc_hamming_window_256_opt(float *x);//optimized version

//mel frequency conversion
float mfcc_freq2mel(float f);//normal frequency to mel frequency
float mfcc_mel2freq(float mel);//mel frequency to normal frequency

//mel filter bank
void mfcc_melFilterBank_create(MelFilterBank* melFilter,
                                         int freqL,
                                         int freqH,
                                         int filter_nb,
                                         int size_data,
                                         int sample_rate);

//fft of windowed signal
void mfcc_fft256_init(float *fft256CoeffTab);
void mfcc_fft256(float *complexTab, float *fft256CoeffTab);



//power spectrum extraction from fft extracted complex table
void mfcc_powerSpectrum(float *complexTab, int size);


#endif /* INCLUDE_FUNCTIONS_H_ */
