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

void testHarness() {

    Heap heap = Heap::instance();

    /***** BEGINNING IMPLEMENTATION FOR RANDOM ALLOC/DEALLOC TESTING ****/
    ulonglong iterations = 100;
    srand(static_cast<unsigned int>(time(nullptr)));
	std::queue<int*> pointerQueue; 
    int* data = nullptr;
    for (int i = 0; i < iterations; i++) {
        int randoGen = rand() % 100;
        if (randoGen % 2 == 0) { //Even allocate
            data = heap.getBlock<int>(randoGen);
			pointerQueue.push(&data[0]);
		}
        else {//Odd free allocation
            if(pointerQueue.size() > 0) { //Make sure item is there
				heap.deleteBlock(pointerQueue.front()); //Grab item at the front of queue FIFO
				pointerQueue.pop(); //Pop item off of queue after deleting it
			}
		}
    }

	std::cout << "Size of the heap used in bytes: " << heap.getHeapUsed() << std::endl;
}

int main(int argCount, int** argVariables) {

	testHarness();
    
    //Heap heap = Heap::instance();

    //int* data = heap.getBlock<int>(10); //Alocate 10 integers

    //data[5] = 10;

    //int *data2 = heap.getBlock<int>(5); //Allocate 5 integers

    //data2[2] = 3;

    //int *data3 = heap.getBlock<int>(10); //Allocate 10 integers

    //heap.deleteBlock<int>(data2); //Release data 2 memory

    //int *data4= heap.getBlock<int>(5); //Allocate 5 integers

    //cout << data2[2] << std::endl;

    //data4[2] = 7;

    //cout << data2[2] << std::endl;
    //cout << data4[2] << std::endl;

    //Keep command line open
    unsigned char val;
    cout << "Enter any key and press enter to quit!" << std::endl;
    cin >>  val;
}