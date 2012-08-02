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
#include "NibbleArray.h"
#include <string.h>

NibbleArray::NibbleArray(size_t numelmts)
{
	init(numelmts);
}

NibbleArray::NibbleArray(size_t numelmts, uint8_t value)
{
	init(numelmts);
	// copies the 4 least significant bits into the 4 most
	// significant bits
        value = ( value & 0x0F ) | ( ( value & 0x0F ) << 4 );
	memset(array, value, size);
}

void NibbleArray::init(size_t numelmts)
{
	if ( numelmts % 2 == 1 )
		size = numelmts/2+1;
	else
		size = numelmts/2;
	array = new uint8_t[size];
}

NibbleArray::~NibbleArray()
{
	delete array;
}

uint8_t NibbleArray::get(size_t index)
{
	size_t byte = index / 2;
	size_t nibble = index % 2;
	if ( nibble == 0 )
		return array[byte] & 0x0F;
	else
		return ( array[byte] & 0xF0 ) >> 4;
}

void NibbleArray::set(size_t index, uint8_t value)
{
	size_t byte = index / 2;
	size_t nibble = index % 2;
	uint8_t val, mask;
	if (nibble == 0)
	{
		val = value & 0x0F;
		mask = 0xF0;
	}
	else
	{
		val = ( value & 0x0F ) << 4;
		mask = 0x0F;
	}
	array[byte] = ( array[byte] & mask ) | val;
}

size_t NibbleArray::fwrite(FILE *stream)
{
    return ::fwrite(array, sizeof(uint8_t), size, stream);
}

size_t NibbleArray::fread(FILE *stream)
{
    return ::fread(array, sizeof(uint8_t), size, stream);
}
