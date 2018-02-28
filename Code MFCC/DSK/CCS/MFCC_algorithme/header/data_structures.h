/*
 * data_structures.h
 *
 *  Created on: 16 févr. 2018
 *      Author: pascal
 */

#ifndef INCLUDE_DATA_STRUCTURES_H_
#define INCLUDE_DATA_STRUCTURES_H_

//--------------------------------------
//  important nomanclature
//--------------------------------------
//
//  met = metric
//  vec = vector
//  tab = table
//  len = length
//  nb  = nombre
//  ind = index
//
//  variable name structure :
//
//  [name of variable]_[attribute]
//
//  ex.: metVec_len -> stand for "metric vector length"


//--------------------------------------
//  constant declaration
//--------------------------------------



//signal data
#define SIGNAL_BLOCK_SIZE       256 //M
#define SIGNAL_BLOCK_OVERLAP    100 //N

//metrics
#define METRIC_VECTOR_LENGTH        13
#define METRIC_VECTOR_TAB_LENGTH    5000

//codebook
#define CODEBOOK_CODEWORDS_NB 16

//mel filter bank
#define MEL_FILTER_NB     20
#define MEL_FILTER_NB_MAX 100


//--------------------------------------
//  data structure use in the algorithm
//--------------------------------------

float x_complex[2*SIGNAL_BLOCK_SIZE];
float w[SIGNAL_BLOCK_SIZE];
#pragma DATA_ALIGN(x, 8);
#pragma DATA_ALIGN(w, 8);



//--------------------
//  METRIC
//--------------------

typedef struct MetVec {

    float met[METRIC_VECTOR_LENGTH];
} MetVec;


typedef struct MetVecTab {

    MetVec metVec[METRIC_VECTOR_TAB_LENGTH];
    int size;
} MetVecTab;


//--------------------
//  MEL FILTER BANK
//--------------------

typedef struct MelFilterBank {

    float melFilter[MEL_FILTER_NB][SIGNAL_BLOCK_SIZE/2];
    int melFilter_nb;

} MelFilterBank;


//--------------------
//  CODEBOOK
//--------------------

typedef struct Codebook {

    MetVec codeword[CODEBOOK_CODEWORDS_NB];
    int speaker_ind;

} Codebook;


//--------------------------------------
//  speaker information data structrure
//--------------------------------------

typedef struct SpeakerData {

    int speaker_ind;
    Codebook codebook;

} SpeakerData;


typedef struct SpeakerDataList {

    SpeakerData speaker_data;
    struct SpeakerBank *next;

} SpeakerDataList;


//--------------------------------------
//  MFCC general structure
//--------------------------------------


typedef struct MFCCModule {

    MelFilterBank melFilterBank;
    float fftCoeffTab[SIGNAL_BLOCK_SIZE];

    float x[SIGNAL_BLOCK_SIZE];
    float x_complex[SIGNAL_BLOCK_SIZE*2];

    int x_size;

} MFCC;






#endif /* INCLUDE_DATA_STRUCTURES_H_ */
