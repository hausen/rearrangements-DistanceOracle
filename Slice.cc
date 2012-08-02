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
#include <iostream>
#include <stdlib.h>

using namespace Permutation;

/**
 * Maximum number of permutations to print.
 */
int LIMIT = 500;

#define min(X,Y) ((X) < (Y) ? (X) : (Y))

int main(int argc, char *argv[])
{
    if ( argc != 4 )
    {
        std::cerr << "Usage: slice /path/to/distances d n"
                  << std::endl;
        return 1;
    }

    try {
        DistanceOracle oracle(argv[1], 13);

        int d = atoi(argv[2]);
        int n = atoi(argv[3]);

        std::vector<rank_t> *ranks = oracle.getSlice(d, n);
        if ( ranks != NULL )
        {
            std::cerr << "There are " << ranks->size() <<
                         " permutations of " << n << " elements " 
                         "that have distance " << d << "." << std::endl;
            int nperms = min( ranks->size(), LIMIT );
            permutation_t p = new element_t[n];
            for (int i=0; i < nperms; ++i)
            {
                rank_t r = ranks->at(i);
                unrank(r, p, n);
                print(std::cout, p, n) << std::endl;
            }
            delete p;
            return 0;
        }
        else
        {
            std::cerr << "Couldn't find a precomputed table of "
                         "distances for " << n << " elements." << std::endl;
            return 1;
        }

        return 0;
    }
    catch (char *err)
    {
        std::cerr << "Distance oracle error! " << err << std::endl;
        free(err);
        return 1;
    }
}
