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

template <class DataType>
class Genetic : public Stoichastic<DataType> {
  
private:
  
  double envToughness;
  uint64_t reproductionAgeMin;
  uint64_t reproductionAgeMax;
  
  void add();
  void rCalcFitness(Graph::LLRB_TreeNode<Heuristic *,DataType> *current, uint64_t instance);
  void calcFitness();
  void determineSurvival();
  void reproduce();
  void get();
  
public:
  Genetic();
  
  
};


template <class DataType>
void Genetic<DataType>::rCalcFitness(Graph::LLRB_TreeNode<Heuristic *,DataType> *current, uint64_t instance) {
  DataType *thisExpectation = current->data->getExpectation();
  DataType fitness = 0;
  
  for (uint64_t ix = 0; ix < this->spaceSize; ix++) {
    fitness += pow((this->space[ix] - thisExpectation[ix]) / (double)this->space);
  }
  
  fitness /= this->spaceSize;
  
  this->candidates->updateKey(current->key, sqrt(fitness), instance);
  
  if (current->getLeaf(LEFT) != NULL) {
    rCalcFitness(current->getLeaf(LEFT), ((current->key == current->getLeaf(LEFT)) ? ++instance : 0));
  }
  
  if (current->getLeaf(RIGHT) != NULL) {
    rCalcFitness(current->getLeaf(RIGHT), ((current->key == current->getLeaf(RIGHT)) ? ++instance : 0));
  }
}

template <class DataType>
void Genetic<DataType>::calcFitness() {
  rCalcFitness(this->candidates->treeNode,0);
}

template <class DataType>
void Genetic<DataType>::determineSurvival() {
  
}

template <class DataType>
void Genetic<DataType>::reproduce() {
  
}

template <class DataType>
void Genetic<DataType>::get() {
  
}

template <class DataType>
Genetic<DataType>::Genetic() {
  
}


#endif
