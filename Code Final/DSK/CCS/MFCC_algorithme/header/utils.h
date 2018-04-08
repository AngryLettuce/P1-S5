/*
 * functions.h
 *
 *  Created on: 16 f�vr. 2018
 *      Author: pascal
 */

#ifndef INCLUDE_UTILS_H_
#define INCLUDE_UTILS_H_



//function prefix convention
//
//
//  mfcc = mfcc pipeline function
//


//------------------------------------
//  UTILS FUNCTION
//------------------------------------
void cpy_circTab_int16(short *newTab, short *circTab, short *ind, int size);

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

//manipulation of array
void farray_dotProduct      (float *a1, float *a2, float *dest, int size);
void farray_dotProduct_value(float *a1, float value, float *dest, int size);
void farray_sum         (float *a1, float *a2, float *dest, int size);
void farray_sum_value   (float *a1, float value, float *dest, int size);

void farray_reset(float *a, float value, int size);




#endif /* INCLUDE_UTILS_H_ */
