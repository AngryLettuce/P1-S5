/*
 * test_bench.h
 *
 *  Created on: Feb 27, 2018
 *      Author: Guillaume
 */

#ifndef HEADER_TEST_BENCH_H_
#define HEADER_TEST_BENCH_H_

#define TEST_BENCH_MATRIX_SIZE 1000


int read_csv_float(char *filename, float (*test_bench_matrix)[TEST_BENCH_MATRIX_SIZE], int size, int *lines, int *columns);


#endif /* HEADER_TEST_BENCH_H_ */
