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
      LLRB_Tree<Trust<DataType> *, uint64_t> *cia = &(((Genetic *)world)->active);
      vector<Harmony *> *harmony = candidate->getHarmony();
      bool ret = false;
      
      for (uint64_t ix = 0; ix < harmony->size(); ix++) {
        Trust<double> *localComp = cia->search((uint64_t)harmony->at(ix)->reality);
        double toughness = localComp->prediction->confidence;
        
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
      vector<Harmony *> *harmony = candidate->getHarmony();
      double fitness = 0, err, correctness;
      
      candidate->calcExpectation(currentTime);
      
      for (uint64_t ix = 0; ix < harmony->size(); ix++) {
        double output = *(harmony->at(ix)->reality);
        double expectation = *(harmony->at(ix)->expectation);
        err = (output - expectation) / (output);
        err = err * err;
        fitness += err;
      }
      
      fitness = sqrt(fitness/harmony->size());
      
      correctness = harmony->size() / (1.0 + fitness);
      
      candidate->persistance += correctness;
      
      return current->key;
    }
    
    static uint64_t meditateEach(LLRB_TreeNode<HeuristicType *,uint64_t> *current, void *world) {
      Heuristic<HeuristicType,DataType> *candidate = current->data;
      
      candidate->doCorrection();
      
      return current->key;
    }
    
    vector<DataType *> *crossoverA(vector<DataType *> *dad, vector<DataType *> *mom);
    vector<uint64_t> *crossoverB(vector<uint64_t> *dad, vector<uint64_t> *mom);
    
    void add();
    void doEvaluation();
    void doReckoning();
    void doMeditation();
    void doGeneration();
    HeuristicType *reproduce(HeuristicType *father, HeuristicType *mother);
    void get();
    
  public:
    Genetic();
    
    
  };
  
  
  template <class HeuristicType, class DataType>
  void Genetic<HeuristicType,DataType>::doGeneration() {
    vector<HeuristicType *> *mates;
    
    doEvaluation();
    doReckoning();
    
    mates = this->candidates.select(NULL, NULL);
    
    for (uint64_t ix = 0; ix < mates->size(); ix++) {
      if ((((double)rand() / (double)RAND_MAX) < this->accuracy_rate) ||
          (((double)rand() / (double)RAND_MAX) > 0.619)){
        continue;
      }
      
      HeuristicType *child = reproduce(mates->at(rand() % mates->size()), mates->at(rand() % mates->size()));
      
      this->candidates.insert(child, (uint64_t)child);
      
    }
    
    doMeditation();
  }
  
  
  template <class HeuristicType, class DataType>
  void Genetic<HeuristicType,DataType>::doEvaluation() {
    this->candidates.modifyAll(calcFitnessEach, 0);
  }
  
  template <class HeuristicType, class DataType>
  void Genetic<HeuristicType,DataType>::doReckoning() {
    this->candidates.removal(reckoningEach, this);
  }
  
  template <class HeuristicType, class DataType>
  void Genetic<HeuristicType,DataType>::doMeditation() {
    this->candidates.modifyAll(meditateEach, 0);
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
    
    tmp = min((uint64_t)(random() % retSize), (uint64_t)dad->size());
    mCrossover = min((uint64_t)(retSize-tmp), (uint64_t)mom->size());
    crossover = retSize-mCrossover;
    
    /* Each generation should progress, so the size should be greater of
     * two rents
     */
    for (uint64_t mCurrent = 1; mCurrent <= mCrossover; mCurrent++) {
      treeRet.insert(mom->at(mom->size()-mCurrent), (uint64_t)mom->at(mom->size()-mCurrent));
    }
    
    treeSizeStart = treeRet.size();
    
    for (uint64_t dCurrent = 0; dCurrent < dad->size(); dCurrent++) {
      treeRet.insert(dad->at(dCurrent), (uint64_t)dad->at(dCurrent));
      
      if ((treeRet.size()-treeSizeStart) >= crossover) {
        break;
      }
    }
    
    return treeRet.select(NULL,NULL);
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
    vector<double *> *childOutputs, *dOuts, *mOuts;
    vector<Trust<DataType> *> *childTrusts;
    vector<double *> *childExpectations;
    vector<uint64_t> *childHiddenInfo;
    HeuristicType *tmpHeuristic;
    uint64_t tmpNum;
    double *expectations;
    
    childInputs = crossoverA(dad->getInputs(), mom->getInputs());
    
    vector<Harmony *> *dadOutput = dad->getHarmony();
    vector<Harmony *> *momOutput = mom->getHarmony();
    
    dOuts = new vector<double *>();
    mOuts = new vector<double *>();
    
    dOuts->reserve(dadOutput->size());
    mOuts->reserve(momOutput->size());
    
    for (uint64_t ix = 0; ix < dadOutput->size(); ix++) {
      dOuts->push_back(dadOutput->at(ix)->reality);
    }
    
    for (uint64_t ix = 0; ix < momOutput->size(); ix++) {
      mOuts->push_back(momOutput->at(ix)->reality);
    }
    
    childOutputs = crossoverA(dOuts, mOuts);
    childHiddenInfo = crossoverB(dad->getHiddenInfo(), mom->getHiddenInfo());
    childExpectations = new vector<double *>();
    
    childTrusts = new vector<Trust<DataType> *>();
    
    for (uint64_t ix = 0; ix < childOutputs->size(); ix++) {
      Trust<DataType> *tmpTrust = this->answer.search((uint64_t)childOutputs->at(ix));
      childTrusts->push_back(tmpTrust);
    }
    
    /* Mutate */
    tmpNum = rand() % childTrusts->size();
    childTrusts = this->pickRandomTrusts(&(this->answer), childTrusts, tmpNum);
    
    tmpNum = rand() % childInputs->size();
    childInputs = this->pickRandoms(&(this->question), childInputs, tmpNum);
    
    if (childHiddenInfo->size() > 1) {
      childHiddenInfo->pop_back();
      if (rand()%2) {
        childHiddenInfo->push_back(rand() % (childInputs->size() + childTrusts->size()));
      }
      if (rand()%2) {
        childHiddenInfo->push_back(rand() % (childInputs->size() + childTrusts->size()));
      }
    }
    
    expectations = new double[childTrusts->size()];
    childExpectations->reserve(childTrusts->size());
    
    childOutputs->resize(0);
    
    for (uint64_t ix = 0; ix < childTrusts->size(); ix++) {
      Trust<DataType> *tmpTrust = childTrusts->at(ix);
      childOutputs->push_back(tmpTrust->actual);
      expectations[ix] = 0.5;
      childExpectations->push_back(&expectations[ix]);
      if (tmpTrust->prediction == NULL) {
        tmpTrust->prediction = new Prediction<DataType>();
      }
      tmpTrust->prediction->predictions.insert(&expectations[ix], (uint64_t)&expectations[ix]);
      this->active.insert(tmpTrust, (uint64_t)tmpTrust->actual);
    }
    tmpHeuristic = new HeuristicType(childInputs,childOutputs,childExpectations,childHiddenInfo);
    
    childInputs->resize(0);
    childOutputs->resize(0);
    childExpectations->resize(0);
    childTrusts->resize(0);
    
    delete childInputs;
    delete childOutputs;
    delete childExpectations;
    delete childHiddenInfo;
    delete childTrusts;
    
    return tmpHeuristic;
  }
  
  template <class HeuristicType, class DataType>
  void Genetic<HeuristicType,DataType>::get() {
    
  }
  
  template <class HeuristicType, class DataType>
  Genetic<HeuristicType,DataType>::Genetic() {
    
  }
  
}

#endif
