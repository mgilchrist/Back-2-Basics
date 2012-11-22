//
//  Stochastic.h
//  Back-2-Basics
//
//  Created by Marcus Gilchrist on 9/16/12.
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

#ifndef OpenSource_Stochastic_h
#define OpenSource_Stochastic_h

#include "LLRB_Tree.h"
using namespace Graph;
#include "Heap.h"
using namespace Collection;
#include "Optimization.h"

namespace Optimization {
  
  
  
  typedef struct TotalCompetition {
    double competition;
    uint64_t numCompetitors;
  } TotalCompetition;
  
  template <class HeuristicType, class DataType>
  class Stoichastic : public Optimization<HeuristicType,DataType> {
    
  private:
    
    LLRB_Tree<TotalCompetition *, uint64_t> *competitionInArea;
    uint64_t iteration = 0;
    
  protected:
    
    static uint64_t doEpochEach(LLRB_TreeNode<HeuristicType *,uint64_t> *current, void *iteration) {
      
      current->data->calcExpectation(*(uint64_t *)iteration);
      
      return current->key;
    }
    
    void doEpoch();
    void initInternals();
    
    vector<DataType *> *pickRandoms(LLRB_Tree<DataType *, uint64_t> *, uint64_t);
    vector<Trust<DataType> *> *pickRandomTrusts(LLRB_Tree<Trust<DataType> *, uint64_t> *, uint64_t);
    HeuristicType *spawn();
    
  public:
    Stoichastic() {
      competitionInArea = new LLRB_Tree<TotalCompetition *, uint64_t>();
    }
  };
  
  
  template <class HeuristicType, class DataType>
  void Stoichastic<HeuristicType,DataType>::doEpoch() {
    iteration++;
    this->candidates->modifyAll(doEpochEach, &iteration);
  }
  
  template <class HeuristicType, class DataType>
  vector<DataType *> *Stoichastic<HeuristicType,DataType>::pickRandoms(LLRB_Tree<DataType *, uint64_t> *tree, uint64_t requests) {
    
    LLRB_TreeNode<DataType *, uint64_t> *curr, *next;
    LLRB_Tree<DataType *, uint64_t> randoms;
    vector<LLRB_TreeNode<DataType *, uint64_t> *> stack;
    uint64_t tmp;
    uint8_t leftDepth = 0, rightDepth = 0;
    double pivot;
    
    curr = tree->getTreeRoot();
    next = tree->getTreeRoot();
    
    while (next != this->question.nullNode) {
      leftDepth++;
      next = curr->leftOf(curr);
    }
    
    curr = tree->getTreeRoot();
    next = tree->getTreeRoot();
    
    while (next != this->question.nullNode) {
      rightDepth++;
      next = curr->rightOf(curr);
    }
    
    pivot = (double)leftDepth / ((double)(leftDepth+rightDepth));
    
    curr = tree->getTreeRoot();
    next = tree->getTreeRoot();
    
    do {
      while (next != this->question.nullNode) {
        stack.push_back(curr);
        if ((rand() / (double)RAND_MAX) < pivot) {
          next = curr->leftOf(curr);
        } else {
          next = curr->rightOf(curr);
        }
      }
      
      tmp = random() % (uint64_t)powl(2, stack.size());
      DataType *entry = stack[log2l(tmp)]->data;
      randoms.insert(entry, (uint64_t)entry);
    } while (randoms.size() < requests);
    
    return randoms.select(NULL, NULL);
  }
  
  template <class HeuristicType, class DataType>
  vector<Trust<DataType> *> *Stoichastic<HeuristicType,DataType>::pickRandomTrusts(LLRB_Tree<Trust<DataType> *, uint64_t> *tree, uint64_t requests) {
    
    LLRB_TreeNode<Trust<DataType> *, uint64_t> *curr, *next;
    LLRB_Tree<Trust<DataType> *, uint64_t> randoms;
    vector<LLRB_TreeNode<Trust<DataType> *, uint64_t> *> stack;
    uint64_t tmp;
    uint8_t leftDepth = 0, rightDepth = 0;
    double pivot;
    
    curr = tree->getTreeRoot();
    next = tree->getTreeRoot();
    
    while (next != this->answer.nullNode) {
      leftDepth++;
      next = curr->leftOf(curr);
    }
    
    curr = tree->getTreeRoot();
    next = tree->getTreeRoot();
    
    while (next != this->answer.nullNode) {
      rightDepth++;
      next = curr->rightOf(curr);
    }
    
    pivot = (double)leftDepth / ((double)(leftDepth+rightDepth));
    
    curr = tree->getTreeRoot();
    next = tree->getTreeRoot();
    
    do {
      while (next != this->answer.nullNode) {
        stack.push_back(curr);
        if ((rand() / (double)RAND_MAX) < pivot) {
          next = curr->leftOf(curr);
        } else {
          next = curr->rightOf(curr);
        }
      }
      
      tmp = random() % (uint64_t)powl(2, stack.size());
      Trust<DataType> *entry = stack[log2l(tmp)]->data;
      randoms.insert(entry, (uint64_t)entry);
    } while (randoms.size() < requests);
    
    return randoms.select(NULL, NULL);
  }
  
  template <class HeuristicType, class DataType>
  HeuristicType *Stoichastic<HeuristicType,DataType>::spawn() {
    vector<DataType *> *inputEnv, *outputEnv, *expectation;
    vector<Trust<DataType> *> *trusts;
    vector<uint64_t> *hiddenInfo = new vector<uint64_t>();
    double *spawnExpect;
    
    uint64_t tmpSize;
    
    tmpSize = rand() % (uint64_t)log2(this->question.size());
    inputEnv = pickRandoms(&this->question, tmpSize+1);
    
    tmpSize = rand() % (uint64_t)log2(this->question.size());
    trusts = pickRandomTrusts(&this->answer, tmpSize+1);
    
    spawnExpect = new double[trusts->size()];
    
    outputEnv = new vector<DataType *>();
    expectation = new vector<DataType *>();
    
    outputEnv->reserve(trusts->size());
    expectation->reserve(trusts->size());
    
    for (uint64_t ix = 0; ix < trusts->size(); ix++) {
      outputEnv->push_back(trusts->at(ix)->actual);
      expectation->push_back(&spawnExpect[ix]);
    }
    
    hiddenInfo->push_back(rand() % (inputEnv->size() + outputEnv->size()));
    
    
    return new HeuristicType(inputEnv, outputEnv, expectation, hiddenInfo);
  }
  
  template <class HeuristicType, class DataType>
  void Stoichastic<HeuristicType,DataType>::initInternals() {
    uint8_t firstGenSize = log2(this->question.size() + this->answer.size()) + 1.0;
    
    for (uint64_t ix = 0; ix < firstGenSize; ix++) {
      HeuristicType *tmp = spawn();
      this->candidates->insert(tmp, (uint64_t)tmp);
    }
  }
  

}

#endif
