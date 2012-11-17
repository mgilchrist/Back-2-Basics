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

static uint64_t currentTime = -1;

template <class HeuristicType, class DataType>
class Genetic : public Stoichastic<HeuristicType,DataType> {
  
private:
  
  
  double envToughness;
  uint64_t reproductionAgeMin;
  uint64_t reproductionAgeMax;
  
protected:
  
  static double calcFitnessEach(LLRB_TreeNode<HeuristicType *,uint64_t> *current, void *world) {
    current->data->calcExpectation(currentTime);
    vector<HeuristicHarmony *> *harmony = current->data->getHarmony();
    double fitness = 0, err;
    
    for (uint64_t ix = 0; ix < harmony->size(); ix++) {
      err = (*(harmony->at(ix)->output) - *(harmony->at(ix)->expectation)) / *(harmony->at(ix)->output);
      err = pow(err, 2);
      current->data->persistance += 1 / (1 + err);
      
      ((Genetic *)world)->candidatesInArea->search(harmony->at(ix)->output)->push(current, err);
      fitness += err;
    }
    
    fitness /= harmony->size();
    
    return sqrt(fitness);
  }
  
  template <class EntryType>
  static vector<EntryType *> *crossoverA(vector<EntryType *> *dad, vector<EntryType *> *mom) {
    vector<EntryType *> *ret;
    LLRB_Tree<EntryType *, uint64_t> treeRet;
    uint64_t tmp, crossover, mCrossover;
    uint64_t retSize = max(dad->size(), mom->size());
    uint64_t treeSizeStart;
    
    if (dad == mom) {
      return new vector<double *>(dad->begin(), dad->end());
    }
    
    ret = new vector<double *>();
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
  
  template <class EntryType>
  static vector<EntryType *> *crossoverB(vector<EntryType *> *dad, vector<EntryType *> *mom) {
    vector<EntryType *> *ret;
    uint64_t tmp, crossover, mCrossover;
    uint64_t retSize = max(dad->size(), mom->size());
    
    ret = new vector<double *>();
    ret->reserve(retSize);
    
    tmp = min(random() % retSize, dad->size());
    mCrossover = min(retSize-tmp, mom->size());
    crossover = retSize-mCrossover;
    
    for (uint64_t ix = 0; ix < crossover; ix++) {
      ret->push_back(dad->at(ix));
    }
    
    for (uint64_t ix = crossover; ix < retSize; ix++) {
      ret->push_back(mom->at(ix));
    }
    
    return ret;
  }
  
  void add();
  void evaluation();
  void reckoning();
  HeuristicType *reproduce(HeuristicType *father, HeuristicType *mother);
  void get();
  
public:
  Genetic();
  
  
};



template <class HeuristicType, class DataType>
void Genetic<HeuristicType,DataType>::evaluation() {
  this->candidates->modifyAll(calcFitnessEach, 0);
}

template <class HeuristicType, class DataType>
void Genetic<HeuristicType,DataType>::reckoning() {
  
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
  this->candidatesInArea = new LLRB_Tree<Heap<HeuristicType, double>, uint64_t>();
}


#endif
