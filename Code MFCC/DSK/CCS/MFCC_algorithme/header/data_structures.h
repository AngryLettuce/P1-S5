/*
 * data_structures.h
 *
 *  Created on: 16 f�vr. 2018
 *      Author: pascal
 */


#ifndef INCLUDE_DATA_STRUCTURES_H_
#define INCLUDE_DATA_STRUCTURES_H_
#include <stdbool.h>
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

#define MFCC_COEFFICIENT_NB 13

//ADC
#define SAMPLE_RATE 8000

//signal data
#define SIGNAL_BLOCK_SIZE       256 //M
#define SIGNAL_BLOCK_OVERLAP    100 //N

//metrics
#define METRIC_VECTOR_LENGTH        13
#define METRIC_VECTOR_TAB_LENGTH    5000

//codebook
#define CODEBOOK_CODEWORDS_NB 16
#define SPEAKER_NB_MAX 64

//mel filter bank
#define MEL_FILTER_NB     20
#define MEL_FILTER_NB_MAX 100
#define MEL_FILTER_FREQ_LOW 40
#define MEL_FILTER_FREQ_HIGH 4000

//--------------------------------------
//  data structure use in the algorithm
//--------------------------------------

//--------------------
//  METRIC
//--------------------

typedef struct MetVec {

    float met[METRIC_VECTOR_LENGTH];
} MetVec;


typedef struct MetVecTab {

    MetVec metVec[METRIC_VECTOR_TAB_LENGTH];
    char metCodewordInd[METRIC_VECTOR_TAB_LENGTH];
    int metVecTab_size;
    int metVec_size;
} MetVecTab;


//--------------------
// HAMMING WINDOW
//--------------------

typedef struct HammingWindow {

    float h[SIGNAL_BLOCK_SIZE];

} HammingWindow;


//--------------------
//  MEL FILTER BANK
//--------------------

typedef struct MelFilterBank {

    float melFilter[MEL_FILTER_NB][SIGNAL_BLOCK_SIZE/2];
    int index[MEL_FILTER_NB][2];
    int melFilter_nb;
} MelFilterBank;


//--------------------
//  FFT
//--------------------

typedef struct FFTmodule {

    float w[SIGNAL_BLOCK_SIZE];
    short index[SIGNAL_BLOCK_SIZE];
    double dontCare;//force structure to be double aligned (8 bytes)

} FFTmodule;

//--------------------
//  DCT
//--------------------

typedef struct DCTmodule {

    float cosTab[MEL_FILTER_NB * MEL_FILTER_NB];
    float coeff[MEL_FILTER_NB];
} DCTmodule;


//--------------------
//  CODEBOOK
//--------------------

typedef struct Codebook {

    MetVec codeword[CODEBOOK_CODEWORDS_NB];
    int N[CODEBOOK_CODEWORDS_NB]; //use when creating the codewords
    int codeword_nb;
    int metVec_size;

} Codebook;


//--------------------------------------
//  speaker information data structure
//--------------------------------------

typedef struct SpeakerData {

    Codebook codebook;
    int speaker_ind;
    bool isActive;


} SpeakerData;


typedef struct SpeakerDataList {

    SpeakerData speaker_data[SPEAKER_NB_MAX];
    int speaker_nb;

} SpeakerDataList;


//--------------------------------------
//  MFCC general structure
//--------------------------------------


typedef struct MFCCModule {
    //signal array
    float x_complex[SIGNAL_BLOCK_SIZE*2];//will be align to double since, there's a double members and it's the first member of the struct
    float x[SIGNAL_BLOCK_SIZE];
    int x_size;

    HammingWindow hwin;
    MelFilterBank mfb;
    FFTmodule fft;//aligned to 8 byte since there a double size member, will force "w" twiddle to align to double. will in turn also force x_complex to be align to double
    DCTmodule dct;

    //metrics temp vector
    MetVecTab *metVecTab;//will be stored in the RAM

    int mfcc_nb;

} MFCCModule;


//--------------------------------------
//  DSK MEF structure
//--------------------------------------


typedef enum PICstate {

    PIC_INIT = 0,
    PIC_IDLE,
    PIC_COMMUNICATION_DSK,
    PIC_STATE_ERROR,
    PIC_ERROR,
    //use to store the number of state
    PIC_STATE_COUNT
} PICstate;


typedef enum DSKstate {

    DSK_INIT = 0,
    DSK_IDLE,
    DSK_TEST_INIT,
    DSK_TEST_ACQUISITION,
    DSK_TRAIN_INIT,
    DSK_TRAIN_ACQUISITION,
    DSK_TRAIN_CODEBOOK_CONSTRUCTION,
    DSK_ERROR,
    //use to store the number of state
    DSK_STATE_COUNT
} DSKstate;






#endif /* INCLUDE_DATA_STRUCTURES_H_ */
