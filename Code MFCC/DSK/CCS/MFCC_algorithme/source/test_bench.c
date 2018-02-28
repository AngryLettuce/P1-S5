
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


int global_testBench(float (*test_bench_x)[TEST_BENCH_MATRIX_SIZE], float (*test_bench_y)[TEST_BENCH_MATRIX_SIZE]) {

    int success = 1;

    float threshold = 0.001;
    /*
    success *= tb_mfcc_freq2mel("freq2Mel_x1.csv", "freq2Mel_y1.csv", threshold, test_bench_x, test_bench_y);
    success *= tb_mfcc_mel2freq("mel2Freq_x1.csv", "mel2Freq_y1.csv", threshold, test_bench_x, test_bench_y);

    success *= tb_mfcc_hamming_window_256("mfcc_hammingWindow_x1.csv", "mfcc_hammingWindow_y1.csv", threshold, test_bench_x, test_bench_y);
    success *= tb_mfcc_hamming_window_256("mfcc_hammingWindow_x2.csv", "mfcc_hammingWindow_y2.csv", threshold, test_bench_x, test_bench_y);
    success *= tb_mfcc_hamming_window_256("mfcc_hammingWindow_x3.csv", "mfcc_hammingWindow_y3.csv", threshold, test_bench_x, test_bench_y);
    */
    success *= tb_mfcc_fft256("mfcc_fft_x1.csv", "mfcc_fft_y1.csv", threshold, test_bench_x, test_bench_y);
    success *= tb_mfcc_fft256("mfcc_fft_x2.csv", "mfcc_fft_y2.csv", threshold, test_bench_x, test_bench_y);
    success *= tb_mfcc_fft256("mfcc_fft_x3.csv", "mfcc_fft_y3.csv", threshold, test_bench_x, test_bench_y);


    return success;
}


int tb_mfcc_freq2mel(char *filename_x, char *filename_y, float threshold,
                float (*test_bench_x)[TEST_BENCH_MATRIX_SIZE],
                float (*test_bench_y)[TEST_BENCH_MATRIX_SIZE]) {

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

int tb_mfcc_mel2freq(char *filename_x, char *filename_y, float threshold,
                float (*test_bench_x)[TEST_BENCH_MATRIX_SIZE],
                float (*test_bench_y)[TEST_BENCH_MATRIX_SIZE]) {

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


int tb_mfcc_hamming_window_256(char *filename_x, char *filename_y,  float threshold,
                               float (*test_bench_x)[TEST_BENCH_MATRIX_SIZE],
                               float (*test_bench_y)[TEST_BENCH_MATRIX_SIZE]) {

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

int tb_mfcc_fft256(char *filename_x, char *filename_y,  float threshold,
                   float (*test_bench_x)[TEST_BENCH_MATRIX_SIZE],
                   float (*test_bench_y)[TEST_BENCH_MATRIX_SIZE]) {
    int i, lines, columns;
    float RMS = 0;
    float rErrAvg = 0;
    float Ry0, Iy0, Ryr, Iyr, rErr;
    float y[256], y_complex[256];
    float fft256CoeffTab[256];

    if (read_csv_float(filename_x, test_bench_x, &lines, &columns) < 0)
        return 0;

    if (read_csv_float(filename_y, test_bench_y, &lines, &columns) < 0)
        return 0;

    for(i = 0; i < 256; i++){
        y[i] = test_bench_x[i][0];
    }

    mfcc_fft256_init(fft256CoeffTab);
    float2complex(y, y_complex, 256);
    mfcc_fft256(y_complex, fft256CoeffTab);

    for(i = 0; i < lines; i+=2) {

        Ry0 = y_complex[i];
        Iy0 = y_complex[i+1];

        Ryr = test_bench_y[i][0];
        Iyr = test_bench_y[i][1];

        //rErr = fabs((Ry0 - Ryr)/Ry0) + fabs((Iy0 - Iyr)/Iy0);
        //rErrAvg += rErr;

        printf("%lf + %lf, %f + %f \n", Ry0, Iy0, Ryr, Ryr);

        if (rErr > threshold) {
            printf("F - Test \"%s\" failed!, rErr = %f %% (%f %%), i = %d \n", filename_x, rErr*100, threshold*100, i);
            return 0;
        }

        RMS += pow(Ry0 - Ryr, 2) + pow(Iy0 - Iyr, 2);
    }

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
