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

#include "Heap.h"
#include <time.h>
#include <queue>
#include <exception>

void testHarness() {

    Heap heap = Heap::instance();

    /***** BEGINNING IMPLEMENTATION FOR RANDOM ALLOC/DEALLOC TESTING ****/
    ulonglong iterations = 10000;
    srand(static_cast<unsigned int>(time(nullptr)));
	std::queue<int*> pointerQueue; 
    int* data = nullptr;
	int requests;
    for (requests = 0; requests < iterations; requests++) {
        int randoGen = (rand() % 15) + 1;
		if (randoGen % 2 == 0) { //Even allocate
			try{ //If stack cannot get block then throw exception and catch it
				data = heap.getBlock<int>(randoGen);
			}
			catch(std::exception e){
				std::cout << e.what() << std::endl;
				break;
			}
			pointerQueue.push(&data[0]);
		}
		else {//Odd free allocation
			if(pointerQueue.size() > 0) { //Make sure item is there
				heap.deleteBlock(pointerQueue.front()); //Grab item at the front of queue FIFO
				pointerQueue.pop(); //Pop item off of queue after deleting it
			}
		}
		
    }

	std::cout << "Size of the heap used in bytes: " << heap.getHeapUsed() << " after " << requests << " memory allocations" << std::endl;
}

int main(int argCount, int** argVariables) {

	testHarness();
    
    //Keep command line open
    unsigned char val;
    cout << "Enter any key and press enter to quit!" << std::endl;
    cin >>  val;
}