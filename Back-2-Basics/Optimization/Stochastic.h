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
  
  static double glbEnergyCnst = 1.0 / 1024.0;
  static double glbAreaCapacity = 4.0;
  
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
      
      if (expectations->size() < 2) {
        current->data->prediction->confidence = 0.5;
      } else {
        double variance = sqrt(diff / double(expectationCnt-1));
        current->data->prediction->confidence = 1.0 / (1.0 + ((0.05 + variance) / sqrt(expectationCnt)));
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
    HeuristicType *spawn();
    
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
  HeuristicType *Stoichastic<HeuristicType,DataType>::spawn() {
    vector<DataType *> *inputEnv, *outputEnv, *expectation;
    vector<Trust<DataType> *> *trusts;
    LLRB_Tree<Info *, uint64_t> infoTree;
    vector<Info *> *hiddenInfo = new vector<Info *>();
    HeuristicType *tmp;
    vector<uint64_t> randIn, randOut;
    uint64_t layer, hiddenWidth, tmpNum;
    
    inputEnv = this->question.select(NULL,NULL);
    trusts = this->answer.select(NULL,NULL);
    
    outputEnv = new vector<DataType *>();
    expectation = new vector<DataType *>();
    
    outputEnv->reserve(trusts->size());
    expectation->reserve(trusts->size());
    
    for (uint64_t ix = 0; ix < trusts->size(); ix++) {
      double *spawnExpect = new double(0.0);
      outputEnv->push_back(trusts->at(ix)->actual);
      expectation->push_back(spawnExpect);
      
      if (trusts->at(ix)->prediction == NULL) {
        trusts->at(ix)->prediction = new Prediction<DataType>();
      }
    }
    
    for (uint64_t ix = 0; ix < inputEnv->size(); ix++) {
      randIn.push_back(ix);
    }
    
    for (uint64_t ix = 0; ix < outputEnv->size(); ix++) {
      randOut.push_back(ix);
    }
    
    hiddenWidth = max(inputEnv->size(), outputEnv->size());
    
    
    for (uint64_t ix = 0; ix < randOut.size(); ix++) {
      layer = 7;
      tmpNum = ix;
      
      while (layer != 0) {
        Info *newConn = new Info;
        newConn->c.layer = layer;
        newConn->c.position = tmpNum;
        layer -= (rand() % layer) + 1;
        newConn->c.inputLayer = layer;
        tmpNum = random() % hiddenWidth;
        newConn->c.inputPosition = tmpNum;
        
        infoTree.insert(newConn, newConn->k);
      }
    }
    
    hiddenInfo = infoTree.select(NULL, NULL);
     
    
    tmp = new HeuristicType(inputEnv, outputEnv, expectation, hiddenInfo);
    
    this->candidates.insert(tmp, (uint64_t)tmp);
    
    /* Estimate energy use */
    tmp->energy = (double)hiddenInfo->size() * glbEnergyCnst;
    
    inputEnv->resize(0);
    outputEnv->resize(0);
    expectation->resize(0);
    hiddenInfo->resize(0);
    
    delete inputEnv;
    delete outputEnv;
    delete expectation;
    delete hiddenInfo;
    
    return tmp;
  }
  
  template <class HeuristicType, class DataType>
  void Stoichastic<HeuristicType,DataType>::initInternals() {
    
    for (uint64_t ix = 0; ix < 1; ix++) {
      spawn();
    }
    
  }
  
  
}

#endif
