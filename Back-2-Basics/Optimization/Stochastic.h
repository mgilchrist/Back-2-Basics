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
  
  static double glbEnergyCnst = 1.0 / 64.0;
  static double glbAreaCapacity = 1.0;
  
  /*  typedef struct TotalCompetition {
   double competition = 0.0;
   uint64_t numCompetitors = 0;
   } TotalCompetition;
   */
  template <class HeuristicType, class DataType>
  class Stoichastic : public Optimization<HeuristicType,DataType> {
    
  private:
    
    uint64_t iteration = 0;
    
  protected:
    
    //  LLRB_Tree<TotalCompetition *, uint64_t> *competitionInArea;
    LLRB_Tree<Trust<DataType> *, uint64_t> active;
    
    static uint64_t calcConsensus(LLRB_TreeNode<Trust<double> *,uint64_t> *current, void *reserved) {
      
      vector<double *> *expectations = current->data->prediction->predictions.select(NULL, NULL);
      double sum = 0.0, average = 0.0, diff = 0.0;
      uint64_t expectationCnt = 0;
      
      for (uint64_t ix = 0; ix < expectations->size(); ix++) {
        double expect = *(expectations->at(ix));
        if (expect > PPM) {
          sum += expect;
          expectationCnt++;
        }
      }
      
      if (expectationCnt) {
        average = max(sum / (double)expectationCnt, PPM);
      }
      
      for (uint64_t ix = 0; ix < expectations->size(); ix++) {
        double expect = *(expectations->at(ix));
        if (expect > PPM) {
          double tmp = expect-average;
          diff += pow(tmp,2);
        }
      }
      
      current->data->prediction->expectation = average;
      current->data->prediction->confidence = 0.0;
      
      if (expectationCnt == 1) {
        current->data->prediction->confidence = 0.95 * (1.0 - diff);
      } else {
        double variance = sqrt(diff / double(expectationCnt-1));
        current->data->prediction->confidence = 1.0 / (1.0 + (variance / sqrt(expectationCnt)));
      }
      
      return current->key;
    }
    
    static uint64_t doEpochEach(LLRB_TreeNode<HeuristicType *,uint64_t> *current, void *iteration) {
      
      current->data->calcExpectation(*(uint64_t *)iteration);
      
      return current->key;
    }
    
    void doEpoch();
    virtual void doGeneration() =0;
    
    vector<DataType *> *pickRandoms(LLRB_Tree<DataType *, uint64_t> *, vector<DataType *> *originals, uint64_t);
    vector<Trust<DataType> *> *pickRandomTrusts(LLRB_Tree<Trust<DataType> *, uint64_t> *, vector<Trust<DataType> *> *originals, uint64_t);
    HeuristicType *spawn(uint64_t);
    
  public:
    Stoichastic() {
      
    }
    
    void initInternals();
  };
  
  
  template <class HeuristicType, class DataType>
  void Stoichastic<HeuristicType,DataType>::doEpoch() {
    iteration++;
    
    this->candidates.modifyAll(doEpochEach, &iteration);
    active.modifyAll(calcConsensus, NULL);
    
    doGeneration();
  }
  
  template <class HeuristicType, class DataType>
  vector<DataType *> *Stoichastic<HeuristicType,DataType>::pickRandoms(LLRB_Tree<DataType *, uint64_t> *tree, vector<DataType *> *originals, uint64_t requests) {
    
    LLRB_TreeNode<DataType *, uint64_t> *curr;
    LLRB_Tree<DataType *, uint64_t> randoms;
    vector<LLRB_TreeNode<DataType *, uint64_t> *> stack;
    uint64_t tmp;
    uint8_t leftDepth = 0, rightDepth = 0;
    double pivot;
    
    curr = tree->treeRoot;
    
    while (curr != randoms.leftOf(curr)) {
      leftDepth++;
      curr = randoms.leftOf(curr);
    }
    
    curr = tree->treeRoot;
    
    while (curr != randoms.leftOf(curr)) {
      rightDepth++;
      curr = randoms.rightOf(curr);
    }
    
    pivot = (double)leftDepth / ((double)(leftDepth+rightDepth));
    
    if (originals != NULL) {
      for (uint64_t ix = 0; ix < originals->size(); ix++) {
        randoms.insert(originals->at(ix), (uint64_t)originals->at(ix));
      }
      
      originals->resize(0);
      delete originals;
    }
    
    for (uint64_t ix = 0; ix < requests; ix++) {
      curr = tree->treeRoot;
      
      while (curr != randoms.leftOf(curr)) {
        stack.push_back(curr);
        if ((rand() / (double)RAND_MAX) < pivot) {
          curr = randoms.leftOf(curr);
        } else {
          curr = randoms.rightOf(curr);
        }
      }
      
      tmp = random() % (uint64_t)powl(2, stack.size());
      DataType *entry = stack[log2l(tmp)]->data;
      randoms.insert(entry, (uint64_t)entry);
      stack.resize(0);
    }
    
    return randoms.select(NULL, NULL);
  }
  
  template <class HeuristicType, class DataType>
  vector<Trust<DataType> *> *Stoichastic<HeuristicType,DataType>::pickRandomTrusts(LLRB_Tree<Trust<DataType> *, uint64_t> *tree, vector<Trust<DataType> *> *originals, uint64_t requests) {
    
    LLRB_TreeNode<Trust<DataType> *, uint64_t> *curr;
    LLRB_Tree<Trust<DataType> *, uint64_t> randoms;
    vector<LLRB_TreeNode<Trust<DataType> *, uint64_t> *> stack;
    uint64_t tmp;
    uint8_t leftDepth = 0, rightDepth = 0;
    double pivot;
    
    curr = tree->treeRoot;
    
    while (curr != randoms.leftOf(curr)) {
      leftDepth++;
      curr = randoms.leftOf(curr);
    }
    
    curr = tree->treeRoot;
    
    while (curr != randoms.leftOf(curr)) {
      rightDepth++;
      curr = randoms.rightOf(curr);
    }
    
    pivot = (double)leftDepth / ((double)(leftDepth+rightDepth));
    
    if (originals != NULL) {
      for (uint64_t ix = 0; ix < originals->size(); ix++) {
        randoms.insert(originals->at(ix), (uint64_t)originals->at(ix));
      }
      
      originals->resize(0);
      delete originals;
      
    }
    
    for (uint64_t ix = 0; ix < requests; ix++) {
      curr = tree->treeRoot;
      
      while (curr != randoms.leftOf(curr)) {
        stack.push_back(curr);
        if ((rand() / (double)RAND_MAX) < pivot) {
          curr = randoms.leftOf(curr);
        } else {
          curr = randoms.rightOf(curr);
        }
      }
      
      tmp = random() % (uint64_t)powl(2, stack.size());
      Trust<DataType> *entry = stack[log2l(tmp)]->data;
      randoms.insert(entry, (uint64_t)entry);
      stack.resize(0);
    }
    
    return randoms.select(NULL, NULL);
  }
  
  template <class HeuristicType, class DataType>
  HeuristicType *Stoichastic<HeuristicType,DataType>::spawn(uint64_t spawnNum) {
    vector<DataType *> *inputEnv;
    LLRB_Tree<double *, uint64_t> *expectation;
    vector<Trust<DataType> *> *trusts;
    LLRB_Tree<Info *, uint64_t> infoTree;
    vector<Info *> *hiddenInfo = new vector<Info *>();
    HeuristicType *tmp;
    uint64_t layer, tmpNum, incVal;
    
    if (this->questionCache == NULL) {
      this->questionCache = this->question.select(NULL,NULL);
    }
    
    if (this->answerCache == NULL) {
      this->answerCache = this->answer.select(NULL,NULL);
    }
    
    inputEnv = this->questionCache;
    trusts = this->answerCache;
    
    incVal = log2(trusts->size()); //random() % (uint64_t)(sqrt(randOut.size()) * 2.0);
    spawnNum *= incVal;
    
    expectation = new LLRB_Tree<double *, uint64_t>();
    
    for (uint64_t ix = 0; ix < incVal; ix++) {
      tmpNum = (spawnNum + ix) % trusts->size();
      double *spawnExpect = new double(0.0);
      expectation->insert(spawnExpect, (uint64_t)(trusts->at(tmpNum)->actual));
      
      if (trusts->at(tmpNum)->prediction == NULL) {
        trusts->at(tmpNum)->prediction = new Prediction<DataType>();
      }
    }
    
    for (uint64_t ix = 0; ix < incVal; ix++) {
      layer = 7;
      tmpNum = (spawnNum + ix) % trusts->size();
      
      while (layer != 0) {
        Info *newConn = new Info;
        newConn->c.position = ((uint32_t)layer << LAYER_SHIFT) | (uint32_t)tmpNum;
        layer -= log2((rand() % (1 << layer))) + 1;
        if (layer) {
          tmpNum = random() % this->hiddenWidth;
        } else {
          tmpNum = random() % inputEnv->size();
        }
        newConn->c.inputPosition = ((uint32_t)layer << LAYER_SHIFT) |  (uint32_t)tmpNum;
        
        infoTree.insert(newConn, newConn->k);
      }
    }
    
    hiddenInfo = infoTree.select(NULL, NULL);
     
    tmp = new HeuristicType(inputEnv, trusts, expectation, hiddenInfo);
    
    this->candidates.insert(tmp, (uint64_t)tmp);
    
    /* Estimate energy use */
    tmp->energy = (double)hiddenInfo->size() * glbEnergyCnst;
    
    hiddenInfo->resize(0);
    
    delete expectation;
    delete hiddenInfo;
    
    return tmp;
  }
  
  template <class HeuristicType, class DataType>
  void Stoichastic<HeuristicType,DataType>::initInternals() {
    uint64_t numSpawn = (this->answer.size() / log2(this->answer.size())) + 1;
    
    for (uint64_t ix = 0; ix < numSpawn; ix++) {
      spawn(ix);
    }
    
  }
  
  
}

#endif
