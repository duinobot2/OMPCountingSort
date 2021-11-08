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
 * @file countingsort.h
 * @brief this countingsort.h file contains the declaration of the counting sort function
 * @version 0.1
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#ifndef COUNTINGSORT_H_ /* Include guard */
#define COUNTINGSORT_H_

#include <time.h>

/* Token concatenation used */
#define STARTTIME(id)                             \
    clock_t start_time_42_##id, end_time_42_##id; \
    start_time_42_##id = clock()

#define ENDTIME(id, x)          \
    end_time_42_##id = clock(); \
    x = ((double)(end_time_42_##id - start_time_42_##id)) / CLOCKS_PER_SEC

/**
 * @brief This function order the array A of dimension n
 * 
 * @param A     array
 * @param n     dimension of array
 */
void countingSort(int *A, int n);

#endif