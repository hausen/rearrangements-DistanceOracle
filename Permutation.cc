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
#include "Permutation.h"
#include <string.h>
#include <stdlib.h>

#define swap(a,b) temp = a; a = b; b = temp;

namespace Permutation
{

permutation_t create(char *argv[], element_t *n)
{
    int i = 0;
    while (argv[i] != NULL)
        ++i;
    *n = i;
    permutation_t ret = new element_t[*n];
    bool *seen = new bool[*n+1]();

    for (i=0; i<*n; ++i)
    {
        int elmt = atoi(argv[i]);
        if ( elmt < 1 || elmt > *n || seen[elmt] != false )
        {
            delete ret;
            return NULL;
        }
        seen[elmt] = true;
        ret[i] = elmt;
    }

    return ret;
}

rank_t factorial(element_t n)
{
    register rank_t fat=1;

    for(register element_t i=1; i<=n; ++i)
        fat *= i;

    return fat;
}

std::ostream &print(std::ostream &os, permutation_t p, element_t n)
{
    for (element_t i = 0; i< n; ++i)
    {
        int x = p[i];
        os << x << " ";
    }
    return os;
}

void inverse(permutation_t p, element_t n, permutation_t inv)
{
    for(register element_t i=0; i < n; ++i)
        inv[p[i]-1] = i+1;
}

rank_t rank(permutation_t p, element_t n, permutation_t inv) 
{
    if(n == 1)
        return(0); 

    register element_t temp; // for swap(...)
        
    register element_t s = p[n - 1] - 1; 
    swap(p[n - 1], p[inv[n - 1] - 1]);
    swap(inv[s], inv[n - 1]);
    return(s + n * rank(p, n-1, inv));
}

rank_t rank(permutation_t p, element_t n)
{
    // Most of the time, we will be ranking permutations
    // on the same number of elements, therefore we will keep the
    // allocated space for the inverse and for a copy of the
    // permutation between calls.
    static element_t allocated = n;
    static permutation_t pcopy = new element_t[n];
    static permutation_t inv = new element_t[n];

    if ( n > allocated )
    {
        delete pcopy;
        delete inv;
        pcopy = new element_t[n];
        inv = new element_t[n];
        allocated = n;
    }

    inverse(p, n, inv);
    memcpy(pcopy, p, n*sizeof(element_t));

    return rank(pcopy, n, inv);
}

void unrank(rank_t rank, permutation_t p, element_t n)
{
    register element_t temp; // for swap()
    register element_t i;

    // we must start with the identity permutation
    for (i = 0; i < n; ++i)
        p[i] = i+1;

    // i equals n here

    while (i > 0)
    {
        swap(p[i - 1], p[rank % i]); 
        rank /= i;
        --i;
    } 
}

void cyclicShift( permutation_t p, element_t n,
                         permutation_t shifted )
{
    // Most of the time, we will be shifting permutations
    // on the same number of elements, therefore we will keep the
    // working area preallocated.
    static element_t allocated = n;
    static permutation_t temp = new element_t[n];

    if ( n > allocated )
    {
        delete temp;
        temp = new element_t[n+1];
        allocated = n;
    }

    register int x;
    // Inserir o zero no início da permutação e aplicar deslocamento
    temp[0] = 1;
    for(x = 0; x < n; ++x)
        temp[x+1] = (p[x]+1) % (n+1);
    
    // Colocar a permutação equivalente na ordem correta
    x=0;
    while(x < n+1 && temp[x] != 0)
        ++x;
    x = (++x) % (n+1);

    for(register int y = 0; y < n; ++y, x = (++x) % (n+1))
        shifted[y] = temp[x];
}

}
