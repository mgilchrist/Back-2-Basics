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
//  Deprecated
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

#if 0

template <class HeuristicType, class DataType>
class Metaheuristic
{
  
private:
  Genetic<HeuristicType, int64_t> *optFunction;
  Collection::ArrayList<HeuristicType *, int64_t> *theory;
  int64_t *theoryRating;
  int64_t *theoryExpectation;
  uint64_t numCandidates = STD_NUM_CANDIDATES;
  uint64_t epoch;
  
  void rPostResult(uint64_t current, DataType result);
  DataType rConsensus(uint64_t current);
  
public:
  Metaheuristic();
  Metaheuristic(std::vector<DataType> *input, std::vector<HeuristicType *> *candidates);
  
  virtual void postResult(DataType result);
  virtual void getConsensus(std::vector<int64_t> *);
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
 
 Stack<uint64_t> *selections(Stack<int64_t *>,uint64_t *,uint64_t);
 
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
  Collection::Comparable<HeuristicType *, int64_t> *comp;
  
  theory = new Collection::ArrayList<HeuristicType *,int64_t>(numCandidates);
  comp = new Collection::Comparable<HeuristicType *, int64_t>();
  
  epoch = 0;
  
  
  for (uint64_t ix = 0; ix < numCandidates; ix++) {
    
    thisTheory = candidates->at(ix);
    
    comp->data = thisTheory;
    comp->key = 0.5;
    
    theory->setIndex(ix, comp);
  }
  
}

template <class HeuristicType, class DataType>
void Metaheuristic<HeuristicType,DataType>::postResult(DataType result) {
  
  for (uint64_t ix = 0; ix < theory->getSize(); ix++) {
    theory->atIndex(ix)->data->doCorrection();
  }
}

template <class HeuristicType, class DataType>
void Metaheuristic<HeuristicType,DataType>::getConsensus(std::vector<int64_t> *expectation) {
  //HeuristicType *tmp;
  //uint64_t current = theory->getTreeRoot();
  std::vector<int64_t> *tmp;
  
  for (int ix = 0; ix < theory->getSize(); ix++) {
    if (theory->atIndex(ix)->data == NULL) {
      continue;
    }
    theory->atIndex(ix)->data->calcExpectation();
    
    for (uint64_t jx = 0; jx < OUTPUT_SIZE; jx++) {
      expectation->at(jx) += tmp->at(jx);
    }
    
  }
  
  for (uint64_t jx = 0; jx < OUTPUT_SIZE; jx++) {
    expectation->at(jx) /= theory->getSize();
  }

}

#endif
#endif /* defined(__HelloWorld__Metaheuristic__) */
