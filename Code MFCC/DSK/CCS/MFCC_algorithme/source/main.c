/*
 * main.c
 */
#include <stdio.h>
#include "data_structures.h"
#include "functions.h"
#include "test_bench.h"


float test_bench_matrix[TEST_BENCH_MATRIX_SIZE][TEST_BENCH_MATRIX_SIZE];
#pragma DATA_SECTION(test_bench_matrix, ".EXT_RAM")
/*
//tableau circulaire
int TabDonnees[8] = {1, 2, 3, 4, 5, 6, 7, 8};
#pragma DATA_ALIGN(TabDonnees, 32)
*/


int main(void) {

    char *filename = "C:\\Users\\Guill\\Downloads\\freq2Mel_x1.csv";
    int lines, columns;
    read_csv_float(filename, test_bench_matrix, TEST_BENCH_MATRIX_SIZE, &lines, &columns);

   // float x[256], y[512];
    int i = 0;




    float fft256CoeffTab[256];
    mfcc_fft256_init(fft256CoeffTab);

    for (i = 0; i < 256; i+=2) {

        printf(" %f + %f i\n",fft256CoeffTab[i], fft256CoeffTab[i+1]);
    }

/*
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
*/

}
