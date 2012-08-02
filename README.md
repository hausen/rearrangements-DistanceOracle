DistanceOracle
==============

A C++ API for querying the transposition distance database.

Contains two example programs that show how to use the API:

* Query.cc - finds the distance for a given permutation

* Slice.cc - prints the permutations of n elements whose distance
             equals a given number


Compiling
---------

make

This will create the object files and the executables "query" and
"slice."


Running
-------

    query /path/to/distance/files p1 p2 ... pN

Where _[ p1 p2 ... pN ]_ is a permutation.

    slice /path/to/distance/files n d

Where _n_ is the number of elements and _d_ is the distance.


Copyright Notice
----------------

Copyright © 2012 Jamile Gonçalves, Letícia Bueno, Rodrigo Hausen.

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
