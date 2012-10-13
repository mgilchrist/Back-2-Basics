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
#include "RedBlackTree.h"
#include "Navigation.h"
#include "Network.h"
#include "NeuralNetwork.h"
#include "Genetic.h"
#include "Metaheuristic.h"

#define INPUT_SIZE    32
#define OUTPUT_SIZE   32
#define ITERATIONS    (1024)
#define TEST_SIZE     0x100000


int testHashTable() {
  Collection::HashTable<uint64_t,uint64_t> *hashTable;
  hashTable = new Collection::HashTable<uint64_t,uint64_t>();
  uint64_t *verify = new uint64_t[TEST_SIZE];
  uint64_t index;
  
  cout << "\nTesting HashTable.\n";
  
  for (int ix = 0; ix < 16; ix++) {
    uint64_t value = random();
    hashTable->insert(value, value);
    verify[ix] = value;
  }
  
  for (int ix = 0; ix < 16; ix++) {
    
    if ((index = hashTable->search(verify[ix])) == ERROR) {
      cout << "Index ";
      cout << ix;
      cout << " is not present!\n";
      continue;
    }
    
    if (verify[ix] != hashTable->get(index)) {
      cout << "Index ";
      cout << ix;
      cout << " does not match!\n";
    }
    
    if ((ix % 4) == 3) {
      hashTable->remove(index);
    }
  }
  
  for (int ix = 0; ix < TEST_SIZE; ix++) {
    
    if ((ix % 4) == 3) {
      continue;
    }
    
    if ((index = hashTable->search(verify[ix])) == ERROR) {
      cout << "Index ";
      cout << ix;
      cout << " is not present!\n";
    }
    
    if (verify[ix] != hashTable->get(index)) {
      cout << "Index ";
      cout << ix;
      cout << " does not match!\n";
    }
    
    if ((index % 4) == 3) {
      hashTable->remove(index);
    }
  }
  
  cout << "HashTable:Done\n";
  
  return 0;
}


int testHeap() {
  Collection::Heap<uint64_t,uint64_t> *heap;
  heap = new Collection::Heap<uint64_t,uint64_t>();
  
  uint64_t tmp, tmp1;
  
  cout << "\nTesting Heap.\n";
  
  for (int ix = 0; ix < TEST_SIZE; ix++) {
    uint64_t value = random();
    heap->push(value, value);
  }
  
  if (heap->peek(0) != (tmp = heap->pop())) {
    cout << "Heap Error!\n";
  }
  
  for (int ix = 1; ix < TEST_SIZE; ix++) {
    if (heap->peek(0) != (tmp1 = heap->pop())) {
      cout << "Index ";
      cout << ix;
      cout << " Error!\n";
    }
    
    if (tmp > tmp1) {
      cout << "Index ";
      cout << ix;
      cout << " lower than previous!\n";
    }
    
    tmp = tmp1;
  }
  
  cout << "Heap:Done.\n";
  
  return 0;
}

int testStack() {
  Collection::Stack<uint64_t> *stack;
  stack = new Collection::Stack<uint64_t>();
  
  return 0;
}


int testRBTree() {
  Graph::RedBlackTree<u_int64_t,uint64_t> *rbTree;
  Collection::ArrayList<uint64_t,uint64_t> *arrayList;
  Graph::RedBlackTreeNode<uint64_t,uint64_t> *current;
  uint64_t tmp;
  uint64_t index = 0;
  
  cout << "\nTesting RedBlackTree\n";

  rbTree = new Graph::RedBlackTree<u_int64_t, uint64_t>();
  arrayList = new Collection::ArrayList<uint64_t,uint64_t>(TEST_SIZE);
  
  for (uint64_t ix = 0; ix < TEST_SIZE; ix++) {
    uint64_t value = ix;
    rbTree->insert(value, value);
  }
  
  current = rbTree->firstNode(rbTree->getTreeRoot());
  tmp = current->key;
  
  while (current != NULL) {

    if (current->key < tmp) {
      cout << "Index ";
      cout << index;
      cout << " not sorted!\n";
    }
    
    tmp = current->key;
    current = rbTree->nextNode(current);
    index++;
    
  }
  
  cout << "RedBlackTree:Done\n";
  
  
  
  return 0;
}


/*
 int testLinkedList() {
 
 return 0;
 }
 */

int testArrayList() {
  Collection::ArrayList<uint64_t,uint64_t> *arrayList, *arrayListClone;
  arrayList = new Collection::ArrayList<uint64_t,uint64_t>();
  uint64_t *verify = new uint64_t[arrayList->getSize()];
  uint64_t tmp;
  
  cout << "\nTesting ArrayList\n";
  
  for (int ix = 0; ix < arrayList->getSize(); ix++) {
    uint64_t value = random();
    arrayList->setIndex(ix, new Collection::Comparable<uint64_t,uint64_t>(value, value));
    verify[ix] = value;
  }
  
  for (int ix = 0; ix < arrayList->getSize(); ix++) {
    if (arrayList->atIndex(ix)->data != verify[ix]) {
      cout << "Index ";
      cout << ix;
      cout << " does not match!\n";
    }
  }
  
  delete arrayList;
  delete verify;
  
  arrayList = new Collection::ArrayList<uint64_t,uint64_t>(TEST_SIZE);
  verify = new uint64_t[arrayList->getSize()];
  
  for (int ix = 0; ix < arrayList->getSize(); ix++) {
    uint64_t value = random();
    arrayList->setIndex(ix, new Collection::Comparable<uint64_t,uint64_t>(value, value));
    verify[ix] = value;
  }
  
  for (int ix = 0; ix < arrayList->getSize(); ix++) {
    if (arrayList->atIndex(ix)->data != verify[ix]) {
      cout << "Index ";
      cout << ix;
      cout << " does not match!\n";
    }
  }
  
  delete arrayList;
  delete verify;
  
  arrayList = new Collection::ArrayList<uint64_t,uint64_t>(TEST_SIZE);
  
  for (int ix = 0; ix < arrayList->getSize(); ix++) {
    uint64_t value = random();
    arrayList->setIndex(ix, new Collection::Comparable<uint64_t,uint64_t>(value, value));
  }
  
  arrayListClone = arrayList->cloneSort();
  
  arrayList->inPlaceSort();
  
  tmp = arrayList->atIndex(0)->key;
  
  for (int ix = 1; ix < arrayList->getSize(); ix++) {
    
    if (arrayList->atIndex(ix)->key < tmp) {
      cout << "Index ";
      cout << ix;
      cout << " not sorted!\n";
    }
    
    if (arrayListClone->atIndex(ix)->key < tmp) {
      cout << "Index ";
      cout << ix;
      cout << " not sorted!\n";
    }
    
    if (arrayList->atIndex(ix)->key != arrayListClone->atIndex(ix)->key) {
      cout << "Index ";
      cout << ix;
      cout << " not identical!\n";
    }
    
    tmp = arrayList->atIndex(ix)->key;
  }
  
  delete arrayList;
  delete arrayListClone;
  
  cout << "ArrayList:Done\n";
  
  return 0;
}

#if 1
int testNeuralNetwork() {
  Collection::Array<double> *input = new Collection::Array<double>(INPUT_SIZE);
  NeuralNetwork::NeuralNetwork *NNetwork;
  double *reality = new double[OUTPUT_SIZE];
  double expectation;
  uint64_t iterations = 0;
  uint64_t tmpSize;
  Collection::Stack<double *> *thisInput;
  double errorRate = 0.0;
  uint64_t endPraticeTests = (100 * ITERATIONS / 90);
  uint64_t nonPracticeTests = ITERATIONS - endPraticeTests;
  
  cout << "\nTesting NeuralNetwork\n";
  
  tmpSize = (random() % input->getSize()) + 1;
  thisInput = new Collection::Stack<double *>(tmpSize);
  
  for (int jx = 0; jx < tmpSize; jx++) {
    thisInput->push(input->ptrToIndex(jx));
  }
  
  NNetwork = new NeuralNetwork::NeuralNetwork(thisInput);
    
  do {
    for (int ix = 0; ix < 1 /*OUTPUT_SIZE*/; ix++) {
      for (int jx = 0; jx < thisInput->getSize(); jx++) {
        *(thisInput->atIndex(jx)) = 0.1 * (random()%10);
      }
      
      NNetwork->calculateExpectation();
      expectation = NNetwork->getExpectation();
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
      NNetwork->doCorrection(reality[ix], 0.0);
      
      if (iterations > endPraticeTests) {
        errorRate += ((reality[ix] - expectation) * (reality[ix] - expectation)) * 1000000.0;
      }
    }
    iterations++;
  } while (iterations < ITERATIONS);
  
  if (nonPracticeTests) {
    errorRate /= (double)nonPracticeTests;
    errorRate = sqrt(errorRate);
    cout << "Error Rate is ";
    cout << errorRate;
    cout << "PPM\n";
  }
  
  delete thisInput;
  
  cout << "NeuralNetwork:Done\n";
  
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
  
  double errorRate = 0.0;
  uint64_t endPraticeTests = (100 * ITERATIONS / 90);
  uint64_t nonPracticeTests = ITERATIONS - endPraticeTests;
  
  cout << "\nTesting MetaHeuristic\n";
  
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
      
      if (iterations > endPraticeTests) {
        errorRate += ((reality[ix] - expectation) * (reality[ix] - expectation)) * 1000000.0;
      }
    }
    iterations++;
  } while (iterations < ITERATIONS);
  
  if (nonPracticeTests) {
    errorRate /= (double)nonPracticeTests;
    errorRate = sqrt(errorRate);
    cout << "Error Rate is ";
    cout << errorRate;
    cout << "PPM\n";
  }
  
  delete input;
  delete masterMind;
  
  cout << "MetaHeuristic:Done\n";
  
  return 0;
}
#endif

int main(int argc, const char * argv[])
{
  int ret = 0;
  
  cout << "Back-2-Basics\n";
  cout << "Created by Marcus Gilchrist on 9/24/12.\n";
  cout << "Copyright (c) 2012 Marcus Gilchrist. All rights reserved.\n";
  cout << "This program comes with ABSOLUTELY NO WARRANTY; for details type `show w'.\n";
  cout << "This is free software, and you are welcome to redistribute it\n";
  cout << "under certain conditions; type `show c' for details.\n";
  
  ret |= testHashTable();
  ret |= testArrayList();
  ret |= testHeap();
  ret |= testRBTree();
  ret |= testStack();
  ret |= testNeuralNetwork();
  ret |= testMetaheuristic();
  ret |= testNavigation();
  
  cout << "Finished Testing:";
  cout << ret;
  cout << "\n";
  
  return ret;
}


