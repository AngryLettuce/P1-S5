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

float float_abs(float x);
//float element wise vector multiplication
void fvec_elementWise_mult(float *a, float *b, float *dest, int size);

//float tab to complex tab numbers from DSPC6713 format
void float2complex(float *a, float *b, int size);

//calculate euclidean distance without taking the square root of the result
float euclideanDistPow2(float *a, float *b, int size);

//Moving average for silent recognition

float moving_average(float *beta_acc, int size, int acc_size);
void acc_interval(float *curr_data, float *beta_acc);

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
void mfcc_melFilterBank_create(MelFilterBank* melFilterBank, float freqL,
                               float freqH, int filter_number, int size_data, int sample_rate);

void mfcc_getMelCoeff(float *x, float *coeff, MelFilterBank *melFilterBank);

//fft of windowed signal
void mfcc_fft_init(float *w, short *index, int N);
void mfcc_fft(float *complexTab, float *w, short *index, int N);

//power spectrum extraction from fft extracted complex table
void mfcc_powerSpectrum(float *x, float *x_complex,  int size);

//DCT, frequency to temporal transformation
void mfcc_dct_init(float *cosTab, int in_coeff_nb, int out_coeff_nb);
void mfcc_dct(float *in_coeff, float *out_coeff, float *cosTab, int in_coeff_nb, int out_coeff_nb);


#endif /* INCLUDE_FUNCTIONS_H_ */
