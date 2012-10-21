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
#include "SecureHashTable.h"
#include "Graph.h"
#include "RedBlackTree.h"
#include "LLRB_Tree.h"
#include "Navigation.h"
#include "Network.h"
#include "NeuralNetwork.h"
#include "Genetic.h"
#include "Metaheuristic.h"

#define INPUT_SIZE    32
#define OUTPUT_SIZE   32
#define ITERATIONS    (1024)
#define TEST_SIZE     0x10000


int testHashTable() {
  Collection::HashTable<uint64_t,uint64_t> *hashTable;
  std::vector<uint64_t> verify;
  hashTable = new Collection::HashTable<uint64_t,uint64_t>(TEST_SIZE);
  uint64_t *tmpInt = new uint64_t;
  uint64_t value;
  
  cout << "\nTesting HashTable.\n";
  
  verify.resize(TEST_SIZE);
  
  for (int ix = 0; ix < TEST_SIZE; ix++) {
    
    do {
      value = random();
    } while (!hashTable->get(value, tmpInt));
    
    hashTable->insert(value, value);
    
    verify[ix] = value;
  }
  
  for (int ix = 0; ix < TEST_SIZE; ix++) {
    
    if (hashTable->get(verify[ix],tmpInt) || (*tmpInt != verify[ix])) {
      hashTable->get(verify[ix], tmpInt);
      cout << "Index ";
      cout << ix;
      cout << " does not match!\n";
      //return -1;
    }
    
    //if ((ix % 4) == 3) {
    //  hashTable->remove(index);
    //}
  }
  
  for (int ix = 0; ix < TEST_SIZE; ix++) {
    
    if (hashTable->remove(verify[ix],tmpInt) || (*tmpInt != verify[ix])) {
      cout << "Index ";
      cout << ix;
      cout << " does not match!\n";
      return -1;
    }
    
    //if ((index % 4) != 3) {
    //  hashTable->remove(index);
    //}
  }
  
  delete tmpInt;
  delete hashTable;
  
  cout << "HashTable:Done\n";
  
  return 0;
}

int testSecureHashTable() {
  Collection::SecureHashTable<uint64_t,uint64_t> *hashTable;
  std::vector<uint64_t> verify;
  hashTable = new Collection::SecureHashTable<uint64_t,uint64_t>(TEST_SIZE);
  uint64_t *tmpInt = new uint64_t;
  uint64_t value;
  
  verify.resize(TEST_SIZE);
  
  cout << "\nTesting SecureHashTable.\n";
  
  for (uint64_t ix = 0; ix < TEST_SIZE; ix++) {
    
    do {
      value = random();
    } while (!hashTable->get(value, tmpInt));
    
    hashTable->insert(value, value);
    hashTable->get(value, tmpInt);
      
    verify[ix] = value;
  }
  
  for (uint64_t ix = 0; ix < TEST_SIZE; ix++) {
    
    if (hashTable->get(verify[ix],tmpInt) || (*tmpInt != verify[ix])) {
      hashTable->get(verify[ix], tmpInt);
      cout << "Index ";
      cout << ix;
      cout << " does not match!\n";
    }
    
    //if ((ix % 4) == 3) {
    //  hashTable->remove(index);
    //}
  }
  
  for (uint64_t ix = 0; ix < TEST_SIZE; ix++) {
    
    if (hashTable->remove(verify[ix],tmpInt) || (*tmpInt != verify[ix])) {
      cout << "Index ";
      cout << ix;
      cout << " does not match!\n";
    }
    
    //if ((index % 4) != 3) {
    //  hashTable->remove(index);
    //}
  }
  
  delete tmpInt;
  delete hashTable;
  
  cout << "SecureHashTable:Done\n";
  
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

  rbTree = new Graph::RedBlackTree<uint64_t, uint64_t>();
  arrayList = new Collection::ArrayList<uint64_t,uint64_t>(TEST_SIZE);
  
  for (uint64_t ix = 0; ix < TEST_SIZE; ix++) {
    uint64_t value = random();
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

int testLLRBTree() {
  Graph::LLRB_Tree<uint64_t,uint64_t> *rbTree;
  Collection::ArrayList<uint64_t,uint64_t> *arrayList;
  Graph::LLRB_TreeNode<uint64_t,uint64_t> *current;
  uint64_t tmp;
  uint64_t index = 0;
  
  cout << "\nTesting LLRB_Tree\n";
  
  rbTree = new Graph::LLRB_Tree<u_int64_t, uint64_t>();
  arrayList = new Collection::ArrayList<uint64_t,uint64_t>(TEST_SIZE);
  
  for (uint64_t ix = 0; ix < TEST_SIZE; ix++) {
    uint64_t value = random();
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
  
  cout << "LLRB_Tree:Done\n";
  
  
  
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

int testNeuralNetwork() {
  std::vector<double> *input = new std::vector<double>(INPUT_SIZE);
  NeuralNetwork::NeuralNetwork *NNetwork;
  double *reality = new double[OUTPUT_SIZE];
  double expectation;
  uint64_t iterations = 0;
  uint64_t tmpSize;
  std::vector<double *> *thisInput;
  double errorRate = 0.0;
  uint64_t endPraticeTests = (100 * ITERATIONS / 90);
  uint64_t nonPracticeTests = ITERATIONS - endPraticeTests;
  
  cout << "\nTesting NeuralNetwork\n";
  
  tmpSize = (random() % input->size()) + 1;
  thisInput = new std::vector<double *>(tmpSize);
  
  for (int jx = 0; jx < tmpSize; jx++) {
    thisInput->at(jx) = &input->at(jx);
  }
  
  NNetwork = new NeuralNetwork::NeuralNetwork(thisInput);
    
  do {
    for (int ix = 0; ix < 1 /*OUTPUT_SIZE*/; ix++) {
      for (int jx = 0; jx < thisInput->size(); jx++) {
        *(thisInput->at(jx)) = 0.1 * (random()%10);
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
  
  //NNetwork->getMaximumFlow();
  
  delete thisInput;
  
  cout << "NeuralNetwork:Done\n";
  
  return 0;
}

double calcDistance(double X, double Y, double Z, double tX, double tY, double tZ) {
  return sqrt(pow(X-tX,2)+pow(Y-tY,2)+pow(Z-tZ,2));
}

int testNavigation() {
  Graph::Navigation *navigation;
  vector<Graph::Coordinate *> locs;
  Graph::Coordinate *v, *u;
  Graph::Path *tmpPath;
  uint64_t width = 100;
  uint64_t length = 100;
  double distance;
  uint64_t neighborHops = 1;
  
  cout << "\nTesting Navigation\n";
  
  navigation = new Graph::Navigation();
  locs.resize(width*length);
  
  for (uint64_t ix = 0; ix < width; ix++) {
    for (uint64_t jx = 0; jx < length; jx++) {
      Graph::Coordinate *thisCoordinate = new Graph::Coordinate(ix,jx,0);
      navigation->add(thisCoordinate);
      
      locs[ix+(jx*length)] = thisCoordinate;
    }
  }
  
  for (uint64_t ix = neighborHops; ix < width-neighborHops; ix++) {
    for (uint64_t jx = neighborHops; jx < length-neighborHops; jx++) {
      u = locs[ix+(jx*width)];
      for (uint64_t kx = -neighborHops; kx <= neighborHops; kx ++) {
        for (uint64_t lx = -neighborHops; lx <= neighborHops; lx++) {
          if ((!kx) && (!lx)) {
            v = locs[(ix+kx)+((jx+lx)*width)];
            
            distance = calcDistance(v->X, v->Y, v->Z, u->X, u->Y, u->Z);
            tmpPath = new Graph::Path(v, u, distance);
            navigation->addEdgeObj(tmpPath);
          }
        }
      }
    }
  }
  
  for (int ix = 0; ix < neighborHops; ix++) {
    for (int jx = 0; jx < neighborHops; jx++) {
      u = locs[ix+(jx*width)];
      for (int kx = -ix; kx < width-ix; kx ++) {
        for (int lx = -jx; lx < length-jx; lx++) {
          if ((!kx) && (!lx)) {
            v = locs[(ix+kx)+((jx+lx)*width)];
            
            distance = calcDistance(v->X, v->Y, v->Z, u->X, u->Y, u->Z);
            tmpPath = new Graph::Path(v, u, distance);
            navigation->addEdgeObj(tmpPath);
          }
        }
      }
    }
  }
  
  
  navigation->setStart(locs[0]);
  navigation->setTerminal(locs[length*width-1]);
  
  navigation->getShortestPath();
  
  cout << "\nNavigation:Done\n";
  
  return 0;
}

int testMetaheuristic() {
  
  std::vector<double> *input = new std::vector<double>();
  std::vector<Heuristic *> *candidates = new std::vector<Heuristic *>();
  double *reality = new double[OUTPUT_SIZE];
  double expectation;
  uint64_t iterations = 0;
  uint64_t tmpSize;
  std::vector<double *> *thisInput = new std::vector<double *>();;
  
  input->resize(INPUT_SIZE);
  
  double errorRate = 0.0;
  uint64_t endPraticeTests = (100 * ITERATIONS / 90);
  uint64_t nonPracticeTests = ITERATIONS - endPraticeTests;
  
  cout << "\nTesting MetaHeuristic\n";
  
  candidates->resize(16);
  
  for (int ix = 0; ix < 16; ix ++) {
    tmpSize = (random() % input->size()) + 1;
    thisInput->resize(tmpSize);
    for (int jx = 0; jx < tmpSize; jx++) {
      thisInput->at(jx) = &input->at(jx);
    }
    
    candidates->at(ix) = new NeuralNetwork::NeuralNetwork(thisInput);
  }
  
  delete thisInput;
  
  Metaheuristic<double> *masterMind = new Metaheuristic<double>(input, candidates);
  
  do {
    for (int ix = 0; ix < 1 /*OUTPUT_SIZE*/; ix++) {
      for (int jx = 0; jx < input->size(); jx++) {
        input->at(jx) = 0.1 * (random()%10);
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
  ret |= testSecureHashTable();
  ret |= testArrayList();
  ret |= testHeap();
  //ret |= testRBTree();
  ret |= testLLRBTree();
  ret |= testStack();
  ret |= testNeuralNetwork();
  ret |= testMetaheuristic();
  ret |= testNavigation();
  
  cout << "Finished Testing:";
  cout << ret;
  cout << "\n";
  
  return ret;
}


