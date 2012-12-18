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

const uint64_t glbInputSize = 0x1000;
const uint64_t glbOutputSize = 0x1000;
const uint64_t glbIterations = 0x10000;
const uint64_t glbTestSize = 0x10000;
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


int testLLRBTree() {
  LLRB_Tree<uint64_t,uint64_t> *rbTree;
  vector<uint64_t> *arrayList;
  LLRB_TreeNode<uint64_t,uint64_t> *current;
  uint64_t tmp;
  
  cout << "\nTesting LLRB_Tree\n";
  
  rbTree = new LLRB_Tree<u_int64_t, uint64_t>();
  arrayList = new vector<uint64_t>();
  
  for (uint64_t ix = 0; ix < glbTestSize; ix++) {
    uint64_t value;
    do {
      value = random();
      tmp = rbTree->size();
      rbTree->insert(value, value);
    } while (rbTree->size() == tmp);
    
    arrayList->push_back(value);
#if 0
    current = rbTree->min(rbTree->treeRoot);
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
  
  current = rbTree->min(rbTree->treeRoot);
  tmp = current->key;
  
  cout << rbTree->size();
  cout << ":size\n";
  
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
  
  for (uint64_t ix = 0; ix < arrayList->size(); ix+=log2(arrayList->size())) {
    
    uint64_t curr = arrayList->at(ix);
    
    tmp = rbTree->size();
    
    rbTree->remove(curr,curr);
    
    if (tmp == rbTree->size()) {
      cout << "Nothing Removed!\n";
    }
#if 0
    current = rbTree->min(rbTree->treeRoot);
    tmp = current->key;
    
    for (uint64_t jx = 0; jx < rbTree->size()-1; jx++) {
      current = rbTree->next(current);
      
      if (current->key < tmp) {
        cout << "Removal/Index/Key:";
        cout << ix;
        cout << "/";
        cout << jx;
        cout << "/";
        cout << tmp;
        cout << " not sorted!\n";
      }
      
      tmp = current->key;
      
    }
    
#endif
  }
  
  current = rbTree->min(rbTree->treeRoot);
  tmp = current->key;
  
  cout << rbTree->size();
  cout << ":size\n";
  
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
  std::vector<double *> *thisInput;
  LLRB_Tree<double *, uint64_t> *thisExpect;
  std::vector<Trust<double> *> *thisOutput;
  double errorRate = 0.0;
  double thisError;
  std::vector<Info *> *hiddenInfo = new std::vector<Info *>();
  uint64_t precision = (1 << 16);
  
  
  for (uint32_t ix = 0; ix < glbInputSize; ix++) {
    for (uint32_t jx = 0; jx < log2(glbOutputSize); jx++) {
      Info *info = new Info;
      info->c.inputPosition = ix;
      info->c.position = (4 << LAYER_SHIFT) | jx;
      hiddenInfo->push_back(info);
    }
  }
  
  for (uint32_t ix = 0; ix < log2(glbOutputSize); ix++) {
    for (uint32_t jx = 0; jx < glbOutputSize; jx++) {
      Info *info = new Info;
      info->c.inputPosition = (4 << LAYER_SHIFT) | jx;
      info->c.position = (7 << LAYER_SHIFT) | jx;
      hiddenInfo->push_back(info);
    }
  }
  
  cout << "\nTesting NeuralNetwork\n";
  
  thisInput = new std::vector<double *>();
  thisOutput = new std::vector<Trust<double> *>();
  thisExpect = new LLRB_Tree<double *, uint64_t>();
  
  thisInput->resize(glbInputSize);
  thisOutput->resize(glbOutputSize);  
  
  for (int jx = 0; jx < glbInputSize; jx++) {
    thisInput->at(jx) = new double();
    thisOutput->at(jx)->actual = new double();
  }
  
  for (uint64_t jx = 0; jx < glbOutputSize; jx++) {
    thisExpect->insert(new double(), jx);
  }
  
  NNetwork = new NeuralNetwork::NeuralNetwork(thisInput, thisOutput, thisExpect, hiddenInfo, thisOutput->size());
  
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
        cout << (*thisOutput->at(ix)->actual);
        cout << ":";
        cout << (*thisExpect->search(ix));
        cout << "},";
      }
      cout << "\n\n";
    }
    
    for (int ix = 0; ix < glbOutputSize; ix++) {
      *(thisOutput->at(ix)->actual) = *(thisInput->at(ix));
    }
    
    
    if (!(iterations % (uint64_t)(2*log2(glbIterations)))) {
      errorRate = 0.0;
      
      for (int ix = 0; ix < glbOutputSize; ix++) {
        thisError = (*thisOutput->at(ix)->actual - *(thisExpect->search(ix))) / *thisOutput->at(ix)->actual;
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
  Optimization::Genetic<NeuralNetwork::NeuralNetwork,NeuralNetwork::Neuron,double> *geneticExp;
  uint64_t iterations = 0;
  std::vector<double *> *thisInput, *thisOutput, *thisObjective;
  std::vector<Trust<double> *> *thisTrust;
  double errorRate = 0.0;
  double thisError;
  std::vector<uint64_t> *layers = new std::vector<uint64_t>();
  uint64_t precision = (1 << 16);
  uint64_t hShift = 0, wShift = 0;
  uint64_t width = sqrt(glbOutputSize);
  uint64_t height = glbOutputSize / width;
  
  layers->resize(0);
  
  for (uint64_t ix = 0; ix < 1; ix++) {
    layers->push_back(log2(glbOutputSize));
  }
  
  cout << "\nTesting Genetic\n";
  
  thisInput = new std::vector<double *>();
  thisOutput = new std::vector<double *>();
  thisObjective = new std::vector<double *>();
  
  thisInput->resize(glbInputSize);
  thisOutput->resize(glbOutputSize);
  thisObjective->resize(glbOutputSize);
  
  
  for (int jx = 0; jx < glbInputSize; jx++) {
    thisInput->at(jx) = new double();
    thisOutput->at(jx) = new double();
    thisObjective->at(jx) = new double();
  }
  
  geneticExp = new Optimization::Genetic<NeuralNetwork::NeuralNetwork,NeuralNetwork::Neuron,double>();
  
  geneticExp->addInput(thisInput);
  geneticExp->addOutput(thisOutput);
  
  geneticExp->initInternals();
  
  
  for (int ix = 0; ix < glbOutputSize; ix++) {
    *(thisObjective->at(ix)) = max(((random() % precision) / (precision * 1.0)), 0.05);
    *(thisOutput->at(ix)) = *(thisObjective->at(ix)) + (((rand()/(double)RAND_MAX)*0.1) - 0.05);
  }
  
  do {
#if 1
    for (uint64_t jx = 0; jx < thisInput->size(); jx++) {
      *(thisInput->at(jx)) = *(thisOutput->at(jx));
    }
#endif
    
#if 1
    for (int ix = 0; ix < glbOutputSize; ix++) {
      *(thisOutput->at(ix)) = *(thisObjective->at(((ix+(wShift%width)+(width*(hShift%height))))%glbOutputSize)) + (((rand()/(double)RAND_MAX)*0.1) - 0.05);
    }
#endif
    
    geneticExp->optimizeAnwser();
    
    /* place objective reality for error rate */
    for (int ix = 0; ix < glbOutputSize; ix++) {
      *(thisOutput->at(ix)) = *(thisObjective->at(ix));
    }
    
    thisTrust = geneticExp->answer.select(NULL,NULL);
    
    if (iterations > (glbIterations-4)) {
      for (int ix = 0; ix < glbOutputSize; ix++) {
        cout << "{";
        cout << (*thisTrust->at(ix)->actual);
        cout << ":";
        if (thisTrust->at(ix)->prediction == NULL) {
          cout << "NONE:NONE";
        } else {
          cout << (thisTrust->at(ix)->prediction->expectation);
          cout << ":";
          cout << (thisTrust->at(ix)->prediction->confidence);
        }
        cout << "},";
      }
      cout << "\n\n";
    }
    
    
    if (!(iterations % (uint64_t)(2*log2(glbIterations)))) {
      errorRate = 0.0;
      
      for (int ix = 0; ix < glbOutputSize; ix++) {
        if (thisTrust->at(ix)->prediction == NULL) {
          continue;
        }
        
        thisError = (*thisTrust->at(ix)->actual - (thisTrust->at(ix)->prediction->expectation));
        errorRate += thisError * thisError;
      }
      
      errorRate = sqrt(errorRate/((double)(glbOutputSize)));
      
      cout << "Error Rate: ";
      cout << errorRate;
      cout << "\n";
      cout << "Size/Iteration: ";
      cout << geneticExp->candidates.size();
      cout << "/";
      cout << iterations;
      cout << "\n";
      
    }
    
    geneticExp->error_rate = errorRate;
    
    iterations++;
    /*hShift++;
    hShift %= 3;
    wShift++;
    wShift %= 3;
    */
  } while (iterations < glbIterations);
  
  
  delete thisInput;
  
  cout << "Genetic:Done\n";
  
  return 0;
}

int testNetworkFlow() {
  Network<SimpleHub, SimplePipe> *network;
  SimpleHub *start, *terminal;
  const uint64_t xSize = 1920;
  const uint64_t ySize = 1200;
//  const uint64_t tSize = 60;
  const int xDist = log2(xSize);
  const int yDist = log2(ySize);
  const double a = 1, xO = 1.0, yO = 1.0;
  double capacity[(2*xDist)+1][(2*yDist)+1];
  SimpleHub *hubs[glbSlowTestSize][glbSlowTestSize];
  
  network = new Network<SimpleHub, SimplePipe>();
  network->setStart(start = new SimpleHub());
  network->setTerminal(terminal = new SimpleHub);
  
  for (int ix = 0; ix < ((2*xDist)+1); ix++) {
    for (int jx = 0; jx < ((2*yDist)+1); jx++) {
      capacity[ix][jx] = a * exp(-((pow(ix-xDist, 2)/(2*pow(xO, 2))) + (pow(jx-yDist, 2)/(2*pow(yO, 2)))));
    }
  }
  
  for (int ix = 0; ix < xSize; ix++) {
    for (int jx = 0; jx < glbSlowTestSize; jx++) {
      hubs[ix][jx] = new SimpleHub();
    }
  }
  
  for (int ix = 0; ix < xSize; ix++) {
    for (int jx = 0; jx < glbSlowTestSize; jx++) {
      for (int xOff = -xDist; xOff <= xDist; xOff++) {
        if ((ix + xOff < 0) || (ix + xOff >= xSize)) {
          continue;
        }
        for (int yOff = -yDist; yOff <= yDist; yOff++) {
          if ((jx + yOff < 0) || (jx + yOff >= xSize)) {
            continue;
          }
          new Pipe<SimpleHub,SimplePipe>(hubs[ix+xOff][jx+yOff],hubs[ix][jx],capacity[xOff+xDist][yOff+yDist]);
        }
      }
    }
  }
  
  
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
  
  //ret |= testHashTable();
  //ret |= testSecureHashTable();
  //ret |= testArrayList();
  //ret |= testHeap();
  //ret |= testRBTree();
  ret |= testLLRBTree();
  //ret |= testStack();
  //ret |= testNeuralNetwork();
  ret |= testGenetic();
  //ret |= testMetaheuristic();
  //ret |= testNavigation();
  //ret |= testNetworkFlow();
  
  cout << "Finished Testing:";
  cout << ret;
  cout << "\n";
  
  return ret;
}


