/*
 * dsk_main.h
 *
 *  Created on: 9 mars 2018
 *      Author: pascal
 */

#ifndef HEADER_DSK_H_
#define HEADER_DSK_H_

#include "data_structures.h"


void dsk_main(void);
void dsk_init(void);
void mfcc_init(MFCCModule *mfcc);
void mfcc_get_metrics(float *met, MFCCModule *mfcc);


#endif /* HEADER_DSK_H_ */
