/*
 * DSK_main.c
 *
 *  Created on: 9 mars 2018
 *      Author: pascal
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

        scanf("%c ", &dsk_fsm_command);

        if (dsk_fsm_command == 1) {         // DSK_Delete_User
            dsk_fsm_command = 0;

            if (mfcc_speaker_list.speaker_nb > 0)
            {
                printf("DSK currently deleting user\n");
                mfcc_speaker_list.speaker_nb--;
            }
            else {
                printf("Can't delete users from an empty list\n")
            }
        }
        else if (dsk_fsm_command == 2) {    // DSK_Analyze_User
            dsk_fsm_command = 0;

            printf("DSK currently analyzing user\n");
            if (mfcc_speaker_list.speaker_nb > 1) {
                printf("You could find a user if the code was here ;)\n");
            }
            else {
                printf("Not enough users to make an analysis\n");
            }
        }
        else if (dsk_fsm_command == 3) {    // DSK_Add_User
            dsk_fsm_command = 0;

            if (mfcc_speaker_list.speaker_nb < SPEAKER_MB_MAX) {
                printf("*insert ''Init add user'' code here*\n");
                mfcc_speaker_list.speaker_nb++;

                int TEST_codebookcreation_var;
                while (TEST_codebookcreation_var < 1000);

                printf("Metric Table is full\n");
                printf("User added"\n);
            }
            else {
                printf("User maximum limit reached\n")
            }
        }
        else {
            dsk_fsm_command = 0;
        }
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

    SpeakerDataList mfcc_speaker_list;

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


