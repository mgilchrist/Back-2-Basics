//
//  Optimization.h
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

#ifndef OpenSource_Optimization_h
#define OpenSource_Optimization_h

#include "LLRB_Tree.h"
#include "Heuristic.h"


namespace Optimization {
  
  template <class DataType>
  struct Prediction {
    LLRB_Tree<DataType *, uint64_t> predictions;
    DataType expectation;
    double confidence = 0.0;
  };
  
  template <class DataType>
  struct Trust {
    DataType *actual;
    Prediction<DataType> *prediction = NULL;
  };
  
  template <class HeuristicType, class DataType>
  class Optimization {
    
  private:
    typedef struct RandSection {
      uint64_t begin;
      uint64_t requests;
      double probability;
    } URandom;
    
  protected:
    
    double accuracy_rate = 0.00;
    
    virtual void doEpoch() =0;
    
  public:
    
    LLRB_Tree<DataType *, uint64_t> question;
    LLRB_Tree<Trust<DataType> *, uint64_t> answer;
    
    
  public:
    
    LLRB_Tree<HeuristicType *, uint64_t> candidates;
    
    
    
    Optimization();
    
    void addInput(DataType *);
    void addOutput(DataType *);
    
    void addInput(vector<DataType *> *);
    void addOutput(vector<DataType *> *);
    
    virtual void initInternals() =0;
    void optimizeAnwser();
    
  };
  
  template <class HeuristicType, class DataType>
  Optimization<HeuristicType,DataType>::Optimization() {
    
  }
  
  template <class HeuristicType, class DataType>
  void Optimization<HeuristicType,DataType>::addInput(DataType *input) {
    question.insert(input, (uint64_t)input);
  }
  
  template <class HeuristicType, class DataType>
  void Optimization<HeuristicType,DataType>::addOutput(DataType *output) {
    Trust<DataType> *trust = new Trust<DataType>;
    
    trust->actual = output;
    trust->prediction.expectation = 0.0;
    trust->prediction.prediction = 0.0;
    
    answer.insert(trust, (uint64_t)output);
  }
  
  template <class HeuristicType, class DataType>
  void Optimization<HeuristicType,DataType>::addInput(vector<DataType *> *input) {
    for (uint64_t ix = 0; ix < input->size(); ix++) {
      question.insert(input->at(ix), (uint64_t)(input->at(ix)));
    }
  }
  
  template <class HeuristicType, class DataType>
  void Optimization<HeuristicType,DataType>::addOutput(vector<DataType *> *output) {
    Trust<DataType> *trust;
    
    for (uint64_t ix = 0; ix < output->size(); ix++) {
      trust = new Trust<DataType>;
      
      trust->actual = output->at(ix);
      
      answer.insert(trust, (uint64_t)output->at(ix));
    }
  }
  
  template <class HeuristicType, class DataType>
  void Optimization<HeuristicType,DataType>::optimizeAnwser() {
    doEpoch();
  }
  
  
}

#endif
