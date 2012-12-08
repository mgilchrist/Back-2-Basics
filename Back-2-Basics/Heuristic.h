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

template <class LogicType>
struct Harmony {
  LogicType *logicElement = NULL;
  double *reality = NULL;
  double *expectation = NULL;
};

struct Connection {
  uint8_t inputLayer;
  uint8_t layer;
  uint16_t inputPosition;
  uint16_t position;
};

union Info {
  Connection c;
  uint64_t k;
};

template <class HeuristicType, class LogicType, class DataType>
class Heuristic {
  
public:
  
  double persistance = 0.0;
  double energy = 0.0;
  uint64_t experiencedEpochs = 0;
  bool registered = false;
  
public:
  
  Heuristic() {
    
  }
  
  virtual void calcExpectation(uint64_t) =0;
  virtual void doCorrection() =0;
  
  virtual vector<DataType *> *getInputs() =0;
  virtual vector<Info *> *getHiddenInfo() =0;
  virtual vector<Harmony<LogicType> *> *getHarmony() =0;
  
  virtual void removeOutput(DataType *) =0;
  
  virtual uint64_t timeAlive() =0;
  
  virtual void optimalPrune() =0;
  virtual void probablisticPrune() =0;
  
  virtual void merge(HeuristicType *) =0;
};



#endif
