/* 
 * Course: High Performance Computing 2021/2022
 * 
 * Lecturer: Francesco Moscato	fmoscato@unisa.it
 *
 * Group:
 * Avella Antonello            a.avella19@studenti.unisa.it             
 * D'Andrea Anna               a.dandrea26@studenti.unisa.it 
 * De Pisapia Claudio          c.depisapia1@studenti.unisa.it
 * 
 *
 * Copyright (C) 2021 - All Rights Reserved
 *
 * This file is part of ContestOMP.
 *
 * ContestOMP is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * ContestOMP is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with ContestOMP.  If not, see <http://www.gnu.org/licenses/>.
 */

/**
 * @file main.c
 * @brief this is the main.c file which is the main function that calls the CountingSort function
 * @version 0.1
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#include <omp.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include "countingsort.h"

int main(int argc, char *argv[])
{

    if (argc != 4)
    {
        printf("you must insert the length of array to order,the interval and the number of threads\n");
        exit(1);
    }

    srand(time(NULL));
    long n, val;
    int numT;
    double time, start, end;
    n = atoi(argv[1]);   //array langth
    val = atoi(argv[2]); //max value of element in array
    numT = atoi(argv[3]); //number Threads

#ifdef _OPENMP
    omp_set_num_threads(numT);
#endif

    int *A;

    if ((A = malloc(n * sizeof(int))) == NULL)
        exit(1);
    int i;

    
    for (i = 0; i < n; i++)
    {
        A[i] = rand() % val;
    }

/*
    for(int i=0;i<n;i++)
        printf("A is: %d\n ",A[i]);
*/

#ifdef _OPENMP
    start = omp_get_wtime();
#else
    STARTTIME(0);
#endif

    countingSort(A, n);

#ifdef _OPENMP
    end = omp_get_wtime();
    time = end - start;
#else
    ENDTIME(0, time);
#endif

/*
    for(int i=0;i<n;i++)
        printf("A is: %d\n ",A[i]);
*/

    printf("%d;%f;", numT, time);
    free(A);
    return 0;
}
