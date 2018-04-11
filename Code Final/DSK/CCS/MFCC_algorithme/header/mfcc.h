/*
 * functions.h
 *
 *  Created on: 16 févr. 2018
 *      Author: pascal
 */

#ifndef INCLUDE_MFCC_H_
#define INCLUDE_MFCC_H_

#include "data_structures.h"


//function prefix convention
//
//
//  mfcc = mfcc pipeline function
//

//------------------------------------
//  MFCC FUNCTIONS
//------------------------------------

void mfcc_init(MFCCModule *mfcc, MetVecTab *metVecTab, SpeakerDataList *speakerDataList);
void mfcc_get_metrics(float *met, MFCCModule *mfcc);

void mfcc_set_x(MFCCModule *mfcc, float *mfccCircBuffer, float *mfccCurrPtr);
int mfcc_add_metVec(float *met, MFCCModule *mfcc);
void mfcc_write_metVecTab(MFCCModule *mfcc);

//pre-amplification 1st order FIR filter
float mfcc_preAmpFIR(float x, float x_last);


//apply hamming window to a signal
void mfcc_hamming_window_init(float *h, int size);
void mfcc_hamming_window(float *x, float *h);
//fixed size hamming window
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


//------------------------------------
// Moving average for silent recognition
//------------------------------------

float moving_average(float *beta_acc, int size, int acc_size);
void acc_interval(float curr_data, float *beta_acc);


//------------------------------------
//  MFCC CODEBOOK FUNCTIONS
//------------------------------------

void cb_get_nearestCodeword(float *dist_min, int *ind_min, float *met, Codebook *codebook, int met_size, int curr_codebook_size);
void cb_get_newCodewordCentroid(MetVecTab *metVecTab, Codebook *codebook, int curr_codebook_size);
void cb_construct_codebook(MetVecTab *metVecTab,
                           Codebook *codebook,
                           int codebook_size,
                           int speaker_ind,
                           float epsilon,
                           float distortion_Err);

void cb_clear_metVecTab(MetVecTab *metVecTab);

//------------------------------------
//  SPEAKER INDEX FUNCTION
//------------------------------------

short spkrec_get_speakerInd(float *met,SpeakerDataList *speakerList);
short spkrec_get_modeSpeakerInd(short *speakerBank, short *curr_ind, int mode_size, int bank_size);
short spkrec_get_thresholdSpeakerInd(short new_speakerInd, short curr_speakerInd, short threshold);




#endif /* INCLUDE_MFCC_H_ */
