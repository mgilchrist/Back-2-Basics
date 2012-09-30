//
//  main.cpp
//  Back-2-Basics
//
//  Created by Marcus Gilchrist on 9/12/12.
//  Copyright (c) 2012 Marcus Gilchrist. All rights reserved.
//
//  This program is free software: you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation, either version 3 of the License, or
//  (at your option) any later version.
//
//  This program is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  along with this program.  If not, see <http://www.gnu.org/licenses/>.
//

#include <iostream>
using namespace std;

#include "Array.h"
#include "Heap.h"
#include "HashTable.h"
#include "Graph.h"
#include "Navigation.h"
#include "Network.h"
#include "NeuralNetwork.h"
#include "Genetic.h"
#include "Metaheuristic.h"

#define INPUT_SIZE    32
#define OUTPUT_SIZE   32
#define ITERATIONS    (1024)


int testHashTable() {
  Collection::HashTable<uint64_t,uint64_t> *hashTable;
  hashTable = new Collection::HashTable<uint64_t,uint64_t>();
  
  return 0;
}


int testHeap() {
  Collection::Heap<uint64_t,uint64_t> *heap;
  heap = new Collection::Heap<uint64_t,uint64_t>();
  
  return 0;
}

int testStack() {
  Collection::Stack<uint64_t> *stack;
  stack = new Collection::Stack<uint64_t>();
  
  return 0;
}

/*
int testLinkedList() {
  
  return 0;
}
 */

int testArrayList() {
  Collection::ArrayList<uint64_t,uint64_t> *arrayList;
  arrayList = new Collection::ArrayList<uint64_t,uint64_t>();
  
  return 0;
}

int testNeuralNetwork() {
  Collection::Array<double *> *inputs = new Collection::Array<double *>(16);
  NeuralNetwork::NeuralNetwork *NNetwork;
  NNetwork = new NeuralNetwork::NeuralNetwork(inputs);
  
  return 0;
}

int testNavigation() {
  Graph::Navigation *navigation;
  
  navigation = new Graph::Navigation();
  
  return 0;
}

int testMetaheuristic() {
  
  Collection::Array<double> *input = new Collection::Array<double>(INPUT_SIZE);
  Collection::Array<Heuristic *> *candidates = new Collection::Array<Heuristic *>();
  double *reality = new double[OUTPUT_SIZE];
  double expectation;
  uint64_t iterations = 0;
  uint64_t tmpSize;
  Collection::Stack<double *> *thisInput;
  
  for (int ix = 0; ix < 16; ix ++) {
    tmpSize = (random() % input->getSize()) + 1;
    thisInput = new Collection::Stack<double *>(tmpSize);
    for (int jx = 0; jx < tmpSize; jx++) {
      thisInput->push(input->ptrToIndex(jx));
    }
    
    candidates->setIndex(ix, new NeuralNetwork::NeuralNetwork(thisInput));
    
    delete thisInput;
  }
  
  Metaheuristic<double> *masterMind = new Metaheuristic<double>(input, candidates);
  
  cout << "Training\n";
  do {
    for (int ix = 0; ix < 1 /*OUTPUT_SIZE*/; ix++) {
      for (int jx = 0; jx < input->getSize(); jx++) {
        input->setIndex(jx, 0.1 * (random()%10));
      }
      expectation = masterMind->getConsensus();
      reality[ix] = 1.0;
      /*if ((input->atIndex(ix%INPUT_SIZE)) +
       (input->atIndex((ix+1)%INPUT_SIZE)) +
       (input->atIndex((ix+2)%INPUT_SIZE)) > 2.5) {
       reality[ix] = 1.0;
       } else if ((input->atIndex(ix%INPUT_SIZE)) +
       (input->atIndex((ix+1)%INPUT_SIZE)) +
       (input->atIndex((ix+2)%INPUT_SIZE)) < 0.5) {
       reality[ix] = 0.0;
       }*/
      masterMind->postResult(reality[ix]);
      cout << "Expecting ";
      cout << expectation;
      cout << ", Reality: ";
      cout << reality[ix];
      cout << "\n";
    }
    iterations++;
  } while (iterations < ITERATIONS);
  
  delete input;
  delete masterMind;
  
  return 0;
}


int main(int argc, const char * argv[])
{
  int ret = 0;
  
  cout << "Back-2-Basics\n";
  cout << "Created by Marcus Gilchrist on 9/24/12.\n";
  cout << "Copyright (c) 2012 Marcus Gilchrist. All rights reserved.\n";
  cout << "This program comes with ABSOLUTELY NO WARRANTY; for details type `show w'.\n";
  cout << "This is free software, and you are welcome to redistribute it\n";
  cout << "under certain conditions; type `show c' for details.\n";
  
  ret |= testMetaheuristic();
  
  return ret;
}
