
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <time.h>



#include "data_structures.h"
#include "test_bench.h"
#include "functions.h"
#include "fft_utility.h"

//#define TEST_BENCH_FOLDER "../../../P1-S5/Code MFCC/MATLAB/SpeakerRecognition/testBench/"
//#define TEST_BENCH_FOLDER "../testBench/"
//#define TEST_BENCH_LOG_FOLDER "../../../P1-S5/Code MFCC/DSK/CCS/MFCC_algorithme/test_bench_log/"
#define TEST_BENCH_FOLDER "C:/Users/Guill/Documents/GitHub/P1-S5/Code MFCC/MATLAB/SpeakerRecognition/testBench/"
#define TEST_BENCH_LOG_FOLDER  "C:/Users/Guill/Documents/GitHub/P1-S5/Code MFCC/DSK/CCS/MFCC_algorithme/test_bench_log/"

float test_bench_x[TEST_BENCH_MATRIX_SIZE][TEST_BENCH_MATRIX_SIZE];
float test_bench_y[TEST_BENCH_MATRIX_SIZE][TEST_BENCH_MATRIX_SIZE];
#pragma DATA_SECTION(test_bench_x, ".EXT_RAM")
#pragma DATA_SECTION(test_bench_y, ".EXT_RAM")

float x_complex[2*SIGNAL_BLOCK_SIZE];
float w[SIGNAL_BLOCK_SIZE];
#pragma DATA_ALIGN(x_complex, 8);
#pragma DATA_ALIGN(w, 8);



//utils local function for result writing and display
void write_test_header(FILE *fp, char *name, char *filename_x, char *filename_y, int lines, int columns) {
    fprintf(fp,"------------------------------------------------\n");
    fprintf(fp,"         %s         \n", name);
    fprintf(fp,"------------------------------------------------\n\n");
    fprintf(fp,"    Source file : %s ,  %s\n", filename_x, filename_y);
    fprintf(fp,"    data : %d x %d\n\n", lines, columns);
}

void write_test_result(FILE *fp, char * filename, int success, float RMS, float rErrAvg, float threshold) {
    if (!success) {
        printf(     "\nF - Test \"%s\" failed!, RMS = %f, rErrAvg = %f %% (%f %%)Threshold \n",   filename, RMS, rErrAvg*100, threshold*100);
        fprintf(fp, "\nF - Test \"%s\" failed!, RMS = %f, rErrAvg = %f %% (%f %%)Threshold \n\n", filename, RMS, rErrAvg*100, threshold*100);
    } else {
        printf(     "\nS - Test \"%s\" succeed, RMS = %f, rErrAvg = %f %% (%f %%)Threshold \n",   filename, RMS, rErrAvg*100, threshold*100);
        fprintf(fp, "\nS - Test \"%s\" succeed, RMS = %f, rErrAvg = %f %% (%f %%)Threshold \n\n", filename, RMS, rErrAvg*100, threshold*100);
    }
}

void write_test_subresult_float(FILE *fp, float y0, float yr, float rErr, float threshold, int i, int *success) {
    if (rErr > threshold) {
        fprintf(fp, "%3d :    %6.6f   %6.6f   rErr: %6f %%(%6f %%) <---------- Failed\n", i, y0, yr, rErr*100, threshold*100);
        *success = 0;
    }
    else
        fprintf(fp,"%3d :    %6.6f   %6.6f   rErr: %6f %%(%6f %%)\n", i, y0, yr, rErr*100, threshold*100);
}

void write_test_subresult_complex(FILE *fp, float Ry0, float Iy0, float Ryr, float Iyr, float rErr, float threshold, int i, int *success) {
    fprintf(fp,"%5f  +  %5f j,    %5f   +   %5f j,  rErr: %5f %% (%5f %%) \n", Ry0, Iy0, Ryr, Iyr, 100*rErr, 200*threshold);

    if (rErr > threshold) {
        fprintf(fp,"%5f  +  %5f j,    %5f   +   %5f j,  rErr: %5f %% (%5f %%) <---------- Failed\n", Ry0, Iy0, Ryr, Iyr, 100*rErr, 200*threshold);
        success = 0;
    }
}

// TEST BENCH MAIN FUNCTION

int global_testBench(float g_threshold) {

    //get current date and time
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);


    char filename[200];
    char dir[200] = TEST_BENCH_LOG_FOLDER;

    sprintf(filename, "TB_%d-%02d-%02d_%02dh%02dm%02ds.txt", tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec);

    strcat(dir, filename);

    FILE *fp;
    fp = fopen(dir,"a");
    fprintf(fp,"\n");
    fprintf(fp, "   TB_%d-%02d-%02d_%02dh%02dm%02ds.txt\n", tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec);
    fprintf(fp,"\n");
    fclose(fp);

    int success = 1;

    float threshold = g_threshold;

    //start of unit tests

    success *= tb_mfcc_freq2mel("freq2Mel_x1.csv", "freq2Mel_y1.csv", dir, threshold);
    success *= tb_mfcc_mel2freq("mel2Freq_x1.csv", "mel2Freq_y1.csv", dir, threshold);

    success *= tb_moving_average("moving_average_x1.csv", "moving_average_y1.csv", dir, threshold);

    success *= tb_mfcc_hamming_window_256("mfcc_hammingWindow_x1.csv", "mfcc_hammingWindow_y1.csv", dir, threshold);
    success *= tb_mfcc_hamming_window_256("mfcc_hammingWindow_x2.csv", "mfcc_hammingWindow_y2.csv", dir, threshold);
    success *= tb_mfcc_hamming_window_256("mfcc_hammingWindow_x3.csv", "mfcc_hammingWindow_y3.csv", dir, threshold);

    success *= tb_mfcc_fft256("mfcc_fft_x1.csv", "mfcc_fft_y1.csv", dir, threshold);
    success *= tb_mfcc_fft256("mfcc_fft_x2.csv", "mfcc_fft_y2.csv", dir, threshold);
    success *= tb_mfcc_fft256("mfcc_fft_x3.csv", "mfcc_fft_y3.csv", dir, threshold);


    return success;
}


int tb_mfcc_freq2mel(char *filename_x, char *filename_y, char *logfile, float threshold) {


    FILE* fp;

    int i, lines, columns;
    int success = 1;
    float RMS = 0;
    float rErrAvg = 0;
    float y0,yr,rErr;

    if (read_csv_float(filename_x, test_bench_x, &lines, &columns) < 0)
        return 0;

    if (read_csv_float(filename_y, test_bench_y, &lines, &columns) < 0)
        return 0;

    //open log file to write test result to it
    fp = fopen(logfile, "a");
    //write the header of the test in the log file
    write_test_header(fp, "Frequency to Mel", filename_x, filename_y, lines, columns);

    for(i = 0; i < lines; i++) {
        //output calculation
        y0 = mfcc_freq2mel(test_bench_x[i][0]);
        yr = test_bench_y[i][0];

        //relative error calculation
        rErr = fabs((y0 - yr)/y0);
        rErrAvg += rErr;

        //write subresult to log file
        write_test_subresult_float(fp, y0, yr, rErr, threshold, i, &success);

        RMS += pow(y0 - yr, 2);
    }
    RMS /= lines;
    rErrAvg /= lines;

    //write result to file and command windows
    write_test_result(fp, filename_x, success, RMS, rErrAvg, threshold);

    fclose(fp);
    return success;
}

int tb_mfcc_mel2freq(char *filename_x, char *filename_y, char *logfile, float threshold) {

    FILE* fp;

    int i, lines, columns;
    int success = 1;
    float RMS = 0;
    float rErrAvg = 0;
    float y0,yr,rErr;

    if (read_csv_float(filename_x, test_bench_x, &lines, &columns) < 0)
        return 0;

    if (read_csv_float(filename_y, test_bench_y, &lines, &columns) < 0)
        return 0;

    //open log file to write test result to it
    fp = fopen(logfile, "a");
    //write the header of the test in the log file
    write_test_header(fp, "Mel to Frequency", filename_x, filename_y, lines, columns);

    for(i = 0; i < lines; i++) {

        y0 = mfcc_mel2freq(test_bench_x[i][0]);
        yr = test_bench_y[i][0];

        //relative error calculation
        rErr = fabs((y0 - yr)/y0);
        rErrAvg += rErr;

        //write subresult to log file
        write_test_subresult_float(fp, y0, yr, rErr, threshold, i, &success);

        RMS += pow(y0 - yr, 2);
    }
    RMS /= lines;
    rErrAvg /= lines;

    //write result to file and command windows
    write_test_result(fp, filename_x, success, RMS, rErrAvg, threshold);

    fclose(fp);
    return success;
}


int tb_mfcc_hamming_window_256(char *filename_x, char *filename_y, char *logfile, float threshold) {

    FILE* fp;

    int i, lines, columns;
    int success = 1;
    float RMS = 0;
    float rErrAvg = 0;
    float y0,yr,rErr;
    float y[256];

    if (read_csv_float(filename_x, test_bench_x, &lines, &columns) < 0)
        return 0;

    if (read_csv_float(filename_y, test_bench_y, &lines, &columns) < 0)
        return 0;

    //open log file to write test result to it
    fp = fopen(logfile, "a");
    //write the header of the test in the log file
    write_test_header(fp, "Hamming Window 256", filename_x, filename_y, lines, columns);

    for(i = 0; i < 256; i++){
        y[i] = test_bench_x[i][0];
    }
    mfcc_hamming_window_256(y);

    for(i = 0; i < lines; i++) {

        y0 = y[i];
        yr = test_bench_y[i][0];

        //relative error calculation
        rErr = fabs((y0 - yr)/y0);
        rErrAvg += rErr;

        //write subresult to log file
        write_test_subresult_float(fp, y0, yr, rErr, threshold, i, &success);

        RMS += pow(y0 - yr, 2);
    }
    RMS /= lines;
    rErrAvg /= lines;

    //write result to file and command windows
    write_test_result(fp, filename_x, success, RMS, rErrAvg, threshold);

    fclose(fp);
    return success;
}

int tb_mfcc_fft256(char *filename_x, char *filename_y, char *logfile, float threshold) {

    FILE* fp;

    int i, lines, columns;
    int success = 1;
    float RMS = 0;
    float rErrAvg = 0;
    float Ry0, Iy0, Ryr, Iyr, P0, Pr, rErr;

    float x[256];
    short index[256];


    if (read_csv_float(filename_x, test_bench_x, &lines, &columns) < 0)
        return 0;

    if (read_csv_float(filename_y, test_bench_y, &lines, &columns) < 0)
        return 0;

    //open log file to write test result to it
    fp = fopen(logfile, "a");
    //write the header of the test in the log file
    write_test_header(fp, "Fast Fourier Transform (FFT)", filename_x, filename_y, lines, columns);

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

        write_test_subresult_complex(fp, Ry0, Iy0, Ryr, Iyr, rErr, threshold, i, &success);

        RMS += pow(Ry0 - Ryr, 2) + pow(Iy0 - Iyr, 2);
    }

    RMS /= lines;
    rErrAvg /= lines;

    //write result to file and command windows
    write_test_result(fp, filename_x, success, RMS, rErrAvg, threshold);

    fclose(fp);
    return success;
}



int tb_moving_average(char *filename_x, char *filename_y, char *logfile, float threshold) {

    FILE* fp;

    int i, j, lines_x, lines_y, columns_x, columns_y;
    int success = 1;
    float RMS = 0;
    float rErrAvg = 0;
    float y0,yr,rErr;
    float y[25];
    float *beta_acc = 0;
    int size = 256;
    int acc_size = 100;
    int k = 0;

    if (read_csv_float(filename_x, test_bench_x, &lines_x, &columns_x) < 0)
        return 0;

    if (read_csv_float(filename_y, test_bench_y, &lines_y, &columns_y) < 0)
        return 0;

    //open log file to write test result to it
    fp = fopen(logfile, "a");
    //write the header of the test in the log file
    write_test_header(fp, "Moving Average", filename_x, filename_y, lines_y, columns_y);

    for(j = 0; j < columns_x; j += acc_size){
        for(i = 0; i < acc_size; i++){
            acc_interval(&test_bench_x[0][i + j], beta_acc);
        }

        y[k] = moving_average(beta_acc, size, acc_size);
        k += 1;
        i = 0;
    }

    for(i = 0; i < lines_y; i++) {

        y0 = y[i];
        yr = test_bench_y[i][0];

        //relative error calculation
        rErr = fabs((y0 - yr)/y0);
        rErrAvg += rErr;

        //write subresult to log file
        write_test_subresult_float(fp, y0, yr, rErr, threshold, i, &success);

        RMS += pow(y0 - yr, 2);
    }
    RMS /= lines_y;
    rErrAvg /= lines_y;

    //write result to file and command windows
    write_test_result(fp, filename_x, success, RMS, rErrAvg, threshold);

    fclose(fp);
    return success;
}







int read_csv_float(char *filename, float (*test_bench_matrix)[TEST_BENCH_MATRIX_SIZE], int *lines, int *columns){
    FILE *fp;

    char dir[200] = TEST_BENCH_FOLDER;
    char scr[200];

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
