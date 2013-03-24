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
  static const uint64_t ofAge = 1;
  static const uint64_t maxAge = 100;
  
  template <class HeuristicType, class LogicType, class DataType>
  class Genetic : public Stoichastic<HeuristicType,DataType> {
    
  protected:
    
    
    static bool checkSurvivalEach(LLRB_TreeNode<HeuristicType *,uint64_t> *current, void *world) {
      Heuristic<HeuristicType,LogicType,DataType> *candidate = current->data;
      LLRB_Tree<Trust<DataType> *, uint64_t> *cia = &(((Genetic *)world)->active);
      vector<Harmony<LogicType> *> *harmony = candidate->getHarmony();
      
      
      if (!candidate->registered) {
        if (candidate->experiencedEpochs > (pow(candidate->hiddenWidth, 2) * maxAge)) {
          return true;
        }
        return false;
      }
      
      for (uint64_t ix = 0; ix < harmony->size(); ix++) {
        Trust<double> *localComp = cia->search((uint64_t)harmony->at(ix)->reality);
        
        if (harmony->at(ix) != NULL) {
          if (harmony->at(ix)->logicElement == NULL) {
            Trust<double> *localComp = cia->search((uint64_t)harmony->at(ix)->reality);
            localComp->prediction->predictions.remove(harmony->at(ix)->expectation, (uint64_t)harmony->at(ix)->expectation);
            delete harmony->at(ix)->expectation;
            harmony->at(ix)->expectation = NULL;
            
            if (!localComp->prediction->predictions.size()) {
              cia->remove(localComp, (uint64_t)harmony->at(ix)->reality);
              delete localComp->prediction;
              localComp->prediction = NULL;
            }
            delete harmony->at(ix);
            harmony->at(ix) = NULL;
            continue;
          }
        } else {
          continue;
        }
        
        if (localComp == NULL) {
          continue;
        }
        uint64_t envSize = localComp->prediction->predictions.size();
        double toughness = (localComp->prediction->confidence * envSize) - glbAreaCapacity;
        
        if (toughness > 0.0) {
          toughness /= envSize;
        } else {
          toughness = 0.0;
        }
        
        candidate->persistance -= (toughness+glbToughness);
      }
      
      if (candidate->persistance < 0.0) {
        for (uint64_t ix = 0; ix < harmony->size(); ix++) {
          Trust<double> *localComp = cia->search((uint64_t)harmony->at(ix)->reality);
          if (localComp == NULL) {
            continue;
          }
          if (localComp->prediction->predictions.search((uint64_t)harmony->at(ix)->expectation)) {
            localComp->prediction->predictions.remove(harmony->at(ix)->expectation, (uint64_t)harmony->at(ix)->expectation);
            delete harmony->at(ix)->expectation;
            harmony->at(ix)->expectation = NULL;
          }
          
          if (!localComp->prediction->predictions.size()) {
            cia->remove(localComp, (uint64_t)harmony->at(ix)->reality);
            delete localComp->prediction;
            localComp->prediction = NULL;
          }
        }
        
        return true;
      }
      
      return false;
    }
    
    static uint64_t calcFitnessEach(LLRB_TreeNode<HeuristicType *,uint64_t> *current, void *world) {
      Heuristic<HeuristicType,LogicType,DataType> *candidate = current->data;
      vector<Harmony<LogicType> *> *harmony = candidate->getHarmony();
      double fitness = 0.0, err;
      uint64_t harmonySize = 0;
      
      candidate->calcExpectation(currentTime);
      
      for (uint64_t ix = 0; ix < harmony->size(); ix++) {
        if ((harmony->at(ix) != NULL) && (harmony->at(ix)->logicElement != NULL)) {
          double output = *(harmony->at(ix)->reality);
          double expectation = *(harmony->at(ix)->expectation);
          err = (output - expectation);
          err = pow(err,2);
          fitness += err;
          harmonySize++;
        }
      }
      
      if ((candidate->experiencedEpochs > (pow(candidate->hiddenWidth, 2) * ofAge)) && (!candidate->registered)) {
        for (uint64_t ix = 0; ix < harmony->size(); ix++) {
          Trust<double> *localComp = ((Genetic *)world)->answer.search((uint64_t)harmony->at(ix)->reality);
          if (localComp->prediction == NULL) {
            continue;
          }
          localComp->prediction->predictions.insert(harmony->at(ix)->expectation, (uint64_t)harmony->at(ix)->expectation);
          ((Genetic *)world)->active.insert(localComp, (uint64_t)harmony->at(ix)->reality);
        }
        candidate->registered = true;
      }
      
      fitness /= harmonySize;
      fitness = sqrt(fitness);
      
      candidate->persistance += (double)harmonySize * ((pow(harmonySize, 1.618) / pow(((Genetic *)world)->answer.size(), 1.618))) * (1.0 - fitness);
      
      return current->key;
    }
    
    static uint64_t meditateEach(LLRB_TreeNode<HeuristicType *,uint64_t> *current, void *world) {
      Heuristic<HeuristicType,LogicType,DataType> *candidate = current->data;
      
      candidate->doCorrection();
      
      return current->key;
    }
    
    static void mutate(vector<Info *> *vect, uint64_t outputWidth,
                       uint64_t inputWidth, uint64_t hiddenWidth, double error_rate);
    
    vector<DataType *> *crossover(vector<DataType *> *dad, vector<DataType *> *mom);
    
    void add();
    void evaluateFitness();
    void checkSurvival();
    void doMeditation();
    void doGeneration();
    HeuristicType *reproduce(HeuristicType *father, HeuristicType *mother);
    
  public:
    Genetic();
    
    
  };
  
  
  template <class HeuristicType, class LogicType, class DataType>
  void Genetic<HeuristicType,LogicType,DataType>::doGeneration() {
    vector<HeuristicType *> *mates;
    
    evaluateFitness();
    checkSurvival();
    
    mates = this->candidates.select(NULL, NULL);
    
    for (uint64_t ix = 0; ix < mates->size(); ix++) {
      HeuristicType *dad = mates->at(ix);
      HeuristicType *mom = mates->at(rand() % mates->size());
      
      if ((dad->experiencedEpochs < dad->hiddenWidth) || (mom->experiencedEpochs < dad->hiddenWidth)) {
        continue;
      }
      
      double repoRate = 1.0 / (200.0 + dad->energy + mom->energy);
      double rVal = rand() / (double)RAND_MAX;
      
      repoRate *= this->error_rate;
      
      if (rVal < repoRate) {
        
        dad->optimalPrune();
        
        if (mom != dad) {
          mom->optimalPrune();
        }
        
        HeuristicType *child = reproduce(dad, mom);
        
        this->candidates.insert(child, (uint64_t)child);
      }
    }
    
    //if ((rand() / (double)RAND_MAX) < this->error_rate) {
    //this->spawn();
    //}
    
    doMeditation();
  }
  
  
  template <class HeuristicType, class LogicType, class DataType>
  void Genetic<HeuristicType,LogicType,DataType>::evaluateFitness() {
    this->candidates.modifyAll(calcFitnessEach, this);
  }
  
  template <class HeuristicType, class LogicType, class DataType>
  void Genetic<HeuristicType,LogicType,DataType>::checkSurvival() {
    this->candidates.removal(checkSurvivalEach, this);
  }
  
  template <class HeuristicType, class LogicType, class DataType>
  void Genetic<HeuristicType,LogicType,DataType>::doMeditation() {
    this->candidates.modifyAll(meditateEach, 0);
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
  void Genetic<HeuristicType,LogicType,DataType>::mutate(vector<Info *> *vect,
                                                         uint64_t outputWidth,
                                                         uint64_t inputWidth,
                                                         uint64_t hiddenWidth,
                                                         double error_rate) {
    uint64_t mutations = (double)vect->size() * error_rate;
    uint32_t pos;
    Info *entry;
    uint64_t randVal = 0;
    
    for (uint64_t ix = 0; ix < mutations; ix++) {
      if (!randVal) {
        randVal = random();
      }
      entry = vect->at(rand()%vect->size());
      
      if (randVal & 0x01) {;
        pos = entry->c.position ^ (1 << (rand() % LAYER_SHIFT));
        
        if (INFO_LAYER(entry->c.position) == 7) {
          entry->c.position = (pos & LAYER_MASK) | pos % outputWidth;
        } else {
          entry->c.position = (pos & LAYER_MASK) | pos % hiddenWidth;
        }
      } else {
        pos = entry->c.inputPosition ^ (1 << (rand() % LAYER_SHIFT));
        
        if (INFO_LAYER(entry->c.inputPosition)) {
          entry->c.inputPosition = (pos & LAYER_MASK) | pos % hiddenWidth;
        } else {
          entry->c.inputPosition = (pos & LAYER_MASK) | pos % inputWidth;
        }
      }
      randVal >>= 1;
    }
  }
  
  template <class HeuristicType, class LogicType, class DataType>
  HeuristicType *Genetic<HeuristicType,LogicType,DataType>::reproduce(HeuristicType *dad, HeuristicType *mom) {
    vector<double *> *childInputs;
    vector<Trust<DataType> *> *childTrusts;
    LLRB_Tree<double *, uint64_t> childExpectations;
    vector<Info *> *childHiddenInfo, *tmpInfo;
    LLRB_Tree<Info *, uint64_t> infoTree;
    HeuristicType *tmpHeuristic;
    double mutation_rate = this->error_rate;
    
    if (this->questionCache == NULL) {
      this->questionCache = this->question.select(NULL,NULL);
    }
    
    if (this->answerCache == NULL) {
      this->answerCache = this->answer.select(NULL,NULL);
    }
    
    childInputs = this->questionCache;
    childTrusts = this->answerCache;
    
    tmpInfo = dad->getHiddenInfo();
    mutate(tmpInfo, childTrusts->size(), childInputs->size(),
           dad->hiddenWidth, mutation_rate);
    
    for (uint64_t ix = 0; ix < tmpInfo->size(); ix++) {
      infoTree.insert(tmpInfo->at(ix), tmpInfo->at(ix)->k);
    }
    
    tmpInfo = mom->getHiddenInfo();
    mutate(tmpInfo, childTrusts->size(), childInputs->size(),
           mom->hiddenWidth, mutation_rate);
    
    for (uint64_t ix = 0; ix < tmpInfo->size(); ix++) {
      Info *thisInfo = tmpInfo->at(ix);
      if (INFO_LAYER(thisInfo->c.position) != 7) {
        thisInfo->c.position = (thisInfo->c.position & LAYER_MASK) | INFO_POSITION(thisInfo->c.position + dad->hiddenWidth);
      }
      if (!(INFO_LAYER(thisInfo->c.inputPosition))) {
        thisInfo->c.inputPosition = (thisInfo->c.inputPosition & LAYER_MASK) | INFO_POSITION(thisInfo->c.inputPosition + dad->hiddenWidth);
      }
      infoTree.insert(thisInfo, thisInfo->k);
    }
    
    childHiddenInfo = infoTree.select(NULL, NULL);
    
    for (uint64_t ix = 0; ix < childHiddenInfo->size(); ix++) {
      if (INFO_LAYER(childHiddenInfo->at(ix)->c.position) == 7) {
        double *reality = childTrusts->at(INFO_POSITION(childHiddenInfo->at(ix)->c.position))->actual;
        double *childExpect = new double(0.0);
        childExpectations.insert(childExpect, (uint64_t)reality);
        
        if (childTrusts->at(ix)->prediction == NULL) {
          childTrusts->at(ix)->prediction = new Prediction<DataType>();
        }
      }
    }
    
    tmpHeuristic = new HeuristicType
      (
       childInputs,
       childTrusts,
       &childExpectations,
       childHiddenInfo,
       dad->hiddenWidth+mom->hiddenWidth
       );
    
    tmpHeuristic->persistance = dad->persistance + mom->persistance;
    tmpHeuristic->energy *= childHiddenInfo->size() * glbEnergyCnst;
    
    childHiddenInfo->resize(0);
    
    delete childHiddenInfo;
    
    return tmpHeuristic;
  }
  
  template <class HeuristicType, class LogicType, class DataType>
  Genetic<HeuristicType,LogicType,DataType>::Genetic() {
    
  }
  
}

#endif
