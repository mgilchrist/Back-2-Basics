//
//  Genetic.h
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

#ifndef OpenSource_Genetic_h
#define OpenSource_Genetic_h

#include "Stochastic.h"
#include "Heuristic.h"

namespace Optimization {
  
  
  static uint64_t currentTime = -1;
  static const double glbToughness = 0.1;
  static const double glbAreaCapacity = 1.414;
  
  template <class HeuristicType, class DataType>
  class Genetic : public Stoichastic<HeuristicType,DataType> {
    
  protected:
    
    
    static bool reckoningEach(LLRB_TreeNode<HeuristicType *,uint64_t> *current, void *world) {
      Heuristic<HeuristicType,DataType> *candidate = current->data;
      LLRB_Tree<Trust<DataType> *, uint64_t> *cia = &((Genetic *)world)->active;
      vector<HeuristicHarmony *> *harmony = candidate->getHarmony();
      bool ret = false;
      
      for (uint64_t ix = 0; ix < harmony->size(); ix++) {
        Trust<double> *localComp = cia->search(NULL, (uint64_t)harmony->at(ix));
        double toughness = localComp->prediction->confidence;
        
        localComp = cia->search(NULL, (uint64_t)harmony->at(ix)->output);
        
        toughness = max((toughness * localComp->prediction->predictions.size())-glbAreaCapacity,0.0);
        toughness /= localComp->prediction->predictions.size();
        
        candidate->persistance -= (toughness+glbToughness);
      }
      
      if (candidate->persistance < 0.0) {
        ret = true;
      }
      
      return ret;
    }
    
    static uint64_t calcFitnessEach(LLRB_TreeNode<HeuristicType *,uint64_t> *current, void *world) {
      Heuristic<HeuristicType,DataType> *candidate = current->data;
      vector<HeuristicHarmony *> *harmony = candidate->getHarmony();
      double fitness = 0, err, correctness;
      
      candidate->calcExpectation(currentTime);
      
      for (uint64_t ix = 0; ix < harmony->size(); ix++) {
        err = (*(harmony->at(ix)->output) - *(harmony->at(ix)->expectation)) / *(harmony->at(ix)->output);
        err = err * err;
        fitness += err;
      }
      
      fitness = sqrt(fitness/harmony->size());
      
      correctness = harmony->size() / (1.0 + fitness);
      
      candidate->persistance += correctness;
      
      return current->key;
    }
    
    vector<DataType *> *crossoverA(vector<DataType *> *dad, vector<DataType *> *mom);
    vector<uint64_t> *crossoverB(vector<uint64_t> *dad, vector<uint64_t> *mom);
    
    void add();
    void doEvaluation();
    void doReckoning();
    void doGeneration();
    HeuristicType *reproduce(HeuristicType *father, HeuristicType *mother);
    void get();
    
  public:
    Genetic();
    
    
  };
  
  
  template <class HeuristicType, class DataType>
  void Genetic<HeuristicType,DataType>::doGeneration() {
    doEvaluation();
    doReckoning();
  }
  
  
  template <class HeuristicType, class DataType>
  void Genetic<HeuristicType,DataType>::doEvaluation() {
    this->candidates.modifyAll(calcFitnessEach, 0);
  }
  
  template <class HeuristicType, class DataType>
  void Genetic<HeuristicType,DataType>::doReckoning() {
    this->candidates.selectRemove(reckoningEach, this);
  }
  
  template <class HeuristicType, class DataType>
  vector<DataType *> *Genetic<HeuristicType,DataType>::crossoverA(vector<DataType *> *dad, vector<DataType *> *mom) {
    vector<DataType *> *ret;
    LLRB_Tree<DataType *, uint64_t> treeRet;
    uint64_t tmp, crossover, mCrossover;
    uint64_t retSize = max(dad->size(), mom->size());
    uint64_t treeSizeStart;
    
    if (dad == mom) {
      return new vector<double *>(dad->begin(), dad->end());
    }
    
    ret = new vector<DataType *>();
    ret->reserve(retSize);
    
    tmp = min(random() % retSize, dad->size());
    mCrossover = min(retSize-tmp, mom->size());
    crossover = retSize-mCrossover;
    
    /* Each generation should progress, so the size should be greater of
     * two rents
     */
    for (uint64_t mCurrent = mom->size()-1; mCurrent >= (uint64_t)mom->size()-crossover; mCurrent++) {
      treeRet.insert(mom->at(mCurrent), (uint64_t)mom->at(mCurrent));
    }
    
    treeSizeStart = treeRet.size();
    
    for (uint64_t dCurrent = 0; dCurrent < (dad->size()-1); dCurrent++) {
      treeRet.insert(dad->at(dCurrent), (uint64_t)dad->at(dCurrent));
      
      if ((treeRet.size()-treeSizeStart) >= crossover) {
        break;
      }
    }
    
    /* That last entry was needed. So one parent is subset of other
     * Return larger of two.
     */
    if (treeRet.size() != retSize) {
      if (retSize == dad->size()) {
        ret->assign(dad->begin(), dad->end());
      } else {
        ret->assign(mom->begin(), mom->end());
      }
      
      return ret;
    }
    
    treeRet.addAllTo(ret);
    
    return ret;
  }
  
  template <class HeuristicType, class DataType>
  vector<uint64_t> *Genetic<HeuristicType,DataType>::crossoverB(vector<uint64_t> *dad, vector<uint64_t> *mom) {
    vector<uint64_t> *ret;
    uint64_t tmp, crossover, mCrossover;
    uint64_t retSize = max(dad->size(), mom->size());
    
    ret = new vector<uint64_t>();
    ret->reserve(retSize);
    
    tmp = min((uint64_t)(random() % retSize), (uint64_t)dad->size());
    mCrossover = min((uint64_t)(retSize-tmp), (uint64_t)mom->size());
    crossover = retSize-mCrossover;
    
    for (uint64_t ix = 0; ix < crossover; ix++) {
      ret->push_back(dad->at(ix));
    }
    
    for (uint64_t ix = crossover; ix < retSize; ix++) {
      ret->push_back(mom->at(ix));
    }
    
    return ret;
  }
  
  template <class HeuristicType, class DataType>
  HeuristicType *Genetic<HeuristicType,DataType>::reproduce(HeuristicType *dad, HeuristicType *mom) {
    vector<double *> *childInputs;
    vector<double *> *childOutputs;
    vector<double *> *childExpectations;
    vector<uint64_t> *childHiddenInfo;
    double *expectations;
    
    childInputs = crossoverA(dad->getInputs(), mom->getInputs());
    childOutputs = crossoverA(dad->getOutputs(), mom->getOutputs());
    childHiddenInfo = crossoverB(dad->getHiddenInfo(), mom->getHiddenInfo());
    childExpectations = new vector<double *>();
    
    expectations = new double[childOutputs->size()];
    this->space.insert(expectations, (uint64_t)expectations);
    
    for (uint64_t ix = 0; ix < childOutputs->size(); ix++) {
      childExpectations->push_back(&expectations[ix]);
    }
    
    return new HeuristicType(childInputs,childOutputs,childExpectations,childHiddenInfo);
  }
  
  template <class HeuristicType, class DataType>
  void Genetic<HeuristicType,DataType>::get() {
    
  }
  
  template <class HeuristicType, class DataType>
  Genetic<HeuristicType,DataType>::Genetic() {
    
  }
  
}

#endif
