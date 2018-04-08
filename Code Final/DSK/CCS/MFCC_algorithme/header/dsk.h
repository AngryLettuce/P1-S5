/*
 * dsk_main.h
 *
 *  Created on: 9 mars 2018
 *  Last edited on: 2 avril 2018
 *      Authors: Pascal L, Charles C
 */

#ifndef HEADER_DSK_H_
#define HEADER_DSK_H_

typedef enum DSKstate {

    DSK_INIT,
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

#include "data_structures.h"

void dsk_main(void);
void dsk_init(void);
void mfcc_init(MFCCModule *mfcc, MetVecTab *metVecTab);
void mfcc_get_metrics(float *met, MFCCModule *mfcc);
void mfcc_main(short state, float silence_threshold);
void fsm_delete_user(void);
void fsm_add_user(void);
void fsm_analyze_user(void);

char btn_dbnc(void);
void interrupt itr7_fsm_btn_pressed(void);
void c_int11(void);

#endif /* HEADER_DSK_H_ */
