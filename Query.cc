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

int main(int argc, char *argv[])
{
    if ( argc < 3 )
    {
        std::cerr << "Usage: query /path/to/distances p1 p2 ... pN"
                  << std::endl;
        return 1;
    }

    element_t n;
    permutation_t p = create(&argv[2], &n);

    if ( p == NULL )
    {
        std::cerr << "Couldn't parse the permutation!" << std::endl;
        return 1;
    }

    try
    {
        DistanceOracle oracle(argv[1], 13);

        int d = oracle.query(p, n);

        if ( d > -1 )
            std::cout << "Distance: " << d << std::endl;
        else
            std::cout << "Unknown distance" << std::endl;
    }
    catch (char *err)
    {
        std::cerr << "Distance oracle error! " << err << std::endl;
        free(err);
        return 1;
    }
}
