
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

#include "data_structures.h"
#include "test_bench.h"
#include "functions.h"
#include "fft_utility.h"

//#define TEST_BENCH_FOLDER "../../../P1-S5/Code MFCC/MATLAB/SpeakerRecognition/testBench/"
#define TEST_BENCH_FOLDER "../testBench/"


float test_bench_x[TEST_BENCH_MATRIX_SIZE][TEST_BENCH_MATRIX_SIZE];
float test_bench_y[TEST_BENCH_MATRIX_SIZE][TEST_BENCH_MATRIX_SIZE];
#pragma DATA_SECTION(test_bench_x, ".EXT_RAM")
#pragma DATA_SECTION(test_bench_y, ".EXT_RAM")

float x_complex[2*SIGNAL_BLOCK_SIZE];
float w[SIGNAL_BLOCK_SIZE];
#pragma DATA_ALIGN(x_complex, 8);
#pragma DATA_ALIGN(w, 8);



int global_testBench() {

    int success = 1;

    float threshold = 0.001;
    /*
    success *= tb_mfcc_freq2mel("freq2Mel_x1.csv", "freq2Mel_y1.csv", threshold);
    success *= tb_mfcc_mel2freq("mel2Freq_x1.csv", "mel2Freq_y1.csv", threshold);

    success *= tb_mfcc_hamming_window_256("mfcc_hammingWindow_x1.csv", "mfcc_hammingWindow_y1.csv", threshold);
    success *= tb_mfcc_hamming_window_256("mfcc_hammingWindow_x2.csv", "mfcc_hammingWindow_y2.csv", threshold);
    success *= tb_mfcc_hamming_window_256("mfcc_hammingWindow_x3.csv", "mfcc_hammingWindow_y3.csv", threshold);
    */
    success *= tb_mfcc_fft256("mfcc_fft_x1.csv", "mfcc_fft_y1.csv", threshold);
    success *= tb_mfcc_fft256("mfcc_fft_x2.csv", "mfcc_fft_y2.csv", threshold);
    success *= tb_mfcc_fft256("mfcc_fft_x3.csv", "mfcc_fft_y3.csv", threshold);


    return success;
}


int tb_mfcc_freq2mel(char *filename_x, char *filename_y, float threshold) {

    int i, lines, columns;
    float RMS = 0;
    float rErrAvg = 0;
    float y0,yr,rErr;

    if (read_csv_float(filename_x, test_bench_x, &lines, &columns) < 0)
        return 0;

    if (read_csv_float(filename_y, test_bench_y, &lines, &columns) < 0)
        return 0;

    for(i = 0; i < lines; i++) {

        y0 = mfcc_freq2mel(test_bench_x[i][0]);
        yr = test_bench_y[i][0];


        rErr = fabs((y0 - yr)/y0);
        rErrAvg += rErr;

        printf("%f %f rErr: %f %%\n", y0, yr, rErr*100);

        if (rErr > threshold) {
            printf("F - Test \"%s\" failed!, rErr = %f %% (%f %%), i = %d \n", filename_x, rErr*100, threshold*100, i);
            return 0;
        }

        RMS += pow(y0 - yr, 2);
    }

    RMS /= lines;
    rErrAvg /= lines;

    printf("S - Test \"%s\" succeed, RMS = %f, rErrAvg = %f %% (%f %%)Threshold \n", filename_x, RMS, rErrAvg*100, threshold*100);
    return 1;
}

int tb_mfcc_mel2freq(char *filename_x, char *filename_y, float threshold) {

    int i, lines, columns;
    float RMS = 0;
    float rErrAvg = 0;
    float y0,yr,rErr;

    if (read_csv_float(filename_x, test_bench_x, &lines, &columns) < 0)
        return 0;

    if (read_csv_float(filename_y, test_bench_y, &lines, &columns) < 0)
        return 0;

    for(i = 0; i < lines; i++) {

        y0 = mfcc_mel2freq(test_bench_x[i][0]);
        yr = test_bench_y[i][0];

        rErr = fabs((y0 - yr)/y0);
        rErrAvg += rErr;

        printf("%f %f rErr: %f %%\n", y0, yr, rErr*100);

        if (rErr > threshold) {
            printf("F - Test \"%s\" failed!, rErr = %f %% (%f %%), i = %d \n", filename_x, rErr*100, threshold*100, i);
            return 0;
        }

        RMS += pow(y0 - yr, 2);
    }

    RMS /= lines;
    rErrAvg /= lines;

    printf("S - Test \"%s\" succeed, RMS = %f, rErrAvg = %f %% (%f %%)Threshold \n", filename_x, RMS, rErrAvg*100, threshold*100);
    return 1;
}


int tb_mfcc_hamming_window_256(char *filename_x, char *filename_y,  float threshold) {

    int i, lines, columns;
    float RMS = 0;
    float rErrAvg = 0;
    float y0,yr,rErr;
    float y[256];

    if (read_csv_float(filename_x, test_bench_x, &lines, &columns) < 0)
        return 0;

    if (read_csv_float(filename_y, test_bench_y, &lines, &columns) < 0)
        return 0;

    for(i = 0; i < 256; i++){
        y[i] = test_bench_x[i][0];
    }
    mfcc_hamming_window_256(y);

    for(i = 0; i < lines; i++) {

        y0 = y[i];
        yr = test_bench_y[i][0];

        rErr = fabs((y0 - yr)/y0);
        rErrAvg += rErr;

        printf("%f %f rErr: %f %%\n", y0, yr, rErr*100);

        if (rErr > threshold) {
            printf("F - Test \"%s\" failed!, rErr = %f %% (%f %%), i = %d \n", filename_x, rErr*100, threshold*100, i);
            return 0;
        }

        RMS += pow(y0 - yr, 2);
    }

    RMS /= lines;
    rErrAvg /= lines;

    printf("S - Test \"%s\" succeed, RMS = %f, rErrAvg = %f %% (%f %%)Threshold \n", filename_x, RMS, rErrAvg*100, threshold*100);
    return 1;
}

int tb_mfcc_fft256(char *filename_x, char *filename_y,  float threshold) {
    int i, lines, columns;
    float RMS = 0;
    float rErrAvg = 0;
    float Ry0, Iy0, Ryr, Iyr, P0, Pr, rErr;

    float x[256];
    short index[256];


    if (read_csv_float(filename_x, test_bench_x, &lines, &columns) < 0)
        return 0;

    if (read_csv_float(filename_y, test_bench_y, &lines, &columns) < 0)
        return 0;

    for(i = 0; i < 256; i++){
        x[i] = test_bench_x[i][0];
    }
    float2complex(x, x_complex, 256);

    mfcc_fft_init(w, index, 256);
    mfcc_fft(x_complex, w, index, 256);


    for(i = 0; i < lines; i++) {

        Ry0 = x_complex[i*2];
        Iy0 = x_complex[i*2+1];

        Ryr = test_bench_y[i][0];
        Iyr = test_bench_y[i][1];

        P0 = Ry0*Ry0 + Iy0*Iy0;
        Pr = Ryr*Ryr + Iyr*Iyr;

        rErr = fabs((P0 - Pr)/(P0 + 0.0001));
        rErrAvg += rErr;

        printf("%f + %f j, %f + %f j, %f %% (%f %%) \n", Ry0, Iy0, Ryr, Iyr, 100*rErr, 200*threshold);

        if (rErr > 2*threshold) {
            printf("F - Test \"%s\" failed!, rErr = %f %% (%f %%), i = %d \n", filename_x, rErr*100, threshold*200, i);
            return 0;
        }

        RMS += pow(Ry0 - Ryr, 2) + pow(Iy0 - Iyr, 2);
    }


    mfcc_powerSpectrum(x, x_complex, 256);


    RMS /= lines;
    rErrAvg /= lines;

    printf("S - Test \"%s\" succeed, RMS = %f, rErrAvg = %f %% (%f %%)Threshold \n", filename_x, RMS, rErrAvg*100, threshold*100);
    return 1;
}















int read_csv_float(char *filename, float (*test_bench_matrix)[TEST_BENCH_MATRIX_SIZE], int *lines, int *columns){
    FILE *fp;

    char dir[] = TEST_BENCH_FOLDER;
    char scr[100];

    strcpy(scr, filename);
    strcat(dir, scr);

    fp = fopen(dir, "r");

    float value;

    if (fp == NULL)
    {
        printf("Could not open file \"%s\" \n", filename);
        return -1;
    }

    fscanf(fp, "%d,%d", lines, columns);

    int i, j;

    for (i = 0; i < *lines; i++)
    {
        for (j = 0; j < *columns; j++)

        {
            fscanf(fp, "%f,", &value);
            test_bench_matrix[i][j]= value;
        }
    }

    fclose(fp);
    return 1;
}
