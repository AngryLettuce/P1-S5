/*
 * functions.c
 *
 *  Created on: 16 févr. 2018
 *      Author: pascal
 */

#include "utils.h"
#include "math.h"


#define M_PI 3.1415926535897932384




//--------------------------------------------
//  UTILS FUNCTIONS
//--------------------------------------------

void cpy_circTab_int16_backward(short *newTab, short *circTab, short *ind, int size, int data_nb) {

    int i;
    for(i = 0; i < data_nb; i++) {
        newTab[i] = *ind;
        ind--;
        if (ind < circTab)
            ind = circTab + size - 1;
    }
}

void cpy_circTab_f32_backward(float *newTab, float *circTab, float *ind, int size, int data_nb) {

    int i;
    for(i = 0; i < data_nb; i++) {
        newTab[i] = *ind;
        ind--;
        if (ind < circTab)
            ind = circTab + size - 1;
    }
}

void cpy_circTab_int16_forward(short *newTab, short *circTab, short *ind, int size, int data_nb) {

    int i;
    for(i = data_nb-1; i >= 0; i--) {
        newTab[i] = *ind;
        ind--;
        if (ind < circTab)
            ind = circTab + size - 1;
    }
}

void cpy_circTab_f32_forward(float *newTab, float *circTab, float *ind, int size, int data_nb) {

    int i;
    for(i = data_nb-1; i >= 0; i--) {
        newTab[i] = *ind;
        ind--;
        if (ind < circTab)
            ind = circTab + size - 1;
    }
}



float float_abs(float x) {
    if (x < 0.0)
        return -x;
    return x;
}


//float element wise vector multiplication
void fvec_elementWise_mult(float *a, float *b, float *dest, int size) {

    int i;
    for(i = 0; i < size; i++) {

        dest[i] = a[i] * b[i];
    }
}

//float to complex number DSPC6713 format
void float2complex(float *a, float *b, int size) {

    int i;
    for(i = 0; i < size; i++) {
        b[i*2] = a[i];
        b[i*2 + 1] = 0;
    }
}

//calculate euclidean distance without taking the square root of the result
float euclideanDistPow2(float *a, float *b, int size) {

    int i;
    float acc = 0;

    for(i = 0; i < size; i++) {
        acc = acc + (a[i] - b[i])*(a[i] - b[i]);
    }

    return acc;
}



//manipulation of array
void farray_dotProduct(float *a1, float *a2, float *dest, int size) {
    int i;
    for(i = 0; i < size; i++)
        dest[i] = a1[i] * a2[i];
}

void farray_dotProduct_value(float *a1, float value, float *dest, int size) {
    int i;
    for(i = 0; i < size; i++)
        dest[i] = a1[i] * value;
}

void farray_sum(float *a1, float *a2, float *dest, int size) {
    int i;
    for(i = 0; i < size; i++)
        dest[i] = a1[i] + a2[i];
}


void farray_sum_value(float *a1, float value, float *dest, int size) {
    int i;
    for(i = 0; i < size; i++)
        dest[i] = a1[i] + value;
}

void farray_reset(float *a, float value, int size) {
    int i;
    for(i = 0; i < size; i++)
        a[i] = value;
}













