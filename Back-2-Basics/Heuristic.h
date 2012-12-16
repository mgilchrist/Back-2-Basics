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
//#include "LLRB_Tree.h"

template <class LogicType>
struct Harmony {
  LogicType *logicElement = NULL;
  double *reality = NULL;
  double *expectation = NULL;
};

#define LAYER_SHIFT 28

#define POSITION_MASK (((uint32_t)1 << LAYER_SHIFT) - 1)
#define LAYER_MASK    ((uint32_t)(POSITION_MASK ^ 0xFFFFFFFF))

#define INFO_LAYER(x) (x >> LAYER_SHIFT)
#define INFO_POSITION(x) (x & POSITION_MASK)

struct Connection {
  uint32_t inputPosition;
  uint32_t position;
};

union Info { /* NOT RIGHT!! TODO */
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
  //LLRB_Tree<Info *, uint64_t> *hiddenInfo;
  
public:
  
  Heuristic() {
    
  }
  
  virtual void calcExpectation(uint64_t) =0;
  virtual void doCorrection() =0;
  
  virtual vector<DataType *> *getInputs() =0;
  virtual vector<Info *> *getHiddenInfo() =0;
  virtual vector<Harmony<LogicType> *> *getHarmony() =0;
  
  virtual void optimalPrune() =0;
  virtual void probablisticPrune() =0;
};



#endif
