/*
Copyright 2012 Jamile Gonçalves, Letícia Bueno, Rodrigo Hausen.

This file is part of DistanceOracle.

DistanceOracle is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

DistanceOracle is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with DistanceOracle. If not, see <http://www.gnu.org/licenses/>.
*/
#include "DistanceOracle.h"
#ifndef _GNU_SOURCE
#define _GNU_SOURCE
#endif
#include <errno.h>
#include <fstream>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

using namespace Permutation;

DistanceOracle::DistanceOracle(const char *dir, int maxN)
throw(char *)
{
    struct stat buf;
    if ( stat(dir, &buf) == -1 )
    {
        char *err;
        asprintf( &err, "Cannot stat directory \"%s\": %s.",
                  dir, strerror(errno) );
        throw err;
    }
    if ( !S_ISDIR(buf.st_mode) )
    {
        char *err;
        asprintf(&err, "Path \"%s\" is not a directory.", dir);
        throw err;
    }

    MAX_ELEMENTS = maxN;
    distances = new NibbleArray*[MAX_ELEMENTS+1];
    distances[0] = new NibbleArray(1, 0x0);

    // abrir os arquivos dist1.bin ... distMAX_ELEMENTS.bin
    // se não existir o arquivo distX.bin -> distances[X] = null;
    // se existir, lê as distâncias do arquivo e coloca no array
    // distances
    for (int n=1; n<= MAX_ELEMENTS; ++n)
    {
        char *filename;
        asprintf(&filename, "%s/dist%d.bin", dir, n);

        FILE *arquivo = fopen(filename, "rb");

        if ( arquivo != NULL && ferror(arquivo) == 0 )
        {
            rank_t nbytes = Permutation::factorial(n);
            distances[n] = new NibbleArray(nbytes);

            std::cerr << "Reading " << nbytes/2 << " bytes from "
                      << filename << "... ";

            if ( distances[n]->fread(arquivo) < nbytes/2 )
            {
                std::cerr << "ERROR: " << strerror(errno)
                          << std::endl;
                std::cerr << "Maybe " << filename
                          << " is corrupted." << std::endl;
                delete distances[n];
                distances[n] = NULL;
            }
            else
            {
                std::cerr << "done!" << std::endl;
            }
        }
        else
            distances[n] = NULL;

        fclose(arquivo);
    }
}

/**
 * Renumbers the elements of a permutation, subtracting
 * them by val.
 */
inline void subtract( permutation_t elmts, int n, int val,
                      permutation_t result )
{
    for (register int i=0; i<n; ++i)
        result[i] = elmts[i] - val;
}

// for qsort in reduce_simple
int compareElements(const void *x, const void *y)
{
    element_t a = *(element_t *) x;
    element_t b = *(element_t *) y;
    return (a-b);
}

/**
 * Eliminates all 1-cycles of a permutation from which the
 * leftmost and rightmost 1-cycles have already been removed.
 * The reduced permutation is stored in result, which must have
 * size newn.
 */
inline void reduce_simple( permutation_t elmts, int n, int newn,
                           permutation_t result )
{
    register int i;

    // find the greatest element
    element_t maxElmt = 0;
    for (i = 0; i < n; ++i)
        if ( elmts[i] > maxElmt )
            maxElmt = elmts[i];

    element_t reduced[newn], sorted[newn],
              renumbering[maxElmt+1];
    register element_t previous = 0;
    register int count = 0;

    // Step 1: for each sorted subsequence of adjacent
    // elements, store the leftmost (smallest) one in
    // the array reduced
    // e.g.: elmts = [8,4,5,7,6]
    //         ||     | \_/ | |
    //         ||     |  |  | |
    //         \/     v  v  v v
    //       reduced=[8, 4 ,7,6]
    for(i=0; i < n; ++i)
    {
        if(elmts[i] != previous+1)
            reduced[count++] = elmts[i];
        previous = elmts[i];
    }

    // Step 2: renumber the array

    // Step 2.1: sort the remaining elements
    // e.g.: reduced=[8,4,7,6]  =>  sorted=[4,6,7,8]
    memcpy( sorted, reduced, newn*sizeof(element_t) );
    qsort( sorted, newn, sizeof(element_t), compareElements );

    // Step 2.2: with the elements sorted, renumber
    // the reduced permutation
    // e.g.:  from  sorted=[4,6,7,8]  obtain
    //        renumbering=[ 4 => 1, 6 => 2, 7 => 3, 8 => 4 ]
    //        and then
    //        result = renumbering([8,4,7,6]) = [4,1,3,2]
    for(i = 0; i < newn; ++i)
    {
        renumbering[sorted[i]] = i+1;
    }
    for(i = 0; i < newn; ++i)
    {
        result[i] = renumbering[reduced[i]];
    }
}

int DistanceOracle::query(permutation_t elmts, element_t n)
{
    // if the rightmost elements are already ordered, simply
    // disregard them
    // e.g.: [1,2,3,8,4,5,7,6,9,10] -> [1,2,3,8,4,5,7,6]
    while (elmts[n-1] == n)
        --n;

    if(n < MAX_ELEMENTS+1 && distances[n] != NULL)
        return distances[n]->get(rank(elmts, n));
    else
    {
        // find the leftmost element which is not ordered
        // store its index in i
        // e.g.: for [1,2,3,8,4,5,7,6] we have i=3
        register int i=0;
        while (elmts[i] == i+1)
            ++i;

        if ( i > 0 )
        {
            // disregard the leftmost ordered elements
            // e.g.: [1,2,3,8,4,5,7,6] -> [8,4,5,7,6]
            n = n-i;
            elmts = &elmts[i];

            // see if this permutation without the leftmost
            // and rightmost ordered elements is already
            // good enough
            if(n < MAX_ELEMENTS+1 && distances[n] != NULL)
            {
                element_t result[n];
                // renumber the remaining elements
                // e.g.: [8,4,5,7,6] - 3 = [5,1,2,4,3]
                subtract(elmts, n, i, result);
                return distances[n]->get(
                                   Permutation::rank(result, n));
            }
        }

        // in this point, we got rid of the ordered elements
        // to the left and to the right of the permutation,
        // but we may need to renumber the remaning elements
        // and remove the remaining 1-cycles

        // we'll first estimate the size of the reduced
        // permutation
        register element_t previous = 0;
        register int count = 0; // no. elmts in reduced perm

        for(register int j=0; j < n; j++)
        {
            if(elmts[j] != previous+1)
                count++;
            previous = elmts[j];
        }

        // if the reduced size is good enough, we effectively
        // reduce the permutation and query the oracle
        if(count < MAX_ELEMENTS +1 && distances[count] != NULL)
        {
            element_t result[count];
            reduce_simple(elmts, n, count, result);
            return distances[count]->get(
                                   Permutation::rank(result, count));
        }
        else
            return -1;
    }
}

std::vector<rank_t> *DistanceOracle::getSlice( distance_t d,
                                               element_t n )
{
    if(n <= MAX_ELEMENTS && distances[n] != NULL)
    {
        NibbleArray *array = distances[n];
        rank_t nperms = Permutation::factorial(n);
        std::vector<rank_t> *ret = new std::vector<rank_t>();
        for (register rank_t i=0; i<nperms; ++i)
            if ( array->get(i) == d )
                ret->push_back(i);
        return ret;
    }
    else
    {
        return NULL;
    }
}

