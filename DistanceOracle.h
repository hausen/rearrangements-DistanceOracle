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
#ifndef DISTANCEORACLE_H
#define DISTANCEORACLE_H

#include "NibbleArray.h"
#include "Permutation.h"
#include <vector>
#include <stdint.h>

class DistanceOracle
{
    public:
        /**
         * Constructor.
         * @param dir path to a directory that has the dist*.bin
         *            files containing the distances
         * @param maxN the number of elements for the largest
         *             permutations whose distances will be read
         *             from the disk
         */
        DistanceOracle(const char *dir, int maxN) throw(char *);

        /**
         * Returns the distance of a permutation.
         * @param p the permutation
         * @param n number of elements of p
         * @return the distance of the permutation or -1 if
         *         not known
         */
        int query( Permutation::permutation_t p,
                   Permutation::element_t n );

        /**
         * Returns the ranks of the permutations whose distance
         * equals d.
         */
        std::vector<Permutation::rank_t> *getSlice(
                   Permutation::distance_t d,
                   Permutation::element_t n );

    private:
        int MAX_ELEMENTS;
        NibbleArray **distances;
};

#endif // DISTANCEORACLE_H
