/*
 * functions.c
 *
 *  Created on: 16 févr. 2018
 *      Author: pascal
 */

#include "mfcc.h"
#include "utils.h"
#include "fft_utility.h"

#include "math.h"

#include "DSPF_sp_cfftr2_dit.h"


#define M_PI 3.1415926535897932384



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

//general purpose hamming window
void mfcc_hamming_window_init(float *h, int size) {
    int i;
    for(i = 0; i < size; i++)
        h[i] = 0.54 - 0.46*cos(2*M_PI*i/(size-1));
}
void mfcc_hamming_window(float *x, float *h) {
    int i;
    for(i = 0; i < 256; i++)
        x[i] = x[i] * h[i];
}

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

    float mel[MEL_FILTER_NB_MAX] = {0};
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


void mfcc_getMelCoeff(float *x, float *coeff, MelFilterBank *melFilterBank) {

    int i, j;
    int filter_nb = melFilterBank->melFilter_nb;
    float (*filters)[SIGNAL_BLOCK_SIZE/2] = melFilterBank->melFilter;
    int (*index)[2] = melFilterBank->index;

    for(i = 0; i < filter_nb; i++) {
        coeff[i] = 0;
        for(j = index[i][0] + 1; j <= index[i][1] - 1; j++) {
            coeff[i] += x[j] * filters[i][j];
        }
        coeff[i] = log10(coeff[i]);
    }
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

//--------------------------------------------
//  Moving Average
//--------------------------------------------


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



//--------------------------------------------
//  Discret Cosine Transform
//--------------------------------------------

void mfcc_dct_init(float *cosTab, int in_coeff_nb, int out_coeff_nb) {

    int n, k;
    for(n = 0; n < out_coeff_nb; n++)
    for(k = 0; k < in_coeff_nb; k++)
        cosTab[n*in_coeff_nb + k] = cos(n * M_PI * (k + 0.5) / in_coeff_nb);
}

void mfcc_dct(float *in_coeff, float *out_coeff, float *cosTab, int in_coeff_nb, int out_coeff_nb) {

    int n, k;
    for(n = 0; n < out_coeff_nb; n++) {

        out_coeff[n] = 0;
        for(k = 0; k < in_coeff_nb; k++)
            out_coeff[n] += in_coeff[k] * cosTab[n*in_coeff_nb + k];
    }
}




//------------------------------------
//  MFCC CODEBOOK FUNCTIONS
//------------------------------------

void cb_get_nearestCodeword(float *dist_min, int *ind_min, float *met, Codebook *codebook, int met_size, int curr_codebook_size) {

    int i;
    float dist_min_last;

    *dist_min = 99999999;
    for(i = 0; i < curr_codebook_size; i++) {

        dist_min_last = euclideanDistPow2(codebook->codeword[i].met, met, met_size);

        if (dist_min_last < *dist_min) {
            *dist_min = dist_min_last;
            *ind_min = i;
        }
    }
}

/*
void cb_get_newCodewordCentroid(MetVecTab *metVecTab, Codebook *codebook, int curr_codebook_size) {

    int i, j, ind_min;
    float dist_min, dist_min_last;
    int metVec_size = metVecTab->metVec_size;
    int metVecTab_size = metVecTab->metVecTab_size;

    for(i = 0; i < metVecTab_size; i++) {

        //find the closest codeword to the metrics
        cb_get_nearestCodeword(&dist_min, &ind_min, metVecTab->metVec[i].met, codebook, metVec_size, curr_codebook_size);

        //add current codebook's codeword with metric vector associated with it
        farray_sum(codebook->codeword[ind_min].met, metVecTab->metVec[i].met,
                   codebook->codeword[ind_min].met, metVec_size);

        //keep track of number of metrics associated with each codewords
        codebook->N[ind_min] += 1;
    }

    for(i = 0; i < curr_codebook_size; i++) {
        // C = C_acc ./ N;
        farray_dotProduct_value(codebook->codeword[i].met, 1./(float)codebook->N[i],
                                codebook->codeword[i].met, metVec_size);
    }
}
*/

void cb_construct_codebook(MetVecTab *metVecTab,
                           Codebook *codebook,
                           int codebook_size,
                           int speaker_ind,
                           float epsilon,
                           float distortion_Err) {

    Codebook codebook_acc;
    float distortion;

    int m = 1;
    int i, j, ind_min;
    int iter, iter_limit = 100;

    float dist_min, dist_min_last, dist_acc, dist_acc_last;
    int metVec_size = metVecTab->metVec_size;
    int metVecTab_size = metVecTab->metVecTab_size;

    //reset codebook
    for(i = 0; i < codebook_size; i++) {
        farray_reset(codebook->codeword[i].met, 0, metVec_size);
    }

    //find first centroid from mean of all metrics
    for(i = 0; i < metVecTab_size; i++) {

        //add current codebook's codeword with metric vector associated with it
        farray_sum(codebook->codeword[0].met, metVecTab->metVec[i].met,
                   codebook->codeword[0].met, metVec_size);
    }
    // C = C_acc ./ N
    farray_dotProduct_value(codebook->codeword[0].met, 1./(float)metVecTab_size,
                            codebook->codeword[0].met, metVec_size);

    //main loop
    while(m < codebook_size) {

        distortion = 1000;
        dist_acc_last = 0;

        //split sequence
        for(i = m-1; i >= 0; i--) {
            //split each codebook into 2 new codebook
            farray_dotProduct_value(codebook->codeword[i].met,      1 - epsilon,
                                    codebook->codeword[2*i+1].met,    metVec_size);
            farray_dotProduct_value(codebook->codeword[i].met,      1 + epsilon,
                                    codebook->codeword[2*i].met,    metVec_size);
        }
        //update codewords nb
        m *= 2;
        iter = 0;
        //reset codebook accumulator
        for(i = 0; i < m; i++) {
            codebook_acc.N[i] = 0;
            farray_reset(codebook_acc.codeword[i].met, 0, metVec_size);
        }
        while(distortion > distortion_Err && iter < iter_limit) {

            //-------------------------------------------------
            // get new centroid from new jsplitted codewords

            for(i = 0; i < metVecTab_size; i++) {

                //find the closest codeword to the metrics
                cb_get_nearestCodeword(&dist_min, &ind_min, metVecTab->metVec[i].met, codebook, metVec_size, m);

                //add current codebook's codeword with metric vector associated with it
                farray_sum(codebook_acc.codeword[ind_min].met, metVecTab->metVec[i].met,
                           codebook_acc.codeword[ind_min].met, metVec_size);

                //keep track of number of metrics associated with each codewords
                codebook_acc.N[ind_min] += 1;
            }

            for(i = 0; i < m; i++) {

                if (codebook_acc.N[i] == 0)
                    codebook_acc.N[i] = 1;

                // C = C_acc ./ N;
                farray_dotProduct_value(codebook_acc.codeword[i].met, 1./(float)codebook_acc.N[i],
                                        codebook->codeword[i].met, metVec_size);
            }


            //-------------------------------------------------
            // get distortion

            dist_acc = 0;
            for(i = 0; i < metVecTab_size; i++) {

                cb_get_nearestCodeword(&dist_min, &ind_min, metVecTab->metVec[i].met, codebook, metVec_size, m);
                dist_acc += dist_min;
            }

            if (dist_acc != 0)
                distortion = abs(dist_acc_last - dist_acc) / dist_acc;
            else
                distortion = 1;

            //set rpevious dist_acc to last
            dist_acc_last = dist_acc;

            iter += 1;
        }
    }
}




void cb_clear_metVecTab(MetVecTab *metVecTab);



//------------------------------------
//  SPEAKER RECOGNITION CODEBOOK FUNCTIONS
//------------------------------------

short spkrec_get_speakerInd(float *met,SpeakerDataList *speakerList)   {
    short speakerInd;
    short i, j;
    float temp = 0;
    float minTemp = 10000;

    for(i=0;i<speakerList->speaker_nb;i++)
    {
        if(speakerList->speaker_data[i].isActive)
        {
            for(j=0;j<speakerList->speaker_data[i].codebook.codeword_nb;j++)
            {
                temp = euclideanDistPow2(met,speakerList->speaker_data[i].codebook.codeword[j].met,speakerList->speaker_data[i].codebook.metVec_size);
                if(temp < minTemp)
                {
                    minTemp = temp;
                    speakerInd = speakerList->speaker_data[i].speaker_ind;
                }
            }
        }
    }

    return speakerInd;
}

short spkrec_get_modeSpeakerInd(short *speakerBank, short *curr_ind, int mode_size, int bank_size)  {

    short modeSpeakerInd;
    short i,j,k;
    static short AccSpeaker[SPEAKER_NB_MAX];
    short temp = 0;
    short maxInd = 0;

    for(i=0; i<SPEAKER_NB_MAX; i++)
    {
        AccSpeaker[i] = 0;
    }

    for(j=0; j<mode_size; j++)
    {
        AccSpeaker[*curr_ind--]++;

        if(curr_ind < speakerBank )
           {
               curr_ind = speakerBank + bank_size - 1;
           }
    }

    for(k=0; k<SPEAKER_NB_MAX; k++)
    {
        temp = AccSpeaker[k];

        if(temp>maxInd)
        {
            maxInd = temp;
            modeSpeakerInd = k;
        }
    }

    return modeSpeakerInd;
}

short spkrec_get_thresholdSpeakerInd(short new_speakerInd, short curr_speakerInd, short threshold)    {

    static short last_speakerInd = -1;
    static short compt = 0;

    if(last_speakerInd == new_speakerInd)
    {
       compt+=1;
    }
    else
    {
       compt = 0;
    }

    last_speakerInd = new_speakerInd;

    if(compt>=threshold)
    {

        return last_speakerInd;
    }
    else
    {
        return curr_speakerInd;
    }
}


short spkrec_get_currentSpeaker(float *met,SpeakerDataList *speakerList,short curr_speakerInd,short threshold, int mode_size, int bank_size){
    short speakerInd;
    short modeSpeakerInd;
    short new_speakerInd;
    static short speakerBank[SIGNAL_BLOCK_SIZE] = {-1};
    static short *curr_ind = speakerBank;

    speakerInd = spkrec_get_speakerInd(met, speakerList);

    *curr_ind++ = speakerInd;

    if(curr_ind >= speakerBank + SIGNAL_BLOCK_SIZE)
    {
        curr_ind = speakerBank;
    }

    modeSpeakerInd = spkrec_get_modeSpeakerInd(speakerBank, curr_ind, mode_size, bank_size);

    new_speakerInd = spkrec_get_thresholdSpeakerInd(modeSpeakerInd, curr_speakerInd, threshold);

    return new_speakerInd;
}
