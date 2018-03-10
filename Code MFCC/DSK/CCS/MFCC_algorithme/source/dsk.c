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

#include "dsk.h"
#include "data_structures.h"
#include "mfcc.h"
#include "utils.h"
#include "fft_utility.h"

static MFCCModule mfcc;



/*------------------------------------------------------*/
/*
 *   dsk_main :
 *
 *   contain the FSM ("MEF") of the DSK and the MFCC algorithm routine
 *
 *
 */
/*------------------------------------------------------*/

void dsk_main(void) {













}





void dsk_init(void) {


}

void mfcc_init(MFCCModule *mfcc) {

    mfcc->x_size = SIGNAL_BLOCK_SIZE;
    mfcc->mfcc_nb = MFCC_COEFFICIENT_NB;

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






