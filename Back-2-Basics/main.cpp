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

const uint64_t glbInputSize = 0x100;
const uint64_t glbOutputSize = 0x100;
const uint64_t glbIterations = 0x1000;
const uint64_t glbTestSize = 0x80000;


int testHashTable() {
  Collection::HashTable<uint64_t,uint64_t> *hashTable;
  std::vector<uint64_t> verify;
  hashTable = new Collection::HashTable<uint64_t,uint64_t>(glbTestSize);
  uint64_t *tmpInt = new uint64_t;
  uint64_t value;
  
  cout << "\nTesting HashTable.\n";
  
  verify.resize(glbTestSize);
  
  for (int ix = 0; ix < glbTestSize; ix++) {
    
    do {
      value = random();
    } while (!hashTable->get(value, tmpInt));
    
    hashTable->insert(value, value);
    
    verify[ix] = value;
  }
  
  for (int ix = 0; ix < glbTestSize; ix++) {
    
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
  
  for (int ix = 0; ix < glbTestSize; ix++) {
    
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
  hashTable = new Collection::SecureHashTable<uint64_t,uint64_t>(glbTestSize);
  uint64_t *tmpInt = new uint64_t;
  uint64_t value;
  
  verify.resize(glbTestSize);
  
  cout << "\nTesting SecureHashTable.\n";
  
  for (uint64_t ix = 0; ix < glbTestSize; ix++) {
    
    do {
      value = random();
    } while (!hashTable->get(value, tmpInt));
    
    hashTable->insert(value, value);
    hashTable->get(value, tmpInt);
    
    verify[ix] = value;
  }
  
  for (uint64_t ix = 0; ix < glbTestSize; ix++) {
    
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
  
  for (uint64_t ix = 0; ix < glbTestSize; ix++) {
    
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
  
  for (int ix = 0; ix < glbTestSize; ix++) {
    uint64_t value = random();
    heap->push(value, value);
  }
  
  if (heap->peek(0) != (tmp = heap->pop())) {
    cout << "Heap Error!\n";
  }
  
  for (int ix = 1; ix < glbTestSize; ix++) {
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
  arrayList = new Collection::ArrayList<uint64_t,uint64_t>(glbTestSize);
  
  for (uint64_t ix = 0; ix < glbTestSize; ix++) {
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
  arrayList = new Collection::ArrayList<uint64_t,uint64_t>(glbTestSize);
  
  for (uint64_t ix = 0; ix < glbTestSize; ix++) {
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
  
  arrayList = new Collection::ArrayList<uint64_t,uint64_t>(glbTestSize);
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
  
  arrayList = new Collection::ArrayList<uint64_t,uint64_t>(glbTestSize);
  
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
  std::vector<double> *input = new std::vector<double>(glbInputSize);
  NeuralNetwork::NeuralNetwork *NNetwork;
  double *reality = new double[glbOutputSize];
  std::vector<double> *expectation;
  uint64_t iterations = 0;
  std::vector<double *> *thisInput;
  double errorRate = 0.0;
  uint64_t endPraticeTests = (100 * glbIterations / 90);
  uint64_t nonPracticeTests = glbIterations - endPraticeTests;
  std::vector<uint64_t> *layers = new std::vector<uint64_t>();
  uint64_t precision = (1 << 16);
  
  layers->resize(3);
  
  layers->at(0) = glbOutputSize/16;
  layers->at(1) = glbOutputSize/16;
  layers->at(2) = glbOutputSize;
  
  cout << "\nTesting NeuralNetwork\n";
  
  thisInput = new std::vector<double *>(glbInputSize);
  
  for (int jx = 0; jx < glbInputSize; jx++) {
    thisInput->at(jx) = &input->at(jx);
  }
  
  NNetwork = new NeuralNetwork::NeuralNetwork(thisInput, layers);
  
  for (uint64_t jx = 0; jx < thisInput->size(); jx++) {
    *(thisInput->at(jx)) = (rand() % precision) / (precision * 1.0);
  }
  
  do {
    /*for (uint64_t jx = 0; jx < thisInput->size(); jx++) {
      *(thisInput->at(jx)) = (rand() % 256) / 256.0;
    }*/
    
    
    NNetwork->calculateExpectation();
    expectation = NNetwork->getExpectation();
    
    for (int ix = 0; ix < glbOutputSize; ix++) {
      reality[ix] = *(thisInput->at(ix));
    }
    
    NNetwork->doCorrection(reality, 0.01);
    
    if (iterations > endPraticeTests) {
      for (int ix = 0; ix < glbOutputSize; ix++) {
        errorRate += ((reality[ix] - expectation->at(ix)) * (reality[ix] - expectation->at(ix))) * 1000000.0;
      }
    }
    iterations++;
  } while (iterations < glbIterations);
  
  for (int ix = 0; ix < glbOutputSize; ix++) {
    cout << "{";
    cout << (uint64_t)(reality[ix] * precision);
    cout << ":";
    cout << (uint64_t)(expectation->at(ix) * precision);
    cout << "},";
  }
  
  
  if (nonPracticeTests) {
    errorRate /= (double)(nonPracticeTests * glbOutputSize);
    errorRate = sqrt(errorRate);
    cout << "Error Rate is ";
    cout << errorRate;
    cout << "PPM\n";
  }
  
  
  //NNetwork->g
  NNetwork->getMaximumFlow();
  
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
  Collection::Stack<Graph::Path *> *shortPath;
  uint64_t width = 100;
  uint64_t length = 100;
  double distance;
  int neighborHops = 1;
  
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
  
  for (uint64_t ix = neighborHops; ix < (width-neighborHops); ix++) {
    for (uint64_t jx = neighborHops; jx < (length-neighborHops); jx++) {
      u = locs[ix+(jx*width)];
      for (int kx = (-neighborHops); kx <= neighborHops; kx ++) {
        for (int lx = (-neighborHops); lx <= neighborHops; lx++) {
          if ((jx == (length/2)) && (ix < (width*0.8)) && (ix > (width*0.2))) {
            continue;
          }
          
          if ((kx) || (lx)) {
            v = locs[(ix+kx)+((jx+lx)*width)];
            
            distance = calcDistance(v->X, v->Y, v->Z, u->X, u->Y, u->Z);
            navigation->addEdge(v, u, distance);
          }
        }
      }
    }
  }
  
  /* Width sides */
  
  for (uint64_t ix = 0; ix < neighborHops; ix++) {
    for (uint64_t jx = neighborHops; jx < (length-neighborHops); jx++) {
      u = locs[ix+(jx*width)];
      for (uint64_t kx = 0; kx < (ix+neighborHops); kx ++) {
        for (int lx = (-neighborHops); lx < neighborHops; lx++) {
          if ((kx != ix) || (lx)) {
            v = locs[(kx)+((jx+lx)*width)];
            
            distance = calcDistance(v->X, v->Y, v->Z, u->X, u->Y, u->Z);
            navigation->addEdge(v, u, distance);
          }
        }
      }
    }
  }
  
  for (uint64_t ix = (width-neighborHops); ix < width; ix++) {
    for (uint64_t jx = neighborHops; jx < (length-neighborHops); jx++) {
      u = locs[ix+(jx*width)];
      for (uint64_t kx = (ix-neighborHops); kx < width; kx ++) {
        for (int lx = (-neighborHops); lx < neighborHops; lx++) {
          if ((kx != ix) || (lx)) {
            v = locs[(kx)+((jx+lx)*width)];
            
            distance = calcDistance(v->X, v->Y, v->Z, u->X, u->Y, u->Z);
            navigation->addEdge(v, u, distance);
          }
        }
      }
    }
  }
  
  /* length sides */
  
  for (uint64_t ix = neighborHops; ix < (width-neighborHops); ix++) {
    for (uint64_t jx = 0; jx < neighborHops; jx++) {
      u = locs[ix+(jx*width)];
      for (int kx = (-neighborHops); kx < neighborHops; kx++) {
        for (uint64_t lx = 0; lx < (jx+neighborHops); lx++) {
          if ((kx) || (lx != jx)) {
            v = locs[(ix+kx)+((lx)*width)];
            
            distance = calcDistance(v->X, v->Y, v->Z, u->X, u->Y, u->Z);
            navigation->addEdge(v, u, distance);
          }
        }
      }
    }
  }
  
  for (uint64_t ix = neighborHops; ix < (width-neighborHops); ix++) {
    for (uint64_t jx = (length-neighborHops); jx < length; jx++) {
      u = locs[ix+(jx*width)];
      for (int kx = (-neighborHops); kx < neighborHops; kx ++) {
        for (uint64_t lx = (jx-neighborHops); lx < length; lx++) {
          if ((kx) || (lx != jx)) {
            v = locs[(ix+kx)+((lx)*width)];
            
            distance = calcDistance(v->X, v->Y, v->Z, u->X, u->Y, u->Z);
            navigation->addEdge(v, u, distance);
          }
        }
      }
    }
  }
  
  
  navigation->setStart(locs[(neighborHops)+((neighborHops) * width)]);
  navigation->setTerminal(locs[length*width-1]);
  
  if ((shortPath = navigation->getShortestPath()) != NULL) {
    while (shortPath->peek() != NULL) {
      tmpPath = shortPath->pop();
      u = tmpPath->getBackward();
      v = tmpPath->getForward();
      
      cout << "{";
      cout << u->X;
      cout << ":";
      cout << u->Y;
      cout << ":";
      cout << u->Z;
      cout << "},";
    }
    
    cout << "{";
    cout << v->X;
    cout << ":";
    cout << v->Y;
    cout << ":";
    cout << v->Z;
    cout << "},";
  }
  
  cout << "Navigation:Done\n";
  
  return 0;
}

int testMetaheuristic() {
  
  std::vector<double> *input = new std::vector<double>();
  std::vector<Heuristic *> *candidates = new std::vector<Heuristic *>();
  double *reality = new double[glbOutputSize];
  std::vector<double> *expectation = new std::vector<double>();
  uint64_t iterations = 0;
  uint64_t tmpSize;
  std::vector<double *> *thisInput = new std::vector<double *>();;
  
  input->resize(glbInputSize);
  
  double errorRate = 0.0;
  uint64_t endPraticeTests = (100 * glbIterations / 90);
  uint64_t nonPracticeTests = glbIterations - endPraticeTests;
  std::vector<uint64_t> *layers = new std::vector<uint64_t>();
  
  layers->resize(3);
  
  layers->at(0) = glbOutputSize/8;
  layers->at(1) = glbOutputSize/8;
  layers->at(2) = glbOutputSize;
  
  cout << "\nTesting MetaHeuristic\n";
  
  candidates->resize(16);
  expectation->resize(glbOutputSize);
  
  for (int ix = 0; ix < 16; ix ++) {
    tmpSize = (random() % input->size()) + 1;
    thisInput->resize(tmpSize);
    for (int jx = 0; jx < tmpSize; jx++) {
      thisInput->at(jx) = &input->at(jx);
    }
    
    candidates->at(ix) = new NeuralNetwork::NeuralNetwork(thisInput, layers);
  }
  
  delete thisInput;
  
  Metaheuristic<double> *masterMind = new Metaheuristic<double>(input, candidates);
  
  do {
    for (int ix = 0; ix < 1 /*glbOutputSize*/; ix++) {
      for (int jx = 0; jx < input->size(); jx++) {
        input->at(jx) = 0.1 * (random()%10);
      }
      masterMind->getConsensus(expectation);
      reality[ix] = 1.0;
      /*if ((input->atIndex(ix%glbInputSize)) +
       (input->atIndex((ix+1)%glbInputSize)) +
       (input->atIndex((ix+2)%glbInputSize)) > 2.5) {
       reality[ix] = 1.0;
       } else if ((input->atIndex(ix%glbInputSize)) +
       (input->atIndex((ix+1)%glbInputSize)) +
       (input->atIndex((ix+2)%glbInputSize)) < 0.5) {
       reality[ix] = 0.0;
       }*/
      masterMind->postResult(reality[ix]);
      
      if (iterations > endPraticeTests) {
        errorRate += ((reality[ix] - expectation->at(ix)) * (reality[ix] - expectation->at(ix))) * 1000000.0;
      }
    }
    iterations++;
  } while (iterations < glbIterations);
  
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
  //ret |= testMetaheuristic();
  ret |= testNavigation();
  
  cout << "Finished Testing:";
  cout << ret;
  cout << "\n";
  
  return ret;
}


