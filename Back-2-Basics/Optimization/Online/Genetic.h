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
  
  template <class EntryType>
  static uint64_t addEach(LLRB_TreeNode<EntryType *,uint64_t> *current, void *vect) {
    ((vector<double *> *)vect)->push_back(current->data);
    
    return current->key;
  }
  
  static double calcFitnessEach(LLRB_TreeNode<HeuristicType *,uint64_t> *current, void *world) {
    current->data->calcExpectation(currentTime);
    vector<HeuristicHarmony *> *harmony = current->data->getHarmony();
    double fitness = 0, tmp;
    
    for (uint64_t ix = 0; ix < harmony->size(); ix++) {
      tmp = pow((*(harmony->at(ix)->output) - *(harmony->at(ix)->expectation)) / *(harmony->at(ix)->output), 2);
      ((Genetic *)world)->candidatesInArea->search(harmony->at(ix)->output)->push(current, tmp);
      fitness += tmp;
    }
    
    fitness /= harmony->size();
    
    return sqrt(fitness);
  }
  
  template <class EntryType>
  static vector<EntryType *> *crossoverA(vector<EntryType *> *dad, vector<EntryType *> *mom) {
    vector<EntryType *> *ret;
    LLRB_Tree<EntryType *, uint64_t> treeRet;
    uint64_t crossover;
    uint64_t retSize = max(dad->size(), mom->size());
    uint64_t treeSizeStart;
    
    if (dad == mom) {
      return new vector<double *>(dad->begin(), dad->end());
    }
    
    ret = new vector<double *>();
    ret->reserve(dad->size());
    
    crossover = random() % dad->size();
    
    
    /* Each generation should progress, so the size should be greater of
     * two rents 
     */
    for (uint64_t mCurrent = mom->size()-1; mCurrent >= (uint64_t)crossover; mCurrent++) {
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
    
    treeRet.modifyAll(addEach, ret);
    
    return ret;
  }
  
  template <class EntryType>
  static vector<EntryType *> *crossoverB(vector<EntryType *> *dad, vector<EntryType *> *mom) {
    vector<EntryType *> *ret;
    
    
    return ret;
  }
  
  void add();
  void calcFitness();
  void calcSurvival();
  HeuristicType *reproduce(HeuristicType *father, HeuristicType *mother);
  void get();
  
public:
  Genetic();
  
  
};



template <class HeuristicType, class DataType>
void Genetic<HeuristicType,DataType>::calcFitness() {
  this->candidates->modifyAll(calcFitnessEach, 0);
}

template <class HeuristicType, class DataType>
void Genetic<HeuristicType,DataType>::calcSurvival() {
  
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
  
  for (uint64_t ix = 0; ix < childOutputs->size(); ix++) {
    childExpectations->push_back(&expectations[ix]);
    this->space.insert(&expectations[ix], (uint64_t)&expectations[ix]);
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
