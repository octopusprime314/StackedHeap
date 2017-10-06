/*This file is part of StackedHeap.

StackedHeap is free software : you can redistribute it and / or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

StackedHeap is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with StackedHeap.If not, see <http://www.gnu.org/licenses/>.*/

#pragma once

typedef unsigned long long ulonglong;

class MemBlock {
	

public:
    MemBlock() {};

    MemBlock(ulonglong location, ulonglong length) { //Stores a location and length of the data allocated
        _blockLength = length;
        _blockLocation = location;
    }

    ulonglong getBlockLength() { return _blockLength; }
    ulonglong getBlockPosition() { return _blockLocation; }

private:

    ulonglong _blockLength;
    ulonglong _blockLocation;

};