/*
 * test_bench.h
 *
 *  Created on: Feb 27, 2018
 *      Author: Guillaume
 */


#ifndef HEADER_TEST_BENCH_H_
#define HEADER_TEST_BENCH_H_

#define TEST_BENCH_MATRIX_SIZE 500
#define TEST_BENCH_MATRIX_SIZE_J 128

int read_csv_float(char *filename, float (*test_bench_matrix)[TEST_BENCH_MATRIX_SIZE_J], int *lines, int *columns);

int global_testBench(float g_threshold);

//utils local function for result writing and display
void write_test_header              (FILE *fp, char *name, char *filename_x, char *filename_y, int lines, int columns);
void write_test_result              (FILE *fp, char * filename, int success, float RMS, float rErrAvg, float threshold);
void write_test_subresult_float     (FILE *fp, float y0, float yr, float rErr, float threshold, int i, int *success);
void write_test_subresult_complex   (FILE *fp, float Ry0, float Iy0, float Ryr, float Iyr, float rErr, float threshold, int i, int *success);


int tb_mfcc_preAmpFIR           (char *filename_x, char *filename_y, char* logfile, float threshold);
int tb_mfcc_hamming_window_256  (char *filename_x, char *filename_y, char* logfile, float threshold);
int tb_mfcc_freq2mel            (char *filename_x, char *filename_y, char* logfile, float threshold);
int tb_mfcc_mel2freq            (char *filename_x, char *filename_y, char* logfile, float threshold);
int tb_mfcc_melFilterBank_create(char *filename_x, char *filename_y, char* logfile, float threshold);
int tb_mfcc_getMelCoeff         (char *filename_x, char *filename_y, char* logfile, float threshold);
int tb_mfcc_fft256              (char *filename_x, char *filename_y, char* logfile, float threshold);
int tb_mfcc_powerSpectrum       (char *filename_x, char *filename_y, char* logfile, float threshold);
int tb_mfcc_dct                 (char *filename_x, char *filename_y, char* logfile, float threshold);
int tb_moving_average           (char *filename_x, char *filename_y, char *logfile, float threshold);
int tb_mfcc_get_metrics         (char *filename_x, char *filename_y, char *logfile, float threshold);
int tb_cb_construct_codebook    (char *filename_x, char *filename_y, char *logfile, float threshold);



#endif /* HEADER_TEST_BENCH_H_ */
