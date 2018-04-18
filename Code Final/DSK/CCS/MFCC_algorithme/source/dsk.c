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
#include "pitchCalculation.h"
#include "filtreFIR300Hz.h"
#include "utils.h"
#include "fft_utility.h"

#include "CoeffFIR300HzN20.dat"

#include "C6713Helper_UdeS.h"
#include <dsk6713.h>
#include <dsk6713_led.h>
#include <dsk6713_dip.h>
#include <csl_mcbsp.h>
#include <csl.h>
#include "SPI_driver.h"
#include "codebook.h"




#define LOAD_CODEBOOK_DATA 1

//---------------------------------
//MODE
//---------------------------------

short dsk_mode = DSK_SPEAKER;

//---------------------------------
//PARAM INDEX
//---------------------------------

int index_mode_size = INDEX_MODE_SIZE_SPEAKER;
int index_accumulator_nb = INDEX_ACCUMULATOR_THRESHOLD_SPEAKER;

//---------------------------------
//COMMUNICATION
//---------------------------------
bool  dsk_dataIn_flag = 0;
bool  dsk_dataIn_parsed_flag = 0;
Uint8 dsk_dataIn = 0;
Uint8 dsk_dataIn_last = 0;
Uint8 dsk_indexCurr = SPEAKER_IND_UNKNOW; //vaut 14 = orateur inconnue
Uint8 dsk_stateCurr = 0; //Uint8 � remettre � short si probl�me

short dsk_indIn = 0;
short dsk_cmdIn = 0;

char dsk_state = 0;

char dsk_fsm_command = 0;

volatile bool btn_pressed_flag = false;


//---------------------------------
//Tableau donnees brute AIC
//---------------------------------
short adcRecord[SIGNAL_BLOCK_SIZE] = {0};
short adcCurrSample = 0;
short *adcCurrPtr = adcRecord;
int adcCurrentInd = 0;
bool adcSample_rdy = 0;
bool adcInitDone = 0;

//---------------------------------
//Tableau donnees coefficient mfcc
//---------------------------------
float mfccCircBuffer[SIGNAL_BLOCK_SIZE] = {0};
float mfccCurrSample = 0;
float *mfccCurrPtr = mfccCircBuffer;


//---------------------------------
//Tableau donnees coefficient mfcc
//---------------------------------
short spkerIndBuffer[INDEX_BUFFER_SIZE] = {SPEAKER_IND_UNKNOW};
short *spkerCurrPtr = spkerIndBuffer;
short spkerModeInd = SPEAKER_IND_UNKNOW;
short last_spkerModeInd = -1;

//---------------------------------
//Tableau donnees detection pitch
//---------------------------------
short pitchCircBuffer[SIGNAL_BLOCK_SIZE] = {0};
short *pitchCurrPtr = pitchCircBuffer;
long autoCorrBuffer[SIGNAL_BLOCK_SIZE] = {0};


//---------------------------------
//Param d�tection silence
//---------------------------------
short sample_acc = 0;
float beta_acc = 0;
int adcSample_acc = 0;
bool  silenceDetection_rdy = 0;
short silence_acc = 0;


//---------------------------------
//STRUCTURE
//---------------------------------
SpeakerDataList mfcc_speaker_list;
MFCCModule      mfcc;
MetVecTab       mfcc_metVecTab;
#pragma DATA_SECTION(mfcc_metVecTab,".EXT_RAM");

/*------------------------------------------------------
 *   dsk_main :
 *
 *   contain the FSM ("MEF") of the DSK and the MFCC algorithm routine
 *
 *
 ----------------------------------------------------*/

void dsk_main(void) {

    dsk_mode = DSK_SPEAKER;
    dsk_state = DSK_INIT;
    dsk_indexCurr = SPEAKER_IND_UNKNOW;
    dsk_stateCurr = dsk_state;


    // Main FSM loop
    while(1) {

        // dataIn parse section
        dsk_dataIn_parsed_flag = 1;//ensure that if there's new data (dsk_dataIn_flag raised) it has been parsed
        if (dsk_dataIn != 0xFF) {
            dsk_indIn = dsk_dataIn >> 4;
            dsk_cmdIn = dsk_dataIn & 0x0F;
        }
        // main dsk MEF
        switch(dsk_state) {


        case DSK_INIT:
            // Initialize DSK
            dsk_init();

            // Initialize MFCC structure/algo.
            mfcc_init(&mfcc, &mfcc_metVecTab, &mfcc_speaker_list);
            load_codebook_speaker();
            dsk_state = DSK_IDLE;

            break;


        case DSK_IDLE:
            DSK6713_LED_off(0);
            DSK6713_LED_off(1);
            DSK6713_LED_off(2);
            DSK6713_LED_on(3);
            if(dsk_dataIn_flag && dsk_dataIn_parsed_flag) {

                dsk_indexCurr = SPEAKER_IND_UNKNOW;


                if(dsk_cmdIn == DSK_TEST_INIT || dsk_cmdIn == DSK_TRAIN_INIT || dsk_cmdIn == DSK_CHANGE_MODE) {
                    dsk_state = dsk_cmdIn;
                    DSK6713_LED_off(3);
                    //the code will continue to check for further state/case (no break)
                }
            }
            if (!DSK6713_DIP_get(0)){
                DSK6713_LED_on(0);
                DSK6713_LED_on(1);
                mfcc_write_codebook(&mfcc_speaker_list, filename);
                DSK6713_LED_off(0);
                DSK6713_LED_off(1);
            }
            break;



        case DSK_TEST_INIT:
            if(dsk_dataIn_flag && dsk_dataIn_parsed_flag) {

                dsk_dataIn_flag = 0;// in data not new anymore
                if(dsk_cmdIn == DSK_IDLE) {
                    dsk_state = dsk_cmdIn;
                    break;
                }
                // First phase : get the number of speaker to be tested from PC
                if(mfcc_speaker_list.tested_speaker_nb == 0) {
                    if (mfcc_speaker_list.speaker_nb >= dsk_indIn) {

                        mfcc_speaker_list.tested_speaker_nb = dsk_indIn;
                        mfcc_speaker_list.trained_speaker_ind = 0;//use as a temporary index
                    }
                    else
                        dsk_state = DSK_ERROR_INSUFFICENT_TRAINED_SPEAKER;
                }
                // Second phase : get the index of all speaker to be tested from PC
                else if (mfcc_speaker_list.trained_speaker_ind < mfcc_speaker_list.tested_speaker_nb ) {

                    if (mfcc_speaker_list.speaker_data[dsk_indIn].codebook.codeword_nb != 0) {
                        mfcc_speaker_list.tested_speaker_ind[mfcc_speaker_list.trained_speaker_ind] = dsk_indIn;
                        mfcc_speaker_list.trained_speaker_ind++;
                    }
                    else {
                        mfcc_speaker_list.tested_speaker_nb = 0;
                        dsk_state = DSK_ERROR_SELECTED_SPEAKER_NOT_TRAINED;
                        break;
                    }

                    if (mfcc_speaker_list.trained_speaker_ind == mfcc_speaker_list.tested_speaker_nb) {
                        //DSK6713_waitusec(1000000);
                        dsk_state = DSK_TEST_ACQUISITION;
                    }
                }
            }
            break;


        case DSK_TRAIN_INIT:

            dsk_dataIn_flag = 0;// in data not new anymore

            if(dsk_dataIn_parsed_flag) {
                //dsk_dataIn_flag = 0;// in data not new anymore

                DSK6713_waitusec(1000000);

                dsk_indexCurr = dsk_indIn;
                mfcc_speaker_list.trained_speaker_ind = dsk_indIn;
                dsk_state = DSK_TRAIN_ACQUISITION;
            }
            break;


        case DSK_TRAIN_ACQUISITION:

            DSK6713_LED_on(0);
            if (adcSample_rdy == 1) {
                adcSample_rdy = 0;
                dsk_indexCurr = dsk_indIn;
                mfcc_main(&dsk_state, SILENCE_THRESHOLD);
            }

            if (dsk_cmdIn == DSK_IDLE)
                dsk_state = DSK_TRAIN_CODEBOOK_CONSTRUCTION;

            break;

        case DSK_TEST_ACQUISITION:

            if (adcSample_rdy == 1) {
                adcSample_rdy = 0;
                mfcc_main(&dsk_state, SILENCE_THRESHOLD);
            }

            if (dsk_cmdIn == DSK_IDLE) {

                mfcc_speaker_list.tested_speaker_nb = 0;
                dsk_state = DSK_IDLE;
            }

            break;



        case DSK_TRAIN_CODEBOOK_CONSTRUCTION:

            DSK6713_LED_off(0);
            DSK6713_LED_on(1);
            cb_construct_codebook(&mfcc_metVecTab,
                                  &mfcc_speaker_list.speaker_data[mfcc_speaker_list.trained_speaker_ind].codebook,
                                  CODEBOOK_CODEWORDS_NB,
                                  mfcc_speaker_list.trained_speaker_ind , 0.001, 0.005);
            DSK6713_LED_off(1);
            mfcc_speaker_list.speaker_nb++;
            mfcc_metVecTab.metVecTab_size = 0;//reset metVecTab to 0
            dsk_state = DSK_IDLE;
            dsk_cmdIn = DSK_IDLE;
            dsk_dataIn = 0xFF;
            break;


        case DSK_CHANGE_MODE:

            switch(dsk_indIn){

            case DSK_SPEAKER:

                index_mode_size = INDEX_MODE_SIZE_SPEAKER;
                index_accumulator_nb = INDEX_ACCUMULATOR_THRESHOLD_SPEAKER;
                load_codebook_speaker();

                break;
            case DSK_ANIMAL:

                index_mode_size = INDEX_MODE_SIZE_ANIMAL;
                index_accumulator_nb = INDEX_ACCUMULATOR_THRESHOLD_ANIMAL;
                load_codebook_animal();

                break;
            case DSK_PHONEME:

                index_mode_size = INDEX_MODE_SIZE_PHONEME;
                index_accumulator_nb = INDEX_ACCUMULATOR_THRESHOLD_PHONEME;
                load_codebook_phoneme();

                break;

            }
            dsk_state = DSK_IDLE;
            dsk_cmdIn = DSK_IDLE;
            dsk_dataIn = 0xFF;
            break;



        case DSK_ERROR_INSUFFICENT_TRAINED_SPEAKER:
        case DSK_ERROR_SELECTED_SPEAKER_NOT_TRAINED:
            //make sure to transmit the error state to the PC
            dsk_indexCurr = SPEAKER_IND_UNKNOW;
            DSK6713_LED_on(0);
            DSK6713_LED_on(1);
            DSK6713_LED_on(2);
            DSK6713_LED_on(3);

            if(dsk_dataIn_flag && dsk_dataIn_parsed_flag && dsk_cmdIn == DSK_IDLE)
                dsk_state = dsk_cmdIn;
            break;

        }

        dsk_stateCurr = dsk_state;
    }
}




void load_codebook_speaker() {

    if (LOAD_CODEBOOK_DATA == 0)
        return;

    mfcc_read_codebook(&mfcc_speaker_list.speaker_data[0].codebook, codebook0_speaker);
    mfcc_read_codebook(&mfcc_speaker_list.speaker_data[1].codebook, codebook1_speaker);
    mfcc_read_codebook(&mfcc_speaker_list.speaker_data[2].codebook, codebook2_speaker);
    mfcc_read_codebook(&mfcc_speaker_list.speaker_data[3].codebook, codebook3_speaker);
    mfcc_read_codebook(&mfcc_speaker_list.speaker_data[4].codebook, codebook4_speaker);
    mfcc_read_codebook(&mfcc_speaker_list.speaker_data[5].codebook, codebook5_speaker);
    mfcc_read_codebook(&mfcc_speaker_list.speaker_data[6].codebook, codebook6_speaker);
    mfcc_read_codebook(&mfcc_speaker_list.speaker_data[7].codebook, codebook7_speaker);
    mfcc_read_codebook(&mfcc_speaker_list.speaker_data[8].codebook, codebook8_speaker);
    mfcc_read_codebook(&mfcc_speaker_list.speaker_data[9].codebook, codebook9_speaker);
    mfcc_read_codebook(&mfcc_speaker_list.speaker_data[10].codebook, codebookA_speaker);
    mfcc_read_codebook(&mfcc_speaker_list.speaker_data[11].codebook, codebookB_speaker);
    mfcc_read_codebook(&mfcc_speaker_list.speaker_data[12].codebook, codebookC_speaker);
    mfcc_read_codebook(&mfcc_speaker_list.speaker_data[13].codebook, codebookD_speaker);
    mfcc_speaker_list.speaker_nb = 14;
}

void load_codebook_animal() {

    if (LOAD_CODEBOOK_DATA == 0)
        return;

    mfcc_read_codebook(&mfcc_speaker_list.speaker_data[0].codebook, codebook0_animal);
    mfcc_read_codebook(&mfcc_speaker_list.speaker_data[1].codebook, codebook1_animal);
    mfcc_read_codebook(&mfcc_speaker_list.speaker_data[2].codebook, codebook2_animal);
    mfcc_read_codebook(&mfcc_speaker_list.speaker_data[3].codebook, codebook3_animal);
    mfcc_read_codebook(&mfcc_speaker_list.speaker_data[4].codebook, codebook4_animal);
    mfcc_read_codebook(&mfcc_speaker_list.speaker_data[5].codebook, codebook5_animal);
    mfcc_read_codebook(&mfcc_speaker_list.speaker_data[6].codebook, codebook6_animal);
    mfcc_read_codebook(&mfcc_speaker_list.speaker_data[7].codebook, codebook7_animal);
    mfcc_read_codebook(&mfcc_speaker_list.speaker_data[8].codebook, codebook8_animal);
    mfcc_read_codebook(&mfcc_speaker_list.speaker_data[9].codebook, codebook9_animal);
    mfcc_read_codebook(&mfcc_speaker_list.speaker_data[10].codebook, codebookA_animal);
    mfcc_read_codebook(&mfcc_speaker_list.speaker_data[11].codebook, codebookB_animal);
    mfcc_read_codebook(&mfcc_speaker_list.speaker_data[12].codebook, codebookC_animal);
    mfcc_read_codebook(&mfcc_speaker_list.speaker_data[13].codebook, codebookD_animal);
    mfcc_speaker_list.speaker_nb = 14;
}

void load_codebook_phoneme() {

    if (LOAD_CODEBOOK_DATA == 0)
        return;

    mfcc_read_codebook(&mfcc_speaker_list.speaker_data[0].codebook, codebook0_phoneme);
    mfcc_read_codebook(&mfcc_speaker_list.speaker_data[1].codebook, codebook1_phoneme);
    mfcc_read_codebook(&mfcc_speaker_list.speaker_data[2].codebook, codebook2_phoneme);
    mfcc_read_codebook(&mfcc_speaker_list.speaker_data[3].codebook, codebook3_phoneme);
    mfcc_read_codebook(&mfcc_speaker_list.speaker_data[4].codebook, codebook4_phoneme);
    mfcc_read_codebook(&mfcc_speaker_list.speaker_data[5].codebook, codebook5_phoneme);
    mfcc_read_codebook(&mfcc_speaker_list.speaker_data[6].codebook, codebook6_phoneme);
    mfcc_read_codebook(&mfcc_speaker_list.speaker_data[7].codebook, codebook7_phoneme);
    mfcc_read_codebook(&mfcc_speaker_list.speaker_data[8].codebook, codebook8_phoneme);
    mfcc_read_codebook(&mfcc_speaker_list.speaker_data[9].codebook, codebook9_phoneme);
    mfcc_read_codebook(&mfcc_speaker_list.speaker_data[10].codebook, codebookA_phoneme);
    mfcc_read_codebook(&mfcc_speaker_list.speaker_data[11].codebook, codebookB_phoneme);
    mfcc_read_codebook(&mfcc_speaker_list.speaker_data[12].codebook, codebookC_phoneme);
    mfcc_read_codebook(&mfcc_speaker_list.speaker_data[13].codebook, codebookD_phoneme);
    mfcc_speaker_list.speaker_nb = 14;

}


void mfcc_main(char *state, float silence_threshold) {

    int i;
    bool mfcc_rdy = 0; // vaut 1 lorsque "SIGNAL_BLOCK_OVERLAP" nouvelles donn�es sont ajout� au buffer adcRecord
    float sound_amplitude = 0;

    //------------------------------------
    //  D�tection de silence
    //------------------------------------

    acc_interval(abs((float)adcCurrSample), &beta_acc);

    //printf("ADC: %d\n", adcCurrSample);

    if (silenceDetection_rdy == 1) {

        sound_amplitude = moving_average(&beta_acc, SIGNAL_BLOCK_SIZE, SIGNAL_BLOCK_OVERLAP);
        silenceDetection_rdy = 0;
        if (sound_amplitude > silence_threshold && adcInitDone == 1) {
            mfcc_rdy = 1;
            silence_acc = 0;
        }
        else {
            //dsk_indexCurr = SPEAKER_IND_UNKNOW;
            silence_acc += 1;
        }
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
        mfcc_set_x(&mfcc, mfccCircBuffer, mfccCurrPtr);
        //get metric vectors
        mfcc_get_metrics(met_curr.met, &mfcc);

    }


    //------------------------------------
    //  Extraction PITCH
    //------------------------------------

    //routine pitch, duplication de buffer circulaire (filtre FIR d'ordre 20)
    static short bufferFIR20[21] = {0};
    static short bufferPreAutoCorr[SIGNAL_BLOCK_SIZE] = {0};
    static long bufferPostAutoCor[SIGNAL_BLOCK_SIZE] = {0};

    cpy_circTab_int16_forward(bufferFIR20, adcRecord, adcCurrPtr, SIGNAL_BLOCK_SIZE, 21);

    //ajout valeur filtr� ua tableau circulaire
    *pitchCurrPtr = filterFIR_sym_OrdN(20, coeffFIR300HzN20, bufferFIR20 + 20);
    pitchCurrPtr++;
    if (pitchCurrPtr >= pitchCircBuffer + SIGNAL_BLOCK_SIZE)
        pitchCurrPtr = pitchCircBuffer;

    if (mfcc_rdy == 1) {

        //extraction du pitch avec autocorr
        cpy_circTab_int16_forward(bufferPreAutoCorr, pitchCircBuffer, pitchCurrPtr, SIGNAL_BLOCK_SIZE, SIGNAL_BLOCK_SIZE);

        autoCorrASM(bufferPreAutoCorr, bufferPostAutoCor);

        met_curr.met[0] = (autoCorr_getpitch(bufferPostAutoCor, SAMPLE_RATE, SIGNAL_BLOCK_SIZE) - 150) / 25;

    }
    else {
        if (silence_acc >= (int)(SPEAKER_TIMEOUT * SAMPLE_RATE / SIGNAL_BLOCK_OVERLAP)) {
            dsk_indexCurr = SPEAKER_IND_UNKNOW;
            for(i = 0; i < INDEX_BUFFER_SIZE; i++) {
                spkerIndBuffer[i] = SPEAKER_IND_UNKNOW;
            }
        }

        return;
    }




    switch(*state) {
    case DSK_TRAIN_ACQUISITION:
    //------------------------------------
    //  TRAIN
    //------------------------------------
        if (!mfcc_add_metVec(met_curr.met, &mfcc)) {

            *state = DSK_TRAIN_CODEBOOK_CONSTRUCTION;

            ////DEBUG SEULEMENT
            //DSK6713_LED_on(0);
            //mfcc_write_metVecTab(&mfcc);
            //DSK6713_LED_off(0);

            return;
        }



        break;


    case DSK_TEST_ACQUISITION:
    //------------------------------------
    //  TEST
    //------------------------------------

        //ajout de l'index extrait et d�tect� dans le buffer circulaire d'index


        *spkerCurrPtr = spkrec_get_speakerInd(met_curr.met, &mfcc_speaker_list);
        spkerCurrPtr++;
        if (spkerCurrPtr >= spkerIndBuffer + INDEX_BUFFER_SIZE)
            spkerCurrPtr =  spkerIndBuffer;

        //index extracted from mode
        spkerModeInd = spkrec_get_modeSpeakerInd(spkerIndBuffer, spkerCurrPtr - 1, index_mode_size, INDEX_BUFFER_SIZE);

        //index extracted from accumulator
        dsk_indexCurr = spkrec_get_thresholdSpeakerInd(spkerModeInd, &last_spkerModeInd, dsk_indexCurr, index_accumulator_nb);


        break;
    }
}







interrupt void c_int11(void){

    adcSample_rdy = 1;

    if(adcCurrentInd == SIGNAL_BLOCK_SIZE) {               //Index tableau
        adcCurrentInd = 0;
        adcInitDone = 1;
    }
    adcRecord[adcCurrentInd] = input_left_sample();    //Tableau de donn�es brutes
    adcCurrSample = adcRecord[adcCurrentInd];
    adcCurrPtr = adcRecord + adcCurrentInd;
    //utilis� pour la d�tection de silence
    adcSample_acc++;
    if (adcSample_acc >= SIGNAL_BLOCK_OVERLAP) {
        silenceDetection_rdy = 1;
        adcSample_acc = 0;
    }

    output_left_sample(adcRecord[adcCurrentInd]);              //Obligatoire pour permettre de r�activer l'Interupt
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
    configAndStartTimer0(2812500);
    init_ext_intr();

    DSK6713_waitusec(1000000);

}
