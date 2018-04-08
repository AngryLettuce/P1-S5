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

volatile char dsk_fsm_command = 0;
volatile bool btn_pressed_flag = false;
SpeakerDataList mfcc_speaker_list;

//Tableau donnees brute AIC
short record[record_length];
#pragma DATA_SECTION(record,".EXT_RAM")


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

    // Initialize AIC
    comm_intr();

    // Main FSM loop
    while(1) {


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

interrupt void c_int11(void){

    static int j = 0;

    if(j==record_length)                //Index tableau
                j=0;

    record[j] = input_left_sample();    //Tableau de données brutes
    j++;

    output_left_sample(0);              //Obligatoire pour permettre de réactiver l'Interupt
}


void dsk_init(void) {

    printf("DSK Init = 100%%\n");

    // GPIO configuration
    GPIO_Handle GPIO_handle = GPIO_open(GPIO_DEV0, GPIO_OPEN_RESET);
    // Pin 7 (Button Pressed interrupt)
    GPIO_pinEnable(GPIO_handle, GPIO_PIN7);
    GPIO_pinDirection(GPIO_handle, GPIO_PIN7, GPIO_INPUT);
    GPIO_intPolarity(GPIO_handle, GPIO_GPINT7, GPIO_RISING);
    // Pin 8 (Delete Command/Up)
    GPIO_pinEnable(GPIO_handle, GPIO_PIN8);
    GPIO_pinDirection(GPIO_handle, GPIO_PIN8, GPIO_INPUT);
    // Pin 9 (Analyze/Down)
    GPIO_pinEnable(GPIO_handle, GPIO_PIN9);
    GPIO_pinDirection(GPIO_handle, GPIO_PIN9, GPIO_INPUT);
    // Pin 10 (Add/OK)
    GPIO_pinEnable(GPIO_handle, GPIO_PIN10);
    GPIO_pinDirection(GPIO_handle, GPIO_PIN10, GPIO_INPUT);

    // IRQ configuration
    IRQ_globalDisable();
    IRQ_map(IRQ_EVT_EXTINT7, 7);
    IRQ_enable(IRQ_EVT_EXTINT7);
    IRQ_nmiEnable();
    IRQ_globalEnable();

    // Table emptying
    mfcc_speaker_list.speaker_nb = 0;
    int i = 0;
    for (i = 0; i < 16; i++)
    {
        mfcc_speaker_list.speaker_data[i].codebook.codeword_nb = 0;
    }
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
