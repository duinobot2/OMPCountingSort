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
 * Source Code for sequential version:
 * https://it.wikipedia.org/wiki/Counting_sort
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
 * @file countingsort.c
 * @brief this countingsort.c file contains the implementation of the counting sort algorithm
 * @version 0.1
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include "countingsort.h"


/**
 * @brief This function order the array A of dimension n
 * 
 * @param A     array
 * @param n     dimension of array
 */
void countingSort(int *A, int n)
{
    int max = A[0];
    int min = A[0];
    int i;

#pragma omp parallel
    {
        int local_max = max, local_min = min;

#pragma omp for nowait
        for (int i = 1; i < n; i++)
        {

            int local_result;

            local_result = A[i];

            if (local_result > local_max)
            {
                local_max = local_result;
            }
            else if (local_result < local_min)
            {
                local_min = local_result;
            }
        }

#pragma omp critical
        {
            if (local_max > max)
                max = local_max;

            if (local_min < min)
                min = local_min;
        }
    }

    int *C;
    int lenC = max - min + 1;
    if ((C = malloc(lenC * sizeof(int))) == NULL)
        exit(1);

#pragma omp parallel for private(i) shared(n, C, lenC)
    for (i = 0; i < lenC; i++)
    {
        C[i] = 0;
    }

#pragma omp parallel for private(i) shared(n, A, min, lenC) reduction(+ : C[:lenC])
    for (i = 0; i < n; i++)
    {
        C[A[i] - min] += 1; 
    }

    int k = 0; 

    for (int i = 0; i < lenC; i++)
    {
        while (C[i] > 0)
        { 
            A[k] = i + min;
            k = k + 1;
            C[i] = C[i] - 1;
        }
    }

    free(C);
}