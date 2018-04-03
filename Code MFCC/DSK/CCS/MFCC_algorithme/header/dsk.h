/*
 * dsk_main.h
 *
 *  Created on: 9 mars 2018
 *  Last edited on: 2 avril 2018
 *      Authors: Pascal L, Charles C
 */

#ifndef HEADER_DSK_H_
#define HEADER_DSK_H_

#define DELETE_CMD 1
#define ANALYZE_CMD 2
#define ADD_CMD 3

#include "data_structures.h"

void dsk_main(void);
void dsk_init(void);
void mfcc_init(MFCCModule *mfcc, MetVecTab *metVecTab);
void mfcc_get_metrics(float *met, MFCCModule *mfcc);
void fsm_delete_user(void);
void fsm_add_user(void);
void fsm_analyze_user(void);
char btn_dbnc(void);
void interrupt itr7_fsm_btn_pressed(void);

#endif /* HEADER_DSK_H_ */
