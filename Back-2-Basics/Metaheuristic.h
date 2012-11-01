//
//  Metaheuristic.h
//  Back-2-Basics
//
//  Created by Marcus Gilchrist on 9/9/12.
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

#ifndef __HelloWorld__Metaheuristic__
#define __HelloWorld__Metaheuristic__

#include <iostream>

#include "ArrayList.h"
#include "Optimization.h"
#include "Heuristic.h"
#include <vector>


#define STD_NUM_CANDIDATES  4
#define OUTPUT_SIZE 64

template <class HeuristicType, class DataType>
class Metaheuristic {
  
private:
  Genetic<HeuristicType,double> *optFunction;
  Collection::ArrayList<HeuristicType *, double> *theory;
  double *theoryRating;
  double *theoryExpectation;
  uint64_t numCandidates = STD_NUM_CANDIDATES;
  uint64_t epoch;
  
  void rPostResult(uint64_t current, DataType result);
  DataType rConsensus(uint64_t current);
  
public:
  Metaheuristic();
  Metaheuristic(std::vector<DataType> *input, std::vector<HeuristicType *> *candidates);
  
  virtual void postResult(DataType result);
  virtual void getConsensus(std::vector<double> *);
};

//
//  Metaheuristic.cpp
//  HelloWorld
//
//  Created by Marcus Gilchrist on 9/9/12.
//  Copyright (c) 2012 Marcus Gilchrist. All rights reserved.
//


/*
 class Stoichastic : public Optimization {
 
 private:
 
 public:
 Stoichastic();
 
 Stack<uint64_t> *selections(Stack<double *>,uint64_t *,uint64_t);
 
 };
 */

/*
template <class DataType>
void Metaheuristic<DataType>::rPostResult(uint64_t current, DataType result) {
  
  Heuristic *tmp;
  
  if ((tmp = theory->nodeAtIndex(theory->getLeft(current))) != NULL) {
    tmp->doCorrection(result, 0.0);
    rPostResult(theory->getLeft(current), result);
  }
  
  if ((tmp = theory->nodeAtIndex(theory->getRight(current))) != NULL) {
    tmp->doCorrection(result, 0.0);
    rPostResult(theory->getRight(current), result);
  }
}
 */

template <class HeuristicType, class DataType>
Metaheuristic<HeuristicType,DataType>::Metaheuristic() {
  
}

template <class HeuristicType, class DataType>
Metaheuristic<HeuristicType,DataType>::Metaheuristic(std::vector<DataType> *input, std::vector<HeuristicType *> *candidates) {
  HeuristicType *thisTheory;
  Collection::Comparable<HeuristicType *, double> *comp;
  
  theory = new Collection::ArrayList<HeuristicType *,double>(numCandidates);
  comp = new Collection::Comparable<HeuristicType *, double>();
  
  epoch = 0;
  
  
  for (int ix = 0; ix < numCandidates; ix++) {
    
    thisTheory = candidates->at(ix);
    
    comp->data = thisTheory;
    comp->key = 0.5;
    
    theory->setIndex(ix, comp);
  }
  
}

template <class HeuristicType, class DataType>
void Metaheuristic<HeuristicType,DataType>::postResult(DataType result) {
  
  for (int ix = 0; ix < theory->getSize(); ix++) {
    theory->atIndex(ix)->data->doCorrection(&result,0.0);
  }
}

template <class HeuristicType, class DataType>
void Metaheuristic<HeuristicType,DataType>::getConsensus(std::vector<double> *expectation) {
  //HeuristicType *tmp;
  //uint64_t current = theory->getTreeRoot();
  std::vector<double> *tmp;
  
  for (int ix = 0; ix < theory->getSize(); ix++) {
    if (theory->atIndex(ix)->data == NULL) {
      continue;
    }
    theory->atIndex(ix)->data->calculateExpectation();
    tmp = theory->atIndex(ix)->data->getExpectation();
    
    for (uint64_t jx = 0; jx < OUTPUT_SIZE; jx++) {
      expectation->at(jx) += tmp->at(jx);
    }
    
  }
  
  for (uint64_t jx = 0; jx < OUTPUT_SIZE; jx++) {
    expectation->at(jx) /= theory->getSize();
  }

}


#endif /* defined(__HelloWorld__Metaheuristic__) */
