/*
 * test_bench.h
 *
 *  Created on: Feb 27, 2018
 *      Author: Guillaume
 */

#ifndef HEADER_TEST_BENCH_H_
#define HEADER_TEST_BENCH_H_

#define TEST_BENCH_MATRIX_SIZE 1000


int read_csv_float(const char *filename, float (*test_bench_matrix)[TEST_BENCH_MATRIX_SIZE], int *lines, int *columns);

int global_testBench(float (*test_bench_x)[TEST_BENCH_MATRIX_SIZE], float (*test_bench_y)[TEST_BENCH_MATRIX_SIZE]);



int tb_freq2Mel(const char *filename_x, const char *filename_y,  float threshold,
                float (*test_bench_x)[TEST_BENCH_MATRIX_SIZE],
                float (*test_bench_y)[TEST_BENCH_MATRIX_SIZE]);

int tb_mel2Freq(const char *filename_x, const char *filename_y,  float threshold,
                float (*test_bench_x)[TEST_BENCH_MATRIX_SIZE],
                float (*test_bench_y)[TEST_BENCH_MATRIX_SIZE]);

int tb_mfcc_(const char *filename_x, const char *filename_y,  float threshold,
             float (*test_bench_x)[TEST_BENCH_MATRIX_SIZE],
             float (*test_bench_y)[TEST_BENCH_MATRIX_SIZE]);

int tb_mfcc_preAmpFIR(const char *filename_x, const char *filename_y,  float threshold,
                      float (*test_bench_x)[TEST_BENCH_MATRIX_SIZE],
                      float (*test_bench_y)[TEST_BENCH_MATRIX_SIZE]);

int tb_mfcc_hamming_window_256(const char *filename_x, const char *filename_y,  float threshold,
                               float (*test_bench_x)[TEST_BENCH_MATRIX_SIZE],
                               float (*test_bench_y)[TEST_BENCH_MATRIX_SIZE]);

int tb_mfcc_freq2mel(const char *filename_x, const char *filename_y,  float threshold,
                     float (*test_bench_x)[TEST_BENCH_MATRIX_SIZE],
                     float (*test_bench_y)[TEST_BENCH_MATRIX_SIZE]);

int tb_mfcc_mel2freq(const char *filename_x, const char *filename_y,  float threshold,
                     float (*test_bench_x)[TEST_BENCH_MATRIX_SIZE],
                     float (*test_bench_y)[TEST_BENCH_MATRIX_SIZE]);

int tb_mfcc_melFilterBank_create(const char *filename_x, const char *filename_y,  float threshold,
                                 float (*test_bench_x)[TEST_BENCH_MATRIX_SIZE],
                                 float (*test_bench_y)[TEST_BENCH_MATRIX_SIZE]);

int tb_mfcc_fft256_init(const char *filename_x, const char *filename_y,  float threshold,
                        float (*test_bench_x)[TEST_BENCH_MATRIX_SIZE],
                        float (*test_bench_y)[TEST_BENCH_MATRIX_SIZE]);

int tb_mfcc_fft256(const char *filename_x, const char *filename_y,  float threshold,
                   float (*test_bench_x)[TEST_BENCH_MATRIX_SIZE],
                   float (*test_bench_y)[TEST_BENCH_MATRIX_SIZE]);

int tb_mfcc_powerSpectrum(const char *filename_x, const char *filename_y,  float threshold,
                          float (*test_bench_x)[TEST_BENCH_MATRIX_SIZE],
                          float (*test_bench_y)[TEST_BENCH_MATRIX_SIZE]);





#endif /* HEADER_TEST_BENCH_H_ */
