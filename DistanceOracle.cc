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

// for quicksort
int compareElements(const void *x, const void *y)
{
    element_t a = *(element_t *) x;
    element_t b = *(element_t *) y;
    return (a-b);
}

/**
 * Método para obter a permutação reduzida
 */
permutation_t reduced_permutation( permutation_t elmts,
                                   element_t n, element_t newn)
{
    permutation_t reduced = new element_t[n];
    permutation_t vet = new element_t[n];

    register element_t i = 0, j = 0;

    // step 1: collapse elements that are in order
    // e. g.: [1,2,3,8,4,5,7,6,9,10] -> [8,4,7,6]            
    //         \___/   \_/     \__/
    //       (omitted)   4  (omitted)
    for(i=0; i < n-1; ++i)
    {
        if(elmts[i] != elmts[i+1] -1)
        {
            reduced[j] = elmts[i+1];
            ++j;
        }            
        if(j == newn)
            break;
    }

    // step 2: sort the remaining elements.
    // e. g.: [8,4,7,6] -> [4,6,7,8]
    permutation_t order = new element_t[newn];

    memcpy( order, reduced, newn*sizeof(element_t) );

    qsort(order, n, sizeof(element_t), compareElements);

    // step 3: use the sorting to renumber the
    //         remaining elements.
    // e. g.: [8,4,7,6] -> [4,1,3,2]
    for(i = 0; i < newn; ++i)
        vet[order[i]] = i+1;

    for(i = 0; i < newn; ++i)
        reduced[i] = vet[reduced[i]];

    delete order;
    delete vet;

    return reduced;
}

int DistanceOracle::query(permutation_t elmts, element_t n)
{
    rank_t rank = Permutation::rank(elmts, n);
    int count;

    if(n <= MAX_ELEMENTS && distances[n] != NULL)
        return distances[n]->get(rank);
    else
    {
        // Number of elements of the reduced permutation
        if(elmts[0] == 1)
            count = -1;
        else
            count = 1;
        
        for(register element_t i=0; i < n-1; ++i)
        {
            if(elmts[i] != elmts[i+1] - 1)
                ++count;
        }

        if(count <= MAX_ELEMENTS)
        {
            permutation_t reduced = reduced_permutation( elmts, n,
                                                         count );
            int d = query(reduced, count);
            delete reduced;
            return d;
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

