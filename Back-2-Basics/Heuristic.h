//
//  Heuristic.h
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

#ifndef OpenSource_Heuristic_h
#define OpenSource_Heuristic_h

#include "Collection.h"
#include "Stack.h"
#include <vector>


struct Harmony {
  void *logicElement = NULL;
  double *reality = NULL;
  double *expectation = NULL;
};

template <class HeuristicType, class DataType>
class Heuristic {
  
public:
  
  double persistance = 0.0;
  double energy = 0.0;
  
public:
  
  Heuristic() {
    
  }
  
  Heuristic(std::vector<double *> *input,
            std::vector<double *> *output,
            std::vector<double *> *expectation,
            std::vector<uint64_t> *hiddenInfo,
            double persistance) {
    HeuristicType(input, output, expectation, hiddenInfo);
    this->persistance = persistance;
    
    /* Estimate energy use */
    energy = (double)input->size();
    
    for (uint64_t ix = 0; ix < hiddenInfo->size(); ix++) {
      energy *= (double)hiddenInfo->at(ix);
    }
    
    energy *= (double)output->size();
  }
  
  virtual void calcExpectation(uint64_t) =0;
  virtual void doCorrection() =0;
  
  virtual vector<DataType *> *getInputs() =0;
  virtual vector<uint64_t> *getHiddenInfo() =0;
  virtual vector<Harmony *> *getHarmony() =0;
  
  virtual void removeOutput(DataType *) =0;
  
  virtual uint64_t timeAlive() =0;
  
  virtual void optimalPrune() =0;
  virtual void probablisticPrune() =0;
  
  virtual void merge(HeuristicType *) =0;
};



#endif
