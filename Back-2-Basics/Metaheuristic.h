//
//  Metaheuristic.h
//  HelloWorld
//
//  Created by Marcus Gilchrist on 9/9/12.
//  Copyright (c) 2012 Marcus Gilchrist. All rights reserved.
//

#ifndef __HelloWorld__Metaheuristic__
#define __HelloWorld__Metaheuristic__

#include <iostream>

#include "ArrayList.h"

#define STD_NUM_CANDIDATES  4


template <class OptimizationType, class HeuristicType, class DataType>
class Metaheuristic {
  
private:
  OptimizationType *optFunction;
  Collection::ArrayList<HeuristicType *, double> *theory;
  double *theoryRating;
  double *theoryExpectation;
  uint64_t numCandidates = STD_NUM_CANDIDATES;
  uint64_t epoch;
  
  void rPostResult(uint64_t current, DataType result);
  DataType rConsensus(uint64_t current);
  
public:
  Metaheuristic();
  Metaheuristic(Collection::Array<DataType> *input);
  
  virtual void postResult(DataType result);
  virtual DataType getConsensus();
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


template <class OptimizationType, class HeuristicType, class DataType>
void Metaheuristic<OptimizationType,HeuristicType,DataType>::rPostResult(uint64_t current, DataType result) {
  
  HeuristicType *tmp;
  
  if ((tmp = theory->nodeAtIndex(theory->getLeft(current))) != NULL) {
    tmp->doCorrection(result, 0.0);
    rPostResult(theory->getLeft(current), result);
  }
  
  if ((tmp = theory->nodeAtIndex(theory->getRight(current))) != NULL) {
    tmp->doCorrection(result, 0.0);
    rPostResult(theory->getRight(current), result);
  }
}

template <class OptimizationType, class HeuristicType, class DataType>
Metaheuristic<OptimizationType,HeuristicType,DataType>::Metaheuristic() {
  
}

template <class OptimizationType, class HeuristicType, class DataType>
Metaheuristic<OptimizationType,HeuristicType,DataType>::Metaheuristic(Collection::Array<DataType> *input) {
  HeuristicType *thisTheory;
  Collection::Comparable<HeuristicType *, double> comp;
  uint64_t tmpSize;
  Collection::Stack<double *> *thisInput;
  
  theory = new Collection::ArrayList<HeuristicType *,double>(numCandidates);
  
  epoch = 0;
  
  
  for (int ix = 0; ix < numCandidates; ix++) {
    tmpSize = (random() % input->getSize()) + 1;
    thisInput = new Collection::Stack<double *>(tmpSize);
    for (int jx = 0; jx < thisInput->getSize(); jx++) {
      thisInput->push(input->ptrToIndex(jx));
    }
    
    thisTheory = new HeuristicType(input);
    
    comp.data = thisTheory;
    comp.key = 0.5;
    
    theory->setIndex(ix, comp);
    delete thisInput;
  }
  
}

template <class OptimizationType, class HeuristicType, class DataType>
void Metaheuristic<OptimizationType,HeuristicType,DataType>::postResult(DataType result) {
  /*
  HeuristicType *tmp;
  
  uint64_t current = theory->getTreeRoot();
  
  if ((tmp = theory->nodeAtIndex(current)) != NULL) {
    tmp->doCorrection(result, 0.0);
    rPostResult(current, result);
  }
   */
  
  for (int ix = 0; ix < theory->getSize(); ix++) {
    theory->atIndex(ix).data->doCorrection(result,0.0);
  }
}

template <class OptimizationType, class HeuristicType, class DataType>
DataType Metaheuristic<OptimizationType,HeuristicType,DataType>::rConsensus(uint64_t current) {
  
  HeuristicType *tmp;
  double expectation = 0.0;
  
  if ((tmp = theory->nodeAtIndex(theory->getLeft(current))) != NULL) {
    expectation += (tmp->getExpectation()) +
    rConsensus(theory->getLeft(current));
    cout << "current left expectation:";
    cout << expectation;
    cout << " \n";
  }
  
  if ((tmp = theory->nodeAtIndex(theory->getRight(current))) != NULL) {
    expectation += (tmp->getExpectation()) +
    rConsensus(theory->getRight(current));
    cout << "current right expectation:";
    cout << expectation;
    cout << " \n";

  }
  
  return expectation;
  
}

template <class OptimizationType, class HeuristicType, class DataType>
DataType Metaheuristic<OptimizationType,HeuristicType,DataType>::getConsensus() {
  //HeuristicType *tmp;
  //uint64_t current = theory->getTreeRoot();
  double expectation = 0.0;
  
  for (int ix = 0; ix < theory->getSize(); ix++) {
    if (theory->atIndex(ix).data == NULL) {
      continue;
    }
    theory->atIndex(ix).data->calculateExpectation();
    expectation += theory->atIndex(ix).data->getExpectation();
    
  }
  
  expectation /= theory->getSize();

  
  return expectation;
  
}


#endif /* defined(__HelloWorld__Metaheuristic__) */
