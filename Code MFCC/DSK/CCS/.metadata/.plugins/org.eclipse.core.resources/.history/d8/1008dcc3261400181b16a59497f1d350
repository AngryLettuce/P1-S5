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


//float element wise vector multiplication
void fvec_elementWise_mult(float *a, float *b, float *dest, int size);


//------------------------------------
//  MFCC FUNCTIONS
//------------------------------------


//apply hamming window to a signal
void mfcc_hamming_window_256(float *x);
void mfcc_hamming_window_256_opt(float *x);//optimized version

//mel frequency conversion
float mfcc_freq2mel(float f);//normal frequency to mel frequency
float mfcc_mel2freq(float mel);//mel frequency to normal frequency








#endif /* INCLUDE_FUNCTIONS_H_ */
