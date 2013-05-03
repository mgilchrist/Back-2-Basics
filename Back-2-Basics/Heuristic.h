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

//#include "Graph.h"
#include "LLRB_Tree.h"
using namespace Tree;
#include "Collection.h"
#include "Stack.h"
#include <vector>

template <class LogicType>
struct Harmony {
  LogicType *logicElement = NULL;
  double *reality = NULL;
  double *expectation = NULL;
};

//#define LAYER_SHIFT 28

//#define POSITION_MASK (((uint32_t)1 << LAYER_SHIFT) - 1)
//#define LAYER_MASK    ((uint32_t)(0xFFFFFFFF << LAYER_SHIFT))

//#define INFO_LAYER(x) (x >> LAYER_SHIFT)
//#define INFO_POSITION(x) (x & POSITION_MASK)

struct Connection {
  uint32_t inputPosition;
  uint32_t inputLayer;
  uint32_t position;
  uint32_t layer;
};

union Info_union {
  struct
  {
    uint32_t inputPosition;
    uint32_t inputLayer;
    uint32_t position;
    uint32_t layer;
  };
  uint32_t ca[4];
};

class Info { /* NOT RIGHT!! TODO */
  
public:
  Info_union c;
  
  bool operator== (Info &param) {
    for (int ix = 0; ix < 4; ix++) {
      if (c.ca[ix] != param.c.ca[ix]) {
        return false;
      }
    }
    
    return true;
  }
  
  bool operator< (Info &param) {
    for (int ix = 0; ix < 4; ix++) {
      if (c.ca[ix] < param.c.ca[ix]) {
        return true;
      } else if (c.ca[ix] > param.c.ca[ix]) {
        break;
      }
    }
    
    return false;
  }
  
  bool operator> (Info &param) {
    for (int ix = 3; ix >= 0; ix++) {
      if (c.ca[ix] > param.c.ca[ix]) {
        return true;
      } else if (c.ca[ix] < param.c.ca[ix]) {
        break;
      }
    }
    
    return false;
  }
  
  bool operator<= (Info &param) {
    for (int ix = 0; ix < 4; ix++) {
      if (c.ca[ix] < param.c.ca[ix]) {
        return true;
      } else if (c.ca[ix] > param.c.ca[ix]) {
        return false;
      }
    }
    
    return true;
  }
  
  bool operator>= (Info &param) {
    for (int ix = 3; ix >= 0; ix++) {
      if (c.ca[ix] > param.c.ca[ix]) {
        return true;
      } else if (c.ca[ix] < param.c.ca[ix]) {
        return false;
      }
    }
    
    return true;
  }
};

template <class DataType>
struct Prediction {
  LLRB_Tree<DataType *, uint64_t> predictions;
  DataType expectation = 0;
  double confidence = 0.0;
};

template <class DataType>
struct Trust {
  DataType *actual;
  Prediction<DataType> *prediction = NULL;
};

template <class HeuristicType, class LogicType, class DataType>
class Heuristic {
  
public:
  
  double persistance = 0.0;
  double energy = 0.0;
  uint64_t experiencedEpochs = 0;
  bool registered = false;
  uint32_t hiddenWidth = 0;
  //LLRB_Tree<Info *, uint64_t> *hiddenInfo;
  
  virtual void deinitialize() =0;
  
public:
  
  Heuristic() {
    
  }
  
  //~Heuristic() {
  //  deinitialize();
  //}
  
  virtual void calcExpectation(uint64_t) =0;
  virtual void doCorrection() =0;
  
  virtual vector<DataType *> *getInputs() =0;
  virtual vector<Info *> *getHiddenInfo() =0;
  virtual vector<Harmony<LogicType> *> *getHarmony() =0;
  
  virtual void optimalPrune() =0;
  virtual void probablisticPrune() =0;
};



#endif
