/*
 * test_bench.h
 *
 *  Created on: Feb 27, 2018
 *      Author: Guillaume
 */

#ifndef HEADER_TEST_BENCH_H_
#define HEADER_TEST_BENCH_H_

#define TEST_BENCH_MATRIX_SIZE 1000


int read_csv_float(char *filename, float (*test_bench_matrix)[TEST_BENCH_MATRIX_SIZE], int *lines, int *columns);

int global_testBench(float g_threshold);




int tb_mfcc_                    (char *filename_x, char *filename_y, char* logfile, float threshold);
int tb_mfcc_preAmpFIR           (char *filename_x, char *filename_y, char* logfile, float threshold);
int tb_mfcc_hamming_window_256  (char *filename_x, char *filename_y, char* logfile, float threshold);
int tb_mfcc_freq2mel            (char *filename_x, char *filename_y, char* logfile, float threshold);
int tb_mfcc_mel2freq            (char *filename_x, char *filename_y, char* logfile, float threshold);
int tb_mfcc_melFilterBank_create(char *filename_x, char *filename_y, char* logfile, float threshold);
int tb_mfcc_fft256              (char *filename_x, char *filename_y, char* logfile, float threshold);
int tb_mfcc_powerSpectrum       (char *filename_x, char *filename_y, char* logfile, float threshold);
int tb_moving_average           (char *filename_x, char *filename_y, char *logfile, float threshold);




#endif /* HEADER_TEST_BENCH_H_ */
