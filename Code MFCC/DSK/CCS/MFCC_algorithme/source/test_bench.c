
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

#include "test_bench.h"
#include "functions.h"
#include "fft_utility.h"

#define TEST_BENCH_FOLDER "../../../P1-S5/Code MFCC/MATLAB/SpeakerRecognition/testBench/"


int global_testBench(float (*test_bench_x)[TEST_BENCH_MATRIX_SIZE], float (*test_bench_y)[TEST_BENCH_MATRIX_SIZE]) {

    int success = 1;

    float threshold = 0.001;
    success *= tb_freq2Mel(
                    "../../../P1-S5/Code MFCC/MATLAB/SpeakerRecognition/testBench/freq2Mel_x1.csv",
                    "../../../P1-S5/Code MFCC/MATLAB/SpeakerRecognition/testBench/freq2Mel_y1.csv",
                    threshold, test_bench_x, test_bench_y);
    success *= tb_freq2Mel(
                    "../../../P1-S5/Code MFCC/MATLAB/SpeakerRecognition/testBench/freq2Mel_x2.csv",
                    "../../../P1-S5/Code MFCC/MATLAB/SpeakerRecognition/testBench/freq2Mel_y2.csv",
                    threshold, test_bench_x, test_bench_y);
    success *= tb_freq2Mel(
                    "../../../P1-S5/Code MFCC/MATLAB/SpeakerRecognition/testBench/freq2Mel_x3.csv",
                    "../../../P1-S5/Code MFCC/MATLAB/SpeakerRecognition/testBench/freq2Mel_y3.csv",
                    threshold, test_bench_x, test_bench_y);

    return success;
}


int tb_freq2Mel(const char *filename_x, const char *filename_y, float threshold,
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

        rErr = abs((y0 - yr)/y0);
        rErrAvg += rErr;

        if (rErr > threshold) {
            printf("F - Test \"%s\" failed!, rErr = %f (%f), i = %d \n", filename_x, rErr, threshold, i);
            return 0;
        }

        RMS += pow(y0 - yr, 2);
    }

    RMS /= lines;
    rErr /= lines;

    printf("S - Test \"%s\" succeed, RMS = %f, rErrAvg = %f (%f)Threshold \n", filename_x, RMS, rErrAvg, threshold);
    return 1;
}



int read_csv_float(const char *filename, float (*test_bench_matrix)[TEST_BENCH_MATRIX_SIZE], int *lines, int *columns){
    FILE *fp;

    fp = fopen(filename, "r");

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
<<<<<<< HEAD
=======

>>>>>>> 09bf0997b8e77d8f7b82e49d8a04e64a5ba97595
    return 1;
}
