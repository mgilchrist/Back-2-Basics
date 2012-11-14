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
    double fitness = 0, tmp;
    
    for (uint64_t ix = 0; ix < harmony->size(); ix++) {
      tmp = pow((*(harmony->at(ix)->output) - *(harmony->at(ix)->expectation)) / *(harmony->at(ix)->output), 2);
      ((Genetic *)world)->candidatesInArea->search(harmony->at(ix)->output)->push(current, tmp);
      fitness += tmp;
    }
    
    fitness /= harmony->size();
    
    return sqrt(fitness);
  }
  
  static vector<uint64_t> *crossover(vector<uint64_t> *dad, vector<uint64_t> *mom,
                                     double dFitness, double mFitness) {
    vector<uint64_t> *ret;
    double fitness, tmp;
    double crossover;
    
    if ((dad->size() != mom->size()) || (dad == mom)) {
      return new vector<uint64_t>(dad->begin(), dad->end());
    }
    
    ret = new vector<uint64_t>();
    ret->reserve(dad->size());
    
    fitness = (dFitness-mFitness) / dFitness;
    
    tmp = (rand() / RAND_MAX) * (rand() / RAND_MAX);
    crossover = ((tmp / 2) + 0.5) + fitness;
    
    if (crossover <= 0.0) {
      ret->assign(dad->begin(), dad->end());
    } else if (crossover >= 1.0) {
      ret->assign(mom->begin(), mom->end());
    } else {
      for (uint64_t ix = 0; ix < (uint64_t)(crossover*dad->size()); ix++) {
        ret->push_back(dad->at(ix));
      }
      for (uint64_t ix = (uint64_t)(crossover*dad->size()); ix < mom->size(); ix++) {
        ret->push_back(mom->at(ix));
      }
    }
    
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
HeuristicType *Genetic<HeuristicType,DataType>::reproduce(HeuristicType *father, HeuristicType *mother) {
  
}

template <class HeuristicType, class DataType>
void Genetic<HeuristicType,DataType>::get() {
  
}

template <class HeuristicType, class DataType>
Genetic<HeuristicType,DataType>::Genetic() {
  this->candidatesInArea = new LLRB_Tree<Heap<HeuristicType, double>, uint64_t>();
}


#endif
