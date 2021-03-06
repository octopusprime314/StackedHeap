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
#include "MemBlock.h"
#include <iostream>
#include <map>
using namespace std;


class Heap {

public:
    Heap() {};
    ~Heap() {};

    //Static heap object only gets initialized once when called for the first time
    static Heap instance() {
        static Heap heap;
        return heap;
    }

    //Returns a pointer with the templated type
    template <typename T>  T* getBlock(size_t count) {

        //Length in bytes
        ulonglong byteSize = sizeof(T)*count;

        T* block = nullptr;

        //Search through list and choose an opening that will fit in between mem blocks
        //Grab the first element if there is one
        if (_usedBlocks.size() > 1) {
            auto prevMemBlock = _usedBlocks.begin(); //Deference map to value
            std::map<ulonglong, MemBlock> nextBlocks(++_usedBlocks.begin(), _usedBlocks.end()); //Truncate first member
            
            bool foundBlockOpening = false; //Used to indicate a block opening in stack

            for (auto memBlock : nextBlocks) {
                ulonglong endOfBlock = prevMemBlock->second.getBlockPosition() + prevMemBlock->second.getBlockLength();
                ulonglong distanceBetwBlocks = memBlock.second.getBlockPosition() - endOfBlock;
				               
                //If a block of memory is small enough to fit between two blocks
                if (byteSize <= distanceBetwBlocks) {
                    //Snug that allocation in there
                    block = reinterpret_cast<T*>(&_heap[endOfBlock]); //Convert data stream from unsigned byte to template type
                    //std::cout << "Added pointer: " << (ulonglong)&block[0] << std::endl;
					_usedBlocks[(ulonglong)&block[0]] = MemBlock(&_heap[endOfBlock] - &_heap[0], byteSize); //Add to list of used blocks
                    foundBlockOpening = true; //Successfully inserted into a block between 2 allocations
                    break;
                }
                prevMemBlock++;
            }
            if (!foundBlockOpening) { //If an opening was not found then allocate at the end of the stack location
                if(!allocateBlock(block, byteSize)) throw exception("Not enough memory!");
            }
        }
        else { //If unused just allocate a block
            if(!allocateBlock(block, byteSize)) throw exception("Not enough memory!");
        }

        return block;
    }

    //Allocates a chunk off the stack and modifies reference to a pointer
    template <typename T>  bool allocateBlock(T*& block, ulonglong byteSize) {
		
		if(_heapPointer + byteSize > HEAP_LENGTH) { //Stack is not big enough to service the memory request
			return false;
		}

        block = reinterpret_cast<T*>(&_heap[_heapPointer]); //Convert data stream from unsigned byte to template type
        _usedBlocks[(ulonglong)&block[0]] = MemBlock(_heapPointer, byteSize); //Add to list of used blocks
        _heapPointer += byteSize; //number of bytes (unsigned chars) allocated so increment pointer

		return true;
    }

    //Opens a block of memory for use
    template <typename T>  void deleteBlock(T* data) {

        ulonglong pointerAddress = (ulonglong)&data[0]; //Grabs the location in memory

        //Find block if it exists
        auto existingBlock = _usedBlocks.find(pointerAddress);
        if (existingBlock != _usedBlocks.end()) {
            _usedBlocks.erase(existingBlock); //Erase the block so it can be used later
			//std::cout << "Removed pointer: " << pointerAddress << std::endl;
        }
        else { //trying to release memory that is not allocated
            std::cout << "Removing block that isn't allocated at " << pointerAddress << std::endl;
			//Display all pointer locations
			for (auto memBlock : _usedBlocks) {
				std::cout << memBlock.first << std::endl;
			}
        }
		
		if (_usedBlocks.size() > 1) {
			//Adjust _heapPointer tail in case an allocation was removed at the end
			std::map<ulonglong, MemBlock> nextBlocks(++_usedBlocks.begin(), _usedBlocks.end()); //Truncate first member
		
			ulonglong memoryTail = nextBlocks.begin()->second.getBlockPosition() + nextBlocks.begin()->second.getBlockLength();
			for (auto memBlock : nextBlocks) {
				ulonglong lastByteLocation = memBlock.second.getBlockPosition() + memBlock.second.getBlockLength();
				if(memoryTail < lastByteLocation) {
					memoryTail = lastByteLocation;
				}
			}
			_heapPointer = memoryTail;
		}
		else if(_usedBlocks.size() == 1) {
			_heapPointer = _usedBlocks.begin()->second.getBlockPosition() + _usedBlocks.begin()->second.getBlockLength();
		}
		else {
			_heapPointer = 0;
		}
    }

	ulonglong getHeapUsed(){ return _heapPointer; }

private:

    static const ulonglong HEAP_LENGTH = 10000; //Heap contains 10 MB on the stack to play with :)
    unsigned char _heap[HEAP_LENGTH];
    
    ulonglong _heapPointer; //Acts at the tail index into the stack heap
    std::map<ulonglong, MemBlock> _usedBlocks; //Hash table to keep track of the index and length of allocation, hash indexed with pointer value
};