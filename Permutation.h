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
#ifndef PERMUTATION_H
#define PERMUTATION_H
#include <iostream>
#include <stdint.h>

namespace Permutation
{

/**
 * Data type that represents an element of a permutation
 */
typedef uint8_t element_t;

/**
 * Data type that represents a permutation
 */
typedef element_t* permutation_t;

/**
 * Data type that represents the distance of a permutation.
 */
typedef uint8_t distance_t;

/**
 * Data type that represents the rank of a permutation.
 */
typedef uint64_t rank_t;

/**
 * Parses an array of strings and creates a permutation.
 * The last element of the array must be a NULL.
 */
permutation_t create(char *args[], element_t *n);

/**
 * Returns the factorial of a number.
 * @param n - the number
 * @return n!, the factorial of n
 */
rank_t factorial(element_t n);

/**
 * Prints a permutation into a stream.
 */
std::ostream &print(std::ostream &os, permutation_t p, element_t n);

/**
 * Calculates the inverse permutation. The inverse permutation is
 * placed in <i>inv</i>, which must have been previously allocated.
 * @param p - a permutation of {1, .., n}.
 * @param n - the number of elements
 * @param inv - where the inverse permutation will be stored
 */
void inverse(permutation_t p, element_t n, permutation_t inv);

/**
 * Ranks a permutation. The rank is the position of a permutation
 * in a lexicographically sorted list of all n! permutations.
 * This function always ranks the identity permutation as n!-1.
 * This function is not thread safe.
 * @param p - a permutation of {1, .., n}.
 * @param n - the number of elements
 * @return The rank of p, a number in the range 0..n!-1.
 */
rank_t rank(permutation_t p, element_t n);

/**
 * Unranks a permutation, i. e., finds the permutation that corresponds
 * to a given rank. The permutation is placed in <i>p</i>, which must have
 * been previously allocated.
 * @param rank - the rank, a number in the range 0..n!-1
 * @param p - where the unranked permutation will be placed
 * @param n - the number of elements
 * @see rank(permutation_t p, element_t n)
 */
void unrank(rank_t rank, permutation_t p, element_t n);

/**
 * Performs a cyclic shift <i>p</i>+1, storing the result in <i>equiv</i>.
 * The permutation equiv must have been previously allocated.
 * This function is not thread safe.
 * @param p - a permutation of {1, .., n}.
 * @param n - the number of elements
 * @param equiv - the cyclically shifted permutation
 */
void cyclicShift(permutation_t p, element_t n, permutation_t shifted);

} // end namespace permutation

#endif // PERMUTATION_H
