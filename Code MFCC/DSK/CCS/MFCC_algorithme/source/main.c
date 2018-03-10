/*
 * main.c
 */
#include <stdio.h>
#include "data_structures.h"
#include "mfcc.h"
#include "test_bench.h"





/*
//tableau circulaire
int TabDonnees[8] = {1, 2, 3, 4, 5, 6, 7, 8};
#pragma DATA_ALIGN(TabDonnees, 32)
*/







/*
void main(void) {

   int j = 0;

   short *ind = circTab + 4;
   short newTab[256];

   while(1){
        short input = (*ind) + 1; //read new input sample from ADC
        ind = asm_storeCircularTab256(ind, input);

        j++;
        if (j >= LENGHT){
            j = 0;
            cpy_circTab_int16(newTab, circTab, ind, 256);
        }
    }
}
*/

//interrupt void c_int11(void){

//}




int main(void) {
    global_testBench(0.005);

    return 1;
}



/*

int main123(void) {



   // float x[256], y[512];
    int i = 0;




    float fft256CoeffTab[256];
    mfcc_fft256_init(fft256CoeffTab);

    for (i = 0; i < 256; i+=2) {

        printf(" %f + %f i\n",fft256CoeffTab[i], fft256CoeffTab[i+1]);
    }


    while(1){

        for(i = 0; i < 256; i++) {
            x[i] = i;
        }

        mfcc_hamming_window_256(x);
        printf("chat");
        mfcc_hamming_window_256_opt(x);
        printf("chat");

        for(i = 0; i < 512; i++) {
            if (i % 2 == 0)
                y[i] = x[i/2];
            else
                y[i] = 0;
        }

        DSPF_sp_cfftr4_dif(y,y,256);
        printf("chat");
    }

    return 1;
}
*/
