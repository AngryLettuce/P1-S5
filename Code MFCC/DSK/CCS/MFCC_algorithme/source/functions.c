/*
 * functions.c
 *
 *  Created on: 16 f�vr. 2018
 *      Author: pascal
 */

#include "functions.h"

#include "math.h"

#include "fft_utility.h"
#include "DSPF_sp_cfftr2_dit.h"


//for FFT implementation
#define MAX 200
#define M_PI 3.1415926535897932384




//--------------------------------------------
//  UTILS FUNCTIONS
//--------------------------------------------


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


//--------------------------------------------
//  HAMMING WINDOW
//--------------------------------------------

//pre-emphasis 1st order FIR filter
float mfcc_preAmpFIR(float x, float x_last) {
    return x - 0.95*x_last;
}


//--------------------------------------------
//  HAMMING WINDOW
//--------------------------------------------

//apply hamming window to a signal
void mfcc_hamming_window_256(float *x) {

    static const float hamming_window[256] = {0.08,0.080139632,0.080558444,0.08125618,0.082232418,0.083486566,0.08501786,0.086825373,0.088908006,0.091264495,0.093893409,0.096793154,0.099961967,0.103397926,0.107098944,0.111062774,0.115287011,0.119769089,0.124506288,0.129495732,0.134734391,0.140219085,0.145946484,0.151913112,0.158115346,0.16454942,0.171211429,0.178097329,0.185202937,0.192523942,0.200055898,0.207794233,0.215734248,0.223871124,0.232199921,0.240715582,0.249412937,0.258286707,0.267331503,0.276541836,0.285912112,0.295436645,0.305109651,0.314925258,0.324877507,0.334960356,0.345167684,0.355493294,0.365930917,0.376474217,0.387116792,0.397852183,0.40867387,0.419575286,0.43054981,0.441590782,0.452691497,0.463845217,0.47504517,0.486284557,0.497556555,0.508854319,0.520170992,0.531499704,0.542833575,0.554165727,0.565489278,0.576797356,0.588083093,0.59933964,0.610560161,0.621737846,0.632865908,0.643937592,0.654946175,0.665884975,0.676747351,0.687526708,0.698216503,0.708810244,0.719301502,0.729683906,0.739951154,0.750097012,0.760115322,0.77,0.779745046,0.789344544,0.798792666,0.808083676,0.817211933,0.826171896,0.834958125,0.843565286,0.851988154,0.860221615,0.868260671,0.876100441,0.883736166,0.89116321,0.898377064,0.905373349,0.912147817,0.918696356,0.92501499,0.931099882,0.93694734,0.942553812,0.947915896,0.953030335,0.957894025,0.962504013,0.966857501,0.970951846,0.974784561,0.97835332,0.981655957,0.984690466,0.987455005,0.989947896,0.992167626,0.994112846,0.995782376,0.997175203,0.99829048,0.999127531,0.999685848,0.999965091,0.999965091,0.999685848,0.999127531,0.99829048,0.997175203,0.995782376,0.994112846,0.992167626,0.989947896,0.987455005,0.984690466,0.981655957,0.97835332,0.974784561,0.970951846,0.966857501,0.962504013,0.957894025,0.953030335,0.947915896,0.942553812,0.93694734,0.931099882,0.92501499,0.918696356,0.912147817,0.905373349,0.898377064,0.89116321,0.883736166,0.876100441,0.868260671,0.860221615,0.851988154,0.843565286,0.834958125,0.826171896,0.817211933,0.808083676,0.798792666,0.789344544,0.779745046,0.77,0.760115322,0.750097012,0.739951154,0.729683906,0.719301502,0.708810244,0.698216503,0.687526708,0.676747351,0.665884975,0.654946175,0.643937592,0.632865908,0.621737846,0.610560161,0.59933964,0.588083093,0.576797356,0.565489278,0.554165727,0.542833575,0.531499704,0.520170992,0.508854319,0.497556555,0.486284557,0.47504517,0.463845217,0.452691497,0.441590782,0.43054981,0.419575286,0.40867387,0.397852183,0.387116792,0.376474217,0.365930917,0.355493294,0.345167684,0.334960356,0.324877507,0.314925258,0.305109651,0.295436645,0.285912112,0.276541836,0.267331503,0.258286707,0.249412937,0.240715582,0.232199921,0.223871124,0.215734248,0.207794233,0.200055898,0.192523942,0.185202937,0.178097329,0.171211429,0.16454942,0.158115346,0.151913112,0.145946484,0.140219085,0.134734391,0.129495732,0.124506288,0.119769089,0.115287011,0.111062774,0.107098944,0.103397926,0.099961967,0.096793154,0.093893409,0.091264495,0.088908006,0.086825373,0.08501786,0.083486566,0.082232418,0.08125618,0.080558444,0.080139632,0.08};
    int i;

    for(i = 0; i < 256; i++) {

        x[i] = x[i] * hamming_window[i];
    }
}

//optimized function
void mfcc_hamming_window_256_opt(float *x) {

    static const float hamming_window[256] = {0.08,0.080139632,0.080558444,0.08125618,0.082232418,0.083486566,0.08501786,0.086825373,0.088908006,0.091264495,0.093893409,0.096793154,0.099961967,0.103397926,0.107098944,0.111062774,0.115287011,0.119769089,0.124506288,0.129495732,0.134734391,0.140219085,0.145946484,0.151913112,0.158115346,0.16454942,0.171211429,0.178097329,0.185202937,0.192523942,0.200055898,0.207794233,0.215734248,0.223871124,0.232199921,0.240715582,0.249412937,0.258286707,0.267331503,0.276541836,0.285912112,0.295436645,0.305109651,0.314925258,0.324877507,0.334960356,0.345167684,0.355493294,0.365930917,0.376474217,0.387116792,0.397852183,0.40867387,0.419575286,0.43054981,0.441590782,0.452691497,0.463845217,0.47504517,0.486284557,0.497556555,0.508854319,0.520170992,0.531499704,0.542833575,0.554165727,0.565489278,0.576797356,0.588083093,0.59933964,0.610560161,0.621737846,0.632865908,0.643937592,0.654946175,0.665884975,0.676747351,0.687526708,0.698216503,0.708810244,0.719301502,0.729683906,0.739951154,0.750097012,0.760115322,0.77,0.779745046,0.789344544,0.798792666,0.808083676,0.817211933,0.826171896,0.834958125,0.843565286,0.851988154,0.860221615,0.868260671,0.876100441,0.883736166,0.89116321,0.898377064,0.905373349,0.912147817,0.918696356,0.92501499,0.931099882,0.93694734,0.942553812,0.947915896,0.953030335,0.957894025,0.962504013,0.966857501,0.970951846,0.974784561,0.97835332,0.981655957,0.984690466,0.987455005,0.989947896,0.992167626,0.994112846,0.995782376,0.997175203,0.99829048,0.999127531,0.999685848,0.999965091,0.999965091,0.999685848,0.999127531,0.99829048,0.997175203,0.995782376,0.994112846,0.992167626,0.989947896,0.987455005,0.984690466,0.981655957,0.97835332,0.974784561,0.970951846,0.966857501,0.962504013,0.957894025,0.953030335,0.947915896,0.942553812,0.93694734,0.931099882,0.92501499,0.918696356,0.912147817,0.905373349,0.898377064,0.89116321,0.883736166,0.876100441,0.868260671,0.860221615,0.851988154,0.843565286,0.834958125,0.826171896,0.817211933,0.808083676,0.798792666,0.789344544,0.779745046,0.77,0.760115322,0.750097012,0.739951154,0.729683906,0.719301502,0.708810244,0.698216503,0.687526708,0.676747351,0.665884975,0.654946175,0.643937592,0.632865908,0.621737846,0.610560161,0.59933964,0.588083093,0.576797356,0.565489278,0.554165727,0.542833575,0.531499704,0.520170992,0.508854319,0.497556555,0.486284557,0.47504517,0.463845217,0.452691497,0.441590782,0.43054981,0.419575286,0.40867387,0.397852183,0.387116792,0.376474217,0.365930917,0.355493294,0.345167684,0.334960356,0.324877507,0.314925258,0.305109651,0.295436645,0.285912112,0.276541836,0.267331503,0.258286707,0.249412937,0.240715582,0.232199921,0.223871124,0.215734248,0.207794233,0.200055898,0.192523942,0.185202937,0.178097329,0.171211429,0.16454942,0.158115346,0.151913112,0.145946484,0.140219085,0.134734391,0.129495732,0.124506288,0.119769089,0.115287011,0.111062774,0.107098944,0.103397926,0.099961967,0.096793154,0.093893409,0.091264495,0.088908006,0.086825373,0.08501786,0.083486566,0.082232418,0.08125618,0.080558444,0.080139632,0.08};
    int i;

    for(i = 0; i < 256; i+=8) {

        //force the DSP to do 2 multiplication in parallel and use 8 parallel adder
        x[i]   = x[i]   * hamming_window[i];
        x[i+1] = x[i+1] * hamming_window[i+1];
        x[i+2] = x[i+2] * hamming_window[i+2];
        x[i+3] = x[i+3] * hamming_window[i+3];
        x[i+4] = x[i+4] * hamming_window[i+4];
        x[i+5] = x[i+5] * hamming_window[i+5];
        x[i+6] = x[i+6] * hamming_window[i+6];
        x[i+7] = x[i+7] * hamming_window[i+7];
    }
}




//--------------------------------------------
//  Mel filter bank
//--------------------------------------------

//normal frequency to mel frequency
float mfcc_freq2mel(float f) {

    return  1125.0*log(1.0 + f/700.0);
}

//mel frequency to normal frequency
float mfcc_mel2freq(float mel) {

    return 700.0*exp(mel/1125.0) - 700.0;
}

//mel filter bank construct
void mfcc_melFilterBank_create(MelFilterBank* melFilterBank, float freqL, float freqH, int filter_number, int size_data, int sample_rate) {
    /*-------------------------------------------
    construct the data to be contained within "melFilter"
     * (voir la fonction dans matlab : "melFilterBank.m"
    -------------------------------------------*/
    int N = filter_number + 2;

    float mel[MEL_FILTER_NB_MAX] =  {0};
    float f[MEL_FILTER_NB_MAX] = {0};

    mel[N-1] = mfcc_freq2mel(freqH);
    mel[0]   = mfcc_freq2mel(freqL);

    float delta_mel = (mel[N-1] - mel[0]) / (N - 1);


    int i;
    for (i = 1;  i < N; i ++)
    {
        mel[i] = mel[i-1] + delta_mel;
    }



    for (i = 0;  i < N; i ++)
    {
        f[i]  = mfcc_mel2freq(mel[i]);
        f[i]  = floor((size_data)*f[i] / (sample_rate/2));
    }

    int n;

    for (i = 1; i < filter_number+1; i++)
    {
        melFilterBank->index[i-1][0] = f[i-1];
        melFilterBank->index[i-1][1] = f[i+1];
        for (n = 0;  n < size_data; n++)
        {
            if (n >= f[i-1] && n <= f[i])
                melFilterBank->melFilter[i-1][n] = (n - f[i-1]) / (f[i] - f[i-1]);
            else if (n > f[i] && n <= f[i+1])
                melFilterBank->melFilter[i-1][n] = (f[i+1] - n) / (f[i+1] - f[i]);
            else
                melFilterBank->melFilter[i-1][n] = 0;
        }
    }
    melFilterBank->melFilter_nb = filter_number;
}

//--------------------------------------------
//  FAST FOURIER TRANSFORM (FFT)
//--------------------------------------------
// DSPF_sp_cfftr2_dit(float *src, float *dst, short n);
// voir librairie du DSP, "DSPLIB"

void mfcc_fft_init(float *w, short *index, int N) {

    tw_genr2fft(w, N); // Generate coefficient table
    bit_rev(w, N >> 1); // Bit-reverse coefficient table

    bitrev_index(index, N);//generate index table for bit reverse output
}


void mfcc_fft(float *complexTab, float *w, short* index, int N) {

    DSPF_sp_cfftr2_dit(complexTab, w, N);
    DSPF_sp_bitrev_cplx(complexTab, index, N);
}

//--------------------------------------------
//  Power spectrum
//--------------------------------------------

void mfcc_powerSpectrum(float *x, float *x_complex,  int size) {
    /*-------------------------------------------
    construct the data to be contained within "melFilter"
     * (voir la fonction dans matlab : "powerSpectral.m"
    -------------------------------------------*/
    int i = 0;
    int index = 0;

    for (i = 0; i <= size; i += 2)
    {
        x[index] = (x_complex[i] * x_complex[i]) + (x_complex[i+1] * x_complex[i+1]);
        index ++;
    }

}



float moving_average(float *beta_acc, int size, int acc_size){
    static float alpha_p = 0;
    static float alpha = 0;
    float average;

    average = (alpha_p * (size - 2 * acc_size)/acc_size + alpha + *beta_acc)/size;
    // Reasign the old values for the next iteration
    alpha_p = alpha;
    alpha   = *beta_acc;
    *beta_acc = 0;

    return average;

}

void acc_interval(float *curr_data, float *beta_acc){

    *beta_acc += *curr_data;
}


void mfcc_getMelCoeff(float *x, float *coeff, MelFilterBank *melFilterBank) {

    int i, j;
    int filter_nb = melFilterBank->melFilter_nb;
    float (*filters)[SIGNAL_BLOCK_SIZE/2] = melFilterBank->melFilter;

    for(i = 0; i < filter_nb; i++) {
        coeff[i] = 0;
        for(j = melFilterBank->melFilter[i][0]; j <= melFilterBank->melFilter[i][1]; j++) {
            coeff[i] += x[j] * filters[i][j];
        }
    }
}



void mfcc_dct_init(float *cosTab, int in_coeff_nb, int out_coeff_nb) {

    int n, k;
    for(n = 0; n < out_coeff_nb; n++)
    for(k = 0; k < in_coeff_nb; k++)
        cosTab[n*in_coeff_nb + k] = cos(n * M_PI * (k - 0.5) / in_coeff_nb);
}

void mfcc_dct(float *in_coeff, float *out_coeff, float *cosTab, int in_coeff_nb, int out_coeff_nb) {

    int n, k;
    for(n = 0; n < out_coeff_nb; n++) {

        out_coeff[n] = 0;
        for(k = 0; k < in_coeff_nb; k++)
            out_coeff[n] += in_coeff[k] * cosTab[n*in_coeff_nb + k];
    }
}


/*
void mfcc_dct

function [ C ] = discreteCosineTransform( y )

% return the discrete cosine transform of y;
K = size(y,2);
C = zeros(1,K);#


k = 1:K;

for n = 0:K-1
    C(n + 1) = sum(y.*cos(n.*(k - 0.5)*pi/K));
end

end
*/



