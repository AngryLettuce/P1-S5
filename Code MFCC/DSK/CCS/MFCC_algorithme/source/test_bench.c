#include <stdio.h>
#include <stdlib.h>
#include "test_bench.h"


int read_csv_float(char *filename, float (*test_bench_matrix)[TEST_BENCH_MATRIX_SIZE], int size, int *lines, int *columns){
    FILE *fp;
    fp = fopen(filename, "r");

    float value;

    if (fp == NULL)
    {
        printf("Could not open file %s \n", filename);
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

