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
using namespace Collection;
#include "Heap.h"
#include "HashTable.h"
#include "SecureHashTable.h"
#include "Graph.h"
using namespace Graph;
#include "LLRB_Tree.h"
#include "Navigation.h"
#include "Network.h"
#include "NeuralNetwork.h"
#include "Genetic.h"
#include "Metaheuristic.h"

const uint64_t glbInputSize = 0x80;
const uint64_t glbOutputSize = 0x80;
const uint64_t glbIterations = 0x1000;
const uint64_t glbTestSize = 0x100000;
const uint64_t glbSlowTestSize = 0x10000;


int testHashTable() {
  HashTable<uint64_t,uint64_t> *hashTable;
  std::vector<uint64_t> verify;
  hashTable = new HashTable<uint64_t,uint64_t>(glbTestSize);
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
  SecureHashTable<uint64_t,uint64_t> *hashTable;
  std::vector<uint64_t> verify;
  hashTable = new SecureHashTable<uint64_t,uint64_t>(glbSlowTestSize);
  uint64_t *tmpInt = new uint64_t;
  uint64_t value;
  
  verify.resize(glbSlowTestSize);
  
  cout << "\nTesting SecureHashTable.\n";
  
  for (uint64_t ix = 0; ix < glbSlowTestSize; ix++) {
    
    do {
      value = random();
    } while (!hashTable->get(value, tmpInt));
    
    hashTable->insert(value, value);
    hashTable->get(value, tmpInt);
    
    verify[ix] = value;
  }
  
  for (uint64_t ix = 0; ix < glbSlowTestSize; ix++) {
    
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
  
  for (uint64_t ix = 0; ix < glbSlowTestSize; ix++) {
    
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
  Heap<uint64_t,uint64_t> *heap;
  heap = new Heap<uint64_t,uint64_t>();
  
  uint64_t tmp, tmp1;
  
  cout << "\nTesting Heap.\n";
  
  for (int ix = 0; ix < glbTestSize; ix++) {
    uint64_t value = random();
    heap->push(value, value);
  }
  
  if (heap->at(0) != (tmp = heap->pop())) {
    cout << "Heap Error!\n";
  }
  
  for (int ix = 1; ix < glbTestSize; ix++) {
    if (heap->at(0) != (tmp1 = heap->pop())) {
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
  Stack<uint64_t> *stack;
  stack = new Stack<uint64_t>();
  
  return 0;
}

int testLLRBTree() {
  LLRB_Tree<uint64_t,uint64_t> *rbTree;
  ArrayList<uint64_t,uint64_t> *arrayList;
  LLRB_TreeNode<uint64_t,uint64_t> *current;
  uint64_t tmp;
  
  cout << "\nTesting LLRB_Tree\n";
  
  rbTree = new LLRB_Tree<u_int64_t, uint64_t>();
  arrayList = new ArrayList<uint64_t,uint64_t>(glbTestSize);
  
  for (uint64_t ix = 0; ix < glbTestSize; ix++) {
    do {
      uint64_t value = random();
      tmp = rbTree->size();
      rbTree->insert(value, value);
    } while (rbTree->size() == tmp);
    
#if 0
    current = rbTree->min(rbTree->getTreeRoot());
    tmp = current->key;
    
    for (uint64_t ix = 0; ix < rbTree->size()-1; ix++) {
      current = rbTree->next(current);
      
      if (current->key < tmp) {
        cout << "Index/Key:";
        cout << ix;
        cout << "/";
        cout << tmp;
        cout << " not sorted!\n";
      }
      
      tmp = current->key;
      
    }
    
#endif
  }
  
  current = rbTree->min(rbTree->getTreeRoot());
  tmp = current->key;
  
  for (uint64_t ix = 0; ix < rbTree->size()-1; ix++) {
    current = rbTree->next(current);
    
    if (current->key < tmp) {
      cout << "Index/Key:";
      cout << ix;
      cout << "/";
      cout << tmp;
      cout << " not sorted!\n";
    }
    
    tmp = current->key;
    
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
  ArrayList<uint64_t,uint64_t> *arrayList, *arrayListClone;
  arrayList = new ArrayList<uint64_t,uint64_t>();
  uint64_t *verify = new uint64_t[arrayList->getSize()];
  uint64_t tmp;
  
  cout << "\nTesting ArrayList\n";
  
  for (int ix = 0; ix < arrayList->getSize(); ix++) {
    uint64_t value = random();
    arrayList->setIndex(ix, new Comparable<uint64_t,uint64_t>(value, value));
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
  
  arrayList = new ArrayList<uint64_t,uint64_t>(glbTestSize);
  verify = new uint64_t[arrayList->getSize()];
  
  for (int ix = 0; ix < arrayList->getSize(); ix++) {
    uint64_t value = random();
    arrayList->setIndex(ix, new Comparable<uint64_t,uint64_t>(value, value));
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
  
  arrayList = new ArrayList<uint64_t,uint64_t>(glbTestSize);
  
  for (int ix = 0; ix < arrayList->getSize(); ix++) {
    uint64_t value = random();
    arrayList->setIndex(ix, new Comparable<uint64_t,uint64_t>(value, value));
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
  NeuralNetwork::NeuralNetwork *NNetwork;
  uint64_t iterations = 0;
  std::vector<double *> *thisInput, *thisOutput, *thisExpect;
  double errorRate = 0.0;
  double thisError;
  std::vector<uint64_t> *layers = new std::vector<uint64_t>();
  uint64_t precision = (1 << 16);
  
  layers->resize(0);
  
  for (uint64_t ix = 0; ix < 1; ix++) {
    layers->push_back(log2(glbOutputSize));
  }
  
  cout << "\nTesting NeuralNetwork\n";
  
  thisInput = new std::vector<double *>();
  thisOutput = new std::vector<double *>();
  thisExpect = new std::vector<double *>();
  
  thisInput->resize(glbInputSize);
  thisOutput->resize(glbOutputSize);
  thisExpect->resize(glbOutputSize);
  
  
  for (int jx = 0; jx < glbInputSize; jx++) {
    thisInput->at(jx) = new double();
    thisOutput->at(jx) = new double();
  }
  
  for (uint64_t jx = 0; jx < glbOutputSize; jx++) {
    thisExpect->at(jx) = new double();
  }
  
  NNetwork = new NeuralNetwork::NeuralNetwork(thisInput, thisOutput, thisExpect, layers);
  
  for (uint64_t jx = 0; jx < thisInput->size(); jx++) {
    *(thisInput->at(jx)) = (random() % precision) / (precision * 1.0);
  }
  
  do {
    /*for (uint64_t jx = 0; jx < thisInput->size(); jx++) {
     *(thisInput->at(jx)) = (rand() % 256) / 256.0;
     }*/
    
    
    NNetwork->calcExpectation(iterations);
    
    if (iterations > (glbIterations-4)) {
      for (int ix = 0; ix < glbOutputSize; ix++) {
        cout << "{";
        cout << (*thisOutput->at(ix));
        cout << ":";
        cout << (*thisExpect->at(ix));
        cout << "},";
      }
      cout << "\n\n";
    }
    
    for (int ix = 0; ix < glbOutputSize; ix++) {
      *(thisOutput->at(ix)) = *(thisInput->at(ix));
    }
    
    
    if (!(iterations % (uint64_t)(2*log2(glbIterations)))) {
      errorRate = 0.0;
      
      for (int ix = 0; ix < glbOutputSize; ix++) {
        thisError = (*thisOutput->at(ix) - *(thisExpect->at(ix))) / *thisOutput->at(ix);
        errorRate += thisError * thisError;
      }
      
      cout << "Error Rate is ";
      cout << sqrt(errorRate/((double)(glbOutputSize)));
      cout << "\n";
    }
    
    NNetwork->doCorrection();
    
    iterations++;
    
  } while (iterations < glbIterations);
  
  
  //NNetwork->g
  //NNetwork->getMaximumFlow();
  
  delete thisInput;
  
  cout << "NeuralNetwork:Done\n";
  
  return 0;
}

int testNavigation() {
  Navigation *navigation;
  vector<Coordinate *> locs;
  Coordinate *v, *u;
  Path *tmpPath;
  vector<Path *> *shortPath;
  uint64_t width = 100;
  uint64_t length = 100;
  double distance;
  int neighborHops = 1;
  
  cout << "\nTesting Navigation\n";
  
  navigation = new Navigation();
  locs.resize(width*length);
  
  for (uint64_t ix = 0; ix < width; ix++) {
    for (uint64_t jx = 0; jx < length; jx++) {
      Coordinate *thisCoordinate = new Coordinate(ix,jx,0);
      
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
            
            distance = Navigation::calcDistance(v->X, v->Y, v->Z, u->X, u->Y, u->Z);
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
            
            distance = Navigation::calcDistance(v->X, v->Y, v->Z, u->X, u->Y, u->Z);
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
            
            distance = Navigation::calcDistance(v->X, v->Y, v->Z, u->X, u->Y, u->Z);
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
            
            distance = Navigation::calcDistance(v->X, v->Y, v->Z, u->X, u->Y, u->Z);
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
            
            distance = Navigation::calcDistance(v->X, v->Y, v->Z, u->X, u->Y, u->Z);
            navigation->addEdge(v, u, distance);
          }
        }
      }
    }
  }
  
  
  navigation->setStart(locs[(neighborHops)+((neighborHops) * width)]);
  navigation->setTerminal(locs[length*width-1]);
  
  if ((shortPath = navigation->getShortestPath()) != NULL) {
    cout << "Path Hops ";
    cout << shortPath->size();
    while ((!shortPath->empty()) && ((tmpPath = shortPath->back()) != NULL)) {
      u = tmpPath->getBackward();
      v = tmpPath->getForward();
      
      cout << "{";
      cout << u->X;
      cout << ":";
      cout << u->Y;
      cout << ":";
      cout << u->Z;
      cout << "},";
      shortPath->resize(shortPath->size()-1);
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

int testGenetic() {
  Optimization::Genetic<NeuralNetwork::NeuralNetwork, double> *geneticExp;
  uint64_t iterations = 0;
  std::vector<double *> *thisInput, *thisOutput, *thisExpect;
  double errorRate = 0.0;
  double thisError;
  std::vector<uint64_t> *layers = new std::vector<uint64_t>();
  uint64_t precision = (1 << 16);
  
  layers->resize(0);
  
  for (uint64_t ix = 0; ix < 1; ix++) {
    layers->push_back(log2(glbOutputSize));
  }
  
  cout << "\nTesting Genetic\n";
  
  thisInput = new std::vector<double *>();
  thisOutput = new std::vector<double *>();
  thisExpect = new std::vector<double *>();
  
  thisInput->resize(glbInputSize);
  thisOutput->resize(glbOutputSize);
  thisExpect->resize(glbOutputSize);
  
  
  for (int jx = 0; jx < glbInputSize; jx++) {
    thisInput->at(jx) = new double();
    thisOutput->at(jx) = new double();
  }
  
  for (uint64_t jx = 0; jx < glbOutputSize; jx++) {
    thisExpect->at(jx) = new double();
  }
  
  geneticExp = new Optimization::Genetic<NeuralNetwork::NeuralNetwork, double>();
  
  geneticExp->addInput(thisInput);
  geneticExp->addOutput(thisOutput);
  
  for (uint64_t jx = 0; jx < thisInput->size(); jx++) {
    *(thisInput->at(jx)) = (random() % precision) / (precision * 1.0);
  }
  
  do {
    /*for (uint64_t jx = 0; jx < thisInput->size(); jx++) {
     *(thisInput->at(jx)) = (rand() % 256) / 256.0;
     }*/
    
    
    geneticExp->optimizeAnwser();
    
    if (iterations > (glbIterations-4)) {
      for (int ix = 0; ix < glbOutputSize; ix++) {
        cout << "{";
        cout << (*thisOutput->at(ix));
        cout << ":";
        cout << (*thisExpect->at(ix));
        cout << "},";
      }
      cout << "\n\n";
    }
    
    for (int ix = 0; ix < glbOutputSize; ix++) {
      *(thisOutput->at(ix)) = *(thisInput->at(ix));
    }
    
    
    if (!(iterations % (uint64_t)(2*log2(glbIterations)))) {
      errorRate = 0.0;
      
      for (int ix = 0; ix < glbOutputSize; ix++) {
        thisError = (*thisOutput->at(ix) - *(thisExpect->at(ix))) / *thisOutput->at(ix);
        errorRate += thisError * thisError;
      }
      
      cout << "Error Rate is ";
      cout << sqrt(errorRate/((double)(glbOutputSize)));
      cout << "\n";
    }
    
    iterations++;
    
  } while (iterations < glbIterations);
  
  
  //NNetwork->g
  //NNetwork->getMaximumFlow();
  
  delete thisInput;
  
  cout << "Genetic\n";
  
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
  ret |= testGenetic();
  //ret |= testMetaheuristic();
  ret |= testNavigation();
  
  cout << "Finished Testing:";
  cout << ret;
  cout << "\n";
  
  return ret;
}


