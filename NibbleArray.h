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
#ifndef NIBBLEARRAY_H
#define NIBBLEARRAY_H
#include <stdint.h>
#include <stdio.h>
#include <sys/types.h>

class NibbleArray
{
public:
	NibbleArray(size_t numelmts);
	NibbleArray(size_t numelmts, uint8_t value);
	~NibbleArray();
	uint8_t get(size_t index);
	void set(size_t index, uint8_t value);
	size_t fwrite(FILE *stream);
	size_t fread(FILE *stream);

private:
	void init(size_t numelmts);

	uint8_t *array;
	size_t size;
};

#endif // NIBBLEARRAY_H
