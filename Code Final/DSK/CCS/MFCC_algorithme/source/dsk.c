/*
 * DSK_main.c
 *
 *  Created on: 9 mars 2018
 *  Last edited on: 2 avril 2018
 *      Authors: Pascal L, Charles C
 */


#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include <string.h>
#include <time.h>

#include <csl_gpio.h>
#include <csl_irq.h>

#include "dsk.h"
#include "data_structures.h"
#include "mfcc.h"
#include "utils.h"
#include "fft_utility.h"
#include "C6713Helper_UdeS.h"
#include <dsk6713.h>
#include <dsk6713_led.h>
#include <dsk6713_dip.h>
#include <csl_mcbsp.h>
#include <csl.h>
#include "SPI_driver.h"

volatile char dsk_state = 0;
volatile char dsk_fsm_command = 0;


volatile bool btn_pressed_flag = false;
SpeakerDataList mfcc_speaker_list;

//Tableau donnees brute AIC
short adcRecord[SIGNAL_BLOCK_SIZE] = {0};
short adcCurrSample = 0;
short *adcCurrPtr = adcRecord;
bool adcSample_rdy = 0;

//#pragma DATA_SECTION(record,".EXT_RAM")

short index = 0;
Uint8 data;
int adcCurrentInd = 0;
bool adcInitDone = 0;

//Tableau donnees coefficient mfcc
float mfccCircBuffer[SIGNAL_BLOCK_SIZE] = {0};
float mfccCurrSample = 0;
float *mfccCurrPtr = mfccCircBuffer;
//#pragma DATA_SECTION(record,".EXT_RAM")


//Param détection silence
short sample_acc = 0;
float beta_acc = 0;
int adcSample_acc = 0;
bool silenceDetection_rdy = 0;

MFCCModule dsk_mfcc;

MetVecTab dsk_metVecTab;
#pragma DATA_SECTION(dsk_metVecTab,".EXT_RAM");

/*------------------------------------------------------
 *   dsk_main :
 *
 *   contain the FSM ("MEF") of the DSK and the MFCC algorithm routine
 *
 *
 ----------------------------------------------------*/

void dsk_main(void) {

    dsk_state = DSK_INIT;

    // Main FSM loop
    while(1) {

        switch(dsk_state) {

            case DSK_INIT:
                // Initialize DSK
                dsk_init();
                // Initialize MFCC structure/algo.
                mfcc_init(&dsk_mfcc, &dsk_metVecTab);

                dsk_state = DSK_TEST_ACQUISITION;

                break;

            case DSK_TEST_ACQUISITION:
                // Initialize DSK
                if (adcSample_rdy == 1) {
                    mfcc_main(dsk_state, 100);
                    adcSample_rdy = 0;
                }
                break;
        }
        /*
        switch(dsk_fsm_command) {
        case DELETE_CMD :
            fsm_delete_user();
            break;
        case ANALYZE_CMD :
            fsm_analyze_user();
            break;
        case ADD_CMD :
            fsm_add_user();
            break;
        }
        */
    }
}

interrupt void c_int11(void){

    adcSample_rdy = 1;

    if(adcCurrentInd == SIGNAL_BLOCK_SIZE) {               //Index tableau
        adcCurrentInd = 0;
        adcInitDone = 1;
    }
    adcRecord[adcCurrentInd] = input_left_sample();    //Tableau de données brutes
    adcCurrSample = adcRecord[adcCurrentInd];
    adcCurrPtr = adcRecord + adcCurrentInd;
    //utilisé pour la détection de silence
    adcSample_acc++;
    if (adcSample_acc >= SIGNAL_BLOCK_OVERLAP) {
        silenceDetection_rdy = 1;
        adcSample_acc = 0;
    }

    output_left_sample(adcRecord[adcCurrentInd]);              //Obligatoire pour permettre de réactiver l'Interupt
    adcCurrentInd++;
}


void dsk_init(void) {

    DSK6713_init();
    DSK6713_LED_init();
    DSK6713_DIP_init();
    // Initialize AIC
    comm_intr();
    DSK6713_waitusec(50);

    SPI_init();
    configAndStartTimer0(2812500*3);
    init_ext_intr();


    // Table emptying
    mfcc_speaker_list.speaker_nb = 0;
    int i = 0;
    for (i = 0; i < 16; i++)
    {
        mfcc_speaker_list.speaker_data[i].codebook.codeword_nb = 0;
        mfcc_speaker_list.speaker_data[i].isActive = 0;
    }
}

//------------------------------------
//  MFCC FUNCTIONS
//------------------------------------

void mfcc_main(short state, float silence_threshold) {

    bool mfcc_rdy = 0; // vaut 1 lorsque "SIGNAL_BLOCK_OVERLAP" nouvelles données sont ajouté au buffer adcRecord
    float sound_amplitude = 0;

    //------------------------------------
    //  Détection de silence
    //------------------------------------

    acc_interval(abs((float)adcCurrSample), &beta_acc);

    //printf("ADC: %d\n", adcCurrSample);

    if (silenceDetection_rdy == 1) {

        sound_amplitude = moving_average(&beta_acc, SIGNAL_BLOCK_SIZE, SIGNAL_BLOCK_OVERLAP);
        silenceDetection_rdy = 0;
        if (sound_amplitude > silence_threshold && adcInitDone == 1)
            mfcc_rdy = 1;
    }

    //------------------------------------
    //  Extraction MFCC
    //------------------------------------

    static MetVec met_curr;
    static short bufferFirOrd1[2] = {0};
    cpy_circTab_int16_backward(bufferFirOrd1, adcRecord, adcCurrPtr, SIGNAL_BLOCK_SIZE, 2);


    //filtre fir d'ordre 1
    *mfccCurrPtr = mfcc_preAmpFIR((float)bufferFirOrd1[0], 0.95*((float)bufferFirOrd1[1]));
    mfccCurrPtr++;
    if (mfccCurrPtr >= mfccCircBuffer + SIGNAL_BLOCK_SIZE)
        mfccCurrPtr = mfccCircBuffer;


    if (mfcc_rdy == 1) {

        //set the proper x to mfcc struct
        mfcc_set_x(&dsk_mfcc, mfccCircBuffer, mfccCurrPtr);
        //get metric vectors
        mfcc_get_metrics(met_curr.met, &dsk_mfcc);


        if (!mfcc_add_metVec(met_curr.met, &dsk_mfcc)) {

            //DEBUG SEULEMENT
            DSK6713_LED_on(0);
            mfcc_write_metVecTab(&dsk_mfcc);
            DSK6713_LED_off(0);
        }
    }
}


void mfcc_init(MFCCModule *mfcc, MetVecTab *metVecTab) {

    mfcc->x_size = SIGNAL_BLOCK_SIZE;
    mfcc->mfcc_nb = MFCC_COEFFICIENT_NB;
    mfcc->metVecTab = metVecTab;
    mfcc->metVecTab->metVecTab_size = 0;
    mfcc->metVecTab->metVec_size = METRIC_VECTOR_LENGTH;

    //construct the hamming window table
    mfcc_hamming_window_init(mfcc->hwin.h, mfcc->x_size);

    //construct the melfilter bank
    mfcc_melFilterBank_create(&mfcc->mfb, MEL_FILTER_FREQ_LOW, MEL_FILTER_FREQ_HIGH, MEL_FILTER_NB, (mfcc->x_size) >> 1, SAMPLE_RATE);

    //generate the twiddle table, reverse index table, and cosine table for FFT and DCT
    mfcc_fft_init(mfcc->fft.w, mfcc->fft.index, mfcc->x_size);
    mfcc_dct_init(mfcc->dct.cosTab, mfcc->mfb.melFilter_nb, mfcc->mfb.melFilter_nb);
}


void mfcc_get_metrics(float *met, MFCCModule *mfcc) {

    //mfcc pipeline
    mfcc_hamming_window (mfcc->x        , mfcc->hwin.h      );
    float2complex       (mfcc->x        , mfcc->x_complex   , mfcc->x_size      );
    mfcc_fft            (mfcc->x_complex, mfcc->fft.w       , mfcc->fft.index   , mfcc->x_size );
    mfcc_powerSpectrum  (mfcc->x        , mfcc->x_complex   , mfcc->x_size      );//divide size by 2 (equivalent to right shift 1 bit)
    mfcc_getMelCoeff    (mfcc->x        , mfcc->dct.coeff   ,&mfcc->mfb         );
    mfcc_dct            (mfcc->dct.coeff, met               , mfcc->dct.cosTab  , mfcc->mfb.melFilter_nb    , mfcc->mfcc_nb );

    //pitch pipeline (value store at the location of the first mfcc coefficient, which do not have any speaker dependant information)
}


void fsm_delete_user(void) {
    if (mfcc_speaker_list.speaker_nb > 0)
    {
        printf("Delete user from position? : ");
        scanf("%d", &dsk_fsm_command);
        printf("DSK currently deleting user at position %d\n", dsk_fsm_command);
        mfcc_speaker_list.speaker_data[dsk_fsm_command].codebook.codeword_nb = 0;
        mfcc_speaker_list.speaker_nb--;
    }
    else {
        printf("Can't delete users from an empty list\n");
    }
}

void fsm_add_user(void) {
    if (mfcc_speaker_list.speaker_nb < SPEAKER_NB_MAX) {
        char first_free_user = 0;
        while(mfcc_speaker_list.speaker_data[first_free_user].codebook.codeword_nb != 0) first_free_user++;
        mfcc_speaker_list.speaker_data[first_free_user].codebook.codeword_nb = 1;
        mfcc_speaker_list.speaker_nb++;
        printf("New user added at position %d\n", first_free_user);
    }
    else {
        printf("Maximum user limit reached\n");
    }
}

void fsm_analyze_user(void) {
    printf("DSK currently analyzing user\n");
    if (mfcc_speaker_list.speaker_nb > 1) {
        printf("Analysis complete\n");
    }
    else {
        printf("Not enough users to make an analysis\n");
    }
}

char btn_dbnc(void) {
    return 0;
}

void interrupt itr7_fsm_btn_pressed(void) {
    btn_pressed_flag = true;
}
