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
  static const double glbToughness = 0.0;
  static const uint64_t ofAge = 180;
  
  template <class HeuristicType, class LogicType, class DataType>
  class Genetic : public Stoichastic<HeuristicType,DataType> {
    
  protected:
    
    
    static bool reckoningEach(LLRB_TreeNode<HeuristicType *,uint64_t> *current, void *world) {
      Heuristic<HeuristicType,LogicType,DataType> *candidate = current->data;
      LLRB_Tree<Trust<DataType> *, uint64_t> *cia = &(((Genetic *)world)->active);
      vector<Harmony<LogicType> *> *harmony = candidate->getHarmony();
      
      if (candidate->experiencedEpochs < ofAge) {
        return false;
      }
      
      for (uint64_t ix = 0; ix < harmony->size(); ix++) {
        Trust<double> *localComp = cia->search((uint64_t)harmony->at(ix)->reality);
        if (localComp == NULL) {
          continue;
        }
        uint64_t envSize = localComp->prediction->predictions.size();
        double toughness = (localComp->prediction->confidence * envSize) - glbAreaCapacity;
        
        if (toughness >= 0.0) {
          toughness /= envSize;
        } else {
          toughness = 0.0;
        }
        
        candidate->persistance -= (toughness+glbToughness);
      }
      
      if ((candidate->persistance < 0.0) && (candidate->registered)) {
        for (uint64_t ix = 0; ix < harmony->size(); ix++) {
          Trust<double> *localComp = cia->search((uint64_t)harmony->at(ix)->reality);
          localComp->prediction->predictions.remove(harmony->at(ix)->expectation, (uint64_t)harmony->at(ix)->expectation);
          
          if (!localComp->prediction->predictions.size()) {
            cia->remove(localComp, (uint64_t)harmony->at(ix)->reality);
            //delete localComp->prediction;
          }
        }
        
        return true;
      }
      
      return false;
    }
    
    static uint64_t calcFitnessEach(LLRB_TreeNode<HeuristicType *,uint64_t> *current, void *world) {
      Heuristic<HeuristicType,LogicType,DataType> *candidate = current->data;
      vector<Harmony<LogicType> *> *harmony = candidate->getHarmony();
      double fitness = 0, err;
      
      candidate->calcExpectation(currentTime);
      
      for (uint64_t ix = 0; ix < harmony->size(); ix++) {
        double output = *(harmony->at(ix)->reality);
        double expectation = *(harmony->at(ix)->expectation);
        err = (output - expectation);
        err = err * err;
        fitness += err;
      }
      
      if (candidate->experiencedEpochs > ofAge) {
        for (uint64_t ix = 0; ix < harmony->size(); ix++) {
          Trust<double> *localComp = ((Genetic *)world)->answer.search((uint64_t)harmony->at(ix)->reality);
          localComp->prediction->predictions.insert(harmony->at(ix)->expectation, (uint64_t)harmony->at(ix)->expectation);
          ((Genetic *)world)->active.insert(localComp, (uint64_t)harmony->at(ix)->reality);
        }
        candidate->registered = true;
      }
      
      fitness = sqrt(fitness/harmony->size());
      
      candidate->persistance += (harmony->size() / pow((0.95 + fitness), 2));
      
      return current->key;
    }
    
    static uint64_t meditateEach(LLRB_TreeNode<HeuristicType *,uint64_t> *current, void *world) {
      Heuristic<HeuristicType,LogicType,DataType> *candidate = current->data;
      
      candidate->doCorrection();
      
      return current->key;
    }
    
    vector<DataType *> *combine(vector<DataType *> *dad, vector<DataType *> *mom);
    vector<DataType *> *crossover(vector<DataType *> *dad, vector<DataType *> *mom);
    
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
  
  
  template <class HeuristicType, class LogicType, class DataType>
  void Genetic<HeuristicType,LogicType,DataType>::doGeneration() {
    vector<HeuristicType *> *mates;
    
    doEvaluation();
    doReckoning();
    
    mates = this->candidates.select(NULL, NULL);
    
    for (uint64_t ix = 0; ix < mates->size(); ix++) {
      HeuristicType *dad = mates->at(ix);
      HeuristicType *mom = mates->at(rand() % mates->size());
      
      if ((dad->experiencedEpochs < ofAge) || (mom->experiencedEpochs < ofAge)) {
        continue;
      }
      
      double repoRate = 1.0 / (30.0 + dad->energy + mom->energy);
      double rVal = rand() / (double)RAND_MAX;
      
      repoRate *= 1.0 - this->accuracy_rate;
      
      if (rVal < repoRate) {
        HeuristicType *child = reproduce(dad, mom);
        
        dad->optimalPrune();
        
        if (mom != dad) {
          mom->optimalPrune();
        }
          
        this->candidates.insert(child, (uint64_t)child);
      }
    }
    
    doMeditation();
  }
  
  
  template <class HeuristicType, class LogicType, class DataType>
  void Genetic<HeuristicType,LogicType,DataType>::doEvaluation() {
    this->candidates.modifyAll(calcFitnessEach, this);
  }
  
  template <class HeuristicType, class LogicType, class DataType>
  void Genetic<HeuristicType,LogicType,DataType>::doReckoning() {
    this->candidates.removal(reckoningEach, this);
  }
  
  template <class HeuristicType, class LogicType, class DataType>
  void Genetic<HeuristicType,LogicType,DataType>::doMeditation() {
    this->candidates.modifyAll(meditateEach, 0);
  }
  
  template <class HeuristicType, class LogicType, class DataType>
  vector<DataType *> *Genetic<HeuristicType,LogicType,DataType>::combine(vector<DataType *> *dad, vector<DataType *> *mom) {
    LLRB_Tree<DataType *, uint64_t> treeRet;
    
    for (uint64_t mCurrent = 0; mCurrent < mom->size(); mCurrent++) {
      treeRet.insert(mom->at(mCurrent), (uint64_t)mom->at(mCurrent));
    }
    
    for (uint64_t dCurrent = 0; dCurrent < dad->size(); dCurrent++) {
      treeRet.insert(dad->at(dCurrent), (uint64_t)dad->at(dCurrent));
    }
    
    return treeRet.select(NULL,NULL);
  }
  
  template <class HeuristicType, class LogicType, class DataType>
  vector<DataType *> *Genetic<HeuristicType,LogicType,DataType>::crossover(vector<DataType *> *dad, vector<DataType *> *mom) {
    LLRB_Tree<DataType *, uint64_t> treeRet;
    uint64_t tmp, crossover, mCrossover;
    uint64_t retSize = max(dad->size(), mom->size());
    uint64_t treeSizeStart;
    
    if (dad == mom) {
      return new vector<double *>(dad->begin(), dad->end());
    }
    
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
  
  template <class HeuristicType, class LogicType, class DataType>
  HeuristicType *Genetic<HeuristicType,LogicType,DataType>::reproduce(HeuristicType *dad, HeuristicType *mom) {
    vector<double *> *childInputs;
    vector<double *> *childOutputs;
    vector<Trust<DataType> *> *childTrusts;
    vector<double *> *childExpectations;
    vector<Info *> *childHiddenInfo, *tmpInfo;
    LLRB_Tree<Info *, uint64_t> infoTree;
    HeuristicType *tmpHeuristic;
    uint64_t tmpNum, layer;
    
    
    childInputs = this->question.select(NULL,NULL);
    childTrusts = this->answer.select(NULL,NULL);
    
    childOutputs = new vector<DataType *>();
    childOutputs->reserve(childTrusts->size());
    childExpectations = new vector<double *>();
    childExpectations->reserve(childTrusts->size());
    
    for (uint64_t ix = 0; ix < childTrusts->size(); ix++) {
      double *childExpect = new double(0.0);
      childOutputs->push_back(childTrusts->at(ix)->actual);
      childExpectations->push_back(childExpect);
      
      if (childTrusts->at(ix)->prediction == NULL) {
        childTrusts->at(ix)->prediction = new Prediction<DataType>();
      }
    }
    
    tmpInfo = dad->getHiddenInfo();
    
    for (uint64_t ix = 0; ix < tmpInfo->size(); ix++) {
      infoTree.insert(tmpInfo->at(ix), tmpInfo->at(ix)->k);
    }
    
    tmpInfo = mom->getHiddenInfo();
    
    for (uint64_t ix = 0; ix < tmpInfo->size(); ix++) {
      infoTree.insert(tmpInfo->at(ix), tmpInfo->at(ix)->k);
    }
    
    
    tmpNum = infoTree.size();
    
    layer = 7;
    
    for (uint64_t ix = 0; ix < log2(infoTree.size()); ix++) {
      while (layer != 0) {
        Info *newConn = new Info;
        newConn->c.layer = layer;
        newConn->c.position = random() % childInputs->size();
        layer -= (rand() % layer) + 1;
        newConn->c.inputLayer = layer;
        newConn->c.inputPosition = random() % childInputs->size();
        
        infoTree.insert(newConn, newConn->k);
      }
    }
    
    childHiddenInfo = infoTree.select(NULL, NULL);
    
    tmpHeuristic = new HeuristicType(childInputs,childOutputs,childExpectations,childHiddenInfo);
    
    tmpHeuristic->energy *= childHiddenInfo->size() * glbEnergyCnst;
    
    childInputs->resize(0);
    childOutputs->resize(0);
    childExpectations->resize(0);
    childHiddenInfo->resize(0);
    childTrusts->resize(0);
    
    delete childInputs;
    delete childOutputs;
    delete childExpectations;
    delete childHiddenInfo;
    delete childTrusts;
    
    return tmpHeuristic;
  }
  
  template <class HeuristicType, class LogicType, class DataType>
  void Genetic<HeuristicType,LogicType,DataType>::get() {
    
  }
  
  template <class HeuristicType, class LogicType, class DataType>
  Genetic<HeuristicType,LogicType,DataType>::Genetic() {
    
  }
  
}

#endif
