/*
 * DSK_main.c
 *
 *  Created on: 9 mars 2018
 *  Last edited on: 2 avril 2018
 *      Authors: Pascal L, Charles C
 */


#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <time.h>

#define DSK_H_INIT
#include "dsk.h"
#include "data_structures.h"
#include "mfcc.h"
#include "utils.h"
#include "fft_utility.h"

volatile char dsk_fsm_command;
SpeakerDataList mfcc_speaker_list;

/*------------------------------------------------------
 *   dsk_main :
 *
 *   contain the FSM ("MEF") of the DSK and the MFCC algorithm routine
 *
 *
 ----------------------------------------------------*/

void dsk_main(void) {

    // Initialize DSK
    dsk_init();
    // Main FSM loop
    while(1) {

        printf("Enter key plz");
        scanf("%d", &dsk_fsm_command);

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

        dsk_fsm_command = 0;
    }
}



void dsk_init(void) {

    printf("DSK Init = 100%%\n");
}

void mfcc_init(MFCCModule *mfcc, MetVecTab *metVecTab) {

    mfcc->x_size = SIGNAL_BLOCK_SIZE;
    mfcc->mfcc_nb = MFCC_COEFFICIENT_NB;
    mfcc->metVecTab = metVecTab;

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


void mfcc_construct_codebook() {




}

void fsm_delete_user(void) {
    if (mfcc_speaker_list.speaker_nb > 0)
    {
        printf("Delete user at which position");
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
