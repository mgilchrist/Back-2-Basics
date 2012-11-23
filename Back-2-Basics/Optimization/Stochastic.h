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
    double competition = 0.0;
    uint64_t numCompetitors = 0;
  } TotalCompetition;
  
  template <class HeuristicType, class DataType>
  class Stoichastic : public Optimization<HeuristicType,DataType> {
    
  private:
    
    LLRB_Tree<TotalCompetition *, uint64_t> *competitionInArea;
    LLRB_Tree<Trust<DataType> *, uint64_t> active;
    uint64_t iteration = 0;
    
  protected:
    
    static uint64_t calcExpectation(LLRB_TreeNode<Trust<double> *,uint64_t> *current, void *iteration) {
      
      vector<double *> *expectations = current->data->prediction->predictions.select(NULL, NULL);
      double sum = 0.0, average, diff = 1.0;
      
      for (uint64_t ix = 0; ix < expectations->size(); ix++) {
        sum += *(expectations->at(ix));
      }
      
      average = sum / expectations->size();
      
      for (uint64_t ix = 0; ix < expectations->size(); ix++) {
        double tmp = *(expectations->at(ix))-average;
        diff *= tmp * tmp;
      }
      
      current->data->prediction->expectation = average;
      current->data->prediction->confidence = ((average * 0.05) /
                                               sqrt(diff/expectations->size()));
      
      return current->key;
    }
    
    static uint64_t doEpochEach(LLRB_TreeNode<HeuristicType *,uint64_t> *current, void *iteration) {
      
      current->data->calcExpectation(*(uint64_t *)iteration);
      
      return current->key;
    }
    
    void doEpoch();
    
    vector<DataType *> *pickRandoms(LLRB_Tree<DataType *, uint64_t> *, uint64_t);
    vector<Trust<DataType> *> *pickRandomTrusts(LLRB_Tree<Trust<DataType> *, uint64_t> *, uint64_t);
    HeuristicType *spawn();
    
  public:
    Stoichastic() {
      competitionInArea = new LLRB_Tree<TotalCompetition *, uint64_t>();
      
    }
    
    void initInternals();
  };
  
  
  template <class HeuristicType, class DataType>
  void Stoichastic<HeuristicType,DataType>::doEpoch() {
    iteration++;
    this->candidates.modifyAll(doEpochEach, &iteration);
    
    active.modifyAll(calcExpectation, NULL);
  }
  
  template <class HeuristicType, class DataType>
  vector<DataType *> *Stoichastic<HeuristicType,DataType>::pickRandoms(LLRB_Tree<DataType *, uint64_t> *tree, uint64_t requests) {
    
    LLRB_TreeNode<DataType *, uint64_t> *curr;
    LLRB_Tree<DataType *, uint64_t> randoms;
    vector<LLRB_TreeNode<DataType *, uint64_t> *> stack;
    uint64_t tmp;
    uint8_t leftDepth = 0, rightDepth = 0;
    double pivot;
    
    curr = tree->getTreeRoot();
    
    while (curr != this->question.nullNode) {
      leftDepth++;
      curr = curr->leftOf(curr);
    }
    
    curr = tree->getTreeRoot();
    
    while (curr != this->question.nullNode) {
      rightDepth++;
      curr = curr->rightOf(curr);
    }
    
    pivot = (double)leftDepth / ((double)(leftDepth+rightDepth));
    
    curr = tree->getTreeRoot();
    
    do {
      while (curr != this->question.nullNode) {
        stack.push_back(curr);
        if ((rand() / (double)RAND_MAX) < pivot) {
          curr = curr->leftOf(curr);
        } else {
          curr = curr->rightOf(curr);
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
    
    LLRB_TreeNode<Trust<DataType> *, uint64_t> *curr;
    LLRB_Tree<Trust<DataType> *, uint64_t> randoms;
    vector<LLRB_TreeNode<Trust<DataType> *, uint64_t> *> stack;
    uint64_t tmp;
    uint8_t leftDepth = 0, rightDepth = 0;
    double pivot;
    
    curr = tree->getTreeRoot();
    
    while (curr != this->answer.nullNode) {
      leftDepth++;
      curr = curr->leftOf(curr);
    }
    
    curr = tree->getTreeRoot();
    
    while (curr != this->answer.nullNode) {
      rightDepth++;
      curr = curr->rightOf(curr);
    }
    
    pivot = (double)leftDepth / ((double)(leftDepth+rightDepth));
    
    curr = tree->getTreeRoot();
    
    do {
      while (curr != this->answer.nullNode) {
        stack.push_back(curr);
        if ((rand() / (double)RAND_MAX) < pivot) {
          curr = curr->leftOf(curr);
        } else {
          curr = curr->rightOf(curr);
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
    HeuristicType *tmp;
    
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
      
      if (trusts->at(ix)->prediction == NULL) {
        trusts->at(ix)->prediction = new Prediction<DataType>();
      }
      trusts->at(ix)->prediction->predictions.insert(&spawnExpect[ix],
                                                      (uint64_t)&spawnExpect[ix]);
        
      active.insert(trusts->at(ix), (uint64_t)trusts->at(ix));
    }
    
    hiddenInfo->push_back(rand() % (inputEnv->size() + outputEnv->size()));
    
    tmp = new HeuristicType(inputEnv, outputEnv, expectation, hiddenInfo);
    
    this->candidates.insert(tmp, (uint64_t)tmp);
    
    /* Estimate energy use */
    tmp->energy = (double)inputEnv->size();
    
    for (uint64_t ix = 0; ix < hiddenInfo->size(); ix++) {
      tmp->energy *= (double)hiddenInfo->at(ix);
    }
    
    tmp->energy *= (double)outputEnv->size();
    
    return tmp;
  }
  
  template <class HeuristicType, class DataType>
  void Stoichastic<HeuristicType,DataType>::initInternals() {
    uint8_t firstGenSize = log2(this->question.size() + this->answer.size()) + 1;
    
    for (uint64_t ix = 0; ix < firstGenSize; ix++) {
      spawn();
    }
    
  }
  

}

#endif
