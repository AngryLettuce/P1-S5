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

//get the whole circTab and cpy it content to new table
void cpy_circTab_int16(short *newTab, short *circTab, short *ind, int size) {

    int i;
    short *tabEnd = circTab + size;
    for(i = 0; i < size; i++) {
        newTab[i] = *ind;
        ind++;
        if (ind >= tabEnd)
            ind = circTab;
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
    acc = acc / (float) size;

    return acc;
}

